#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#include "/opt/NR/numerical_recipes.c/nrutil.h"
#include "/opt/NR/numerical_recipes.c/nrutil.c"
#include "/opt/NR/numerical_recipes.c/gaussj.c"


const double minX = -10.;
const double minY = -10.;
const double maxX = 10.;
const double maxY = 10.;
const unsigned int l_krok = 100;
const unsigned int N = 200;


//zwraca wartosc funkcji
double f(double x, double y)
{
	double temp = exp(-pow(x + M_PI/2, 2) -pow(y - M_PI/2, 2));
	return sin(x)*sin(y) - temp;
}

//zwraca losowa liczbe z podanego przedzialu
double d_rand(const double min, const double max)
{
	double r = (double)rand() / RAND_MAX;
	return r*(max - min) + min;
}

//sprawdza czy liczby wyszly poza zakres
//zwraca odowiedz na pytanie czy konieczne jest ponowne losowanie
bool check_bounds(double xx, double yy)
{
	if( (minX - xx) <= 0 && (xx - maxX) <= 0)
	{
		if( (minY - yy) <= 0 && (yy - maxY) <= 0)
		{
			return false; //zakresy sie zgadzaja, nie trzeba wiecej losowac
		}
	}
	return true;
}

int main()
{
	//-------------------------- alokacja pamieci
	double** w = malloc(N*sizeof(double)); //tworzymy N wedrowcow
	for(int i = 0; i < N; i++)
	{
		w[i] = malloc(2*sizeof(double)); //miejsce na (x, y)
		w[i][0] = w[i][1] = 5.; //punkt startowy (5, 5)
	}

	FILE* fp1 = fopen("w0.dat", "w+");
	FILE* fp2 = fopen("T.dat", "w+");

	for(unsigned int IT = 0; IT <= 20; IT++) //petla po obnizaniu temperatury
	{
		double T = 10./pow(2, IT);
		for(unsigned int k = 0; k < l_krok; k++) //petla po krokach
		{
			for(unsigned int i = 0; i < N; i++) //petla po wedrowcach
			{
				double deltaX, deltaY;
				//losowanie przesuniec z [-1; 1]
				do
				{
					deltaX = d_rand(-1., 1.);
					deltaY = d_rand(-1., 1.);
				}
				while(check_bounds(w[i][0]+deltaX, w[i][1]+deltaY));

				double temp = f(w[i][0]+deltaX, w[i][1]+deltaY) - f(w[i][0], w[i][1]);

				//sprawdzenie polozenia
				if( temp < 0)
				{
					//akceptacja nowych polozen
					w[i][0] += deltaX;
					w[i][1] += deltaY;
				}
				else if( d_rand(0., 1.) - exp( -temp/T ) < 0 )
				{
					w[i][0] += deltaX;
					w[i][1] += deltaY;
				}
				if(i == 0) //zapisanie danych dla pierwszego wedrowca
				{
					fprintf(fp1, "%g\n", f(w[i][0], w[i][1]) );
				}
			}
		}
		if(IT == 0 || IT == 7 || IT == 20)
		{
			for(int a = 0; a < N; a++)
				fprintf(fp2, "%g  %g\n", w[a][0], w[a][1]);
		}
		fprintf(fp2, "\n\n");
	}

	fclose(fp1);
	fclose(fp2);

	int index = 0;
	for(int a = 1; a < N; a++)
	{
		if( f(w[a][0], w[a][1]) - f(w[index][0], w[index][1]) < 0)
			index = a;
	}
	printf("Minimum globalne dla (%g, %g) wynosi ", w[index][0], w[index][1]);
	printf("%g\n", f(w[index][0], w[index][1]) );

	//------------------------------ dealokacja pamieci
	for(int i = 0; i < N; i++)
	{
		free(w[i]);
	}
	free(w);
	
	return 0;
}