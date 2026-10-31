#include "head.h"
Job *head = NULL;
void insert_first(pid_t pid, char *cmd,int state)
{
    Job *n = malloc(sizeof(Job));

    if (n == NULL)
        return;

    n->pid = pid;
    strcpy(n->cmd, cmd);
    n->state=state;

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

void delete_job(pid_t pid)
{

    Job *temp = head;
    Job *prev=NULL;

    while(temp!=NULL){
        if(temp->pid==pid){
            if(prev==NULL){
                head=temp->next;
            }
            else    
                prev->next=temp->next;
                
            free(temp);
            return;
        }
    }
    prev=temp;
    temp = temp->next;
    
}

void print_jobs(void)
{
    Job *t = head;
    int i = 1;

    while (t != NULL)
    {
        if(t->state == RUNNING)
            printf("[%d] Running\t%s\n", i, t->cmd);
        else 
            printf("[%d] Stopped\t%s\n", i, t->cmd);
        i++;
        t = t->next;
    }
}