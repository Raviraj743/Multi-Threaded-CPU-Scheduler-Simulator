#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

typedef struct {
    int pid;
    int burst_time;
    int remaining_time;
} Process;

void* run_process(void* arg) {
    Process* p = (Process*)arg;
    printf("Process %d started.\n", p->pid);
    sleep(p->burst_time);
    printf("Process %d finished.\n", p->pid);
    return NULL;
}

void fcfs_scheduler(Process processes[], int n) {
    printf("\n--- FCFS Scheduling ---\n");
    pthread_t threads[n];
    for (int i = 0; i < n; i++) {
        pthread_create(&threads[i], NULL, run_process, &processes[i]);
        pthread_join(threads[i], NULL);
    }
}

void round_robin_scheduler(Process processes[], int n, int quantum) {
    printf("\n--- Round Robin Scheduling (Quantum: %d sec) ---\n", quantum);
    int done;
    do {
        done = 1;
        for (int i = 0; i < n; i++) {
            if (processes[i].remaining_time > 0) {
                done = 0;
                int time_slice = (processes[i].remaining_time > quantum) ? quantum : processes[i].remaining_time;
                printf("Process %d executing for %d sec\n", processes[i].pid, time_slice);
                sleep(time_slice);
                processes[i].remaining_time -= time_slice;
                if (processes[i].remaining_time <= 0) {
                    printf("Process %d finished.\n", processes[i].pid);
                }
            }
        }
    } while (!done);
}

int main() {
    Process processes[] = {
        {1, 3, 3},
        {2, 5, 5},
        {3, 2, 2}
    };
    int n = sizeof(processes)/sizeof(processes[0]);

    fcfs_scheduler(processes, n);

    // Reset remaining times for Round Robin
    Process rr_processes[] = {
        {1, 3, 3},
        {2, 5, 5},
        {3, 2, 2}
    };
    round_robin_scheduler(rr_processes, n, 2);

    return 0;
}
