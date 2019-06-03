#ifndef SIGHAN_H 
#define SIGHAN_H 

#include "includes.h"
#include "aux.h"

void siginit_handler(int sig_num)
{
    user_log("Chat can't be terminated using CTRL+C! Use'exit' command to quit.");
}


#endif // SIGHAN_H 
