/*
  dois processo que manda os sinal entre eles
  tratar o ctrl+c (SIGINT que fecha a parada)
  10 char para cd nome e 500 de mensagem

  user    ~> rw
  group   ~> w
  others  ~> w

  fechar: /dev/mqueue e deleta saporra

  http://wiki.inf.ufpr.br/maziero/lib/exe/fetch.php?media=socm:socm-texto-09.pdf
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

#define CREATOR_PERMISSON 0662
#define CONSUMER_PERMISSON 0222
#define W_PERMISSON 0
#define QUEUE_PREFIX "/chat-"
#define MAX_USERNAME_SIZE 10
#define MAX_MSG_SIZE 512
#define MAX_MSG 10
#define FIFO_NAME "/fifo"

// mq_maxmsg não pode ser maior que o valor definido em
// /proc/sys/fs/mqueue/queues_max

extern int errno;

struct msg_args
{
  mqd_t q;
  char *msg;
};

void debug_log(char *str)
{
  printf("DEBUG: %s\n", str);
}

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

char *gen_queue_name(char *name)
{
  return strcat(QUEUE_PREFIX, name);
}

void config_mq(struct mq_attr *attr)
{
  attr->mq_flags = 0;
  attr->mq_maxmsg = MAX_MSG;
  attr->mq_msgsize = MAX_MSG_SIZE;
  attr->mq_curmsgs = 0;
}

int main(int argc, char const *argv[])
{

  mqd_t q1;

  pthread_t t_receive;
  pthread_t t_send;

  char *msg = "olá tudo bem";
  char msg2[8200];

  struct mq_attr attr;

  config_mq(&attr);

  debug_log("Creating FIFO");
  q1 = mq_open(FIFO_NAME, O_RDWR | O_CREAT, 0666, &attr);

  if (q1 == -1)
  {
    perror("mq_open");
    return -1;
  }

  if (mq_getattr(q1, &attr) == -1)
  {
    perror("mq_getattr");
    return -1;
  }

  printf("Maximum # of messages on queue: %ld\n", attr.mq_maxmsg);
  printf("Maximum message size: %ld\n", attr.mq_msgsize);

  struct msg_args read_params;

  read_params.q = q1;
  read_params.msg = msg2;

  debug_log("Starting thread to send message");
  pthread_create(&t_send, NULL, &send_message, msg);

  debug_log("Starting thread to receive messages");
  pthread_create(&t_receive, NULL, &verify_new_message, &read_params);

  printf("\n\n");

  /* mq_unlink(FIFO_NAME); */

  return 0;
}
