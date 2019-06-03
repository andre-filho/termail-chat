#if !defined(MENU_H)
#define MENU_H

#include "includes.h"

void init()
{
  printf("           ..--\"\"|\n");
  printf("           |     |\n");
  printf("           | .---'\n");
  printf("    (\\-.---| |-------------.\n");
  printf("    / \\) \\ | |              \\\n");
  printf("    |:.  | | |               |\n");
  printf("    |:.  | |o| T E R M A I L |\n");
  printf("    |:.  | `\"`               |\n");
  printf("    |:.  |_  ___   __ _  __ /\n");
  printf("    `\"\"\"\"`\"\"\"\"|=`|\"\"\"\"\"\"\"`\n");
  printf("              |=_|\n");
  printf("              |= |\n\n");
  printf("----------------------------------------------\n");
  printf("Welcome to the TERMAIL chat!\n");
  printf("Developed by:\n");
  printf("André de S C Filho - 15/0005521\n");
  printf("Matheus Batista S  - 15/0018029\n");
  printf("----------------------------------------------\n\n");
}

void list_users()
{
    struct dirent *d;
    DIR *dir = opendir(MQ_DIR);
    
    char *token;
    char *name;

    user_log("Showing all connected users...");

    while((d = readdir(dir)) != NULL){
        if(d->d_type == DT_REG)
        {
            char delim[] = "-";
            token = strtok(d->d_name, delim);
            name = strtok(NULL, delim);

            printf("%s\n", name);
        }
    }
    closedir(dir);
}

#endif // MENU_H
