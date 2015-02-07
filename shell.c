#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
/*don't know the best way to find the file location*/\
/*
Next going to start creating the shared memory and the mount function
*/

/*
static struct { //maby this should be a array
    char* pfe;
    char* pbs;
    char* mount;
}Locations = {
    .pfe = "pfe/pfe",
    .pbs = "pbs/pbs",
    .mount = "mount"
};
*/
void findFileLocation(char * command){
    char* both = malloc(strlen(command) *2 + 2); //going to have to unallocate this
    strcpy(both, command);
    strcat(both, "/");
    strcat(both, command);
    strcpy(command, both);
    free(both);
}


int main(int argc, char** argv){
    puts("inside shell");
    char* input=malloc((50)*sizeof(char));
    printf("\n\tEnter a string : ");
    pid_t pid;
    while(1){
        fgets(input,50,stdin);
        printf("your ouput %s", input );
        
        
        
        char* pch = strtok(input, " \n");
        char* arguments = "";
        char* command = pch;
        while(pch != NULL)
        {
            printf("%s\n",arguments);
            pch = strtok (NULL, " ");
            asprintf(&arguments, "%s %s", arguments, pch); //concantinate strings
        }
        
        
        findFileLocation(command);
        
        printf("destination %s asdf \n\n", command);
        
        pid = fork();
        if(pid > 0) //parent
        {
            //wait
        }
        else if (pid < 0) //error
        {
            exit(-1);
        }
        else{
            execl(command,arguments,NULL); //so it works when you put shell in pbs file but it doesn't work otherwise
        }
        
        
        
        break;
    }
    puts("exited shell");
    return 0;
}