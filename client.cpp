#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/errno.h>

#include <fstream>

#define CENTRAL_MAILBOX 1200    //Central Mailbox number
#define MSGTXTLEN 128   // msg text length

struct MessageBody{
    long mtype;
    char mtext[MSGTXTLEN];
};

//MAIN function
int main() {
    //Setup local variables
    int result, length;

    MessageBody data;
    fflush(stdout);

    // Create the Central Servers Mailbox
    int msqidA = msgget(CENTRAL_MAILBOX, 0666 | IPC_CREAT);
    if (msqidA < 0) {
        printf("failed to create message queue with msgqid = %d\n", msqidA);
        return 1;
    }

    // Create the mailbox for this process and store it's IDs
    int msqidB = msgget((CENTRAL_MAILBOX + 1), 0666 | IPC_CREAT);
    if (msqidB < 0) {
        printf("failed to create message queue with msgqid = %d\n", msqidB);
        return 1;
    }

    //Initialize the message to be sent
    data.mtext[0] = 'a';
    data.mtype = 1; // set the type of message

    /* The length is essentially the size of
       the structure minus sizeof(mtype) */
    length = sizeof(data) - sizeof(MessageBody::mtype);

    // Send the current temp to the central server
    result = msgsnd(msqidA, &data, length, 0);

    if (result < 0) {
        printf("msgsnd failed, rc = %d\n", result);
        return 1;
    }

    //Wait for a new message from the central server
    result = msgrcv(msqidB, &data, length, 1, 0);

    if (result < 0) {
        printf("msgrcv failed, rc=%d\n", result);
        return 1;
    }

    char received_data = data.mtext[0];
    printf("\nReceived data: %c\n", received_data);
    fflush(stdout);
}
