#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#define KEY 37
#define SHM_KEY 38


int main() {

  int sem_id=semget(KEY,1,0644);
  struct sembuf direct;
  direct.sem_flg=SEM_UNDO;
  direct.sem_num=0;
  direct.sem_op=-1;
  printf("[%d] before access\n",sem_id);
  semop(sem_id,&direct,1);
  printf("[%d] after access\n",sem_id);
  int fd=open("story",O_RDWR | O_APPEND,0644);
  if (fd==-1) {
    printf("%s\n",strerror(errno));
    direct.sem_op=1;
    semop(sem_id,&direct,1);
    return 0;
  }
  char s[256];
  printf("input your line:\n\n");
  fgets(s,256,stdin);
  int shm_id=shmget(SHM_KEY,strlen(s),IPC_CREAT | IPC_EXCL | 0644);
  
  write(fd,s,strlen(s));
  close(fd);
  direct.sem_op=1;
  semop(sem_id,&direct,1);
  printf("[%d] released semaphore\n",sem_id);

  return 0;


}
