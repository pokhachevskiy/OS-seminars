#define _GNU_SOURCE
#include <sys/types.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char* argv[])
{
  
  int msqid[100];
  char pathname[]="/tmp/server";
  key_t*  key;
  int i,len;
  int fd;
  char name[15];

  if (argc != 2) {
    printf ("Please, call with the @param = number of semaphores\n");
    exit (-1);
  }

  int numberOfSemaphores = atoi(argv[1]);
  long *s = (long*)calloc(numberOfSemaphores, sizeof(long));
  if (!s) {
    printf ("Can't allocate memory for semaphores \n");
    exit (-1);
  }

  key = (key_t*)calloc(numberOfSemaphores, sizeof(key_t));
  if (!key) {
    printf ("Can't allocate memory for keys\n");
    exit (-1);
  }
  // printf("%d \n", numberOfSemaphores);
  
  struct mymsgbuf {
    long optype;
    long init;
    long sNum;
    long pid;
  } mysem;
  /* Attach message queue  */
  if ((fd = open(pathname, O_EXCL | O_CREAT, 0)) < 0) {
    printf ("Can't create ruling file!\n");
    exit(-1);
  }

  for (i = 0; i < numberOfSemaphores; i++){
    sprintf(name, "%s%d", pathname, i);
    printf ("here! %s \n", name);
    key[i] = ftok(name, 0);
    if ((msqid[i] = msgget(key[i], 0666 | IPC_CREAT)) < 0){
      printf("Can\'t get msqid %d\n", i);
      exit(-1);
    }
  }
  printf ("after ftoks\n");
  /* Send information */
  while (1){
    int maxlen = sizeof(mysem);

    //FIFO 
    for (i = 0; i < numberOfSemaphores; i++) {

      if (( len = msgrcv(msqid[i], (struct msgbuf *) &mysem, maxlen, 0, 0)) < 0){

       printf("Can\'t receive message from queue %d \n", len);
       exit(-1);
      }
      // killer option.
      if (mysem.optype == 2){
        printf("I'm closing\n");
        msgctl(msqid[i], IPC_RMID, (struct msqid_ds *) NULL);
        execl("/bin/rm", "rm", "-f", pathname, NULL);
        exit(-1);
      }

      if (mysem.optype == 'i') {
        printf("initialization process, sNum = %li init = %li \n", mysem.sNum, mysem.init);
        s[mysem.sNum] = mysem.init;
      }

      if (mysem.optype == 'p') {
        printf("p process, sNum = %li pid = %li \n", mysem.sNum, mysem.pid);
        if (s[i] > 0) {
          s[i] -= 1;
          mysem.optype = mysem.pid;
        } else {
          continue;
        }
      }

      if (mysem.optype == 'v') {
        printf("v process, sNum = %li pid = %li \n", mysem.sNum, mysem.pid);
        mysem.optype = mysem.pid;
        s += 1;
      }


      if (msgsnd(msqid[i], (struct msgbuf *) &mysem, len, 0) < 0){
        printf("Can\'t send message to queue\n");
        msgctl(msqid[i], IPC_RMID, (struct msqid_ds *) NULL);
        exit(-1);
      }
    }
  }

  return 0;    
}
