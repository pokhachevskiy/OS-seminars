#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h> //-lm

double result[4];
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
   
   mythid = pthread_self();

   a = a+1;

   printf("Thread %u, Calculation result = %d\n", mythid, a);

   return NULL;
}

int main()
{
   pthread_t thid[3], mythid; 
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
