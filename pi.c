#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h> //-lm
int numberOfthreads = 5;



double result[numberOfthreads];
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
   nThread = (int*)dummy[0];
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
   pthread_t thid[numberOfthreads]; 
   int       result[3];

   result[0] = pthread_create( &thid[0], (pthread_attr_t *)NULL, mythread, NULL);
   
   if(result[0] != 0){
      printf ("Error on thread create, return value = %d\n", result);
      exit(-1);
   }   
   result[1] = pthread_create( &thid[1], (pthread_attr_t *)NULL, mythread, NULL);
   
   if(result[1] != 0){
      printf ("Error on thread create, return value = %d\n", result);
      exit(-1);
   }   
   result[2] = pthread_create( &thid[2], (pthread_attr_t *)NULL, mythread, NULL);
   
   if(result[2] != 0){
      printf ("Error on thread create, return value = %d\n", result);
      exit(-1);
   }   

   
   printf("Thread created, thid = %u\n", thid);

   mythid = pthread_self();
   
   a = a+1;
   
   printf("Thread %u, Calculation result = %d\n", mythid, a);

   pthread_join(thid[0], (void **)NULL);
   pthread_join(thid[1], (void **)NULL);
   pthread_join(thid[2], (void **)NULL);

   return 0;
}
