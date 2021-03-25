#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "/opt/NR/numerical_recipes.c/nrutil.h"
#include "/opt/NR/numerical_recipes.c/nrutil.c"
#include "/opt/NR/numerical_recipes.c/gaussj.c"

float x_min = -5.;
float x_max = 5.;

//implementacja funkcji 
double f(unsigned int which, double x) 
{
	//w zaleznosci od pierwszego argumentu, wybierana jest odpowiednia funkcja
	if(which == 1)
		return 1./(x*x + 1.);
	if(which == 2)
		return cos(2.*x);
	return 0;
}

//funkcja liczaca wartosci drugich pochodnych
void wyzM(float h, float* yw, float** w, int n, float alfa, float beta)
{
	//float lambda = h/(h+h) = 0.5  |  1-lambda = 0.5
	float** A = matrix(1, n, 1, n);
	//naszym d jest w (zostanie nadpisane)
	for(int i = 1; i <= n; i++)
		for(int j = 1; j <= n; j++)
			A[i][j] = 0.; //wyzerowanie wszystkich elementow
	for(int i = 1; i <= n; i++)
	{
		if(i == 1 || i == n)
		{	
			A[i][i] = 1.;
			if(i == 1)
				w[i][1] = alfa;
			else
				w[i][1] = beta;
		}
		else
		{
			w[i][1] = 3/h * ( (yw[i+1] - yw[i])/h - (yw[i] - yw[i-1])/h);
			A[i][i] = 2.;
			A[i][i-1] = A[i][i+1] = 0.5; //pod i nadprzekatna
		}
	}
	
	//rozwianie ukladu Am = d 
	gaussj(A, n, w, 1);
	//"macierz" w zostanie nadpisana
	free_matrix(A, 1, n, 1, n);
}

//funkcja wyliczajaca wartosc funkcji w polozeniu miedzywezlowym
float wyzSx(float h, float* xw, float* yw, float** w, int n, float xx)
{
	int i;
	//szukanie przedzialu
	for(i = 2; i < n; i++)
	{
		if(xw[i-1] <= xx && xx <= xw[i])
			break;
	}
	i--; //przesuniecie wskaznika, bo liczymy s_(i-1)

	float A = (yw[i+1] - yw[i])/h - h * ( w[i+1][1] - w[i][1] )/6;
	float B = yw[i] - w[i][1]*h*h/6;

	float s = w[i][1]*pow( xw[i+1]-xx , 3)/(6.*h) + w[i+1][1]*pow( xx-xw[i] , 3)/(6*h);
	s += A*(xx - xw[i]) + B;
	return s;
}

//funkcja wyliczajaca druga pochodna dla zadania 4 
float wyl2d(float xw)
{
	float delta = 0.01;
	return ( f(1, xw - delta) - 2*f(1, xw) + f(1, xw + delta) )/pow(delta, 2);
}


///////////////////////////////////////////////////////////////////////////////////////////


int main(){

int dim[] = {5, 8, 21, 10}; //tablica wartosci n
int length[] = {4, 3}; //dlugosc przeszukiwania powyzszej tablicy
//n=10 jest spesjalna wartoscia do zrobienia zadania 4

FILE* fp1 = fopen("f1.dat", "w+");
FILE* fp2 = fopen("f2.dat", "w+");
FILE* fp3 = fopen("pochodne.dat", "w+");

for(unsigned int outside = 1; outside <= 2; outside++)
{
	for(unsigned int inner = 1; inner <= length[outside-1]; inner++)
	{
		//---------------------------- alokacja pamieci
		int n = dim[inner-1];
		float h = (x_max - x_min)/(n-1);
		float* xm = vector(1, n);
		float* ym = vector(1, n);
		float** m = matrix(1, n, 1, 1);

		for(unsigned int i = 1; i <= n; i++)
		{
			xm[i] = x_min + (i-1)*h;
			ym[i] = f(outside, xm[i]);
		}

		wyzM(h, ym, m, n, 0, 0);

		//---------------------------- zapis danych
		float krok = 0.01;
		if(outside == 1 && n != 10)
		{
			for( float x = x_min; x < x_max+krok; x+=krok)
			{
				fprintf(fp1, "%12.5g\t%12.5g\n", x, wyzSx(h, xm, ym, m, n, x));
			}
			fprintf(fp1, "\n\n");
		}
		if(outside == 1 && n == 10)
		{
			for(int i = 1; i <= n; i++)
			{
				fprintf(fp3, "%12.5g\t%12.5g\t%12.5g\n", xm[i], m[i][1], wyl2d(xm[i]) );
			}
			fclose(fp3);
		}
		if(outside == 2)
		{	
			for(float x = x_min; x < x_max+krok; x+=krok)
			{
				fprintf(fp2, "%12.5g\t%12.5g\n", x, wyzSx(h, xm, ym, m, n, x));
			}
			fprintf(fp2, "\n\n");
		}

		//----------------------------- dealokacja pamieci
		free_vector(xm, 1, n);
		free_vector(ym, 1, n);
		free_matrix(m, 1, n, 1, 1);
	}
}

fclose(fp1);
fclose(fp2);

	return 0;
}