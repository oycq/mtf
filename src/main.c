#include <stdio.h>
#include <stdint.h>
#include "sfr_iso_oycq.h"
#include "get_config.h"
#include "front_8x8.h"
#include "raw_to_bmp.h"

#define MAX_W 4096
#define MAX_H 4096
#define MAX_ROI_W 512
#define MAX_ROI_H 512
#define MAX_ROI_N 4096

typedef struct
{
    int left;
    int right;
    int up;
    int down;
    float mtf50;
}roi_result_t;

#pragma pack(push, 1)
uint8_t raw_data[MAX_H * MAX_W];
uint8_t img[MAX_H][MAX_W];
uint8_t img_output[MAX_H][MAX_W];
uint8_t img_thresh[MAX_H][MAX_W];
uint8_t nms_map[MAX_H][MAX_W];
double sfr_input[MAX_ROI_H * MAX_ROI_W];
roi_result_t roi_results[MAX_ROI_N];
#pragma pack(pop) 

config_t cfg;

void draw_box(int left, int right, int up, int down, int value)
{
    for (int i = up; i < down; i += 2)
    {
        img_output[i][left] = value;
        img_output[i][right] = value;
    }
    for (int j = left; j < right; j += 2)
    {
        img_output[up][j] = value;
        img_output[down][j] = value;
    }
}

void draw_number(int left, int up, int value, int brightness)
{
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++)
        {
            char a = (int)value / 10 + '0';
            char b = (int)value % 10 + '0';
            if (font8x8_basic[a][i] & (1 << j))
                img_output[up + i][left + j] = brightness;
            if (font8x8_basic[b][i] & (1 << j))
                img_output[up + i][left + 10 + j] = brightness;
        }
    }
}


int check_clarity(void)
{
    int H = cfg.h;
    int W = cfg.w;
    int ROI_W = cfg.roi_w;
    int ROI_H = cfg.roi_h;
    int TOP_MARGIN = cfg.top_margin;
    int roi_index = 0;
    int pass_n = 0;
    //nms_map初始化赋值,
    for (int row = 0; row < H; row ++)
        for (int col = 0; col < W; col ++)
            nms_map[row][col] = 0;
    //img_output初始化
    memcpy(img_output, img, sizeof(img));
    //二值化图像        
    for (int row = 0; row < H; row++)
        for (int col = 0; col <= W; col ++)
        {
            int max_value = 0;
            for (int around = -ROI_W; around < ROI_W; around++)
            {
                int col_around = col + around;
                if ((col_around >=0) && (col_around < W))
                    if (img[row][col_around] > max_value)
                        max_value = img[row][col_around];
            }
            //检测当前像素是不是比周围最亮的像素暗两倍
            if (img[row][col] < (max_value / 2))
            {
                img_thresh[row][col] = 0;
                //img_output[row][col] = 0;
            }
            else
            {
                img_thresh[row][col] = 1;
                //img_output[row][col] = 255;
            }
        }
    //提取roi，并对每一个roi进行sfr分析
    for (int row = 0; row < H - ROI_H; row ++)
    {
        for (int col = 0; col < W - ROI_W; col ++)
        {
            int up = row ;
            int down = row + ROI_H - 1;
            int left = col;
            int right = col + ROI_W - 1;
            //判断是否被极大值抑制
            if ((nms_map[up][left] != 0) || (nms_map[up][right] != 0))
                continue;
            //检测top_margin
            if ((up - TOP_MARGIN) < 0)
                continue;
            if (img_thresh[up][left] != img_thresh[up - TOP_MARGIN][left])
                continue;
            //检查边缘四个角的
            if ((img_thresh[up][left] != img_thresh[down][left]) ||
                (img_thresh[up][right] != img_thresh[down][right]) ||
                (img_thresh[up][left] == img_thresh[up][right]))
                continue;
            //检测边沿是否穿过图像中心
            int centor_row = (up + down) / 2;
            int centor_left = left + ROI_W / 2 - 1;
            int centor_right = centor_left + 1;
            if (img_thresh[centor_row][centor_left] == img_thresh[centor_row][centor_right])
                continue;
            //检查图像每一行是否只有一个边沿
            int is_continuous = 1;
            for (int i = up; i <= down; i++)
            {
                int edge_count = 0;
                for (int j = left; j <= right - 1; j++)
                    if (img_thresh[i][j] != img_thresh[i][j + 1])
                        edge_count += 1;
                if (edge_count != 1)
                    is_continuous = 0;
            }
            if (!is_continuous)
                continue;
            //建立极大值抑制
            for (int i = 0; i < ROI_H; i++)
                for (int j = 0; j < ROI_W; j++)
                    nms_map[up + i][left + j] = 255;
            //计算sfr
            for (int i = 0; i < ROI_H; i++)
                for (int j = 0; j < ROI_W; j++)
                    sfr_input[i * ROI_W + j] = img[up + i][left + j] / 255.0f;
            sfr_result_t sfr_result;
            sfr_result = caculate_sfr(ROI_W, ROI_H, sfr_input);
            //打印结果
            //if ((left != 208-1) || (right != 241-1) || (up != 280-1) || (down != 335-1))
			if ((left == 154-1) && (right == 187-1) && (up == 71-1) && (down != 126-1))
            {
                printf("%.3f %5d %5d %5d %5d %6.3f %6.4f %6.3f\n",
                        sfr_result.sfr[0], left+1, right+1, up+1, down+1, 
                        sfr_result.R2, sfr_result.mtf50, sfr_result.angle);
                for (int i = 0; i < ROI_W; i++)
                {
                    printf("%.3f %.6f\n", i / (float)ROI_W, sfr_result.sfr[i]);
                }
            }
            //结果保存至数组
            roi_results[roi_index].left = left;
            roi_results[roi_index].right = right;
            roi_results[roi_index].up = up;
            roi_results[roi_index].down = down;
            roi_results[roi_index].mtf50 = sfr_result.mtf50;
            roi_index += 1;
        }
    }
    //计算五个指定区域内的mtf50均值
    for (int k = 0; k < 5; k ++)
    {
        //计算指定区域的中心点
        int v_flip = k / 2;
        int h_flip = k % 2;
        int center_row, center_col = 0;
        int size = 0;
        float mtf50_throat = 0;
        int roi_n_throat = 0;
        float field_ratio = 0;
        if (k == 4)
        {
            field_ratio = 0;
            size = cfg.field_a_size;
            mtf50_throat = cfg.field_a_throat;
            roi_n_throat = cfg.field_a_count;
        }
        else
        {
            field_ratio = cfg.field_b_ratio;
            size = cfg.field_b_size;
            mtf50_throat = cfg.field_b_throat;
            roi_n_throat = cfg.field_b_count;
        }
        center_row = H / 2 + (v_flip * 2 - 1) * field_ratio * H / 2;
        center_col = W / 2 + (h_flip * 2 - 1) * field_ratio * W / 2;
        //计算制定区域的box边界
        int left = center_col - size / 2;
        int right = center_col + size / 2 - 1;
        int up = center_row - size / 2;
        int down = center_row + size / 2 - 1;
        draw_box(left, right, up, down, 255);
        //遍历roi区域，判断roi是否落入指定区域内
        float mtf50 = 0;
        int roi_n = 0;
        for (int i = 0; i < roi_index; i ++)
        {
            roi_result_t roi = roi_results[i];
            draw_box(roi.left, roi.right, roi.up, roi.down, 128);
            draw_number(roi.left + 2, roi.up + 2, (int)(roi.mtf50 * 100), 128);
            if (right > roi.left && left < roi.right && down > roi.up && up < roi.down)
            {
                mtf50 += roi.mtf50;
                roi_n += 1;
            }
        }
        //检测是否通过
        mtf50 = mtf50 / roi_n;
        draw_number(left + 2, up + 2, (int)(mtf50 * 100), 255);
        printf("field %.2f  n = %2d > %2d mtf50 = %.2f > %.2f\n",
                field_ratio, roi_n, roi_n_throat, mtf50, mtf50_throat);
        if ((mtf50 > mtf50_throat) && (roi_n > roi_n_throat))
            pass_n += 1;
    }
    if (pass_n == 5)
    {
        printf("clarity is GOOD!\n");
        return 1;
    }
    else
    {
        printf("clarity is BAD! %d \n", pass_n);
        return 0;
    }
}

int main(void)
{
    //读取配置文件
    cfg = get_config();
    //读取raw图
    FILE *img_input_f = fopen(cfg.input_img_path, "rb");
    fread(raw_data, sizeof(unsigned char), cfg.w * cfg.h, img_input_f);
    //根据旋转情况，将raw图写入数组
    for (int i = 0; i < cfg.h; i++)
        for (int j = 0; j < cfg.w; j++)
            if (cfg.rotate)
                img[i][j] = raw_data[j * cfg.h + i];
            else
                img[i][j] = raw_data[i * cfg.w + j];
    //进行清晰度检测
    int success = check_clarity();
    //输出检测结果图片
    static uint8_t img_output_data[MAX_H * MAX_W];
    for (int i = 0; i < cfg.h; i++)
        for (int j = 0; j < cfg.w; j++)
            img_output_data[i * cfg.w + j] = img_output[i][j];
    raw_to_bmp(cfg.output_img_path, cfg.w, cfg.h, img_output_data);

    printf("Done!");
}