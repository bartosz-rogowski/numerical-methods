#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <gsl/gsl_linalg.h>

#define N 4
#define delta 2

int main()
{

///to wskaznik do struktury
gsl_matrix *A = gsl_matrix_calloc(N, N);

///Acopy przyda sie do obliczenia A*A^-1, gdyz macierz A bedzie nadpisywana
gsl_matrix *Acopy = gsl_matrix_calloc(N, N);



///indeksowanie od zera!
for(int i=0; i<N; i++)
{
	for(int j=0; j<N; j++)
	{
		///inicjalizowanie macierzy A wartosciami
		gsl_matrix_set(A, i, j, 1.0/(i+j+delta) );
		gsl_matrix_set(Acopy, i, j, 1.0/(i+j+delta) );
	}
}

///dekompozycja macierzy

gsl_permutation *p = gsl_permutation_calloc(N);
int signum;

gsl_linalg_LU_decomp(A, p, &signum); 


/////////////////Wypisanie elementow - zad 2

double wyznacznik=signum;
printf("Elementy diagonalne miacierzy U: ");
for(int i=0; i<N; ++i)
{
	printf("%g ", gsl_matrix_get(A, i, i));
	wyznacznik*=gsl_matrix_get(A, i, i);
}
printf("\nWyznacznik macierzy A: %g\n", wyznacznik);

////////////////////////////////////// zad 3

///utworzenie kolumn wyrazow wolnych
gsl_vector *b1 = gsl_vector_calloc(N);
gsl_vector *b2 = gsl_vector_calloc(N);
gsl_vector *b3 = gsl_vector_calloc(N);
gsl_vector *b4 = gsl_vector_calloc(N);
gsl_vector *x1 = gsl_vector_calloc(N);
gsl_vector *x2 = gsl_vector_calloc(N);
gsl_vector *x3 = gsl_vector_calloc(N);
gsl_vector *x4 = gsl_vector_calloc(N);

gsl_vector_set(b1, 0, 1);
gsl_vector_set(b2, 1, 1);
gsl_vector_set(b3, 2, 1);
gsl_vector_set(b4, 3, 1);

gsl_linalg_LU_solve(A, p, b1, x1);
gsl_linalg_LU_solve(A, p, b2, x2);
gsl_linalg_LU_solve(A, p, b3, x3);
gsl_linalg_LU_solve(A, p, b4, x4);



///macierz odwrotna sklada sie z kolumn x1,...x4

printf("\nMacierz odwrotna:\n");

///zapisanie wynikow do macierzy A:

for(int i=0; i<N; i++)
{
	gsl_matrix_set(A, i, 0, gsl_vector_get(x1, i));
}
for(int i=0; i<N; i++)
{
	gsl_matrix_set(A, i, 1, gsl_vector_get(x2, i));
}
for(int i=0; i<N; i++)
{
	gsl_matrix_set(A, i, 2, gsl_vector_get(x3, i));
}
for(int i=0; i<N; i++)
{
	gsl_matrix_set(A, i, 3, gsl_vector_get(x4, i));
}

for(int i=0; i<N; i++)
{
	for(int j=0; j<N; j++)
		printf("%6g ", gsl_matrix_get(A, i, j));
	printf("\n");
}



/////////////////////////// zad 4

///tworzymy macierz R - rezultat A*A^-1
gsl_matrix *R = gsl_matrix_calloc(N, N);

///A to nasza A^-1


for(int i=0; i<N; i++)
{
	for(int j=0; j<N; j++)
	{
		gsl_matrix_set(R, i, j, 0);

		for(int k=0; k<N; k++)
		{
			double value = gsl_matrix_get(Acopy, i, k) * gsl_matrix_get(A, k, j);
			double temp = gsl_matrix_get(R, i, j);
			temp+=value;
			gsl_matrix_set(R, i, j, temp);
		}
	}
}

printf("\nMacierz A*A^-1:\n");

for(int i=0; i<N; i++)
{
	for(int j=0; j<N; j++)
		printf("%14g ", gsl_matrix_get(R, i, j));
	printf("\n");
}



////////////////////////// zad 5

double max1 = fabs( gsl_matrix_get(Acopy, 0, 0) );
for(int i=0; i<N; i++)
{
	for(int j=0; j<N; j++)
	{
		if( fabs( gsl_matrix_get(Acopy, i, j) > max1 ) )
			max1 = fabs( gsl_matrix_get(Acopy, i, j) );
	}
}

printf("Wskaznik uwarunkowania macierzy A: %g\n", max1);


double max2 = fabs( gsl_matrix_get(A, 0, 0) );
for(int i=0; i<N; i++)
{
	for(int j=0; j<N; j++)
	{
		if( fabs( gsl_matrix_get(A, i, j) > max2 ) )
			max2 = fabs( gsl_matrix_get(A, i, j) );
	}
}

printf("Wskaznik uwarunkowania macierzy A (odwroconej): %g\n", max2);
printf("Wskaznik uwarunkowania macierzy k(A): %g\n", max1*max2);

///czyszczenie pamieci
gsl_matrix_free(A);
gsl_matrix_free(Acopy);
gsl_permutation_free(p);
gsl_vector_free(b1);
gsl_vector_free(b2);
gsl_vector_free(b3);
gsl_vector_free(b4);
gsl_vector_free(x1);
gsl_vector_free(x2);
gsl_vector_free(x3);
gsl_vector_free(x4);

return 0;
}