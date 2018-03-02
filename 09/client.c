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

  struct mymsgbuf
  {
    long mtype;
    double number;
    long pid;
  } mybuf;
  /* Create or attach message queue  */
  
  key = ftok(pathname, 0);
  
  if ((msqid = msgget(key, 0666) < 0){
    printf("Can\'t get msqid\n");
    exit(-1);
  }


  /* Send information */

  mybuf.mtype = 1;
  double temp;
  scanf("%lf", &temp);
  
  mybuf.pid = getpid();
  mybuf.number = temp; 

  len = sizeof(mybuf);
   
  if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0){
    printf("Can\'t send message to queue\n");
    msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
    exit(-1);
   }

  int maxlen = sizeof(mybuf) + 1;
   
  if (( len = msgrcv(msqid, (struct msgbuf *) &mybuf, maxlen, mybuf.pid, 0)) < 0){
    printf("Can\'t receive message from queue %d \n", len);
    exit(-1);
   }
   
  printf("message type = %ld, info = %lf\n, pid %ld", mybuf.mtype, mybuf.number, mybuf.pid);
  return 0;    
}
