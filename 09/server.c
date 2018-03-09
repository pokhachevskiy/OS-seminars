#define _GNU_SOURCE
#include <sys/types.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "queue.c"


int main(int argc, char* argv[])
{
  struct queue arr[2];
  for (int i = 0; i < 2; i++) {
    arr[i].start = 0;
    arr[i].finish = 0;
  }
  int msqid;
  char pathname[]="/tmp/server";
  key_t  key;
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
  // printf("%d \n", numberOfSemaphores);
  
  struct mymsgbuf {
    long optype;
    long init;
    long sNum;
    long pid;
  } mysem1, mysem;
  /* Attach message queue  */
  if ((fd = open(pathname, O_EXCL | O_CREAT, 0666)) < 0) {
    printf ("Can't create ruling file!\n");
    exit(-1);
  }
  close(fd);

  sprintf(name, "%s%d", pathname, 0);
  printf ("here! %s \n", name);
  key = ftok(name, 0);
  printf ("key = %d\n", key);  
  if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0){
    printf("Can\'t get msqid %d\n", i);
    exit(-1);
  }
  printf ("after ftoks\n");
  char op;
  /* Send information */
  long pid = getpid();
  while (1){

    int maxlen = sizeof(mysem);
    i = 0;
    //FIFO 
      // printf("%li \n", s[0]);

    if (( len = msgrcv(msqid, (struct msgbuf *) &mysem, maxlen, -'v', 0)) < 0){

     printf("Can\'t receive message from queue %d \n", len);
     exit(-1);
    }
    op = mysem.optype;
    //printf("%li \n", s[0]);
    // killer option.
    if (mysem.optype == 2){
      printf("I'm closing\n");
      msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
      execl("/bin/rm", "rm", "-f", pathname, NULL);
      execl("./clean.sh", "./clean.sh", NULL);
      exit(-1);
    }

    else if (mysem.optype == 'i') {
      printf("initialization process, sNum = %li s = %li init = %li \n", mysem.sNum, s[mysem.sNum], mysem.init);
      mysem.optype = mysem.pid;
      s[mysem.sNum] = mysem.init;

      if (msgsnd(msqid, (struct msgbuf *) &mysem, len, 0) < 0){
        printf("Can\'t send message to queue\n");
        msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
        exit(-1);
      }
      printf ("process %c for pid %d done\n", op, mysem.optype);
      // printf("before %d \n", i);
    }

    else if (mysem.optype == 'p') {
      printf("try p process, s[%li] = %li pid = %li \n", mysem.sNum, s[mysem.sNum], mysem.pid);
      if (s[mysem.sNum] > 0) {
        s[mysem.sNum] -= 1;
        mysem.optype = mysem.pid;

        if (msgsnd(msqid, (struct msgbuf *) &mysem, len, 0) < 0){
          printf("Can\'t send message to queue\n");
          msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
          exit(-1);
        }
        printf ("process %c for pid %d done\n", op, mysem.optype);
      } else {
        push(&arr[mysem.sNum], mysem.pid);
        printf ("pushed to queue pid = %li sNum = %li\n", mysem.pid, mysem.sNum);
      }
    }

    else if (mysem.optype == 'v') {
      printf("try v process, s[%li] = %li pid = %li \n", mysem.sNum, s[mysem.sNum], mysem.pid);
      mysem.optype = mysem.pid;
      if (msgsnd(msqid, (struct msgbuf *) &mysem, len, 0) < 0){
        printf("Can\'t send message to queue\n");
        msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
        exit(-1);
      }


      if (size( &arr[mysem.sNum]) != 0) {
        mysem.optype = pop(&arr[mysem.sNum]);
        printf ("sem %d pid %d \n", mysem.sNum, mysem.optype);
      } else {
        s[mysem.sNum] += 1;
        continue;
      }
      

      if (msgsnd(msqid, (struct msgbuf *) &mysem, len, 0) < 0){
        printf("Can\'t send message to queue\n");
        msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
        exit(-1);
      }
      printf ("process %c for pid %d done\n", op, mysem.pid);

      // printf("before msg snd %d \n", i);
      
      
    }
  }

  return 0;    
}
