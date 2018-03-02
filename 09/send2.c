#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>



int main(void)
{
    
    int msqid;
    char pathname[]="09-2a.c";
    key_t  key;
    int i,len;



    struct mymsgbuf
    {
       long mtype;
       char mtext[81];
    } mybuf;
    /* Create or attach message queue  */
    
    key = ftok(pathname, 0);
    
    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0){
       printf("Can\'t get msqid\n");
       exit(-1);
    }


    /* Send information */
    
    for (i = 1; i <= 5; i++)
    {   
        int maxlen = sizeof(mybuf) + 1;
       
       if (( len = msgrcv(msqid, (struct msgbuf *) &mybuf, maxlen, 1, 0)) < 0){

         printf("Can\'t receive message from queue %d \n", len);
         exit(-1);
       }
       
       printf("message type = %ld, info = %s\n", mybuf.mtype, mybuf.mtext);
    }


    for (i = 1; i <= 5; i++){
       
       mybuf.mtype = 2;
        strcpy(mybuf.mtext, "Hello2");

       len = sizeof(mybuf);
       
       if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0){
         printf("Can\'t send message to queue\n");
         msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
         exit(-1);
       }
    }

    /* Send the last message */   
       
    // mybuf.mtype = LAST_MESSAGE;   
    // len         = 0;   
       
    // if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0){
    //    printf("Can\'t send message to queue\n");
    //    msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
    //    exit(-1);
    // }

    return 0;    
}
