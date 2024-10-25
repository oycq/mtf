#include <stdio.h>
#include <stdint.h>
#include "get_dirty_config.h"
#include "front_8x8.h"
#include "raw_to_bmp.h"

#define MAX_W 4096
#define MAX_H 4096

#pragma pack(push, 1)
uint8_t img[MAX_H * MAX_W];
uint8_t qsort_array[MAX_H * MAX_W];
#pragma pack(pop)

config_t cfg;

void draw_number(int left, int up, int value, int brightness)
{
    char a = (value / 100) % 10 + '0'; // 百位
    char b = (value / 10) % 10 + '0';  // 十位
    char c = value % 10 + '0';         // 个位

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (font8x8_basic[a][i] & (1 << j))
                img[(up + i) * cfg.w + left + j] = brightness;
            if (font8x8_basic[b][i] & (1 << j))
                img[(up + i) * cfg.w + left + 10 + j] = brightness;
            if (font8x8_basic[c][i] & (1 << j))
                img[(up + i) * cfg.w + left + 20 + j] = brightness;
        }
    }
}

int compare(const void *a, const void *b) {
    return (*(uint8_t *)a - *(uint8_t *)b);
}

float calculate_brightness(uint8_t *pixels, size_t total_pixels, float ratio) {
    qsort(pixels, total_pixels, sizeof(uint8_t), compare);
    return pixels[(int)(total_pixels * ratio)];
}

int main(int argc, char *argv[])
{
    // 读取配置文件
    if (argc > 1)
        cfg = get_config(argv[1]);
    else
        cfg = get_config("config.ini");
    
    // 读取 raw 图像
    FILE *img_input_f = fopen(cfg.input_img_path, "rb");
    int bytes_to_read = cfg.w * cfg.h;
    fseek(img_input_f, -((long)bytes_to_read), SEEK_END); // 兼容 pgm/raw 格式
    fread(img, sizeof(unsigned char), cfg.w * cfg.h, img_input_f);
    
    // 进行脏污检测
    memcpy(qsort_array, img, cfg.w * cfg.h);
    float bottom_brightness = calculate_brightness(qsort_array, cfg.w * cfg.h, cfg.bottom_ratio);
    float top_brightness = calculate_brightness(qsort_array, cfg.w * cfg.h, cfg.top_ratio);
    
    // 输出检测结果
    printf("\nHistogram%d%%---%.2f---bottom_throat---%.2f\n", (int)(cfg.bottom_ratio * 100), bottom_brightness, cfg.bottom_throat);
    
    printf("\nHistogram%d%%---%.2f---top_throat---%.2f\n", (int)(cfg.top_ratio * 100), top_brightness, cfg.top_throat);

    if (top_brightness > cfg.top_throat)
    {
        if (bottom_brightness < cfg.bottom_throat) 
        {
            printf("Lens is clean\n");
        }
        else
        {
            printf("Lens is dirty\n");
        }
    }
    else
    {
        printf("The light panel is not bright enough.\n");
    }
    
    // 绘制数值到图像上
    draw_number(0, 0, (int)bottom_brightness, 128);
    
    // 输出图像
    raw_to_bmp(cfg.output_img_path, cfg.w, cfg.h, img);

    printf("Done!\n");
}
