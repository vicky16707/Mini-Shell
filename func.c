#include "head.h"
void scan_input(char *prompt_string, char *input){
   
}
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
int check_command_type(char *command,char *internal_commands,char *external_commands){
    
    int i;
    for (i = 0; i < 29; i++){
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

}


