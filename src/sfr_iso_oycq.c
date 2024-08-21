int sfrProc(double **,double **, int*, double *, 
	    unsigned short, int *, double *, int *, int *, double*, double*, 
	    int, int, int);

typedef struct
{
	double * sfr;
	double R2;
	double mtf50;
}sfr_result_t;

double caculate_mtf50(double* sfr, int len)
{
	double mtf50 = 0;
	for (int i = 0; i < len - 1; i++)
	{
		if ((sfr[i] >= 0.5) && (sfr[i+1] <= 0.5))
		{
			mtf50 = i + (sfr[i] - 0.5) / (sfr[i] - sfr[i+1]);
			mtf50 = mtf50 / (len / 2.0f);
			break;
		}
	}
	return mtf50;
}

sfr_result_t caculate_sfr(int width, int height, double* img)
{
	static double* Freq;
	static double* sfr;
	int bin_len, center;
	double slope, off, R2;
	double numcycles = 0;
	int version = 0;
	int g_userangle = 0;
	
	sfrProc(&Freq, &sfr, &bin_len, img, (unsigned short)width, &height, 
	        &slope, &numcycles, &center, &off, &R2, version, 0, g_userangle);
	sfr_result_t sfr_result;
	sfr_result.sfr = sfr;
	sfr_result.R2 = R2;
	sfr_result.mtf50 = caculate_mtf50(sfr, bin_len);
	return sfr_result;
}
