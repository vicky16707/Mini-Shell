#include "head.h"

int main()
{   
    system("clear");                              //cleared
    char external_commands[160][30] = {0};
    char internal_commands[8][7] = {0};
    extract_external_commands(external_commands);//called extract external
    

    char prompt[20] = "minishell";
    char input_string[50];

    char *builtins[] = {"echo","cd", "pwd", "exit", "jobs", "fg", "bg", NULL};
    int i = 0;
    while (builtins[i] != NULL)
    {
        strcpy(internal_commands[i], builtins[i]);//converting string to the 2d array of built in
        i++;
    }

    while (1){  
    
        printf(ANSI_COLOR_GREEN "%s" ANSI_COLOR_RESET "$ ", prompt);       //printing prompt string
        scanf("%[^\n]", input_string);

        if(strncmp(input_string,"PS1=",4)==0){
            int j=4,i=0;
            for ( i ; input_string[i]!='\0' ; i++){
                if(input_string[i]==' '){
                    printf("invalid prompt string");
                    //return 1;
                }
            }  
            if(i<=5){
                printf("invalid prompt string");
                break;
            }
            strcpy(prompt, &input_string[j]); //replacing the promp string
            printf("%s\n", prompt);    
        }
        else{
            char commands[10][20]={0};//commands passed throught the cla
            convert_input_to_commands(input_string, commands); 
            int ret=check_command_type(commands[0], internal_commands, external_commands);     
            if(ret==BUILTIN){
                printf("Builtin command\n");
            }
            else if(ret==EXTERNAL){
                printf("External command\n");
            }
            else{
                printf("No Command \n");
            }
        }
    }
    
    

    return 0;
}
