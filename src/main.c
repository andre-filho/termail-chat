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
//

void register_user(char *username)
{
    printf("Bem vindo ao Chat!\n");
    printf("Qual seu nome de usuário?(Máximo 10 caracteres)\n");
    scanf("%s", username);
}

void create_fifo(char *username, mqd_t *user_q, struct mq_attr attr)
{

    // WIP

    char *queue_name;
    user_q = mq_open(FIFO_NAME, O_RDWR | O_CREAT, 0666, &attr);

}


int main(int argc, char const *argv[])
{

    char *username;
    username = (char *)malloc(MAX_USERNAME_SIZE * sizeof(char));

    register_user(username);

    mqd_t user_q;

    struct mq_attr attr;
    config_mq(&attr);

    printf("CHAT: Olá %s\n", username);

    char *queue_name;
    queue_name = (char *)malloc(sizeof(username) + sizeof(QUEUE_PREFIX));
    gen_queue_name(queue_name, username);

    printf("%s\n", queue_name);

    mqd_t q1;

    pthread_t t_receive;
    pthread_t t_send;

    char *msg = "olá tudo bem";
    char msg2[8200];

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
