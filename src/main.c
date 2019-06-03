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

#include "menu.h"
#include "vars.h"
#include "aux.h"
#include "messages.h"
#include "threading.h"
#include "includes.h"

// mq_maxmsg não pode ser maior que o valor definido em
// /proc/sys/fs/mqueue/queues_max
//

int main(int argc, char const *argv[])
{
    char *username;
    username = (char *)malloc(MAX_USERNAME_SIZE * sizeof(char));

    init();
    register_user(username);

    user_log("Initializing...");
    char *queue_name;
    queue_name = (char *)malloc(sizeof(username) + sizeof(QUEUE_PREFIX));

    gen_queue_name(queue_name, username);

    mqd_t user_q;

    struct mq_attr attr;
    config_mq(&attr);

    user_q = create_fifo(queue_name, attr);
    user_log("Successfully initialized!");

    pthread_t t_receive;
    pthread_t t_send;

    // debug_log("Starting thread to receive messages");
    struct msg_receive read_params;
    read_params.q = user_q;

    pthread_create(&t_receive, NULL, &verify_new_message, &read_params);

    printf("CHAT: Hello %s\n", username);

    struct msg_send send_params;
    send_params.queue_name = queue_name;
    send_params.sender = username;

    char *msg;
    msg = (char *)malloc(MAX_MSG_SIZE * sizeof(char));

    char *receiver_username;
    receiver_username = (char *)malloc(MAX_USERNAME_SIZE * sizeof(char));

    char *all;
    all = (char *)malloc((MAX_MSG_SIZE + (MAX_USERNAME_SIZE * 2) * sizeof(char)));

    char split[] = ":";
    while (1)
    {
        scanf("%[^\n]*c", all);
        getchar();

        username = strtok(all, split);
        receiver_username = strtok(NULL, split);
        msg = strtok(NULL, split);

        send_params.receiver = receiver_username;
        send_params.msg = msg;

        pthread_create(&t_send, NULL, &send_message, &send_params);
    }

    return 0;
}
