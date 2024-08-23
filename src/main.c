#include <stdio.h>
#include <stdint.h>
#include "sfr_iso_oycq.h"
#include "get_config.h"

#define MAX_W 4096
#define MAX_H 4096
#define MAX_ROI_W 512
#define MAX_ROI_H 512

config_t cfg;

#pragma pack(push, 1)
uint8_t img[MAX_H][MAX_W];
uint8_t img_output[MAX_H][MAX_W];
uint8_t img_thresh[MAX_H][MAX_W];
uint8_t nms_map[MAX_H][MAX_W];
double sfr_input[MAX_ROI_H * MAX_ROI_W];
#pragma pack(pop) 

void find_roi(void)
{
    int H = cfg.h;
    int W = cfg.w;
    int ROI_W = cfg.roi_w;
    int ROI_H = cfg.roi_h;
    int TOP_MARGIN = cfg.top_margin;
    int THRESH = cfg.thresh;
    //nms_map初始化赋值, 阈值化图像
    for (int row = 0; row < H; row ++)
        for (int col = 0; col < W; col ++)
        {
            nms_map[row][col] = 0;
            img_thresh[row][col] = 0;
            if (img[row][col] > THRESH)
                img_thresh[row][col] = 1;
        }
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
            for (int i = up; i <= down; i++)
            {
                int edge_count = 0;
                for (int j = left; j <= right - 1; j++)
                    if (img_thresh[i][j] != img_thresh[i][j + 1])
                        edge_count += 1;
                if (edge_count != 1)
                    continue;
            }
            //建立极大值抑制
            for (int i = 0; i < ROI_H; i++)
                for (int j = 0; j < ROI_W; j++)
                    nms_map[up + i][left + j] = 255;
            //绘制
            for (int i = up; i < down; i++)
                for (int j = left; j < right; j++)
                    if ((i == up) || (i == down -1) || (j == left) || (j == right-1))
                    {
                        if (img[i][j] < 128)
                            img_output[i][j] = 128;
                        else
                            img_output[i][j] = 128;
                    }
            //计算sfr
            for (int i = 0; i < ROI_H; i++)
                for (int j = 0; j < ROI_W; j++)
                    sfr_input[i * ROI_W + j] = img[up + i][left + j] / 255.0f;
            if ((left != 208-1) || (right != 241-1) || (up != 280-1) || (down != 335-1))
			//if ((left != 154-1) || (right != 187-1) || (up != 71-1) || (down != 126-1))
                continue;
            sfr_result_t sfr_result;
            sfr_result = caculate_sfr(ROI_W, ROI_H, sfr_input);
            printf("%.3f %5d %5d %5d %5d %6.3f %6.4f %6.3f\n", 
					sfr_result.sfr[0], left+1, right+1, up+1, down+1, 
					sfr_result.R2, sfr_result.mtf50, sfr_result.angle);
            for (int i = 0; i < ROI_W; i++)
            {
                printf("%.3f %.6f\n", i / (float)ROI_W, sfr_result.sfr[i]);
            }
        }
    }
}

int main(void)
{
    cfg = get_config();

    FILE *img_input_f = fopen(cfg.input_img_path, "rb");
    for (int i = 0; i < cfg.h; i++)
        fread(img[i], sizeof(unsigned char), cfg.w, img_input_f);
    fclose(img_input_f);

    memcpy(img_output, img, sizeof(img));

    find_roi();
    FILE *img_output_f = fopen(cfg.output_img_path, "wb");
    for (int i = 0; i < cfg.h; i++)
        fwrite(img_output[i], sizeof(unsigned char), cfg.w, img_output_f);
    fclose(img_output_f);

    printf("Done!");
}