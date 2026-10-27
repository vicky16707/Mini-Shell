
#include "head.h"
int status =0;
pid_t pid;
int main()
{
    system("clear");

    char external_commands[160][30] = {0};
    char internal_commands[8][10] = {0};

    extract_external_commands(external_commands);

    char *builtins[] = {
        "echo", "cd", "pwd", "exit","fg","bg", NULL
    };

    int i = 0;

    while (builtins[i] != NULL){
        strcpy(internal_commands[i], builtins[i]);
        i++;
    }

    char prompt[20] = "minishell";
    char input_string[50];

    while (1){
        printf(ANSI_COLOR_GREEN "%s" ANSI_COLOR_RESET "$ ", prompt);

        scanf(" %[^\n]", input_string);

        if (strncmp(input_string, "PS1=", 4) == 0){

            int j = 4, i = 0;

            for (i; input_string[i] != '\0'; i++){

                if (input_string[i] == ' '){
                    printf("Invalid prompt string");
                    continue;
                }
            }

            strcpy(prompt, input_string + 4);
            continue;
        }
        else{
            char *commands[11];

            char *token = strtok(input_string, " ");
            int j = 0;

            while (token != NULL && j < 10){
                strcpy(commands[j], token);
                j++;
                token = strtok(NULL, " ");
            }
            commands[j] = NULL;

            if (j == 0){//agin goes to the start of the loop if no command is entered
                continue;
            }
            int pipe_present = 0;

            for (int i = 0; commands[i] != NULL; i++)
            {
                if (strcmp(commands[i], "|") == 0)
                {
                    pipe_present = 1;
                    break;
                }
            }

            int ret = check_command_type(commands[0],internal_commands,external_commands);

            if (ret == BUILTIN){
                //printf("Builtin command\n");
                execute_internal_commands(input_string);

            }
            else if (ret == EXTERNAL){
                if(pipe_present){
                    // Handle piped command==
                }
                else{
                    pid = fork();

                    if (pid == 0){
                        char *args[] = {commands[0], NULL};
                        execvp(commands[0], args);
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
            }
            else{
                printf("No Command\n");
            }
        }
    }

    return 0;
}