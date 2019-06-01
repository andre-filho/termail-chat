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
  printf("DEBUG: %s\n", str);
}

char *gen_queue_name(char *name)
{
  return strcat(QUEUE_PREFIX, name);
}

#endif // AUX_H
