/*
  dois processo que manda os sinal entre eles
  tratar o ctrl+c (SIGINT que fecha a parada)
  10 char para cd nome e 500 de mensagem

  user    ~> rw
  group   ~> w
  others  ~> w

  fechar: /dev/mqueue e deleta saporra
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
#define QUEUE_PERMISSIONS 0660
extern int errno;

void do_the_fork_stuff(pid_t f_pid)
{
  f_pid = fork();

  if (f_pid == 0)
  {
    // faz a comunicação lá


  }
}


int main(int argc, char const *argv[])
{
  pid_t fork_pid;

  mqd_t q1;

  struct mq_attr attr;

  attr.mq_flags = 0;
  attr.mq_maxmsg = 30;
  attr.mq_msgsize = 256; 
  attr.mq_curmsgs = 0;

  q1 = mq_open("/porra", O_RDONLY | O_CREAT, QUEUE_PERMISSIONS, &attr);

  if(q1 == -1) {
      int errnum;
      char *errmsg;

      errnum = errno;
      errmsg = strerror(errnum);
      fprintf(stderr, "Error: %s \n", errmsg);
      return -1;
  }

  char *msg = "asdf";
  char *msg2;
  msg2 = (char *) malloc(5 * sizeof(char));

  int send_return;

  send_return = mq_send(q1, msg, sizeof(msg), 1);

  if(send_return != 0) {
      printf("Error on send message to the FIFO %d\n", send_return);
      return -1;
  }

  mq_receive(q1, (void*) msg, sizeof(msg), 1);

  // do_the_fork_stuff(fork_pid);

  printf("%s\n\n", &msg2);
  return 0;
}
