#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "/opt/NR/numerical_recipes.c/nrutil.h"
#include "/opt/NR/numerical_recipes.c/nrutil.c"
#include "/opt/NR/numerical_recipes.c/tred2.c"
#include "/opt/NR/numerical_recipes.c/tqli.c"
#include "/opt/NR/numerical_recipes.c/pythag.c"

#define nx 20
#define ny 20
#define m 10
#define t -0.021

int n = nx*ny;

//pomocnicza funkcja do podejrzenia zawartosci macierzy
void show(float** A)
{
	for(int i = 1; i <= n; i++)
	{
		for(int j = 1; j <= n; j++)
		{
			printf("%12.6g", A[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

int main(){
//------------------------- alokacja pamieci
float **H = matrix(1, n, 1, n);
float *d = vector(1, n);
float *e = vector(1, n);

//------------------------- nadanie wartosci H

for (int i = 1; i <= nx; i++)
{
	for (int j = 1; j <= ny; j++)
	{
		int l = j+(i-1)*ny;
		for (int k = 1; k <= n; k++)
			H[l][k]=0.;
		if(i>1)   
			H[l][l-ny]=t;   //dla i=1 nie ma sasiada z lewej strony
		if(i<nx)  
			H[l][l+ny]=t;   //dla i=nx nie ma sasiada z prawej strony
		H[l][l]=-4.0*t;
		if(j>1)   
			H[l][l-1]=t;    //dla j=1 nie ma sasiada ponizej siatki
		if(j<ny)  
			H[l][l+1]=t;    //dla j=ny nie ma sasiada powyzej siatki
	}
}


tred2(H, n, d, e);
tqli(d, e, n, H); //<-------sprytne wywolanie

//pomocnicze zmienne
float e1, e2;
e1=e2=0.0;
int *indx = ivector(1, n);

//inicjalizacja inxd[]
for (int l = 1; l <= n; l++)
	indx[l]=l;

for (int l = 1; l <= n-1; l++)
{
	for (int k = n; k >= l+1; k--)
	{
		e1=d[k-1];
		e2=d[k];
		int l1 = indx[k-1];
		int l2 = indx[k];
		if(e2<e1)
		{ //zamiana energii -> oraz indeksow
			d[k]=e1;
			d[k-1]=e2;
			indx[k]=l1;
			indx[k-1]=l2;
		}
	}
}

//=====================zapis do pliku==================
FILE *fp;
fp=fopen("dane.dat", "w+");
for (int i = 1; i <= nx; i++)
{
	for (int j = 1; j <= ny; j++)
	{
		int l = j+(i-1)*ny;
		fprintf(fp, "%6d %6d ", i, j);
		for (int k = 1; k <= m; k++)
			fprintf(fp, " %12.6f  ", H[l][ indx[k] ]); //<------ sprytne wywolanie
		fprintf(fp, "\n");
	}
	fprintf(fp, "\n");
}
fclose(fp);

//------------------------- wypisywanie wartosci wlasnych

for (int i = 1; i <= m; i++)
{
	printf("%g\n", d[i]);
}

//------------------------- zwalnianie pamieci
free_matrix(H, 1, n, 1, n);
free_vector(d, 1, n);
free_vector(e, 1, n);
free_ivector(indx, 1, n);
	return 0;
}