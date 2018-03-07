#define _GNU_SOURCE
#include <sys/types.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

const long N = 1;

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
  
  /* Send information */
/*
  mysem.optype = 1;
  double temp;
  scanf("%lf", &temp);
  
  mysem.pid = getpid();
  mysem.number = temp; 

  len = sizeof(mybuf);
   
  if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0){
    printf("Can\'t send message to queue\n");
    msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
    exit(-1);
   }

  int maxlen = sizeof(mybuf) + 1;
   
  if (( len = msgrcv(msqid, (struct msgbuf *) &mybuf, maxlen, mysem.pid, 0)) < 0){
    printf("Can\'t receive message from queue %d \n", len);
    exit(-1);
   }
   
  printf("message type = %ld, info = %lf\n, pid %ld", mysem.optype, mysem.number, mysem.pid); */
  return 0;    
}
