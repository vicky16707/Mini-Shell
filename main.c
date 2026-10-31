/****************************************************************************************************
 * Project Title : Mini Shell

 * Description :
   A command-line shell developed in C that provides an interface to execute builtin and external
   commands. The project uses Linux system calls to create and manage processes and supports
   command execution, piping, signal handling and job control.

 * Features :
   - Executes builtin commands such as echo, cd, pwd, exit, fg, bg and jobs.
   - Executes external commands using fork() and execvp().
   - Supports multiple commands connected using pipes.
   - Handles signals such as SIGINT, SIGTSTP and SIGCHLD.
   - Maintains a job list to manage stopped processes.
   - Supports foreground and background job execution.
   - Implements process creation, process synchronization and inter-process communication.
   - Uses system calls, signals, pipes and linked lists.

 * Author : Vignesh N
 * Roll no : 26010_020
 ****************************************************************************************************/
#include "head.h"
int status = 0;
pid_t pid=0;
char prompt[20];
int child_changed = 0;

int main()
{
    system("clear");

    char external_commands[160][30] = {0};
    char internal_commands[9][10] = {0};

    extract_external_commands(external_commands);//getting external commands from the file

    char *builtins[] = {
        "echo", "cd", "pwd", "exit", "fg", "bg", "jobs", NULL
    };

    int i = 0;

    while (builtins[i] != NULL){
        strcpy(internal_commands[i], builtins[i]);//copying the builtins to the internal_commands array
        i++;
    }

    strcpy(prompt, "minishell");//inintial prompt

    char input_string[50];
    char input_copy[100];

    struct sigaction sa={0};
    sa.sa_handler=signal_handler;
    sa.sa_flags=SA_RESTART;

    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask,SIGINT);
    sigaddset(&sa.sa_mask,SIGTSTP);

    sigaction(SIGINT,&sa,NULL);//ignoring ctrl+c in the shell
    sigaction(SIGTSTP,&sa,NULL);//ignoring ctrl+z in the shell
    sigaction(SIGCHLD,&sa,NULL);//handling the child termination signal


    while (1)
    {
        printf("%s$ ", prompt);
        fflush(stdout);

        if (scanf(" %[^\n]", input_string) != 1){
            break;
        }

        if (strncmp(input_string, "PS1=", 4) == 0){
            int i = 4;
            int valid = 1;
            for (; input_string[i] != '\0'; i++){
                if (input_string[i] == ' ')
                {
                    printf("Invalid prompt string\n");
                    valid=0;
                    break;
                }
            }

            if (valid && input_string[4] != '\0'){
                strcpy(prompt, input_string + 4);//updating the prompt
            }

            continue;
        }
        else{
            char *commands[11];

            strcpy(input_copy, input_string);//keeping a copy of the original input

            char *token = strtok(input_string, " ");
            int j = 0;

            while (token != NULL && j < 10){
                commands[j] = token;//storing each command argument
                j++;
                token = strtok(NULL, " ");
            }

            commands[j] = NULL;//marking the end of the arguments

            if (j == 0){//agin goes to the start of the loop if no command is entered
                continue;
            }
            int background = 0;

            if(strcmp(commands[j - 1], "&") == 0){
                background = 1;
                commands[--j] = NULL;
            }

            if(j == 0){
                continue;
            }
            int pipe_present = 0;

            for (int i = 0; i < j; i++){
                if (strcmp(commands[i], "|") == 0){
                    pipe_present = 1;

                    if (i == 0 || i == j - 1 ) { //if pipe at first or at last loop will not continue
                        printf("Invalid pipe syntax\n");
                        pipe_present = -1;
                        break;
                    }
                }
            }

            if (pipe_present == -1){
                continue;
            }
            else if (pipe_present == 1){
                execute_pipe(commands, j);//executing commands with pipe
            }
            else{
                int ret = check_command_type(commands[0],internal_commands,external_commands);

                if (ret == BUILTIN){
                    execute_internal_commands(input_copy);//executing builtin commands
                }
                else if (ret == EXTERNAL){
                    strcpy(cur_input, input_copy);

                    pid = fork(); //creating a child process

                    if (pid < 0){
                        perror("fork");
                        pid=0;
                    }
                    else if (pid == 0){
                        
                        signal(SIGINT, SIG_DFL);  //restoring default signal handling in child
                        signal(SIGTSTP, SIG_DFL);
                        signal(SIGCHLD, SIG_DFL);

                        execvp(commands[0], commands);
                        perror("execvp");
                        exit(1);
                    }
                    else
                    {
                        if(background==0){
                            waitpid(pid, &status, WUNTRACED);//waiting for child or detecting if it is stopped
                            if(WIFSTOPPED(status)){
                                insert_first(pid, cur_input,STOPPED); //adding stopped job to the list
                            }
                        }
                        else{
                            printf("[%d] %s\n", pid, cur_input);
                            insert_first(pid, cur_input,RUNNING); //adding stopped job to the list
                        }
                        pid=0;
                    }
                }
                else
                {
                    printf("No Command\n");
                }
                
            }
        }
    }

     return 0;
}




