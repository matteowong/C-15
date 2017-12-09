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
  printf("Last line entered:\n\n");
  int shm_id=shmget(SHM_KEY,sizeof(int),IPC_CREAT | IPC_EXCL | 0644);
  int * len;
  if (shm_id==-1) {//if mem already exists then the file has been written to before
    shm_id=shmget(SHM_KEY,sizeof(int),IPC_CREAT | 0644);
    len=shmat(shm_id,0,0);
    //printf("len: %d\n",*len);
    lseek(fd,-1*(*len),SEEK_END);
    char last_line[*len];
    read(fd,last_line,*len);
    printf("%s\n",last_line);
  }
  else {//otherwise this is the first time and there is no need to input the last line
    //printf("new shm made\n");
    len=shmat(shm_id,0,0);
  }
  char s[256];
  printf("input your line:\n\n");
  fgets(s,256,stdin);

  *len=strlen(s);
  //printf("len set to strlen: %d\n",*len);
  shmdt(len);
  
  write(fd,s,strlen(s));
  close(fd);
  direct.sem_op=1;
  semop(sem_id,&direct,1);
  printf("\n[%d] released semaphore\n",sem_id);

  return 0;


}
