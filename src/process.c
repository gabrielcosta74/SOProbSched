#include <stdio.h>
#include <stdlib.h>
#include "process.h"
#include "utils.h"

ProcessQueue* create_process_queue(int capacity) {
    ProcessQueue* queue = malloc(sizeof(ProcessQueue));
    queue->list = malloc(sizeof(Process) * capacity);
    queue->size = 0;
    queue->capacity = capacity;
    return queue;
}

void destroy_process_queue(ProcessQueue* queue) {
    free(queue->list);
    free(queue);
}

void add_process(ProcessQueue* queue, Process proc) {
    if (queue->size < queue->capacity) {
        queue->list[queue->size++] = proc;
    } else {
        // Resize if needed
        queue->capacity *= 2;
        queue->list = realloc(queue->list, sizeof(Process) * queue->capacity);
        queue->list[queue->size++] = proc;
    }
}

Process generate_random_process(int id, int current_time) {
    static int cumulative_arrival = 0; // mantém tempo de chegada cumulativo
    Process p;

    int inter_arrival = (int)generate_exponential(1.5); // tempo entre chegadas
    cumulative_arrival += inter_arrival;

    p.id = id;
    p.arrival_time = cumulative_arrival;
    p.burst_time = (int)generate_exponential(4.0); // média = 4
    if (p.burst_time <= 0) p.burst_time = 1;

    p.priority = rand() % 10;
    p.remaining_time = p.burst_time;
    p.deadline = p.arrival_time + p.burst_time + 10;
    p.period = 0;

    printf("Generated Process %d: chegada=%d, burst=%d, prioridade=%d\n", 
           p.id, p.arrival_time, p.burst_time, p.priority);

    return p;
}



void load_processes_from_file(const char* filename, ProcessQueue* queue) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir ficheiro");
        return;
    }
    Process p;
    while (fscanf(file, "%d %d %d %d", &p.id, &p.arrival_time, &p.burst_time, &p.priority) == 4) {
        p.remaining_time = p.burst_time;
        add_process(queue, p);
    }
    fclose(file);
}
