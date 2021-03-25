#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gsl/gsl_linalg.h>

#define V 0
#define x 1
#define omega 1
#define n 8000
#define h 0.02
#define OMEGA 0.8

// pomocnicza funkcja do obliczania sumy kwadratów
double square_sum(gsl_vector* vec)
{
	double sum=0;
	for (int i = 0; i < n+1; ++i)
	{
		double t = gsl_vector_get(vec, i);
		sum+= t*t;
	}
	return sum;
}


int main(){

double beta, F0;

gsl_vector *b = gsl_vector_calloc(n+1);
gsl_vector *d0 = gsl_vector_calloc(n+1);
gsl_vector *d1 = gsl_vector_calloc(n+1);
gsl_vector *d2 = gsl_vector_calloc(n+1);
gsl_vector *xs = gsl_vector_calloc(n+1);
gsl_vector *xn = gsl_vector_calloc(n+1);

//////////////////////////////// 1* ////////////////////////////////
beta = F0 = 0.0;

//wypelnianie wektora wyrazow wolnych
gsl_vector_set(b, 0, 1);
gsl_vector_set(b, 1, 0);
for(int i=2; i<n+1; ++i)
{
	gsl_vector_set(b, i, F0 * h*h * sin(OMEGA * h * (i-1) ) );
}



gsl_vector_set(d0, 0, 1);
gsl_vector_set(d0, 1, 1);
for (int i = 2; i < n+1; ++i)
{
	gsl_vector_set(d0, i, 1+beta*h);
}


gsl_vector_set(d1, 0, 0);
gsl_vector_set(d1, 1, -1);
for (int i = 2; i < n+1; ++i)
{
	gsl_vector_set(d1, i, omega*omega*h*h - 2 - beta*h);
}


gsl_vector_set(d2, 0, 0);
gsl_vector_set(d2, 1, 0);

for (int i = 2; i < n+1; ++i)
{
	//ten wektor wystarczy ustawic raz 
	gsl_vector_set(d2, i, 1);
}


gsl_vector_set_zero(xs);
gsl_vector_set_zero(xn);


unsigned long int iteration = 0;

while(iteration < 10000)
{
	++iteration;
	for (int i = 0; i < n+1; ++i)
	{
		if(i==0)
		{
			double temp = (gsl_vector_get(b, i) - gsl_vector_get(d1, i) * 1 - gsl_vector_get(d2, i) * 1 )/gsl_vector_get(d0, i);
			gsl_vector_set(xn, i, temp);
		}
		else if(i==1)
		{
			double temp = (gsl_vector_get(b, i) - gsl_vector_get(d1, i) * gsl_vector_get(xs, i-1) - gsl_vector_get(d2, i) * 1 )/gsl_vector_get(d0, i);
			gsl_vector_set(xn, i, temp);
		}
		else
		{
			double temp = (gsl_vector_get(b, i) - gsl_vector_get(d1, i) * gsl_vector_get(xs, i-1) - gsl_vector_get(d2, i) * gsl_vector_get(xs, i-2) )/gsl_vector_get(d0, i);
			gsl_vector_set(xn, i, temp);
		}
	}

	double sn = square_sum(xs);
	double ss = square_sum(xn);

	if( fabs(sn - ss) < 1e-6)
		break;

	for (int i = 0; i < n+1; ++i)
	{
		gsl_vector_set(xs, i, gsl_vector_get(xn, i) );
	}
}

for (int i = 0; i < n+1; ++i)
{
	printf("%5g  %5g\n", i*h, gsl_vector_get(xn, i) );
}
//printf("%d", iteration);

printf("\n\n");
////////////////////////////////////////// 2* /////////////////////////////////

beta = 0.4;

gsl_vector_set(b, 0, 1);
gsl_vector_set(b, 1, 0);
for(int i=2; i<n+1; ++i)
{
	gsl_vector_set(b, i, F0 * h*h * sin(OMEGA * h * (i-1) ) );
}



gsl_vector_set(d0, 0, 1);
gsl_vector_set(d0, 1, 1);
for (int i = 2; i < n+1; ++i)
{
	gsl_vector_set(d0, i, 1+beta*h);
}


gsl_vector_set(d1, 0, 0);
gsl_vector_set(d1, 1, -1);
for (int i = 2; i < n+1; ++i)
{
	gsl_vector_set(d1, i, omega*omega*h*h - 2 - beta*h);
}

gsl_vector_set_zero(xs);
gsl_vector_set_zero(xn);

iteration = 0;

while(iteration < 10000)
{
	++iteration;
	for (int i = 0; i < n+1; ++i)
	{
		if(i==0)
		{
			double temp = (gsl_vector_get(b, i) - gsl_vector_get(d1, i) * 1 - gsl_vector_get(d2, i) * 1 )/gsl_vector_get(d0, i);
			gsl_vector_set(xn, i, temp);
		}
		else if(i==1)
		{
			double temp = (gsl_vector_get(b, i) - gsl_vector_get(d1, i) * gsl_vector_get(xs, i-1) - gsl_vector_get(d2, i) * 1 )/gsl_vector_get(d0, i);
			gsl_vector_set(xn, i, temp);
		}
		else
		{
			double temp = (gsl_vector_get(b, i) - gsl_vector_get(d1, i) * gsl_vector_get(xs, i-1) - gsl_vector_get(d2, i) * gsl_vector_get(xs, i-2) )/gsl_vector_get(d0, i);
			gsl_vector_set(xn, i, temp);
		}
	}

	double sn = square_sum(xs);
	double ss = square_sum(xn);

	if( fabs(sn - ss) < 1e-6)
		break;

	for (int i = 0; i < n+1; ++i)
	{
		gsl_vector_set(xs, i, gsl_vector_get(xn, i) );
	}
}

for (int i = 0; i < n+1; ++i)
{
	printf("%5g  %5g\n", i*h, gsl_vector_get(xn, i) );
}
//printf("%d", iteration);


printf("\n\n");

///////////////////////////////////// 3* //////////////////////////////////////
F0 = 0.1;

gsl_vector_set(b, 0, 1);
gsl_vector_set(b, 1, 0);
for(int i=2; i<n+1; ++i)
{
	gsl_vector_set(b, i, F0 * h*h * sin(OMEGA * h * (i-1) ) );
}



gsl_vector_set(d0, 0, 1);
gsl_vector_set(d0, 1, 1);
for (int i = 2; i < n+1; ++i)
{
	gsl_vector_set(d0, i, 1+beta*h);
}


gsl_vector_set(d1, 0, 0);
gsl_vector_set(d1, 1, -1);
for (int i = 2; i < n+1; ++i)
{
	gsl_vector_set(d1, i, omega*omega*h*h - 2 - beta*h);
}

gsl_vector_set_zero(xs);
gsl_vector_set_zero(xn);

iteration = 0;

while(iteration < 10000)
{
	++iteration;
	for (int i = 0; i < n+1; ++i)
	{
		if(i==0)
		{
			double temp = (gsl_vector_get(b, i) - gsl_vector_get(d1, i) * 1 - gsl_vector_get(d2, i) * 1 )/gsl_vector_get(d0, i);
			gsl_vector_set(xn, i, temp);
		}
		else if(i==1)
		{
			double temp = (gsl_vector_get(b, i) - gsl_vector_get(d1, i) * gsl_vector_get(xs, i-1) - gsl_vector_get(d2, i) * 1 )/gsl_vector_get(d0, i);
			gsl_vector_set(xn, i, temp);
		}
		else
		{
			double temp = (gsl_vector_get(b, i) - gsl_vector_get(d1, i) * gsl_vector_get(xs, i-1) - gsl_vector_get(d2, i) * gsl_vector_get(xs, i-2) )/gsl_vector_get(d0, i);
			gsl_vector_set(xn, i, temp);
		}
	}

	double sn = square_sum(xs);
	double ss = square_sum(xn);

	if( fabs(sn - ss) < 1e-6)
		break;

	for (int i = 0; i < n+1; ++i)
	{
		gsl_vector_set(xs, i, gsl_vector_get(xn, i) );
	}
}

for (int i = 0; i < n+1; ++i)
{
	printf("%5g  %5g\n", i*h, gsl_vector_get(xn, i) );
}
//printf("%d", iteration);



////////////////////////////// deleting memory ////////////////////////////////
gsl_vector_free(b);
gsl_vector_free(d0);
gsl_vector_free(d1);
gsl_vector_free(d2);
gsl_vector_free(xs);
gsl_vector_free(xn);
	return 0;
}