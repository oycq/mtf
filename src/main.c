#include <stdio.h>
#include <stdint.h>
#include "sfr_iso_oycq.h"

#pragma pack(push, 1)
uint8_t img_data[400][400];
double img[200][100];
#pragma pack(pop) 

int main()
{ 
	FILE *img_input = fopen("edge.raw", "rb");
	fread(img_data, 1, 400*400, img_input);
	for (int i=0; i < 200; i++)
		for (int j=0; j < 100; j++)
		{
			img[i][j] = img_data[i+100][j+130] / 255.0f;
		}
	int width = 100;
	int height = 200;
	double * sfr = caculate_sfr(width, height, img);
    if (sfr == NULL) {
        fprintf(stderr, "Error: sfr is a null pointer\n");
        return 1;
    }
    printf("aaaa\n");
	for (int i=0; i < width; i++)
	{
		printf("%d %.2f\n", i, sfr[i]);
	}
}
