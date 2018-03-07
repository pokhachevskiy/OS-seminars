#define _GNU_SOURCE
#include <sys/types.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

const long N = 2;

int main(void)
{
  
  int msqid[100];
  char pathname[]="/tmp/server";
  key_t* key;
  int i,len;
  int fd;
  char name[15];

  struct mymsgbuf {
    long optype;
    long init;
    long sNum;
    long pid;
  } mysem;
  /* Create or attach message queue  */
  key = (key_t*)calloc(N, sizeof(key_t));
  if (!key) {
    printf ("Can't allocate memory for keys\n");
    exit (-1);
  }
  
  if ((fd = open(pathname, O_RDONLY, 0)) < 0) {
    printf ("Can't find cooperator\n");
    exit(-1);
  }
  close(fd); 

  for (i = 0; i < N; i++){
    sprintf(name, "%s%d", pathname, i);

    key[i] = ftok(name, 0);
    if ((msqid[i] = msgget(key[i], 0666)) < 0){
      printf("Can\'t get msqid %d\n", i);
      exit(-1);
    }
  }
  int pid = getpid();
  //semaphore lock
  mysem.optype = 'i';
  mysem.sNum = 0;
  mysem.pid  = pid;
  mysem.init = 1;

  len = sizeof(mysem);
  int maxlen = len;
   
  if (msgsnd(msqid[0], (struct msgbuf *) &mysem, len, 0) < 0){
    printf("Can\'t send message to queue\n");
    msgctl(msqid[0], IPC_RMID, (struct msqid_ds *) NULL);
    exit(-1);
  }
  
  if (( len = msgrcv(msqid[0], (struct msgbuf *) &mysem, maxlen, mysem.pid, 0)) < 0){
    printf("Can\'t receive message from queue %d \n", len);
    exit(-1);
  }

  //semaphore eat
  mysem.optype = 'i';
  mysem.sNum = 1;
  mysem.pid  = pid;
  mysem.init = 0;

  if (msgsnd(msqid[1], (struct msgbuf *) &mysem, len, 0) < 0){
    printf("Can\'t send message to queue\n");
    msgctl(msqid[1], IPC_RMID, (struct msqid_ds *) NULL);
    exit(-1);
  }
  printf("before rcv %d \n", i);
  if (( len = msgrcv(msqid[1], (struct msgbuf *) &mysem, maxlen, mysem.pid, 0)) < 0){
    printf("Can\'t receive message from queue %d \n", len);
    exit(-1);
  }
  // end of initialization
  
  /*if ((chpid = fork()) > 0) {


  }*/




  while(1){
    mysem.optype = 'p';
    mysem.sNum = 1;
    mysem.pid  = pid;

    if (msgsnd(msqid[1], (struct msgbuf *) &mysem, len, 0) < 0){
      printf("Can\'t send message to queue\n");
      msgctl(msqid[1], IPC_RMID, (struct msqid_ds *) NULL);
      exit(-1);
    }

    if (( len = msgrcv(msqid[1], (struct msgbuf *) &mysem, maxlen, mysem.pid, 0)) < 0){
      printf("Can\'t receive message from queue %d \n", len);
      exit(-1);
    }


    int count  = 0;


    mysem.optype = 'v';
    mysem.sNum = 0;
    mysem.pid  = pid;

    if (msgsnd(msqid[0], (struct msgbuf *) &mysem, len, 0) < 0){
      printf("Can\'t send message to queue\n");
      msgctl(msqid[0], IPC_RMID, (struct msqid_ds *) NULL);
      exit(-1);
    }

    if (( len = msgrcv(msqid[0], (struct msgbuf *) &mysem, maxlen, mysem.pid, 0)) < 0){
      printf("Can\'t receive message from queue %d \n", len);
      exit(-1);
    }

  }



  return 0;
}
