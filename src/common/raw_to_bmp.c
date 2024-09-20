#include "raw_to_bmp.h"

// BMP文件头结构
#pragma pack(push, 1)
typedef struct {
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
} BITMAPFILEHEADER;

typedef struct {
    uint32_t biSize;
    int32_t  biWidth;
    int32_t  biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t  biXPelsPerMeter;
    int32_t  biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
} BITMAPINFOHEADER;
#pragma pack(pop)

void raw_to_bmp(const char *filename, int width, int height, const uint8_t *gray_data) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        perror("Failed to open file for writing");
        exit(EXIT_FAILURE);
    }

    // 计算每行的填充字节数（BMP要求每行的大小必须是4字节的倍数）
    int row_padded = (width + 3) & (~3);

    // 文件头
    BITMAPFILEHEADER file_header;
    file_header.bfType = 0x4D42; // "BM"
    file_header.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + row_padded * height + 256 * 4;
    file_header.bfReserved1 = 0;
    file_header.bfReserved2 = 0;
    file_header.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 256 * 4;

    // 信息头
    BITMAPINFOHEADER info_header;
    info_header.biSize = sizeof(BITMAPINFOHEADER);
    info_header.biWidth = width;
    info_header.biHeight = height; // 使用正值，确保数据从下到上存储
    info_header.biPlanes = 1;
    info_header.biBitCount = 8;
    info_header.biCompression = 0; // BI_RGB，表示不压缩
    info_header.biSizeImage = row_padded * height;
    info_header.biXPelsPerMeter = 0;
    info_header.biYPelsPerMeter = 0;
    info_header.biClrUsed = 256; // 256种颜色（灰度）
    info_header.biClrImportant = 256;

    // 写入文件头
    fwrite(&file_header, sizeof(BITMAPFILEHEADER), 1, fp);

    // 写入信息头
    fwrite(&info_header, sizeof(BITMAPINFOHEADER), 1, fp);

    // 写入调色板（灰度调色板）
    for (int i = 0; i < 256; i++) {
        uint8_t color[4] = {i, i, i, 0}; // 蓝，绿，红，保留
        fwrite(color, sizeof(uint8_t), 4, fp);
    }

    // 写入像素数据（从下到上）
    for (int y = height - 1; y >= 0; y--) {
        fwrite(&gray_data[y * width], sizeof(uint8_t), width, fp);
        // 添加填充字节
        uint8_t padding[3] = {0, 0, 0};
        fwrite(padding, sizeof(uint8_t), row_padded - width, fp);
    }

    fclose(fp);
}
