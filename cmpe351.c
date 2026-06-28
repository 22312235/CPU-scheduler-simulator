/* This is code developed by HANA SHAIMI */

#include "cmpe351.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

typedef struct Process
{
    int pid;
    int burst;
    int arrival;
    int priority;
    int remaining;
    int waiting;
    int completion;
    struct Process *next;
} Process;


Process *createProcess(int pid, int burst,
                       int arrival, int priority)
{
    Process *p = (Process *)malloc(sizeof(Process));
    if (!p) { perror("malloc"); exit(EXIT_FAILURE); }
    p->pid        = pid;
    p->burst      = burst;
    p->arrival    = arrival;
    p->priority   = priority;
    p->remaining  = burst;
    p->waiting    = 0;
    p->completion = 0;
    p->next       = NULL;
    return p;
}

void appendProcess(Process **head, int pid, int burst,
                   int arrival, int priority)
{
    Process *node = createProcess(pid, burst, arrival, priority);
    if (!*head) { *head = node; return; }
    Process *temp = *head;
    while (temp->next) temp = temp->next;
    temp->next = node;
}

int countProcesses(Process *head)
{
    int c = 0;
    while (head) { c++; head = head->next; }
    return c;
}

void freeList(Process *head)
{
    while (head) {
        Process *t = head;
        head = head->next;
        free(t);
    }
}

Process *cloneList(Process *head)
{
    Process *copy = NULL;
    while (head) {
        appendProcess(&copy, head->pid, head->burst,
                      head->arrival, head->priority);
        head = head->next;
    }
    return copy;
}


Process *loadProcesses(const char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (!fp) { perror("Input file"); return NULL; }
    Process *head = NULL;
    int pid = 1, b, a, pr;
    while (fscanf(fp, "%d:%d:%d", &b, &a, &pr) == 3)
        appendProcess(&head, pid++, b, a, pr);
    fclose(fp);
    return head;
}


void printResultLine(FILE *out, int id, Process *head, int n)
{
    double sum = 0;
    Process *temp = head;
    fprintf(out, "%d", id);
    printf("%d", id);
    while (temp) {
        fprintf(out, ":%d", temp->waiting);
        printf(":%d", temp->waiting);
        sum += temp->waiting;
        temp = temp->next;
    }
    fprintf(out, ":%.2f\n", sum / n);
    printf(":%.2f\n", sum / n);
}

/* ── FCFS ── */
void runFCFS(Process *head)
{
    int time = 0;
    Process *p = head;
    while (p) {
        if (time < p->arrival) time = p->arrival;
        p->waiting = time - p->arrival;
        time += p->burst;
        p = p->next;
    }
}

/* ── SJF Non-Preemptive ── */
void runSJFNonPreemptive(Process *head, int n)
{
    int completed = 0, time = 0;
    while (completed < n) {
        Process *best = NULL;
        Process *temp = head;
        while (temp) {
            if (temp->remaining > 0 && temp->arrival <= time)
                if (!best || temp->burst < best->burst)
                    best = temp;
            temp = temp->next;
        }
        if (!best) { time++; continue; }
        best->waiting    = time - best->arrival;
        time            += best->burst;
        best->remaining  = 0;
        completed++;
    }
}

/* ── SJF Preemptive ── */
void runSJFPreemptive(Process *head, int n)
{
    int finished = 0, time = 0;
    while (finished < n) {
        Process *best = NULL;
        Process *temp = head;
        while (temp) {
            if (temp->remaining > 0 && temp->arrival <= time)
                if (!best || temp->remaining < best->remaining)
                    best = temp;
            temp = temp->next;
        }
        if (!best) { time++; continue; }
        best->remaining--;
        time++;
        if (best->remaining == 0) {
            best->completion = time;
            best->waiting    = best->completion - best->arrival - best->burst;
            finished++;
        }
    }
}

/* ── Priority Non-Preemptive ── */
void runPriorityNonPreemptive(Process *head, int n)
{
    int completed = 0, time = 0;
    while (completed < n) {
        Process *best = NULL;
        Process *temp = head;
        while (temp) {
            if (temp->remaining > 0 && temp->arrival <= time)
                if (!best || temp->priority < best->priority)
                    best = temp;
            temp = temp->next;
        }
        if (!best)
        { time++; continue; }
        best->waiting    = time - best->arrival;
        time            += best->burst;
        best->remaining  = 0;
        completed++;
    }
}

/* ── Priority Preemptive ── */
void runPriorityPreemptive(Process *head, int n)
{
    int finished = 0, time = 0;
    while (finished < n) {
        Process *best = NULL;
        Process *temp = head;
        while (temp) {
            if (temp->remaining > 0 && temp->arrival <= time)
                if (!best || temp->priority < best->priority)
                    best = temp;
            temp = temp->next;
        }
        if (!best) { time++; continue; }
        best->remaining--;
        time++;
        if (best->remaining == 0) {
            best->completion = time;
            best->waiting    = best->completion - best->arrival - best->burst;
            finished++;
        }
    }
}

/* ── Round Robin ── */
void runRoundRobin(Process *head, int n, int quantum)
{
    int finished = 0, time = 0;
    Process **queue = (Process **)malloc(n * 2 * sizeof(Process *));
    if (!queue) { perror("malloc"); exit(EXIT_FAILURE); }
    int front = 0, back = 0;

    /* enqueue processes that arrive at time 0 */
    Process *temp = head;
    while (temp) {
        if (temp->arrival <= time) queue[back++] = temp;
        temp = temp->next;
    }

    while (finished < n) {
        if (front == back) {
            int next = -1;
            temp = head;
            while (temp) {
                if (temp->remaining > 0 &&
                    (next == -1 || temp->arrival < next))
                    next = temp->arrival;
                temp = temp->next;
            }
            time = next;
            temp = head;
            while (temp) {
                if (temp->arrival <= time && temp->remaining > 0)
                    queue[back++] = temp;
                temp = temp->next;
            }
            continue;
        }

        Process *cur = queue[front++];
        int prevTime = time;
        int slice = cur->remaining < quantum ? cur->remaining : quantum;
        cur->remaining -= slice;
        time += slice;

        temp = head;
        while (temp) {
            if (temp->arrival > prevTime && temp->arrival <= time &&
                temp->remaining > 0 && temp->pid != cur->pid)
                queue[back++] = temp;
            temp = temp->next;
        }

        if (cur->remaining == 0) {
            cur->completion = time;
            cur->waiting    = cur->completion - cur->arrival - cur->burst;
            finished++;
        } else {
            queue[back++] = cur;
        }
    }
    free(queue);
}

/* ── Main ── */
int main(int argc, char *argv[])
{
    char *inputFile  = NULL;
    char *outputFile = NULL;
    int   quantum    = 1;
    int   opt;

    while ((opt = getopt(argc, argv, "t:f:o:")) != -1) {
        switch (opt) {
            case 't':
                quantum = atoi(optarg);
                if (quantum < 1 || quantum > 255) {
                    fprintf(stderr, "Quantum must be between 1 and 255\n");
                    return EXIT_FAILURE;
                }
                break;
            case 'f':
                inputFile = optarg;
                break;
            case 'o':
                outputFile = optarg;
                break;
            default:
                fprintf(stderr, "Usage: %s -t quantum -f input -o output\n", argv[0]);
                return EXIT_FAILURE;
        }
    }

    if (!inputFile || !outputFile) {
        fprintf(stderr, "Missing required arguments\n");
        return EXIT_FAILURE;
    }

    Process *processes = loadProcesses(inputFile);
    if (!processes) return EXIT_FAILURE;

    FILE *out = fopen(outputFile, "w");
    if (!out) {
        perror("Output file");
        freeList(processes);
        return EXIT_FAILURE;
    }

    int n = countProcesses(processes);
    Process *copy;

    copy = cloneList(processes);
    runFCFS(copy);
    printResultLine(out, 1, copy, n);
    freeList(copy);

    copy = cloneList(processes);
    runSJFNonPreemptive(copy, n);
    printResultLine(out, 2, copy, n);
    freeList(copy);

    copy = cloneList(processes);
    runSJFPreemptive(copy, n);
    printResultLine(out, 3, copy, n);
    freeList(copy);

    copy = cloneList(processes);
    runPriorityNonPreemptive(copy, n);
    printResultLine(out, 4, copy, n);
    freeList(copy);

    copy = cloneList(processes);
    runPriorityPreemptive(copy, n);
    printResultLine(out, 5, copy, n);
    freeList(copy);

    copy = cloneList(processes);
    runRoundRobin(copy, n, quantum);
    printResultLine(out, 6, copy, n);
    freeList(copy);

    fclose(out);
    freeList(processes);
    return EXIT_SUCCESS;
}
