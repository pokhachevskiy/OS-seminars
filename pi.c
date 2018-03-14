#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h> //-lm
int numberOfthreads = 5;



double* result;//[numberOfthreads];
int a = 0;
const long N = 100000;


void *mythread(void *dummy)

/*
 *   Parameter void *dummy is used only for type compatibility, and will not be used
 *   at the function now. At the same reason the return value of function has type 
 *   void *, but it wiil be not used at this program.
 */

{

   pthread_t mythid;
   int nThread = *(int*)dummy;
   double f0 = 0.0, f1 = 0.0;
   mythid = pthread_self();
   for (double j = 2 * mythid / n; j < 2 * (mythid + 1)/n; j += 2.0 / N){
      f1 = sqrt(4 - j*j);
      result[nThread] += (f1 + f0)/2*(2.0 / N);
   }
   printf("Thread %u, Calculation result = %d\n", mythid, result[nThread]);

   return NULL;
}

int main(int argc, char* argv)
{
	int res = 0;
	numberOfthreads = argv[1];
	pthread_t *thid = (pthread_t*)calloc(numberOfthreads, sizeof(pthread_t)); 
	result = (double*)calloc(numberOfthreads, sizeof(double));

	for (int j = 0; j < numberOfthreads; j++){
		int numb = j;
		res = pthread_create( &thid[i], (pthread_attr_t *)NULL, mythread, &numb);

		if(res != 0){
			printf ("Error on thread create, return value = %d\n", result);
			exit(-1);
		}   
	}


	// printf("Thread created, thid = %u\n", thid);

	printf("Thread %u, Calculation result = %d\n", mythid, a);


	for (int j = 0; j < numberOfthreads; j++){
		pthread_join(thid[j], (void **)NULL);
	}
	for (int j = 0; j < numberOfthreads; j++){
		sum += result[j];
	}
	printf("%lf \n", sum);
	return 0;
}
