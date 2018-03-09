#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>



int main(void)
{
  int msqid;
  char pathname[]="/tmp/server0";
  key_t  key;
  int i,len;

  struct mymsgbuf{
    long mtype;
    double number;
    long pid;
  } mybuf;
  /* Attach message queue  */
  
  key = ftok(pathname, 0);
  
  if ((msqid = msgget(key, 0)) < 0){
    printf("Can\'t get msqid\n");
    exit(-1);
  }
  mybuf.mtype = 2;
  len = sizeof(mybuf);
   
  if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0){
    printf("Can\'t send message to queue\n");
    msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
    exit(-1);
  }
  return 0;    
}
