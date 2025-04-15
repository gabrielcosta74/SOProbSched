#include <stdio.h>
#include <stdlib.h>
#include "scheduler.h"

int compare_by_burst(const void* a, const void* b) {
    return ((Process*)a)->burst_time - ((Process*)b)->burst_time;
}

int compare_by_priority(const void* a, const void* b) {
    return ((Process*)a)->priority - ((Process*)b)->priority;
}

int compare_by_arrival(const void* a, const void* b) {
    return ((Process*)a)->arrival_time - ((Process*)b)->arrival_time;
}


int compare_by_deadline(const void* a, const void* b) {
    return ((Process*)a)->deadline - ((Process*)b)->deadline;
}

void run_sjf(ProcessQueue* queue) {
    qsort(queue->list, queue->size, sizeof(Process), compare_by_burst);
    run_fcfs(queue);
}

void run_priority(ProcessQueue* queue, int preemptive) {
    qsort(queue->list, queue->size, sizeof(Process), compare_by_priority);
    run_fcfs(queue);
}

void run_round_robin(ProcessQueue* queue, int quantum) {
    int current_time = 0, completed = 0;
    int wait_time = 0, turnaround = 0;
    int* remaining = malloc(sizeof(int) * queue->size);
    for (int i = 0; i < queue->size; i++) remaining[i] = queue->list[i].burst_time;

    printf("\n[RR] Escalonamento com quantum = %d:\n", quantum);

    while (completed < queue->size) {
        for (int i = 0; i < queue->size; i++) {
            Process* p = &queue->list[i];
            if (p->arrival_time <= current_time && remaining[i] > 0) {
                int exec_time = (remaining[i] > quantum) ? quantum : remaining[i];
                current_time += exec_time;
                remaining[i] -= exec_time;
                if (remaining[i] == 0) {
                    int wait = current_time - p->arrival_time - p->burst_time;
                    int turn = current_time - p->arrival_time;
                    printf("Processo %d: Espera = %d, Turnaround = %d\n", p->id, wait, turn);
                    wait_time += wait;
                    turnaround += turn;
                    completed++;
                }
            } else if (p->arrival_time > current_time) {
                current_time++;
            }
        }
    }

    printf("Média de espera: %.2f\n", (float)wait_time / queue->size);
    printf("Média de turnaround: %.2f\n", (float)turnaround / queue->size);
    free(remaining);
}

void run_edf(ProcessQueue* queue) {
    qsort(queue->list, queue->size, sizeof(Process), compare_by_deadline);
    run_fcfs(queue);
}

void run_rm(ProcessQueue* queue) {
    qsort(queue->list, queue->size, sizeof(Process), compare_by_priority); // prioridade == 1/periodo
    run_fcfs(queue);
}

void run_scheduler(ProcessQueue* queue, SchedulingAlgorithm algo, int quantum) {
    switch (algo) {
        case FCFS:
            run_fcfs(queue); break;
        case SJF:
            run_sjf(queue); break;
        case PRIORITY_PREEMPTIVE:
        case PRIORITY_NON_PREEMPTIVE:
            run_priority(queue, algo == PRIORITY_PREEMPTIVE); break;
        case ROUND_ROBIN:
            run_round_robin(queue, quantum); break;
        case RATE_MONOTONIC:
            run_rm(queue); break;
        case EDF:
            run_edf(queue); break;
        default:
            printf("Algoritmo não implementado\n");
    }
}

void run_fcfs(ProcessQueue* queue) {
    qsort(queue->list, queue->size, sizeof(Process), compare_by_arrival);

    int current_time = 0;
    int total_wait = 0, total_turnaround = 0;

    printf("\n[FCFS] Escalonamento:\n");
    for (int i = 0; i < queue->size; i++) {
        Process p = queue->list[i];
        if (current_time < p.arrival_time)
            current_time = p.arrival_time;

        int wait_time = current_time - p.arrival_time;
        int turnaround = wait_time + p.burst_time;
        current_time += p.burst_time;

        printf("Processo %d: Espera = %d, Turnaround = %d\n", p.id, wait_time, turnaround);

        total_wait += wait_time;
        total_turnaround += turnaround;
    }

    printf("Média de espera: %.2f\n", (float)total_wait / queue->size);
    printf("Média de turnaround: %.2f\n", (float)total_turnaround / queue->size);
}

