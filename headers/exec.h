#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <glob.h>
#include <ctype.h>

#include "alias.h"
#include "history.h"
#include "pipe.h"
#include "wild.h"
#include "background.h"
#include "redirect.h"

#define MAX_INPUT 1024


#ifndef EXEC_H
#define EXEC_H


int exec (char ** , int , int);            // Executes the command

int tokenise(char *, Aliases *,History *);           // Breaks input down in words, checks for special characters and handles it accordingly

#endif