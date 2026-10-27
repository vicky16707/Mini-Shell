#include "head.h"

int main()
{
    system("clear");
    char external_commands[160][30] = {0};
    char internal_commands[29][10] = {0};
    extract_external_commands(external_commands);
    char *builtins[] = {"echo", "printf", "read", "cd", "pwd", "pushd", "popd", "dirs", "let", "eval",
						"set", "unset", "export", "declare", "typeset", "readonly", "getopts", "source",
						"exit", "exec", "shopt", "caller", "true", "type", "hash", "bind", "help", NULL};
    int i = 0;
    while (builtins[i] != NULL)
    {
        strcpy(internal_commands[i], builtins[i]);
        i++;
    }

    char prompt[20] = "minishell";
    char input_string[50];

    while (1){  
    
        printf(ANSI_COLOR_GREEN "%s" ANSI_COLOR_RESET "$ ", prompt_string);        scanf("%[^\n]", input);

        if(strncmp(input,"PS1=",4)==0){
            int j=4,i=0;
            for ( i ; input[i]!='\0' ; i++){
                if(input[i]==' '){
                    printf("invlaid prompt string");
                    //return 1;
                }
            }  
            if(i<=5){
                printf("invlaid prompt string");
                break;
            }
            strcpy(prompt_string, &input[j]); //replacing the promp string
            printf("%s\n", prompt_string);    
        }
        else{
            char commands[10][20]={0};//commands passed throught the cla
            int i = 0;

            char *token = strtok(input, " ");//to find the first token

            while (token != NULL ){
                strcpy(commands[i], token);
                i++;
                token = strtok(NULL, " ");
            }            
        }
    }
    //doubt
    // int ret = check_command_type(commands[0],
    //                              internal_commands,
    //                              external_commands);

    // if (ret == BUILTIN)
    //     printf("Builtin command\n");
    // else if (ret == EXTERNAL)
    //     printf("External command\n");
    // else
    //     printf("Command not found\n");

    return 0;
}
