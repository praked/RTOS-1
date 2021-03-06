#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <time.h>

#define SERVER 1L
typedef struct {
    long    msg_to;
    long    msg_fm;
    char    buffer[BUFSIZ];
    
} MESSAGE;
 
int mid;
key_t key;
key_t rkey;
struct msqid_ds buf;
MESSAGE msg;
FILE *inFile;
 
int main(int argc, char** argv) {
 
    //Aquiring Message Queue ID
    key = ftok(".", 'z');
    mid = msgget(key, 0);
    //Display Message Queue and Client ID
    printf("Message Queue ID: %d\n", mid);
    //msg.cmid = mid;
    printf("Client ID: %ld\n", (long)getpid());
    /*
    //Opeining input file, throw an error if invalid file
    inFile = fopen(argv[1], "r");
    if(inFile == NULL){
        printf("Unable to open File = %s\n", argv[1]);
        return 1;
    }
 
    //Copy input characters into msg.buffer, loops breaks when EOF is reached
    int i = 0;
    while(1){
        msg.buffer[i] = fgetc(inFile);
        if(msg.buffer[i]==EOF){
            msg.buffer[i] = '\0';
            break;
        }
        i++;
    }
    */
    while(1){

        printf("Message to server: ");
        scanf(" %[^\n]",msg.buffer);
        //Displaying message before conversion of server
        //printf("%s\n", msg.buffer);
     
        //Getting Client PID and preparing message to message queue
        long iD = (long)getpid();
        msg.msg_to = SERVER;
        msg.msg_fm = (long)getpid();
        
        //Send message to Message Queue for Server, throws and error for invalid input
        if(msgsnd(mid, &msg, sizeof(msg.buffer),IPC_NOWAIT) >= 0){
            clock_t t = clock();
            msgrcv(mid, &msg, sizeof(msg), iD, 0);
            t = clock() - t;
            //Display new converting message.
            printf("Message from server: ");
            printf("%s\n", msg.buffer);
            printf ("\n\n>>> RTT is : %f seconds\n", ((double)t)/CLOCKS_PER_SEC );
            printf("==============================================\n");

        }
     
        

    }
 
    //Removing message queue
    msgctl(mid, IPC_RMID, (struct msqid_ds *) 0);
 
    //Client exits
    return (EXIT_SUCCESS);
}