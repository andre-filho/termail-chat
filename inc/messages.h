#if !defined(MESSAGES_H)
#define MESSAGES_H

#include "includes.h"

void gen_queue_name(char *queue_name, char *username)
{
  strcpy(queue_name, QUEUE_PREFIX);
  strcat(queue_name, username);
}

void register_user(char *username)
{
  printf("Please enter your username: (Max 10 characters)\n");
  scanf("%s", username);
  getchar();
}

mqd_t create_fifo(char *queue_name, struct mq_attr attr)
{
  mqd_t q;
  q = mq_open(queue_name, O_RDWR | O_CREAT, CREATOR_PERMISSON, &attr);

  if (q == -1)
  {
    perror("mq_open");
    exit(1);
  }

  return q;
}

#endif // MESSAGES_H
