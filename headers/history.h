
#ifndef HISTORY_H
#define HISTORY_H


#define MAX_HISTORY 20




typedef struct history{
  char *commands[MAX_HISTORY];
  int count;
}History;

void free_his(History *);                // Frees the stored commands

int add_history(History *,char *);                 // Adds command to history

char * print_history(History *);                    // Prints the stored commands, prompts user to select one


#endif