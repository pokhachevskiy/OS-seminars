#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h> //-lm
int numberOfthreads = 4; //default value


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t bear = PTHREAD_COND_INITIALIZER;
pthread_cond_t full = PTHREAD_COND_INITIALIZER;
int count = 0;
const int N = 6;

void *mythread(void *dummy)

/*
 *   Parameter void *dummy is used only for type compatibility, and will not be used
 *   at the function now. At the same reason the return value of function has type 
 *   void *, but it wiil be not used at this program.
 */

{
   for (int i = 0; i < 3; i++) {
      pthread_mutex_lock(&mutex);
      //pthread_cond_wait(&full, &mutex);
      while (count == N) {
         pthread_cond_wait(&full, &mutex);
         //pthread_cond_wait(&bear, &mutex);
      }
      count += 1; 
      printf("\nadded one to bochka\n");
      if (count == N){
         pthread_cond_signal(&bear);
         printf("\ncount in N\n");
         //pthread_cond_wait(&full, &mutex);
         // pthread_mutex_unlock(&mutex);
      } 
      pthread_mutex_unlock(&mutex);
      //pthread_cond_wait(&bear, &mutex);
         
}
   return NULL;
}

int main()
{
   // double sum = 0.0;
	int res = 0;
   int numb[10];
   printf ("%d \n", numberOfthreads);
   // exit(0);
	pthread_t *thid = (pthread_t*)calloc(numberOfthreads, sizeof(pthread_t)); 

	for (int j = 0; j < numberOfthreads; j++){
		numb[j] = j;
		res = pthread_create( &(thid[j]), (pthread_attr_t *)NULL, mythread, (void*)&numb[j]);

		if(res != 0){
			exit(-1);
		}   
	}


	// printf("Thread created, thid = %u\n", thid);

	// printf("Thread %u, Calculation result = %d\n", mythid, a);

   for (int j = 0; j < 2; j++){
      //pthread_cond_wait(&bear, &mutex);
      while (count < N) {
         pthread_cond_wait(&bear, &mutex);
      }
      printf("\neating\n");

      count = 0;
      pthread_cond_broadcast(&full);
      pthread_mutex_unlock(&mutex);
   }

	return 0;
}
