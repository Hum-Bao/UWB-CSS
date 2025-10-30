#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "task.h"

struct node* head_node;

// Add a task to the list
void add(char* name, int priority, int burst) {
    Task* temp = malloc(sizeof(Task));
    temp->name = name;
    temp->priority = priority;
    temp->burst = burst;
    temp->tid = 0;
    insert(&head_node, temp);
}

void schedule() {
    int time = 0;

    int dispatch_time = -1;

    while (head_node != NULL) {
        struct node* temp = head_node;
        struct node* shortest = head_node;

        // Find the task with the shortest burst time
        while (temp != NULL) {
            if (temp->task->burst <= shortest->task->burst) {
                shortest = temp;
            }
            temp = temp->next;
        }

        dispatch_time++;
        time += shortest->task->burst;
        printf("Running task = [%s] [%d] [%d] for %d units.\n",
               shortest->task->name, shortest->task->priority,
               shortest->task->burst, shortest->task->burst);
        printf("        Time is now: %d\n", time);

        delete (&head_node, shortest->task);
    }

    dispatch_time += time;

    printf("CPU Utilization: %.2f%%\n", ((float)time / dispatch_time) * 100);
}
