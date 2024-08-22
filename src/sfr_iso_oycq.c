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

sfr_result_t caculate_sfr(int width, int height, double* img)
{
	static double* Freq;
	static double* sfr;
	int len, center;
	double slope, off, R2;
	double numcycles = 0;
	int version = 0;
	int g_userangle = 0;
	sfrProc(&Freq, &sfr, &len, img, (unsigned short)width, &height, 
	        &slope, &numcycles, &center, &off, &R2, version, 0, g_userangle);
	
	len = len / 2;
	for (int i = 1; i < len; i++)
	{
		double freq = i / (float)len  * 3.1415926;
		freq = freq / 4.0; //(g_version & 4) is false
		sfr[i] = sfr[i] * freq / sin(freq);
	}
	sfr_result_t sfr_result;
	sfr_result.sfr = sfr;
	sfr_result.R2 = R2;
	sfr_result.angle = atan(slope)* 180.0 / 3.1415926;
	sfr_result.mtf50 = caculate_mtf50(sfr, len);
	return sfr_result;
}
