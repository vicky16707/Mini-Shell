#include "head.h"

void convert_input_to_commands(char *input, char commands[][20]){
    int i = 0;
    char *token = strtok(input, " ");//to find the first token

    while (token != NULL ){
        strcpy(commands[i], token);
        i++;
        token = strtok(NULL, " ");
    }
    
}
void extract_external_commands(char external_commands[][30]){
    FILE *fp;
    char data[100];
    int i=0;
    fp=fopen("external.txt","r");
    if(fp==NULL){
        return ;
    }
    while(fgets(data,sizeof(data),fp)!=NULL){//getting data from the file
        char *token=strtok(data, "\n");//seperating tokrn with help of delim \n
        if(token!=NULL){
            strcpy(external_commands[i],token);
            i++;
        }   
    }   
}
int check_command_type(char *command, char internal_commands[][10],char external_commands[][30])
{
    int i;
    for (i = 0; i < 7; i++){
        if (strcmp(internal_commands[i], command) == 0){
            return BUILTIN;
        }
    }

    for (i = 0; i < 160; i++){
        if (strcmp(external_commands[i], command) == 0){
            return EXTERNAL;
        }
    }
    return NO_COMMAND;
}

void execute_internal_commands(char *input_string){
    if(strncmp(input_string, "cd", 2) == 0){
        chdir(&input_string[3]);
    }
    else if(strncmp(input_string, "exit", 4) == 0){
        exit(0);
    }
    else if(strncmp(input_string, "echo", 4) == 0){
        if(strncmp(input_string, "echo $SHELL", 11) == 0){
            printf("%s\n", getenv("SHELL"));
        }
        else if(strncmp(input_string, "echo $$", 7) == 0){
            printf("%d\n", getpid());
        }
        else if(strncmp(input_string, "echo $?", 7) == 0){
            printf("%d\n", WEXITSTATUS(status));
        }
        else {
            printf("%s\n", input_string + 5);
        }
    } 
    else if(strncmp(input_string, "pwd", 3) == 0){
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s\n", cwd);
        } 
        else {
            perror("getcwd() error");
        }
    }
    else if(strncmp(input_string, "fg", 2) == 0){
        // Handle fg command
    }
    else if(strncmp(input_string, "bg", 2) == 0){
        // Handle bg command
    }
    else if(strncmp(input_string, "jobs", 4) == 0){
        // Handle jobs command
    }

}
void signal_handler(int sig_num){
    if (sig_num == SIGINT){
        printf("\n");
    }
    else if (sig_num == SIGTSTP){
        printf("\n");
    }


}
int execute_pipe(char *commands[], int count)
{
    int start[count + 1];// creating array for the starting index
    int pipe_count = 0;

    start[0] = 0;

    for (int i = 0; i < count; i++){
        if (strcmp(commands[i], "|") == 0){
            commands[i] = NULL;
            start[++pipe_count] = i + 1;//incrementing the pipe_count and storing the index of the next command after the pipe
        }
    }

    if (pipe_count == 0){
        fprintf(stderr, "No pipe found\n");
        return 1;
    }

    int fd[pipe_count][2];//creating an array of fd for the pipes

    for (int i = 0; i < pipe_count; i++){
        if (pipe(fd[i]) == -1)//creating pipes
        {
            perror("pipe");
            return 1;
        }
    }

    int n = pipe_count + 1;

    for (int i = 0; i < n; i++){
        pid_t pid = fork();

        if (pid == -1){
            perror("fork");
            return 1;
        }

        if (pid == 0){
            if (i > 0){
                dup2(fd[i - 1][0], STDIN_FILENO);
            }

            if (i < n - 1){
                dup2(fd[i][1], STDOUT_FILENO);
            }

            for (int j = 0; j < pipe_count; j++){//closing the unused fd
                close(fd[j][0]);
                close(fd[j][1]);
            }

            execvp(commands[start[i]], &commands[start[i]]);

            perror("execvp");
            exit(1);
        }
    }

    for (int i = 0; i < pipe_count; i++){//closing the unused fd in the parent process
        close(fd[i][0]);
        close(fd[i][1]);
    }

    for (int i = 0; i < n; i++){//to wait for all child process to finish
        wait(NULL);
    }

    return 0;
}


