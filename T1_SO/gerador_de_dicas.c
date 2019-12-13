#include "funcs.h"

int main(){
  srand((unsigned)time(NULL));

  char **tips, *path;
  int numberOfTips, pid;
  FILE *file;

  file = getFile("banco_de_dicas.txt", "r"); // recebe o arquivo banco_de_dicas.txt.
  numberOfTips = getNumberOfTips(file); // conta quantas dicas tem no arquivo.
  tips = allocateMatrix(numberOfTips); // aloca uma matriz de char chamada tips.
  storeTipsInMatrix(tips, file);  // guarda as dicas na matriz.
  path = allocatePath(); // aloca uma string com o caminho inicial a ser percorrido.
  
  pid = getpid(); // pega o pid atual.
  
  printf("digraph G {\n");
  travelThroughFolders(tips, numberOfTips, path); // percorre o caminho e cria as dicas.
  if(getpid() == pid)   // Se certifica de que apenas o pai irá realizar o print.
    printf("}\n");

  free(path);
  free(tips);
  fclose(file);

  return 0;
}
