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

#define QUEUE_PERMISSIONS 0666
#define QUEUE_PREFIX "/chat-"
#define MAX_USERNAME_SIZE 10
#define MAX_MSG_SIZE 500

extern int errno;

int print_error()
{
  int errnum;
  char *errmsg;

  errnum = errno;
  errmsg = strerror(errnum);
  fprintf(stderr, "Error: %s \n", errmsg);

  return -1;
}

void * verify_new_message(mqd_t q1, char *msg)
{
  printf("Veryfing new messages");
  // TODO: colocar condição para parar de ler
  for (;;)
  {
    if ((mq_receive(q1, (void *)&msg, sizeof(msg), 0)) < 0)
    {
      perror("mq_recieve:");
      exit(1);
    }
    printf("Recieved msg value: %s\n", msg);
  }
}

void * send_message()
{
  
}

char *gen_queue_name(char *name)
{
  return strcat(QUEUE_PREFIX, name);
}

void config_mq(struct mq_attr attr)
{
  attr.mq_flags = 0;
  attr.mq_maxmsg = 30;
  attr.mq_msgsize = sizeof(MAX_MSG_SIZE);
  attr.mq_curmsgs = 0;
}

int main(int argc, char const *argv[])
{
  pid_t fork_pid;

  mqd_t q1;

  const char *msg = "asdf";
  char *msg2;
  msg2 = (char *)malloc(5 * sizeof(char));

  struct mq_attr attr;

  config_mq(attr);

  q1 = mq_open("/porra", O_RDWR | O_CREAT, QUEUE_PERMISSIONS, &attr);

  printf("DEBUG: open\n");
  if (q1 == -1)
  {
    print_error();
    return -1;
  }

  int send_return;

  printf("DEBUG: antes do send\n");
  send_return = mq_send(q1, (char *)&msg, strlen(msg), 0);

  mq_close(q1);

  if (send_return != 0)
  {
    perror("mq_send");
    return -1;
  }
  q1 = mq_open("/porra", O_RDWR, QUEUE_PERMISSIONS, &attr);

  printf("DEBUG: antes do verify\n");
  verify_new_message(q1, msg2);

  if ((mq_receive(q1, (void *)msg2, sizeof(msg2), (unsigned int *)1) < 0))
  {
    print_error();
    return -1;
  }

  printf("%s\n\n", msg2);

  return 0;
}
