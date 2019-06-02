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
#include "messages.h"
#include "vars.h"

// mq_maxmsg não pode ser maior que o valor definido em
// /proc/sys/fs/mqueue/queues_max
//

void register_user(char *username)
{
    printf("Bem vindo ao Chat!\n");
    printf("Qual seu nome de usuário?(Máximo 10 caracteres)\n");
    scanf("%s", username);
    getchar();
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

void split_message(char *str, char *sender, char *receiver, char *msg)
{
    char *delim = ":";
    char *token;

    token = strtok(str, delim);
    sender = strdup(token);
    token = strtok(NULL, delim);
    receiver = strdup(token);
    token = strtok(NULL, delim);
    msg = strdup(token);

}

int main(int argc, char const *argv[])
{

    char *username;
    username = (char *)malloc(MAX_USERNAME_SIZE * sizeof(char));

    register_user(username);

    user_log("Inicializando CHAT!");
    char *queue_name;
    queue_name = (char *)malloc(sizeof(username) + sizeof(QUEUE_PREFIX));
    gen_queue_name(queue_name, username);

    mqd_t user_q;

    struct mq_attr attr;
    config_mq(&attr);

    user_q = create_fifo(queue_name, attr);
    user_log("CHAT inicializado com sucesso!");

    pthread_t t_receive;
    pthread_t t_send;

    debug_log("Starting thread to receive messages");
    struct msg_receive read_params;
    read_params.q = user_q;

    pthread_create(&t_receive, NULL, &verify_new_message, &read_params);

    printf("CHAT: Olá %s\n", username);

    struct msg_send send_params;
    send_params.queue_name = queue_name;
    send_params.sender = username;

    char *msg;
    msg = (char *)malloc(MAX_MSG_SIZE * sizeof(char));

    char *receiver_username;
    receiver_username = (char *)malloc(MAX_USERNAME_SIZE * sizeof(char));

    printf("Para enviar uma mensagem digite: USUARIO:Mensagem\n");

    while(1)
    {
        printf("Digite o nome do usuário de destino: \n");
        scanf("%s", receiver_username);
        getchar();
        send_params.receiver = receiver_username;

        printf("Digite a mensagem: \n");
        fgets(msg, MAX_MSG_SIZE, stdin);
        send_params.msg = msg;

        pthread_create(&t_send, NULL, &send_message, &send_params);
    }

    return 0;
}
