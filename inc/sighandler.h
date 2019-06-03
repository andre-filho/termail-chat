#ifndef SIGHAN_H 
#define SIGHAN_H 

#include "includes.h"
#include "aux.h"

void siginit_handler(int sig_num)
{
    user_log("Chat can't be terminated using CTRL+C");
}


#endif // SIGHAN_H 
