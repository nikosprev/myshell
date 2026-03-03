
#include "../headers/exec.h"



int background_check(char *str){
  if(strcmp("&",str)==0) return 1;
  else return 0;

}