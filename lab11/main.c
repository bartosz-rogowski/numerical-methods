#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <gsl/gsl_linalg.h>
#include <gsl/gsl_fft_complex.h>
#include <gsl/gsl_errno.h>


#define T 1.0

const double sigma = T/20.;

double function(double t)
{
	double arg = 2 * M_PI / T * t;
	return ( sin(arg) + sin(2*arg) + sin(3*arg) );
}

// losuje liczbe z zakresu -0.5 do 0.5
double U_rand()
{
	double r = (double)rand() / (RAND_MAX +1.);
	return (r - 0.5);
}

//funkcja do podejrzenia wartosci tablicy
void show(double* array, unsigned int dim)
{
	for(unsigned int a = 0; a < dim; a++)
		printf("%g\n", array[a]);
	printf("\n\n");
}

//***************************************************************************************

int main()
{
	FILE* fp1 = fopen("k8.dat", "w+");
	FILE* fp2 = fopen("k10.dat", "w+");
	FILE* fp3 = fopen("k12.dat", "w+");

	const double t_max = 3*T;
	int k[] = {8, 10, 12};
	
	for(unsigned int kk = 0; kk < 3; kk++)
	{
		unsigned int N = pow(2, k[kk]);
		const double dt = t_max/N;

		//---------------------------- alokacja pamieci
		double* f = malloc(2*N*sizeof(double));
		double* g1 = malloc(2*N*sizeof(double));
		double* g2 = malloc(2*N*sizeof(double));

		//---------------------------- wypelnienie tablic
		for(unsigned int i = 0; i < N; i++)
		{
			f[2*i] = function(dt*i) + U_rand();
			switch(kk)
			{
				case 0:
					fprintf(fp1, "%g  %g\n", dt*i, f[2*i]);
				break;

				case 1:
					fprintf(fp2, "%g  %g\n", dt*i, f[2*i]);
				break;

				case 2:
					fprintf(fp3, "%g  %g\n", dt*i, f[2*i]);
				break;
			}
			g1[2*i] = g2[2*i] = exp(-pow(dt*i, 2) / ( 2*pow(sigma, 2) ) ) / (sigma*sqrt(2*M_PI));
			f[2*i+1] = g1[2*i+1] = g2[2*i+1] = 0.;
		}

		gsl_fft_complex_radix2_forward(f, 1, N);
		gsl_fft_complex_radix2_forward(g1, 1, N);
		gsl_fft_complex_radix2_backward(g2, 1, N);

		switch(kk)
		{
			case 0:
				fprintf(fp1, "\n\n");
			break;

			case 1:
				fprintf(fp2, "\n\n");
			break;

			case 2:
				fprintf(fp3, "\n\n");
			break;
		}

		//------------------------- nadpisanie f[] transformata splotu f*g
		for(unsigned int i = 0; i < N; i++)
		{
			double a1, a2, b1, b2;
			a1 = f[2*i];
			b1 = f[2*i+1];
			a2 = g1[2*i] + g2[2*i];
			b2 = g1[2*i+1] + g2[2*i+1];
			f[2*i] = a1*a2 - b1*b2; //Re
			f[2*i+1] = a1*b2 + a2*b1; //Im
		}


		gsl_fft_complex_radix2_backward(f, 1, N);


		//------------------------------ znalezienie max. modulu f[] (tylko Re)
		double f_max = fabs(f[0]);
		for(unsigned int i = 1; i < N; i++)
		{
			if(fabs(f[2*i]) - f_max > 0)
				f_max = fabs(f[2*i]);
		}
		printf("dla k = %2d  f_max = %g\n", k[kk], f_max);


		//--------------------------- zapis danych po odszumieniu
		for(unsigned int i = 0; i < N; i++)
		{
			switch(kk)
			{
				case 0:
					fprintf(fp1, "%g  %g\n", dt*i, f[2*i] * 2.5 / f_max);
				break;

				case 1:
					fprintf(fp2, "%g  %g\n", dt*i, f[2*i] * 2.5 / f_max);
				break;

				case 2:
					fprintf(fp3, "%g  %g\n", dt*i, f[2*i] * 2.5 / f_max);
				break;
			}
		}

		//---------------------------------- dealokacja pamieci
		free(f);
		free(g1);
		free(g2);
	}

	fclose(fp1);
	fclose(fp2);
	fclose(fp3);

	return 0;
}