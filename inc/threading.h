#if !defined(THREDAING_H)
#define THREDAING_H

#include "vars.h"
#include "includes.h"

extern int errno;

struct msg_args
{
  mqd_t q;
  char *msg;
};

void *verify_new_message(void *params)
{
  debug_log("Veryfing new messages");
  struct msg_args *args = (struct msg_args *)params;
  for (;;)
  {
    if ((mq_receive(args->q, (void *)args->msg, MAX_MSG_SIZE, NULL)) < 0)
    {
      perror("mq_receive");
      exit(1);
    }
    printf("Recieved msg value: %s\n", args->msg);
  }
  pthread_exit(NULL);
}

void *send_message(void *msg)
{
  debug_log("Sending message");
  /* struct msg_args *args = (struct msg_args *) params; */
  msg = (char *)msg;

  mqd_t q_send;
  q_send = mq_open(FIFO_NAME, O_RDWR);

  if (q_send == -1)
  {
    perror("mq_open");
    exit(1);
  }

  if ((mq_send(q_send, msg, MAX_MSG_SIZE, 1)) != 0)
  {
    perror("mq_send");
    exit(1);
  }

  mq_close(q_send);
  pthread_exit(NULL);
}

#endif // THREADING_H
