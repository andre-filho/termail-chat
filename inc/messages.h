#if !defined(MESSAGES_H)
#define MESSAGES_H

#include "includes.h"

char *string_splitter(char *str, char *a, char *b)
{
  char *ns = "";
  char target;
  int str_number = 1;

  for (size_t i = 0; i < strlen(str); i++)
  {
    target = str[i];
    // checks if it must be split
    if (target == ':')
    {
      // first split
      if (str_number == 1)
      {
        str_number++;
        a = ns;
        ns = "";
      }
      // second split
      else if (str_number == 2)
      {
        b = ns;
        ns = "";
      }
    }
    strcat(ns, (char *)target);
  }
  // str will be now only the message
  str = ns;
}

void format_message(char *sender, char *receiver, char *message)
{
  char* clean_message;
}

#endif // MESSAGES_H
