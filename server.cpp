#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include <thread>

#include <fstream>

#define CENTRAL_MAILBOX 1200    //Central Mailbox number 
#define MSGTXTLEN 128   // msg text length

struct MessageBody{
    long mtype;
    char mtext[MSGTXTLEN];
};

//MAIN function
int main() {
    printf("\nStarting Server...\n");
    fflush(stdout);

    // Set up local variables
    int result, length, status;

    // Create the Central Servers Mailbox
    int msqidA = msgget(CENTRAL_MAILBOX, 0666 | IPC_CREAT);

    // Create the mailboxes for the other processes and store their IDs
    int msqidB = msgget((CENTRAL_MAILBOX + 1), 0666 | IPC_CREAT);

    MessageBody data;
    data.mtype = 1; // set the type of message

    /* The length is essentially the size
       of the structure minus sizeof(mtype) */
    length = sizeof(data) - sizeof(MessageBody::mtype);

    // While the processes have different temperatures
    while (true){
        // Get new messages from the processes
        result = msgrcv(msqidA, &data, length, 1, 0);

        if (result < 0) {
            printf("msgrcv failed, rc=%d\n", result);
            return 1;
        }

        printf("Message received with code: %c\n", data.mtext[0]);
        fflush(stdout);

        data.mtext[0] = 'b';
        result = msgsnd(msqidB, &data, length, 0);

        if (result < 0) {
            printf("msgsnd failed, rc = %d\n", result);
            return 1;
        }
    }

    printf("\nShutting down Server...\n");

    // Remove the mailbox
    status = msgctl(msqidA, IPC_RMID, 0);
    status = msgctl(msqidB, IPC_RMID, 0);

    // Validate nothing when wrong when trying to remove mailbox
    if (status != 0){
        printf("\nERROR closing mailbox\n");
    }
}
