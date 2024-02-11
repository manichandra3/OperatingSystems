
#ifndef GLOBALS_H
#define GLOBALS_H
#define MAX_PROCESS 100
typedef struct {
  char *name;
  int arrival;
  int cpuburst;
  int turnaround;
  int wait;
} Process;
Process processtable[MAX_PROCESS];
#endif // GLOBALS_H
