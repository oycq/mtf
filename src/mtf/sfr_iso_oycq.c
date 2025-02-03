#include <math.h>
#include "sfr_iso_oycq.h"

int sfrProc(double **,double **, int*, double *, 
	    unsigned short, int *, double *, int *, int *, double*, double*, 
	    int, int, int);

double caculate_mtf50(double* sfr, int len)
{
	double mtf50 = 0;
	for (int i = 0; i < len - 1; i++)
	{
		if ((sfr[i] >= 0.5) && (sfr[i+1] <= 0.5))
		{
			mtf50 = i + (sfr[i] - 0.5) / (sfr[i] - sfr[i+1]);
			mtf50 = mtf50 / len;
			break;
		}
	}
	return mtf50;
}

sfr_result_t caculate_sfr(int width, int height, double* img, const char* method)
{
	static double* Freq;
	static double* sfr;
	int len, center;
	double slope, off, R2;
	double numcycles = 0;
	int version = 0;
	int g_userangle = 0;

	sfr_result_t sfr_result;
	sfr_result.good = 0;
	sfr_result.sfr = 0;
	sfr_result.R2 = 0;
	sfr_result.angle = 0;
	sfr_result.value = 0;

	short result = sfrProc(&Freq, &sfr, &len, img, (unsigned short)width, &height,
	        &slope, &numcycles, &center, &off, &R2, version, 0, g_userangle);

	if (result != 0)
	{
		printf("sfrProc error\n");
		return sfr_result;
	}
	len = len / 2;
	for (int i = 1; i < len; i++)
	{
		double freq = i / (float)len  * 3.1415926;   
		freq = freq / 4.0; //(g_version & 4) is false
		sfr[i] = sfr[i] * freq / sin(freq);
	}
	double mtf50 = caculate_mtf50(sfr, len);
	double freq12_5  = sfr[(int)(len * 0.125)];
	double freq25 = sfr[(int)(len * 0.25)];
	double freq33 = sfr[(int)(len * 0.33)];
	double freq50 = sfr[(int)(len * 0.50)];

	sfr_result.good = 1;
	sfr_result.sfr = sfr;
	sfr_result.R2 = R2;
	sfr_result.angle = atan(slope)* 180.0 / 3.1415926;
	if (strcmp(method, "mtf50") == 0)
		sfr_result.value = mtf50;
	if (strcmp(method, "freq12.5") == 0)
		sfr_result.value = freq12_5;
	if (strcmp(method, "freq25") == 0)
		sfr_result.value = freq25;	
	if (strcmp(method, "freq33") == 0)
		sfr_result.value = freq33;
	if (strcmp(method, "freq50") == 0)
		sfr_result.value = freq50;
	return sfr_result;
}
