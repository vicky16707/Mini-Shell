
#include "head.h"

int main()
{
    system("clear");

    char external_commands[160][30] = {0};
    char internal_commands[29][10] = {0};

    extract_external_commands(external_commands);

    char *builtins[] = {
        "echo", "printf", "read", "cd", "pwd", "pushd", "popd",
        "dirs", "let", "eval", "set", "unset", "export", "declare",
        "typeset", "readonly", "getopts", "source", "exit", "exec",
        "shopt", "caller", "true", "type", "hash", "bind", "help", NULL
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

        if (fgets(input_string, sizeof(input_string), stdin) == NULL){
            break;
        }

        input_string[strcspn(input_string, "\n")] = '\0';

        if (strncmp(input_string, "PS1=", 4) == 0){
            if (strchr(input_string + 4, ' ') != NULL ||input_string[4] == '\0'){
                printf("Invalid prompt string\n");
                continue;
            }

            strcpy(prompt, input_string + 4);
        }
        else{
            char commands[10][20] = {0};
            char *token = strtok(input_string, " ");
            int j = 0;

            while (token != NULL && j < 10){
                strcpy(commands[j], token);
                j++;
                token = strtok(NULL, " ");
            }

            if (j == 0){
                continue;
            }

            int ret = check_command_type(commands[0],internal_commands,external_commands);

            if (ret == BUILTIN){
                //printf("Builtin command\n");
                execute_internal_commands(input_string);

            }
            else if (ret == EXTERNAL){
                printf("External command\n");
                pid_t pid = fork();

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
            else{
                printf("No Command\n");
            }
        }
    }

    return 0;
}