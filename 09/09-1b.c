#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define LAST_MESSAGE 255

int main(void)
{
    int msqid;
    char pathname[]="09-1a.c";
    key_t  key;
    int len, maxlen;

    typedef struct 
    {
      char c[1];
      int i;
      double d;
    } myst;

    struct mymsgbuf
    {
       long mtype;
       char mtext[81];
    } mybuf1;
    struct newmsgbuf
    {
      long mtype;
      myst a;
    } mybuf;
    
    key = ftok(pathname, 0);
    
    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0){
       printf("Can\'t get msqid\n");
       exit(-1);
    }
    
    while (1) {
       
       maxlen = sizeof(myst);
       
       if (( len = msgrcv(msqid, (struct msgbuf *) &mybuf, maxlen, 0, 0)) < 0){

         printf("Can\'t receive message from queue %d \n", len);
         exit(-1);
       }

       if (mybuf.mtype == LAST_MESSAGE) {
          msgctl(msqid, IPC_RMID, (struct msqid_ds *)NULL);
          exit(0);
       }
       
       printf("message type = %ld, info = %c %d %lf\n", mybuf.mtype, mybuf.a.c[0], mybuf.a.i, mybuf.a.d);
    }    

    return 0;       
}
