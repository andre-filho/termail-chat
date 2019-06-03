#if !defined(VARS_H)
#define VARS_H

#define MAX_MSG 10
// #define W_PERMISSON 0
#define MAX_MSG_SIZE 500
#define QUEUE_PREFIX "/chat-"
#define MAX_USERNAME_SIZE 10
#define CREATOR_PERMISSON 0662
// #define CONSUMER_PERMISSON 0222
#define FULL_MSG_SIZE MAX_MSG_SIZE + MAX_USERNAME_SIZE * 2 + 2
#define MQ_DIR "/dev/mqueue/"

#endif // VARS_H
