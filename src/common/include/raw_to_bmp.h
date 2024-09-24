#ifndef RAW_TO_BMP_H
#define RAW_TO_BMP_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// 将RAW灰度图像数据转换为BMP文件
void raw_to_bmp(const char *filename, int width, int height, const uint8_t *gray_data);

#endif
