typedef struct
{
	int good;
	double * sfr;
	double R2;
	double value;
	double angle;
}sfr_result_t;

sfr_result_t caculate_sfr(int width, int height, double* img, const char* method);