#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "process.h"
#include "scheduler.h"

SchedulingAlgorithm parse_algo(const char* str) {
    if (strcmp(str, "FCFS") == 0) return FCFS;
    if (strcmp(str, "SJF") == 0) return SJF;
    if (strcmp(str, "PRIORITY") == 0) return PRIORITY_NON_PREEMPTIVE;
    if (strcmp(str, "PPRIO") == 0) return PRIORITY_PREEMPTIVE;
    if (strcmp(str, "RR") == 0) return ROUND_ROBIN;
    if (strcmp(str, "RM") == 0) return RATE_MONOTONIC;
    if (strcmp(str, "EDF") == 0) return EDF;
    return FCFS;
}

int main(int argc, char* argv[]) {
    srand(time(NULL));

    if (argc < 3) {
        printf("Uso: %s <ALGO> <STATIC|DYNAMIC> [quantum]\n", argv[0]);
        return 1;
    }

    SchedulingAlgorithm algo = parse_algo(argv[1]);
    int is_dynamic = strcmp(argv[2], "DYNAMIC") == 0;
    int quantum = argc >= 4 ? atoi(argv[3]) : 2;

    ProcessQueue* queue = create_process_queue(10);

    if (is_dynamic) {
        for (int i = 0; i < 5; i++) {
            add_process(queue, generate_random_process(i + 1, i * 2));
        }
    } else {
        load_processes_from_file("data/example_input.txt", queue);
    }

    run_scheduler(queue, algo, quantum);
    destroy_process_queue(queue);
    return 0;
}
