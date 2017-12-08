#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/shm.h>
#define KEY 37
#define SHM_KEY 38



int main(int argc, char * argv[]) {
  union semun {
    int val;
    struct semid_ds *buf;
    unsigned short * array;
    struct seminfo * __buf;
  } arg;
  int sem_id;
  //union semun size;
  /*if (argc==3) {
    if (!strcmp(argv[1],"-c")) {
    sem_id=semget(KEY,1,IPC_CREAT | IPC_EXCL | 0600);
    if (sem_id>=0) {
    int size=0;
    sscanf(argv[2],"%d",&size);
    printf("semaphore created: %d\n",sem_id);
    semctl(sem_id,0,SETVAL,size);
    printf("value set: %d\n", semctl(sem_id,0,GETVAL));
    } else {
    printf("semaphore already exists\n");
    }

    } else
    printf("improper input\n");*/
  if (argc==2) {
    if (!strcmp(argv[1],"-c")) {
      sem_id=semget(KEY,1,IPC_CREAT | IPC_EXCL | 0600);
      if (sem_id>=0) {
      arg.val=1;
      semctl(sem_id,0,SETVAL,arg.val);
      printf("semaphore created %d\n",sem_id);
      int fd=open("story",O_CREAT | O_TRUNC,0644);
      printf("file created\n");
      close(fd);

      }
      else
	printf("file and semaphore already exist.\n");
      
    } else if (!strcmp(argv[1],"-v")) {
      sem_id=semget(KEY,1,0600);
      printf("semaphore value: %d\n",semctl(sem_id,0,GETVAL));
    }
    else if (!strcmp(argv[1],"-r")) {
      sem_id=semget(KEY,1,0600);
      printf("semaphore removed: %d\n",semctl(sem_id,0,IPC_RMID));
      execlp("rm","rm","story",NULL);
    }
    else
      printf("improper input\n");
  }
  else
    printf("improper input.\n");

  
 
  return 0;
 

}
