#include "head.h"
int status = 0;
pid_t pid;

stuct sigaction sa;
sa.handler=signal_handler;
sa.flags=0;
sigemptyset(&sa.sa_mask);
sigaddset(&sa.mask,SIGINT);
sigaddset(&sa.maask,SIGTSTP);
sigaction(SIGINT,&sa,NULL);
sigaction(SIGTSTP,&sa,NULL);




int main()
{
    system("clear");

    char external_commands[160][30] = {0};
    char internal_commands[9][10] = {0};

    extract_external_commands(external_commands);

    char *builtins[] = {
        "echo", "cd", "pwd", "exit", "fg", "bg", "jobs", NULL
    };

    int i = 0;

    while (builtins[i] != NULL){
        strcpy(internal_commands[i], builtins[i]);
        i++;
    }

    char prompt[20] = "minishell";
    char input_string[50];

    while (1)
    {
        printf(ANSI_COLOR_GREEN "%s" ANSI_COLOR_RESET "$ ", prompt);

        if (scanf(" %[^\n]", input_string) != 1){
            break;
        }

        if (strncmp(input_string, "PS1=", 4) == 0){
            int i = 4;

            for (; input_string[i] != '\0'; i++){
                if (input_string[i] == ' ')
                {
                    printf("Invalid prompt string\n");
                    break;
                }
            }

            if (input_string[4] != '\0'){
                strcpy(prompt, input_string + 4);
            }

            continue;
        }
        else{
            char *commands[11];

            char *token = strtok(input_string, " ");
            int j = 0;

            while (token != NULL && j < 10){
                commands[j] = token;
                j++;
                token = strtok(NULL, " ");
            }

            commands[j] = NULL;

            if (j == 0){//agin goes to the start of the loop if no command is entered
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
                execute_pipe(commands, j);
            }
            else{
                int ret = check_command_type(
                    commands[0],
                    internal_commands,
                    external_commands
                );

                if (ret == BUILTIN){
                    execute_internal_commands(input_string);
                }
                else if (ret == EXTERNAL){
                    pid = fork();

                    if (pid == 0){
                        signal(SIGINT, SIG_DFL);
                        signal(SIGTSTP, SIG_DFL);
                        execvp(commands[0], commands);
                        perror("execvp");
                        exit(EXIT_FAILURE);
                    }
                    else if (pid > 0){
                        waitpid(pid, NULL, 0);
                    }
                    else{
                        perror("fork");
                    }
                }
                else{
                    printf("No Command\n");
                }
            }
        }
    }

    return 0;
}




