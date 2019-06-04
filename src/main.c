#include "menu.h"
#include "vars.h"
#include "aux.h"
#include "messages.h"
#include "threading.h"
#include "includes.h"

int main(int argc, char const *argv[])
{
    user_log("Initializing...");

    signal(SIGINT, siginit_handler);

    char *username;
    username = (char *)malloc(MAX_USERNAME_SIZE * sizeof(char));

    init();
    register_user(username);

    char *queue_name;
    queue_name = (char *)malloc(sizeof(username) + sizeof(QUEUE_PREFIX));

    gen_queue_name(queue_name, username);

    mqd_t user_queue;
    struct mq_attr attr;

    config_mq(&attr);
    user_queue = create_fifo(queue_name, attr);

    user_log("Successfully initialized!");

    pthread_t t_send;
    pthread_t t_receive;

    struct msg_receive read_params;

    read_params.q = user_queue;

    pthread_create(&t_receive, NULL, &verify_new_message, &read_params);

    struct msg_send send_params;

    send_params.queue_name = queue_name;
    send_params.sender = username;

    printf("CHAT: Hello %s\n", username);
    user_log("Send a message following the <target user>:<your message>.");
    user_log("To list all online users, type 'list'.");
    user_log("To exit type: 'exit'.");

    char exit_confirmation;
    char *msg;
    char *receiver_username;
    char *raw_message_input;
    raw_message_input = (char *)malloc((MAX_MSG_SIZE + (MAX_USERNAME_SIZE * 2) * sizeof(char)));

    const char split[] = ":";
    const char *exit = "exit";
    const char *list_cmd = "list";

    while (1)
    {
        scanf("%[^\n]*c", raw_message_input);
        getchar();

        if (strcmp(raw_message_input, exit) == 0)
        {
            printf("You are about to leave the chat. Are you sure? (y/n)\n");

            scanf("%c", &exit_confirmation);
            getchar();

            if (exit_confirmation == 'y')
            {
                user_log("Exiting...");
                mq_unlink(queue_name);
                return 0;
            }
        }
        else if (strcmp(raw_message_input, list_cmd) == 0)
        {
            list_users();
        }
        else
        {
            receiver_username = strtok(raw_message_input, split);
            msg = strtok(NULL, split);

            send_params.receiver = receiver_username;
            send_params.msg = msg;

            pthread_create(&t_send, NULL, &send_message, &send_params);
        }
    }

    return 0;
}
