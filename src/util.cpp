#include "util.h"

void error(int flag,const char * c){
    if (flag == -1)
    {
        perror(c);
        exit(EXIT_FAILURE);
    }
}