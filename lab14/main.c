#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N1 10000
#define N2 1000

//generator pobierajacy parametry: a, c,
double gen_1(long a, int c, unsigned long m, int flag)
{
	static unsigned long x = 10;
	if(flag == 0)
		x = 10;
	x = (a*x + c) % m;
	//normalizacja
	return x/(m+1.0);
}

void statistics(double* data, long int size, double* mu, double* sigma)
{
	double sum = 0.;
	for(int i = 0; i < size; i++)
	{
		sum += data[i];
	}
	*mu = sum/size;
	sum = 0.;
	for(int i = 0; i < size; i++)
	{
		sum += pow(data[i] - *mu, 2);
	}
	*sigma = sqrt(sum/size);
}

//zlicza wystapienia liczb w danym podprzedziale, zwraca dlugosc kazdego podprzedzialu
double histogram(int* tab, const int K, double xmin, double xmax, double* data, int size)
{
	int j;
	double dx = (xmax - xmin) / K;
	for(int i = 0; i < size; i++)
	{
		j = (int)( (data[i] - xmin) / dx);
		++tab[j];
	}
	return dx;
}

//dystrybuanta
double F(double x, double mu, double t)
{
	if(x - mu <= 0)
		return -1./pow(t, 2) * (-pow(x, 2)/2. + mu*x) + x/t;
	else
		return -1./pow(t, 2) * (pow(x, 2)/2. - mu*x + pow(mu, 2)) + x/t;
}


int main()
{
	//--------------------------------- alokacja pamieci
	double* x1 = calloc(N1, sizeof(double));
	double* x2 = calloc(N1, sizeof(double));
	int* K1 = calloc(12, sizeof(int));
	int* K2 = calloc(12, sizeof(int));
	FILE* fp1 = fopen("U.dat", "w+");
	unsigned long m1 = pow(2, 15);
	unsigned long m2 = pow(2, 32);

	for(int i = 0; i < N1; i++)
	{
		x1[i] = gen_1(123, 1, m1, i);
	}

	for(int i = 0; i < N1; i++)
	{
		x2[i] = gen_1(69069, 1, m2, i);
	}

	for(int i = 0; i < N1-1; i++)
	{
		fprintf(fp1, "%g  %g\n", x1[i], x1[i+1]);
	}

	fprintf(fp1, "\n\n");

	for(int i = 0; i < N1-1; i++)
	{
		fprintf(fp1, "%g  %g\n", x2[i], x2[i+1]);
	}
	fclose(fp1);
	double mu1, mu2, sigma1, sigma2;
	mu1 = mu2 = sigma1 = sigma2 = 0.;
	statistics(x1, N1, &mu1, &sigma1);
	statistics(x2, N1, &mu2, &sigma2);
	printf("\e[1;34mmu1 = %8g\e[0m      blad bezwgledny = %e\n", mu1, fabs(mu1 - 0.5)); 
	printf("\e[1;35msigma1 = %8g\e[0m    blad bezwgledny = %e\n", sigma1, fabs(sigma1 - sqrt(1./12)) );
	printf("\e[1;34mmu2 = %8g\e[0m       blad bezwgledny = %e\n", mu2, fabs(mu2 - 0.5)); 
	printf("\e[1;35msigma1 = %8g\e[0m    blad bezwgledny = %e\n", sigma2, fabs(sigma2 - sqrt(1./12)) );

	double dx1 = histogram(K1, 12, 0., 1., x1, N1);
	double dx2 = histogram(K2, 12, 0., 1., x2, N1);

	FILE* fp2 = fopen("U_hist.dat", "w+");
	for(int i = 0; i < 12; i++)
	{
		fprintf(fp2, "%g  %g\n", i*dx1 + ((i+1)*dx1 - i*dx1)/2, (double)K1[i]/N1);
	}
	fprintf(fp2, "\n\n");
	for(int i = 0; i < 12; i++)
	{
		fprintf(fp2, "%g  %g\n", i*dx2 + ((i+1)*dx2 - i*dx2)/2, (double)K2[i]/N1);
	}

	//--------------------------------- dealokacja pamieci
	fclose(fp2);
	free(x1);
	free(x2);
	free(K1);
	free(K2);


	//===================== ROZKLAD TROJKATNY ========================


	const int ile = 10;
	double* x = calloc(N2, sizeof(double));
	int* K3 = calloc(ile, sizeof(int));
	double* p = calloc(ile, sizeof(double));
	double mu = 4.0;
	double delta = 3.0;
	for(int i = 0; i < N2; i++)
	{
		x[i] = mu + (gen_1(69069, 1, m2, i) + gen_1(69069, 1, m2, i+1) - 1.) * delta;
	}

	FILE* fp3 = fopen("T_hist.dat", "w+");
	double dx3 = histogram(K3, ile, mu-delta, mu+delta, x, N2);
	double chi2 = 0.;
	for(int i = 0; i < ile; i++)
	{
		p[i] = F(mu-delta + (i+1)*dx3, mu, delta) - F(mu-delta + i*dx3, mu, delta);
		chi2 += pow(K3[i] - N2*p[i], 2) / (N2*p[i]);
		fprintf(fp3, "%g  %g %g\n", mu-delta + i*dx3 + ((i+1)*dx3 - i*dx3)/2, (double)K3[i]/N2, p[i]);
	}

	printf("\n\n> \e[1;33mchi^2 = %g\e[0m\n", chi2);
	if(chi2 < 14.06)
		printf("\e[1;32mNie ma podstaw do odrzucenia hipotezy (przy zadanym poziomie ufnosci)\e[0m\n");
	else
		printf("\e[1;31mHipoteza odrzucona (przy zadanym poziomie ufnosci)\e[0m\n");

	fclose(fp3);
	free(p);
	free(x);
	free(K3);

	return 0;
}