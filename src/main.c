#include <stdio.h>
#include "list.h"

typedef void (*task_callback)(void);

void task_one() {
    printf("This is task 1.\n");
}

void task_two() {
    printf("This is task 2.\n");
}

typedef struct {
    task_callback cb;
} task_t;

typedef struct {
    list_t* tasks;
} scheduler_t;

void task_init(task_t* task, task_callback cb) {
    task->cb = cb;
}

void scheduler_init(scheduler_t* scheduler) {
    scheduler->tasks = list_alloc();
}

void scheduler_add_task(scheduler_t* scheduler, task_t* task) {
    list_push_back(scheduler->tasks, task);
}

void scheduler_step(scheduler_t* scheduler) {
    task_t* task = list_pop_front(scheduler->tasks);
    task->cb();
}

void scheduler_free(scheduler_t* scheduler) {
    list_free(scheduler->tasks);
}

int main(void) {

    task_t t1, t2;
    task_init(&t1, task_one);
    task_init(&t2, task_two);

    scheduler_t scheduler;
    scheduler_init(&scheduler);

    scheduler_add_task(&scheduler, &t1);
    scheduler_add_task(&scheduler, &t2);

    while (scheduler.tasks->count != 0) {
        scheduler_step(&scheduler);
    }

    scheduler_free(&scheduler);

    return 0;
}
