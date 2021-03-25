#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "/opt/NR/numerical_recipes.c/nrutil.h"
#include "/opt/NR/numerical_recipes.c/nrutil.c"
#include "/opt/NR/numerical_recipes.c/tred2.c"
#include "/opt/NR/numerical_recipes.c/tqli.c"
#include "/opt/NR/numerical_recipes.c/pythag.c"

#define n 20

//funkcja do mnozenia macierzy z wektorem, nadpisuje rezultat
void multiply(float* result, float** M, float* vec)
{
	for(int i = 1; i <= n; i++)
	{
		float temp = 0.;
		for(int j = 1; j <= n; j++)
		{
			temp += M[i][j]*vec[j];
		}
		result[i] = temp;
	}
}

//funkcja do mnozenia wektora transponowanego z wektorem, zwraca liczbe rzeczywista
float scalar(float* a, float* b)
{
	float result = 0.;
	for (int i = 1; i <= n; i++)
	{
		result += a[i]*b[i];
	}
	return result;
}

//redukcja hotellinga - nadpisuje macierz M
void Hotelling(float** M, float y, float* vec)
{

	for(int i = 1; i <= n; i++)
	{
		for(int j = 1; j <= n; j++)
		{
			M[i][j] -= y*vec[i]*vec[j];
		}
	}
}

//pomocnicza funkcja do podejrzenia zawartosci macierzy
void show(float** M)
{
	for(int i = 1; i <= n; i++)
	{
		for(int j = 1; j <= n; j++)
		{
			printf("%12.6g", M[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}


//////////////////////////////////////////////////////////////////////////////////

int main(){
//----------------------- alokacja pamieci
float **A = matrix(1, n, 1, n);
float **W = matrix(1, n, 1, n);
float *d = vector(1, n);
float *e = vector(1, n);
float *xn = vector(1, n);
float *xs = vector(1, n);

//----------------------- nadanie wartosci
for (int i = 1; i <= n; i++)
{
	for (int j = 1; j <= n; j++)
	{
		A[i][j] = W[i][j] = sqrt(i+j)/1000;
	}
}

tred2(A, n, d, e);
tqli(d, e, n, A); //sprytne wywolanie

//----------------------- metoda potegowa
printf("\t\tlambdy wyliczone:\n");
printf("nr |  z Hotellinga  |\t  z tgli      |   abs(roznica)\t| blad wzgledny\n");
printf("-----------------------------------------------------------------------\n");
for (int k = 1; k <= n; k++)
{
	for (int j = 1; j <= n; j++)
	{
		xs[j] = 1.;
	}
	float lambda; //wartosc wlasna
	for (int i = 1; i <= n+1; i++)
	{
		multiply(xn, W, xs);
		lambda = scalar(xn, xs) / scalar(xs, xs);

		//normowanie wektora xn
		float norm = sqrt( scalar(xn, xn) );
		for (int a = 1; a <= n; a++)
		{
			xn[a] = xn[a] / norm;
		}

		//przepisane wektora
		for (int a = 1; a <= n; a++)
		{
			xs[a] = xn[a];
		}
	}
	Hotelling(W, lambda, xs);
	printf("%2d.|  %12.6g  |   %12.6g  |   %12.6g\t| %12.6g%\n", k, lambda, d[n-k+1], fabs(lambda-d[n-k+1]), fabs(100*fabs(lambda-d[n-k+1])/d[n-k+1]) );
}


//----------------------- dealokacja pamieci
free_matrix(A, 1, n, 1, n);
free_matrix(W, 1, n, 1, n);
free_vector(d, 1, n);
free_vector(e, 1, n);
free_vector(xn, 1, n);
free_vector(xs, 1, n);
	return 0;
}