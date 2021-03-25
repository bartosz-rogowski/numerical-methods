#include <stdio.h>
#include <stdlib.h>
#include <math.h>

float xmin = -5.;
float xmax = 5.;

// implementacja funkcji 1/(x^2 + 1)
double f(double x) { return 1./(x*x+1.); }

//wartosc wielomianu Czebyszewa
double Wn(double x, int n, double* xm, double** fm)
{
	double result = 0.;
	for (int j = 0; j <= n; j++)
	{
		double temp = 1.;
		for (int i = 0; i < j; i++)
		{
			temp *= (x - xm[i]);
		}
			result += (temp * fm[j][j]);
	}
	return result;
}

int main(){
//////////////////////////////// WEZLY ROWNOLEGLE ///////////////////////////
FILE* fp1 = fopen("zad_1.dat", "w+");
for (int it = 1; it <= 4; it++)
{
	int n = 5*it;
	double* xm = malloc((n+1) * sizeof(double));
	double* ym = malloc((n+1) * sizeof(double));
	double** fm = malloc((n+1) * sizeof(double));
	//---------------------- alokacja pamieci
	for (int i = 0; i < n+1; i++)
	{
		xm[i] = xmin + i*(xmax - xmin)/n;
		ym[i] = f(xm[i]);
		fm[i] = malloc((n+1) * sizeof(double));
		fm[i][0] = ym[i];
	}

	for (int j = 1; j <= n; j++)
	{
		for (int i = j; i <= n; i++)
		{
			fm[i][j] = (fm[i][j-1] - fm[i-1][j-1])/(xm[i] - xm[i-j]);
		}
	}

	for( float xx = xmin; xx <= xmax+0.001; xx+=0.01)
	{
		fprintf(fp1, "%12.5g  %12.5g\n", xx, Wn(xx, n, xm, fm));
	}

	//---------------------- dealkoacja pamieci
	for (int i = 0; i < n+1; i++)
	{
		free(fm[i]);
	}
	free(fm);
	free(ym);
	free(xm);
	fprintf(fp1, "\n\n");
}
fclose(fp1);

/////////////////////////// CZEBYSZEW //////////////////////////////

FILE* fp2 = fopen("zad_2.dat", "w+");
for (int it = 1; it <= 4; it++)
{
	int n = 5*it;
	double* xm = malloc((n+1) * sizeof(double));
	double* ym = malloc((n+1) * sizeof(double));
	double** fm = malloc((n+1) * sizeof(double));
	//---------------------- alokacja pamieci
	for (int i = 0; i < n+1; i++)
	{
		xm[i] = ( (xmax - xmin) * cos( M_PI*(2.*i+1.)/(2.*n+2.) ) + (xmin + xmax) )/2.;
		ym[i] = f(xm[i]);
		fm[i] = malloc((n+1) * sizeof(double));
		fm[i][0] = ym[i];
	}

	for (int j = 1; j <= n; j++)
	{
		for (int i = j; i <= n; i++)
		{
			fm[i][j] = (fm[i][j-1] - fm[i-1][j-1])/(xm[i] - xm[i-j]);
		}
	}

	for( float xx = xmin; xx <= xmax; xx+=0.01)
	{
		fprintf(fp2, "%12.5g  %12.5g\n", xx, Wn(xx, n, xm, fm));
	}
	
	//---------------------- dealkoacja pamieci
	for (int i = 0; i < n+1; i++)
	{
		free(fm[i]);
	}
	free(fm);
	free(ym);
	free(xm);
	fprintf(fp2, "\n\n");
}
fclose(fp2);

	return 0;
}