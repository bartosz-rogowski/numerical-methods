#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* Dyrektywy zakladajace, ze te trzy pliki sa skopiowane do aktualnego katalogu. */
//#include "nrutil.h"
//#include "nrutil.c" // To mozna usunac, jesli plik jest dodany w poleceniu kompilacji.
//#include "gaussj.c" // To tez mozna usunac, jesli plik jest dodany w poleceniu kompilacji.

/* Dyrektywy dla Taurusa (nie wymagaja kopiowania plikow, ale Taurus musi dzialac...) */
#include "/opt/NR/numerical_recipes.c/nrutil.h"
#include "/opt/NR/numerical_recipes.c/nrutil.c"
#include "/opt/NR/numerical_recipes.c/gaussj.c"

#define N 400 // rozmiar macierzy M: NxN

int main(void)
{
	float **M, **b;
	//	Alokacja macierzy
	M = matrix(1, N, 1, N);
	b = matrix(1, N, 1, 1);

	// 	Wypelnienie macierzy M i wektora b
	b[1][1]=1;
	b[2][1]=0.1*0;
	for (int i = 1; i <= N; ++i)
	{
		if(i+2 <= N)
			b[i+2][1] = 0.0; //zerujemy wektor od 3. wyrazu

		for (int j = 1; j <= N; ++j)
		{
			if(i!=j)
				M[i][j] = 0.0;
			else
				M[i][j] = 1.0; //wpisujemy 1 na diagonali	
		}
	}

	M[2][1]=-1;
	for(int i=3; i<=N; ++i)
	{
		M[i][i-1]=1*0.1*0.1-2;
	}
	for(int i=3; i<=N; ++i)
	{
		M[i][i-2]=1;
	}


	// b[1][1] = -4.0;
	// b[2][1] = 9.0;
	// b[3][1] = -16.0;

	// M[1][1] = 2.0;
	// M[1][2] = 3.0;
	// M[1][3] = 0.0;
	// M[2][1] = -1.0;
	// M[2][2] = 1.0;
	// M[2][3] = 4.0;
	// M[3][1] = 3.0;
	// M[3][2] = 11.0;
	// M[3][3] = 1.0;




	//	Rozwiazanie ukladu rownan Mx=b - wywolanie procedury:
	gaussj(M, N, b, 1);

	//	Wypisanie rozwiazania, ktore procedura gaussj(M, N, b, 1); zapisala w wektorze b.
	for (int i = 1; i <= N; ++i)
		printf("%g    %g\n", (i-1)*0.1, b[i][1]);

	//	Zwolnienie pamieci
	free_matrix(M, 1, N, 1, N);
	free_matrix(b, 1, N, 1, 1);

	return 0;
}
