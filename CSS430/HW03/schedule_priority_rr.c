#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "task.h"

#define MAX_PRIORITY 10
#define MIN_PRIORITY 1
#define QUANTUM 10

struct node* priority_queue[MAX_PRIORITY] = {NULL};

// Add a task to the list
void add(char* name, int priority, int burst) {
    Task* temp = malloc(sizeof(Task));
    temp->name = name;
    temp->priority = priority;
    temp->burst = burst;
    temp->tid = 0;
    insert(&priority_queue[priority], temp);
}

// Priority Scheduling
void schedule() {

    int time = 0;

    int dispatch_time = -1;

    int run_time = 0;

    int is_empty = 0;

    int index = MAX_PRIORITY;

    int size = 0;
    int pos = 0;
    int count = 0;

    while (!is_empty) {
        struct node* temp = priority_queue[index];

        if (temp == NULL) {
            index--;
            continue;
        }

        if (index <= MIN_PRIORITY) {
            is_empty = 1;
        }

        size = 0;
        while (temp->next != NULL) {
            temp = temp->next;
            size++;
        }

        pos = size;

        while (priority_queue[index] != NULL) {
            temp = priority_queue[index];
            count = 0;
            if (pos < 0) {
                pos = size;
            }

            //Grab elements from back to ensure that tasks are run in lexicographical order
            while (count < pos) {
                temp = temp->next;
                count++;
                if (temp->next == NULL) {
                    pos = count;
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
                   temp->task->name, temp->task->priority, temp->task->burst,
                   run_time);
            printf("        Time is now: %d\n", time);

            if (temp->task->burst <= 0) {
                delete (&priority_queue[index], temp->task);
            }

            pos--;
        }
    }
    dispatch_time += time;

    printf("CPU Utilization: %.2f%%\n", ((float)time / dispatch_time) * 100);
}
