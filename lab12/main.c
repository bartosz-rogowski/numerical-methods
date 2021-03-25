#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define n 8

const float a = 0.;
const float b = 1.;
const double wynik = -0.186486896;

//implementacja funkcji
double f(double x)
{
	return log(pow(x, 3) + 3*pow(x, 2) + x + 0.1) * sin(18*x);
}

///wylicza wartosc w zaleznosci od liczby podprzedzialow i ich szerokosci
double Simpson(int sn, double w)
{
	double result = 0.;
	for(unsigned int i = 0; i <= sn/2 - 1; i++)
	{
		result += ( f(a+(2*i)*w) + 4*f(a+(2*i+1)*w) + f(a+(2*i+2)*w) ) / 3.;
	}
	return result*w;
}

double Milne(int sn, double w)
{
	double result = 0.;
	for(unsigned int i = 0; i <= sn/4 - 1; i++)
	{
		result += ( 7*f(a+(4*i)*w) + 32*f(a+(4*i+1)*w) + 12*f(a+(4*i+2)*w) );
		result += ( 32*f(a+(4*i+3)*w) + 7*f(a+(4*i+4)*w) );
	}
	return result*4*w/90;
}

//zapisuje tablice do pliku
void save2file(FILE** fp, double** D)
{
	for(int i = 0; i <= n; i++)
	{
		for(int j = 0; j <= n; j++)
		{
			fprintf(*fp, "%20.10g  ", D[i][j]);
		}
		fprintf(*fp, "\n");
	}
}

//wyswietla D[w][0] i D[w][w] oraz ich roznice
void compare(double** D)
{
	printf("w\t\t   D[w][0]\t\t  D[w][w]\t\tmodul roznicy\n");
	for(int w = 0; w <= n; w++)
	{
		printf("-----------------------------------------------------------------------------\n");
		printf("%d\t%20.10g\t%20.10g\t%20.10g\t%20.10g\n", w, D[w][0], D[w][w], fabs(wynik-D[w][0]), fabs(wynik-D[w][w]) );
	}
	printf("-----------------------------------------------------------------------------\n");

}

////////////////////////////////////////////////////////////////

int main(){

	//------------------------- alokacja pamieci
	double** D = calloc(n+1, sizeof(double));

	for (unsigned int w = 0; w <= n; w++)
	{
		D[w] = calloc(n+1, sizeof(double));

		//liczba podprzedzialow
		unsigned N = pow(2, w+1);

		//szerokosc podprzedzialu
		double h = (b-a)/N;

		D[w][0] = Simpson(N, h);

		if(w > 0)		
		for (unsigned int k = 1; k <= w; k++)
		{
			D[w][k] = (pow(4, k) * D[w][k-1] - D[w-1][k-1]) / (pow(4, k) - 1);
		}
	}

	FILE* fp1 = fopen("Simpson.dat", "w+");
	save2file(&fp1, D);
	fclose(fp1);
	printf("============================== Metoda Simpsona ==============================\n");
	compare(D);

	//-------------------- wyzerowanie D
	for(int i = 0; i <= n; i++)
		for(int j = 0; j <= n; j++)
			D[i][j] = 0.;

	for (unsigned int w = 0; w <= n; w++)
	{
		//liczba podprzedzialow
		unsigned N = pow(2, w+2);

		//szerokosc podprzedzialu
		double h = (b-a)/N;

		D[w][0] = Milne(N, h);

		if(w > 0)
		for (unsigned int k = 1; k <= w; k++)
		{
			D[w][k] = (pow(4, k) * D[w][k-1] - D[w-1][k-1]) / (pow(4, k) - 1);
		}
	}
	FILE* fp2 = fopen("Milne.dat", "w+");
	save2file(&fp2, D);
	fclose(fp2);
	printf("\n\n\n=============================== Metoda Milne'a ==============================\n");
	compare(D);

	//------------------------ dealokacja pamieci
	for(int i = 0; i <= n; i++)
		free(D[i]);
	free(D);

	return 0;
}