#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "/opt/NR/numerical_recipes.c/nrutil.h"
#include "/opt/NR/numerical_recipes.c/nrutil.c"
#include "/opt/NR/numerical_recipes.c/gauleg.c"
#include "/opt/NR/numerical_recipes.c/gaulag.c"
#include "/opt/NR/numerical_recipes.c/gauher.c"
#include "/opt/NR/numerical_recipes.c/gammln.c"

//implementacja funkcji 
double f(float x, int ver)
{
	switch(ver)
	{
		case 1:
			return 1./(x*sqrt(pow(x, 2)-1));
		break;

		case 2:
			return log(fabs(x))/2.;
		break;

		case 3:
			return log(fabs(x))*exp(-x*x);
		break;

		case 4:
			return sin(2*x)*exp(-2*x);
		break;

		default:
			return 0.;
	}
}

//zapisuje wezly do pliku, pod warunkiem, ze jest ich nie wiecej niz 50
void saveNodes2file(FILE** f, float* nodes, int n)
{
	if(n<51)
	{
		fprintf(*f, "\e[1;32mNodes for n = %d\e[0m\n", n);
		for(int a = 1; a <= n; a++)
		{
			fprintf(*f, "%g   ", nodes[a]);
		}
		fprintf(*f, "\n\n");
	}
}

///////////////////////////////////////////////////////////////////////////////////

int main()
{
	FILE* fp = fopen("out.dat", "w+");
	FILE* fp_ = fopen("nodes.dat", "w+");

	//-------------------------------- zad 1
	fprintf(fp_, "\e[1;31m=========================== ZAD 1 ===========================\e[0m\n");
	const double c_a = M_PI/3;
	for(unsigned n = 2; n <= 100; n++)
	{
		float* x = vector(1, n);
		float* w = vector(1, n);

		gauleg(1., 2., x, w, n);

		double c = 0.;

		for(unsigned i = 1; i <= n; i++)
		{
			c+=w[i]*f(x[i], 1);
		}
		fprintf(fp, "%3d\t%e\n", n, fabs(c-c_a));

		saveNodes2file(&fp_, x, n);
		free_vector(x, 1, n);
		free_vector(w, 1, n);
	}

	fprintf(fp, "\n\n");

	//-------------------------------- zad 2a
	const double c_2a = -0.8700577;
	fprintf(fp_, "\e[1;31m=========================== ZAD 2a ===========================\e[0m\n");
	for(unsigned n = 2; n <= 100; n+=2)
	{
		float* x = vector(1, n);
		float* w = vector(1, n);
		gauher(x, w, n);

		double c = 0.;

		for(unsigned i = 1; i <= n; i++)
		{
			c+=w[i]*f(x[i], 2);
		}
		fprintf(fp, "%3d\t%e\n", n, fabs(c-c_2a));

		saveNodes2file(&fp_, x, n);

		free_vector(x, 1, n);
		free_vector(w, 1, n);
	}


	fprintf(fp, "\n\n");

	//-------------------------------- zad 2b
	fprintf(fp_, "\e[1;31m=========================== ZAD 2b ===========================\e[0m\n");
	for(unsigned n = 2; n <= 100; n++)
	{
		float* x = vector(1, n);
		float* w = vector(1, n);
		gauleg(0., 5., x, w, n);

		double c = 0.;

		for(unsigned i = 1; i <= n; i++)
		{
			c+=w[i]*f(x[i], 3);
		}
		fprintf(fp, "%3d\t%e\n", n, fabs(c-c_2a));

		saveNodes2file(&fp_, x, n);

		free_vector(x, 1, n);
		free_vector(w, 1, n);
	}

	fprintf(fp, "\n\n");

	//-------------------------------- zad 3
	fprintf(fp_, "\e[1;31m=========================== ZAD 3 ===========================\e[0m\n");
	const double c_3a = 2./13.;
	for(unsigned n = 2; n <= 20; n++)
	{
		float* x = vector(1, n);
		float* w = vector(1, n);
		gaulag(x, w, n, 0.);

		double c = 0.;

		for(unsigned i = 1; i <= n; i++)
		{
			c+=w[i]*f(x[i], 4);
		}

		fprintf(fp, "%3d\t%e\n", n, fabs(c-c_3a));

		saveNodes2file(&fp_, x, n);

		free_vector(x, 1, n);
		free_vector(w, 1, n);
	}

	fclose(fp);
	fclose(fp_);
	return 0;
}