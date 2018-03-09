#define _GNU_SOURCE
#include <sys/types.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

const long N = 2;

int main(void)
{
  
  int msqid;
  char pathname[]="/tmp/server";
  char shmname[] = "./count";
  key_t  key;
  key_t  shmkey;
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
  
  if ((fd = open(pathname, O_RDONLY, 0)) < 0) {
    printf ("Can't find cooperator\n");
    exit(-1);
  }
  close(fd); 

  for (i = 0; i < 1; i++){
    sprintf(name, "%s%d", pathname, i);
    printf ("%s\n", name);

    if ((key = ftok(name, 0)) < 0) {
      printf ("cant get key %d \n", i);
      exit(-1);
    }

    printf ("%d \n", key);
    if ((msqid = msgget(key, 0666)) < 0){
      printf("Can\'t get msqid %d\n", i);
      exit(-1);
    }
  }
  shmkey = ftok(shmname, 0);
  int shmid;
  int* count;

  if ((shmid = shmget(shmkey, sizeof(int), 0666 | IPC_CREAT)) < 0) {
    printf("Can\'t get shared memory\n");
    exit(-1);
  }

  count = shmat(shmid, NULL, 0 );
  *count = 0;








  int pid = getpid();
  //semaphore lock
  mysem.optype = 'i';
  mysem.sNum = 0;
  mysem.pid  = pid;
  mysem.init = 1;

  len = sizeof(mysem);
  int maxlen = len;
  printf("before send it bear\n");
   
  if (msgsnd(msqid, (struct msgbuf *) &mysem, len, 0) < 0){
    printf("Can\'t send message to queue\n");
    msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
    exit(-1);
  }
  
  printf("before receive\n");
  if (( len = msgrcv(msqid, (struct msgbuf *) &mysem, maxlen, mysem.pid, 0)) < 0){
    printf("Can\'t receive message from queue %d \n", len);
    exit(-1);
  }
  printf("received\n");
  //semaphore eat
  mysem.optype = 'i';
  mysem.sNum = 1;
  mysem.pid  = pid;
  mysem.init = 0;

  if (msgsnd(msqid, (struct msgbuf *) &mysem, len, 0) < 0){
    printf("Can\'t send message to queue\n");
    msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
    exit(-1);
  }
  printf("before rcv %d \n", i);
  if (( len = msgrcv(msqid, (struct msgbuf *) &mysem, maxlen, mysem.pid, 0)) < 0){
    printf("Can\'t receive message from queue %d \n", len);
    exit(-1);
  }
  // end of initialization
  
  /*if ((chpid = fork()) > 0) {


  }*/




  i = 0;

  while(i < 2){
    i++;

    mysem.optype = 'p';
    mysem.sNum = 1;
    mysem.pid  = pid;

    if (msgsnd(msqid, (struct msgbuf *) &mysem, len, 0) < 0){
      printf("Can\'t send message to queue\n");
      msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
      exit(-1);
    }

    if (( len = msgrcv(msqid, (struct msgbuf *) &mysem, maxlen, getpid(), 0)) < 0){
      printf("Can\'t receive message from queue %d \n", len);
      exit(-1);
    }
    printf("return message after p\n");

    *count  = 0; 
    printf ("%d", *count);


    mysem.optype = 'v';
    mysem.sNum = 0;
    mysem.pid  = pid;

    if (msgsnd(msqid, (struct msgbuf *) &mysem, len, 0) < 0){
      printf("Can\'t send message to queue\n");
      msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
      exit(-1);
    }

    if (( len = msgrcv(msqid, (struct msgbuf *) &mysem, maxlen, pid, 0)) < 0){
      printf("Can\'t receive message from queue %d \n", len);
      exit(-1);
    }

  }



  return 0;
}
