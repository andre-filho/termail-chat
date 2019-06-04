#if !defined(THREDAING_H)
#define THREDAING_H

#include "vars.h"
#include "includes.h"

extern int errno;

struct msg_send
{
    char *queue_name;
    char *msg;
    char *sender;
    char *receiver;
};

struct msg_receive
{
    mqd_t q;
};

void *verify_new_message(void *params)
{
    struct msg_receive *args = (struct msg_receive *)params;

    char *sender;
    char *receiver;
    char *msg;
    msg = (char *)malloc(MAX_MSG_SIZE * sizeof(char));

    const char delim[2] = ":";

    for (;;)
    {
        if ((mq_receive(args->q, msg, MAX_MSG_SIZE, NULL)) < 0)
        {
            perror("mq_receive");
            exit(1);
        }

        sender = strtok(msg, delim);
        receiver = strtok(NULL, delim);
        msg = strtok(NULL, delim);

        printf("%s: %s\n", sender, msg);
    }
    free(msg);
    pthread_exit(NULL);
}

void *send_message(void *params)
{
    struct msg_send *args = (struct msg_send *)params;

    char *queue_name;
    queue_name = (char *)malloc(MAX_USERNAME_SIZE * sizeof(char) + sizeof(QUEUE_PREFIX));

    gen_queue_name(queue_name, args->receiver);

    mqd_t q_send;
    q_send = mq_open(queue_name, O_WRONLY);

    if (q_send == -1)
    {
        if (errno == ENOENT)
        {
            printf("ERROR: UNKOWNUSER %s\n", args->receiver);

            free(queue_name);
            pthread_exit(NULL);
        }
        perror("send_message -> mq_open");
        exit(1);
    }

    char *message;
    message = (char *)malloc(FULL_MSG_SIZE * sizeof(char));

    snprintf(message, FULL_MSG_SIZE, "%s:%s:%s", args->sender, args->receiver, args->msg);

    if ((mq_send(q_send, message, MAX_MSG_SIZE, 1)) == -1)
    {
        if (errno == EAGAIN)
        {
            int tries = 3;

            for (; tries > 0; tries--)
            {
                if ((mq_send(q_send, message, MAX_MSG_SIZE, 1)) == 0)
                {
                    break;
                }
                sleep(3);
            }
            if (tries == 0)
            {
                printf("ERRO: %s\n", message);

                mq_close(q_send);

                free(message);
                free(queue_name);

                pthread_exit(NULL);
            }
        }
        else
        {
            perror("send_message -> mq_send");
            exit(1);
        }
    }

    mq_close(q_send);

    free(message);
    free(queue_name);

    pthread_exit(NULL);
}

#endif // THREADING_H
