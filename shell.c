#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

static struct {
    char* pfe;
    char* pbs;
}Locations = {
    .pfe = "pfe/pfe",
    .pbs = "pbs/pbs"
};

int main(int argc, char** argv){
    puts("inside shell");
    char* input=malloc((50)*sizeof(char));
    printf("\n\tEnter a string : ");
    pid_t pid;
    while(1){
        fgets(input,50,stdin);
        printf("your ouput %s", input );
        pid = fork();
        if(pid > 0) //parent
        {
            
        }
        else if (pid < 0) //error
        {
            exit(-1);
        }
        else{
            execl(Locations.pbs,NULL);
        }
        
        break;
    }
    puts("exited shell");
    return 0;
}