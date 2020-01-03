#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

#define GAME_MAPSIZE 500
#define H_RESOLUTION 640
#define V_RESOLUTION 480
#define GAME_PLAYER_SPEED 1

#define IPC_KEY 2123

#ifndef semun
union semun {
  int val;    /* Value for SETVAL */
  struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
  unsigned short *array;  /* Array for GETALL, SETALL */
  struct seminfo *__buf;  /* Buffer for IPC_INFO
                              (Linux-specific) */
};
#endif
