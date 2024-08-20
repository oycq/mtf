int sfrProc(double **,double **, int*, double *, 
	    unsigned short, int *, double *, int *, int *, double*, double*, 
	    int, int, int);

double* caculate_sfr(int width, int height, float* img)
{
	static double* Freq;
	static double* sfr;
	int bin_len, center;
	double slope, off, R2;
	double numcycles = 0;
	int version = 1;
	int g_userangle = 0;
	
	sfrProc(&Freq, &sfr, &bin_len, img, (unsigned short)width, &height, 
	        &slope, &numcycles, &center, &off, &R2, version, 0, g_userangle);
	return sfr;
}
