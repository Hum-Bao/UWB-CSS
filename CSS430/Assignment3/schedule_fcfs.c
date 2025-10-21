#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "task.h"

struct node* head_node;

void add(char* name, int priority, int burst) {

    Task* temp = malloc(sizeof(Task));
    temp->name = name;
    temp->priority = priority;
    temp->burst = burst;
    temp->tid = 0;
    insert(&head_node, temp);
}

void schedule() {
    struct node* temp = head_node;
    int time = 0;

    int dispatch_time = -1;
    while (head_node != NULL) {
        if (temp->next == NULL) {
            dispatch_time++;
            time += temp->task->burst;
            printf("Running task = [%s] [%d] [%d] for %d units.\n",
                   temp->task->name, temp->task->priority, temp->task->burst,
                   temp->task->burst);
            printf("        Time is now: %d\n", time);
            delete (&head_node, temp->task);
            temp = head_node;
        } else {
            temp = temp->next;
        }
    }
    dispatch_time += time;

    printf("CPU Utilization: %.2f%%\n", ((float)time / dispatch_time) * 100);
}