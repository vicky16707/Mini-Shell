#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio_ext.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
 

#define BUILTIN		1
#define EXTERNAL	2
#define NO_COMMAND  3



typedef struct job {
    pid_t pid;
    char cmd[100];
    struct job *next;
} Job;

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

extern int status ;
//void scan_input(char *prompt, char *input_string);
char *get_command(char *input_string);

//void copy_change(char *prompt, char *input_string);
void convert_input_to_commands(char *input, char commands[][20]);
//void echo(char *input_string, int status);
int check_command_type(char *command, char internal_commands[][10], char external_commands[][30]);
void signal_handler(int sig_num);
void extract_external_commands(char external_commands[30][30]);
void execute_internal_commands(char *input_string);
int execute_pipe(char *commands[], int count);

#endif