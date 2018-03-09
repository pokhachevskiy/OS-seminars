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
  // printf ("%d %d %d", 'i', 'v', 'p');
  
  if ((fd = open(pathname, O_RDONLY, 0)) < 0) {
    printf ("Can't find cooperator\n");
    exit(-1);
  }
  close(fd); 

  for (i = 0; i < 1; i++){
    sprintf(name, "%s%d", pathname, i);

    if ((key = ftok(name, 0)) < 0) {
      printf ("cant get key for %d\n", i);
      exit(-1);
    }
    if ((msqid = msgget(key, 0)) < 0){
      printf("Can\'t get msqid %d\n", i);
      exit(-1);
    }
  }
  int pid = getpid();
  //semaphore lock


  len = sizeof(mysem);
  int maxlen = len;

  // end of initialization
  
  /*if ((chpid = fork()) > 0) {


  }*/
  if ((shmkey = ftok(shmname, 0)) < 0) {
    printf("can't get shmkey\n");
    exit(-1);
  }
  int shmid;
  int* count;

  if ((shmid = shmget(shmkey, sizeof(int), 0666 | IPC_CREAT)) < 0) {
    printf("Can\'t get shared memory\n");
    exit(-1);
  }

  count = shmat(shmid, NULL, 0 );
  //printf ("%d", *count);



  i = 0;

  while(i < 4){
    i++;
    int chpid = fork();
    if (chpid < 0)
    {
      printf("smth is bad and can't fork\n"); 
      exit(-1);
    }
    if (chpid == 0)//child
    {
      pid = getpid();
      for (int j = 0; j < 3; j++) {

        mysem.optype = 'p';
        mysem.sNum = 0; //lock
        mysem.pid  = pid;
        printf ("im bee try to p(lock) %d\n", pid);
        if (msgsnd(msqid, (struct msgbuf *) &mysem, len, 0) < 0){
          printf("Can\'t send message to queue\n");
          msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
          exit(-1);
        }

        if (( len = msgrcv(msqid, (struct msgbuf *) &mysem, sizeof(mysem), pid, 0)) < 0){
          printf("Can\'t receive message from queue %d \n", len);
          exit(-1);
        }
        printf ("%d bee returned\n", pid);
        (*count)++;
        printf ("first bee returned, count %d\n", *count);
        if ((*count) == 6) {  
          mysem.optype = 'v';
          mysem.sNum = 1;
          mysem.pid  = pid;
          printf ("count is six!! \n");
          if (msgsnd(msqid, (struct msgbuf *) &mysem, len, 0) < 0){
            printf("Can\'t send message to queue\n");
            msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
            exit(-1);
          }
          printf ("sended!! \n");
          if (( len = msgrcv(msqid, (struct msgbuf *) &mysem, maxlen, mysem.pid, 0)) < 0){
            printf("Can\'t receive message from queue %d \n", len);
            exit(-1);
          }
          printf ("received!! \n");


        } else {
          mysem.optype = 'v';
          mysem.sNum = 0;
          mysem.pid  = pid;

          if (msgsnd(msqid, (struct msgbuf *) &mysem, len, 0) < 0){
            printf("Can\'t send message to queue\n");
            msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
            exit(-1);
          }
          printf ("sended v(lock) %d\n", mysem.pid);

          if (( len = msgrcv(msqid, (struct msgbuf *) &mysem, maxlen, mysem.pid, 0)) < 0){
            printf("Can\'t receive message from queue %d \n", len);
            exit(-1);
          }

        }
      }
    }

  }



  return 0;
}
