#ifndef AUX_H
#define AUX_H

#include "includes.h"
#include "vars.h"

void config_mq(struct mq_attr *attr)
{
    attr->mq_flags = 0;
    attr->mq_maxmsg = MAX_MSG;
    attr->mq_msgsize = MAX_MSG_SIZE;
    attr->mq_curmsgs = 0;
}

void debug_log(char *str)
{
    printf("DEBUG: %s\n\n", str);
}

void user_log(char *str)
{
    printf("CHAT: %s\n\n", str);
}

void siginit_handler(int sig_num)
{
    user_log("Chat can't be terminated using CTRL+C! Use'exit' command to quit.");
}

#endif // AUX_H
