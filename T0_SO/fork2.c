#include<stdio.h>
#include<unistd.h>

int main(){
  fork();
  sleep(2);
  fork();
  sleep(2);
  fork();
  printf("Este é meu id: %d. Este é o id do meu pai: %d\n", getpid(), getppid());
  sleep(2);
  
  return 0;
}