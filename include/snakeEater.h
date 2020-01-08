#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define IPC_KEY 2123
#define PORTNUM 1312

#ifndef semun
union semun {
  int val;    /* Value for SETVAL */
  struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
  unsigned short *array;  /* Array for GETALL, SETALL */
  struct seminfo *__buf;  /* Buffer for IPC_INFO
                              (Linux-specific) */
};
#endif

void error_check(int i, char* s);