#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "task.h"

#define QUANTUM 10

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

// Priority Scheduling
void schedule() {

    int time = 0;

    int dispatch_time = -1;

    int run_time = 0;

    struct node* temp = head_node;

    int size = 0;
    int count = 0;

    while (temp->next != NULL) {
        temp = temp->next;
        size++;
    }
    int index = size;

    while (head_node != NULL) {
        struct node* temp = head_node;
        count = 0;
        if (index < 0) {
            index = size;
        }
        while (count < index) {
            temp = temp->next;
            count++;
            if (temp->next == NULL) {
                index = count;
                break;
            }
        }

        if (temp->task->burst < QUANTUM) {
            run_time = temp->task->burst;
        } else {
            run_time = QUANTUM;
        }

        dispatch_time++;
        time += run_time;
        temp->task->burst -= run_time;
        printf("Running task = [%s] [%d] [%d] for %d units.\n",
               temp->task->name, temp->task->priority, run_time, run_time);
        printf("        Time is now: %d\n", time);

        if (temp->task->burst <= 0) {
            delete (&head_node, temp->task);
        }
        index--;
    }

    dispatch_time += time;

    printf("CPU Utilization: %.2f%%\n", ((float)time / dispatch_time) * 100);
}
