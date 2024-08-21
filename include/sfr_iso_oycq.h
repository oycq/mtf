typedef struct
{
	double * sfr;
	double R2;
	double mtf50;
}sfr_result_t;

sfr_result_t caculate_sfr(int width, int height, double* img);