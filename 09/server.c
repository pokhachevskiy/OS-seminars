#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>



int main(void)
{
  
  int msqid;
  char pathname[]="/tmp/server";
  key_t  key;
  int i,len;

  struct mymsgbuf {
    long mtype;
    double number;
    long pid;
  } mybuf;
  /* Attach message queue  */
  
  key = ftok(pathname, 0);
  
  if ((msqid = msgget(key, 0666 | IPC_CREAT | IPC_EXCL)) < 0){
    printf("Can\'t get msqid\n");
    exit(-1);
  }


  /* Send information */
  

  while (1){
    int maxlen = sizeof(mybuf) + 1;

    if (( len = msgrcv(msqid, (struct msgbuf *) &mybuf, maxlen, -2, 0)) < 0){

     printf("Can\'t receive message from queue %d \n", len);
     exit(-1);
    }
    if (mybuf.mtype == 2){
      printf("I'm closing\n");
      msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
      exit(-1);
    }

    mybuf.number =  mybuf.number * mybuf.number;
    mybuf.mtype = mybuf.pid;
    len = sizeof(mybuf);

    if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0){
      printf("Can\'t send message to queue\n");
      msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
      exit(-1);
    }
  }
  
  return 0;    
}
