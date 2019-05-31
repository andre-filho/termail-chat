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

#define QUEUE_PERMISSIONS 0666
#define QUEUE_PREFIX "/chat-"
#define MAX_USERNAME_SIZE 10
#define MAX_MSG_SIZE 512

// mq_maxmsg não pode ser maior que o valor definido em
// /proc/sys/fs/mqueue/queues_max
#define MAX_MSG 10

extern int errno;

struct verify_msg_args {
    mqd_t q;
    char *msg;
};

void debug_log(char * str)
{
  printf("DEBUG: %s\n", str);
}

void * verify_new_message(void *params)
{
  debug_log("Veryfing new messages");
  // TODO: colocar condição para parar de ler
  struct verify_msg_args *args = (struct verify_msg_args *) params;
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

void * send_message(void *params)
{

  
}

char *gen_queue_name(char *name)
{
  return strcat(QUEUE_PREFIX, name);
}

void config_mq(struct mq_attr attr)
{
  // TODO: Para essa função fucionar precisa passar uma referência para o
  // attr ou retorna-lo
  attr.mq_flags = 0;
  attr.mq_maxmsg = MAX_MSG;
  attr.mq_msgsize = MAX_MSG_SIZE;
  attr.mq_curmsgs = 0;

}


int main(int argc, char const *argv[])
{

  mqd_t q1;

  pthread_t t_receive;

  /* const char *msg = "asdf"; */
  char msg = 'a';
  char msg2[8200];

  struct mq_attr attr;

  attr.mq_maxmsg = MAX_MSG;
  attr.mq_msgsize = MAX_MSG_SIZE;
  attr.mq_flags = 0;

  debug_log("Creating FIFO");
  q1 = mq_open("/porra", O_RDWR | O_CREAT, QUEUE_PERMISSIONS, &attr);

  if (q1 == -1)
  {
    perror("mq_open");
    return -1;
  }

  if(mq_getattr(q1, &attr) == -1)
  {
    perror("mq_getattr");
    return -1;
  }

  printf("Maximum # of messages on queue: %ld\n", attr.mq_maxmsg);
  printf("Maximum message size: %ld\n", attr.mq_msgsize);

  struct verify_msg_args params;

  params.q = q1;
  params.msg = msg2;

  mqd_t q_send;

  q_send = mq_open("/porra", O_RDWR);

  debug_log("antes do send");
  if((mq_send(q_send, (char *)&msg, sizeof(msg), 1)) != 0)
  {
    perror("mq_send");
    return -1;
  }
  debug_log("Mensagem enviada");
  mq_close(q_send);

  debug_log("Starting thread to receive messages");
  pthread_create(&t_receive, NULL, (void *) &verify_new_message, &params);
  /* q1 = mq_open("/porra", O_RDWR, QUEUE_PERMISSIONS, &attr); */

  /* debug_log("antes do verify"); */
  /* verify_new_message(q1, msg2); */

  /* if ((mq_receive(q1, (void *)msg2, sizeof(msg2), (unsigned int *)1) < 0)) */
  /* { */
  /*   print_error(); */
  /*   return -1; */
  /* } */

  printf("%s\n\n", msg2);

  while(1);

  return 0;
}
