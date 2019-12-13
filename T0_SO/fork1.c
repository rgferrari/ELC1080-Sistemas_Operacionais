#include<stdio.h>
#include<unistd.h>

int main(){
  fork();
  printf("(1) Este é meu id: %d. Este é o id do meu pai: %d\n", getpid(), getppid());
  sleep(2);
  fork();
  printf("(2) Este é meu id: %d. Este é o id do meu pai: %d\n", getpid(), getppid());
  sleep(2);
  fork();
  printf("(3) Este é meu id: %d. Este é o id do meu pai: %d\n", getpid(), getppid());
  sleep(2);
  
  return 0;
}