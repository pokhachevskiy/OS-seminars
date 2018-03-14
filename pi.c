#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h> //-lm
int numberOfthreads = 5; //default value



double* result;
const long N = 100000000;
double sum = 0.0;

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
   int thid = pthread_self();
   double j = 0.0;
   for (double j = 2.0 * nThread / numberOfthreads; j < 2.0 * (nThread + 1)/numberOfthreads; j += 2.0 / N){

      f1 = sqrt(4 - j*j);
      result[nThread] += (f1 + f0)/2.0*(2.0 / N);
      f0 = f1;
   }
   if (nThread == numberOfthreads) {
      for (; j < 2; j += 2.0 / N){

         f1 = sqrt(4 - j*j);
         result[nThread] += (f1 + f0)/2.0*(2.0 / N);
         f0 = f1;
   }
   }
   printf("Thread %u, Calculation result = %lf\n", nThread, result[nThread]);

   return NULL;
}

int main(int argc, char* argv[])
{
   // double sum = 0.0;
	int res = 0;
   int numb[10];
	sscanf(argv[1], "%d", &numberOfthreads);
   printf ("%d \n", numberOfthreads);
   // exit(0);
	pthread_t *thid = (pthread_t*)calloc(numberOfthreads, sizeof(pthread_t)); 
	result = (double*)calloc(numberOfthreads, sizeof(double));

	for (int j = 0; j < numberOfthreads; j++){
		numb[j] = j;
		res = pthread_create( &(thid[j]), (pthread_attr_t *)NULL, mythread, (void*)&numb[j]);

		if(res != 0){
			printf ("Error on thread create, return value = %d\n", result);
			exit(-1);
		}   
	}


	// printf("Thread created, thid = %u\n", thid);

	// printf("Thread %u, Calculation result = %d\n", mythid, a);


	for (int j = 0; j < numberOfthreads; j++){
		pthread_join(thid[j], (void **)NULL);
	}
	for (int j = 0; j < numberOfthreads; j++){
		sum += result[j];
      printf ("\n %d %lf \n", j, result[j]);
	}
	printf("%lf \n", sum);

	return 0;
}
