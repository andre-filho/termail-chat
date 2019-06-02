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

mqd_t create_fifo(char *queue_name, struct mq_attr attr)
{
    mqd_t q;
    q = mq_open(queue_name, O_RDWR | O_CREAT, 0666, &attr);

    if (q == -1)
    {
        perror("mq_open");
        exit(1);
    }

    return q;
}


int main(int argc, char const *argv[])
{

    char *username;
    username = (char *)malloc(MAX_USERNAME_SIZE * sizeof(char));

    register_user(username);

    printf("Inicializando CHAT!\n");
    char *queue_name;
    queue_name = (char *)malloc(sizeof(username) + sizeof(QUEUE_PREFIX));
    gen_queue_name(queue_name, username);

    mqd_t user_q;

    struct mq_attr attr;
    config_mq(&attr);

    user_q = create_fifo(queue_name, attr);
    printf("CHAT inicializado com sucesso!\n");

    pthread_t t_receive;
    pthread_t t_send;

    debug_log("Starting thread to receive messages");
    struct msg_receive read_params;
    read_params.q = user_q;

    pthread_create(&t_receive, NULL, &verify_new_message, &read_params);

    printf("CHAT: Olá %s\n", username);

    struct msg_send send_params;
    send_params.queue_name = queue_name;

    while(1)
    {
        char *msg = "olá tudo bem";
        send_params.msg = msg;
        pthread_create(&t_send, NULL, &send_message, &send_params);
        sleep(10);
    }

    /* mqd_t q1; */

    /* pthread_t t_receive; */
    /* pthread_t t_send; */

    /* char *msg = "olá tudo bem"; */
    /* char msg2[8200]; */

    /* debug_log("Creating FIFO"); */
    /* q1 = mq_open(FIFO_NAME, O_RDWR | O_CREAT, 0666, &attr); */

    /* if (q1 == -1) */
    /* { */
    /*     perror("mq_open"); */
    /*     return -1; */
    /* } */

    /* struct msg_args read_params; */
    /* read_params.q = q1; */
    /* read_params.msg = msg2; */

    /* debug_log("Starting thread to send message"); */
    /* pthread_create(&t_send, NULL, &send_message, msg); */

    /* debug_log("Starting thread to receive messages"); */
    /* pthread_create(&t_receive, NULL, &verify_new_message, &read_params); */

    /* printf("\n\n"); */

    /* /1* mq_unlink(FIFO_NAME); *1/ */

    return 0;
}
