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

#include "aux.h"
#include "threading.h"
#include "includes.h"
#include "vars.h"

// mq_maxmsg não pode ser maior que o valor definido em
// /proc/sys/fs/mqueue/queues_max


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
