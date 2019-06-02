#if !defined(THREDAING_H)
#define THREDAING_H

#include "vars.h"
#include "includes.h"

extern int errno;

struct msg_send
{
    char *queue_name;
    char *msg;
} msg_send;

struct msg_receive
{
    mqd_t q;
} msg_receive;

void *verify_new_message(void *params)
{
    debug_log("Veryfing new messages");
    struct msg_receive *args = (struct msg_receive *)params;

    char *msg;
    msg = (char *)malloc(MAX_MSG_SIZE * sizeof(char));

    for (;;)
    {
        if ((mq_receive(args->q, msg, MAX_MSG_SIZE, NULL)) < 0)
        {
            perror("mq_receive");
            exit(1);
        }
        printf("Recieved msg value: %s\n", msg);
    }
    pthread_exit(NULL);
}

void *send_message(void *params)
{
    debug_log("Sending message");
    struct msg_send *args = (struct msg_send *) params;

    mqd_t q_send;
    q_send = mq_open(args->queue_name, O_RDWR);

    if (q_send == -1)
    {
        perror("mq_open");
        exit(1);
    }

    if ((mq_send(q_send, args->msg, MAX_MSG_SIZE, 1)) != 0)
    {
        perror("mq_send");
        exit(1);
    }

    mq_close(q_send);
    pthread_exit(NULL);
}

#endif // THREADING_H
