#include "head.h"
Job *head = NULL;
void insert_first(pid_t pid, char *cmd)
{
    Job *n = malloc(sizeof(Job));

    if (n == NULL)
        return;

    n->pid = pid;
    strcpy(n->cmd, cmd);

    n->next = head;
    head = n;
}

void delete_first(void)
{
    if (!head) return;

    Job *temp = head;
    head = head->next;
    free(temp);
}

void print_jobs(void)
{
    Job *t = head;
    int i = 1;

    while (t != NULL)
    {
        printf("[%d] Stopped\t%s\n", i, t->cmd);
        i++;
        t = t->next;
    }
}