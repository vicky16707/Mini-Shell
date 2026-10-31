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
#include <errno.h>
 

#define BUILTIN		1
#define EXTERNAL	2
#define NO_COMMAND  3

#define RUNNING 1
#define STOPPED 2



extern char prompt[20] ;
typedef struct job {
    pid_t pid;
    char cmd[100];
    int state; // 0 = running, 1 = stopped
    struct job *next;
} Job;

extern int status;
extern pid_t pid;               // 0 = no child running

extern char prompt[20];
extern char cur_input[100];     // command text of the running child
extern int child_changed;
extern Job *head;


#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

extern int status ;
char *get_command(char *input_string);

void convert_input_to_commands(char *input, char commands[][20]);
int check_command_type(char *command, char internal_commands[][10], char external_commands[][30]);
void signal_handler(int sig_num);
void extract_external_commands(char external_commands[][30]);
void execute_internal_commands(char *input_string);
int execute_pipe(char *commands[], int count);

void sigchld_handler(int signum);

void insert_first(pid_t pid, char *cmd,int state);         // insert at first
void delete_first(void);
void delete_job(pid_t pid);
void print_jobs(void);

#endif