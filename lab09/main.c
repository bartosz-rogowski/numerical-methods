#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "/opt/NR/numerical_recipes.c/nrutil.h"
#include "/opt/NR/numerical_recipes.c/nrutil.c"
#include "/opt/NR/numerical_recipes.c/gaussj.c"

#define frand() ( (double)rand() ) / (RAND_MAX+1.0)
#define m 50
#define N 201


const float x_min = -4.;
const float x_max = 4.;
const float sigma = (x_max - x_min) / 16.;
const float x0 = 2.;


//funkcja
double f(float x)
{
	double res = exp( -pow(x-x0, 2) / ( 2*pow(sigma, 2) ) ) + exp( -pow(x+x0, 2) / ( 2*pow(sigma, 2) ) );
	res *= sin( 14*M_PI*x/(x_max-x_min) );
	return res;
}


//generuje szum
float szum()
{
	return (frand() - 0.5)/5.;
}

//nadpisuje phi wartosciami
void generate_phi(double* x, double* y, double** p)
{
	for(int i = 0; i < N; i++)
	{
		p[0][i] = 1.0;
	}
	for(int j = 1; j < m+1; j++)
	{
		double la, ma, lb, mb;
		la = ma = lb = mb = 0.0;
		for(int a = 0; a < N; a++)
		{
			la += (x[a] * pow(p[j-1][a], 2));
			ma += pow(p[j-1][a], 2);
			if(j>1)
			{
				lb += x[a]*p[j-2][a]*p[j-1][a];
				mb += pow(p[j-2][a], 2);
			}
		}
		// printf("%g\t%g\t%g\t%g\n", la, ma, lb, mb);
		double alfa, beta;
		if( j!=1 )
			beta = lb/mb;
		else
			beta = 0.0;
		alfa = la/ma;
		for(int k = 0; k < N; k++)
		{
			if(j==1)
				p[j][k] = (x[k] - alfa)*p[j-1][k];
			else
				p[j][k] = (x[k] - alfa)*p[j-1][k] - beta*p[j-2][k];
		}
	}
}


/////////////////////////////////////////////////////////////////////////

int main()
{
	//------------------------------------ alokacja pamieci
	double* xw = malloc(N*sizeof(double));
	double* yw = malloc(N*sizeof(double));
	double** phi = malloc((m+1)*sizeof(double));

	FILE* fp = fopen("pkt.dat", "w+");
	for(int i = 0; i < N; i++)
	{
		xw[i] = x_min + i*(x_max-x_min)/(N-1);
		yw[i] = f(xw[i]) + szum(); //do sprawozdania bez szumu (dla porownania)
		fprintf(fp, "%g   %g\n", xw[i], yw[i]);
	}
	fclose(fp);

	for(int i = 0; i < m+1; i++)
	{
		phi[i] = malloc(N*sizeof(double));
	}

	generate_phi(xw, yw, phi);

	FILE* fp1 = fopen("Gram.dat", "w+");

	for(int i = 0; i < N; i++)
	{
		fprintf(fp1, "%g\t", xw[i]);
		for(int j = 0; j < 7; j++)
		{
			fprintf(fp1, "%g\t", phi[j][i]/phi[j][0]);
		}
		fprintf(fp1, "\n");
	}
	fclose(fp1);

	double* s = malloc((m+1)*sizeof(double));
	double* c = malloc((m+1)*sizeof(double));
	
	for(int j = 0; j < m+1; j++)
	{
		s[j] = c[j] = 0.;
		for(int i = 0; i < N; i++)
		{
			s[j] += yw[i] * phi[j][i];
			c[j] += pow(phi[j][i], 2);
		}
	}

	FILE* fp2 = fopen("approx.dat", "w+");

	for(int a = 0; a < 3; a++)
	{
		int mm = 10 + 20*a; //dla m = 10, 30, 50
		double* F = malloc(N*sizeof(double));
		for(int k = 0; k < N; k++)
		{
			F[k] = 0.;
			for(int i = 0; i < mm; i++)
			{
				F[k] += s[i]/c[i] * phi[i][k];
			} 
			fprintf(fp2, "%g   %g\n", xw[k], F[k]);
		}
		fprintf(fp2, "\n\n");
		free(F);
	}
	fclose(fp2);

	//------------------------------------ dealokacja pamieci
	for (int i = 0; i < m+1; i++)
	{
		free(phi[i]);
	}
	free(s);
	free(c);
	free(phi);
	free(yw);
	free(xw);

	return 0;
}