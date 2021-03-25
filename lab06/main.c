#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define it_MAX 30

double licz_r(double* A, double* B, int dim, double x)
{
	B[dim] = 0.;
	for (int k = dim - 1; k >= 0; k--)
	{
		B[k] = A[k+1] + x * B[k+1];
	}
	return A[0] + x * B[0];
}

int main(){

//-------------------------------- alokacja pamieci
int N, n;
N = n = 5;
double x0, x1, Rj, Rj1;
double *a = malloc( (N+1)*sizeof(double) );
double *b = malloc( (N+1)*sizeof(double) );
double *c = malloc( N*sizeof(double) );
a[5] = 1.;
a[4] = 14.;
a[3] = 33.;
a[2] = -92.;
a[1] = -196.;
a[0] = 240.;

//-------------------------------- metoda stycznych z iteracyjnym dzieleniem wielomianu przez dwumian

FILE* fp;
fp = fopen("rez.txt", "w+");

for (int L = 1; L <= N; L++)
{
	n = N - L + 1;
	x0 = 0.;
	fprintf(fp, "\n| L |  iteracja  | przyblizenie x | \t  Rj  \t   | \t   Rj' \t    |\n");
	fprintf(fp, "---------------------------------------------------------------------\n");

	for (int it = 1; it < it_MAX; it++)
	{
		Rj = licz_r(a, b, n, x0);
		Rj1 = licz_r(b, c, n - 1, x0);
		x1 = x0 - Rj/Rj1;

		//zapis do pliku
		fprintf(fp, "| %d |      %d     | %12.6g   | %12.6g   |%12.6g    |\n", L, it, x1, Rj, Rj1);
		
		//warunek wczesniejszego wyjscia z petli
		if ( fabs(x1 - x0) < 1e-7 )
			break;

		x0 = x1;
	}

	//redukcja stopnia wielomianu
	for (int i = 0; i < n; i++)
	{
		a[i] = b[i];
	}
	fprintf(fp, "_____________________________________________________________________\n\n\n");
}

fclose(fp);

//-------------------------------- dealokacja pamieci

free(a);
free(b);
free(c);

	return 0;
}