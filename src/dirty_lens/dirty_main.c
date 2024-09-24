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
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++)
        {
            char a = (int)value / 10 + '0';
            char b = (int)value % 10 + '0';
            if (font8x8_basic[a][i] & (1 << j))
                img[(up + i) * cfg.w + left + j] = brightness;
            if (font8x8_basic[b][i] & (1 << j))
                img[(up + i) * cfg.w + left + 10 + j] = brightness;
        }
    }
}

int compare(const void *a, const void *b) {
    return (*(uint8_t *)a - *(uint8_t *)b);
}

float calculate_bottom_brightness(uint8_t *pixels, size_t total_pixels, float ratio) {
    qsort(pixels, total_pixels, sizeof(uint8_t), compare);
    return pixels[(int)(total_pixels * ratio)];
}


int main(int argc, char *argv[])
{
    //读取配置文件
    if (argc > 1)
        cfg = get_config(argv[1]);
    else
        cfg = get_config("config.ini");
    //读取raw图
    FILE *img_input_f = fopen(cfg.input_img_path, "rb");
    int bytes_to_read = cfg.w * cfg.h;
    fseek(img_input_f, -((long)bytes_to_read), SEEK_END); //兼容pgm/raw格式
    fread(img, sizeof(unsigned char), cfg.w * cfg.h, img_input_f);
    //进行脏污检测
    memcpy(qsort_array, img, cfg.w * cfg.h);
    float value = calculate_bottom_brightness(qsort_array, cfg.w * cfg.h, cfg.ratio);
    //输出检测结果
    printf("\n%d%%---avg---%.2f---throat---%.2f\n", 40, value, cfg.throat);
    if (value < cfg.throat)
        printf("Lens is clean\n");
    else
        printf("Lens is dirty\n");
    draw_number(0, 0, value, 128);
    raw_to_bmp(cfg.output_img_path, cfg.w, cfg.h, img);

    printf("Done!");
}