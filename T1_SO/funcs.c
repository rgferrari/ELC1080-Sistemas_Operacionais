#include "funcs.h"

FILE *getFile(char *filePath, char *mode) // getFile recebe duas strings contendo respectivamente o caminho do arquivo a ser aberto e o modo em que deve ser aberto e retorna um ponteiro para o arquivo aberto.
{
  FILE *file = fopen(filePath, mode);

  if(file == NULL){
    perror("Couldn't open the file");
    exit(0);
  }

  return file;
}

int getNumberOfTips(FILE *file) // getNumberOfTips recebe um arquivo e retorna o número de sentenças separadas por \n, não levando em conta as linhas em branco.
{
  char tip[TIP_SIZE];
  int cont = 0;

  for(int i = 0; fgets(tip, TIP_SIZE, file) != NULL; i++){
    //Checa se a frase não é apenas um \n
    if(strcmp(tip, "\n"))
      cont++;  
  }

  rewind(file);

  return cont;
}

char **allocateMatrix(int numberOfTips) // allocateMatrix recebe o número total de sentenças que serão armazenadas e retorna uma matriz de char com o mesmo número de linhas. 
{
  char **tips = (char**)malloc(numberOfTips*sizeof(char*));

  for(int i = 0; i<numberOfTips; i++)
    tips[i] = (char*)malloc(TIP_SIZE*sizeof(char));

  return tips;
}

void storeTipsInMatrix(char **tips, FILE *file) // storeTipsInMatrix recebe uma matriz de char e um arquivo e armazena as sentenças separadas por \n do arquivo na matriz.
{
  for(int i = 0; fgets(tips[i], TIP_SIZE, file) != NULL; i++){
    //Ignora a frase caso for apenas um \n
    if(!strcmp(tips[i], "\n"))
      i--;
  }
}

char *allocatePath() // allocatePath retorna uma string contendo o caminho inicial.
{
  char *path = (char*)malloc(PATH_SIZE*sizeof(char));

  strcpy(path, "./home");

  return path;
}

void createTip(char **tips, int numberOfTips, char *path) // createTip recebe uma matriz de char com dicas, o número de dicas, o caminho da pasta e cria um arquivo "/dica_do_dia.txt" com uma dica aleatória no caminho recebido.
{
  char tipPath[1024];

  strcpy(tipPath, path);

  strcat(tipPath, "/dica_do_dia.txt");

  FILE *new_file = getFile(tipPath, "w");

  int aleatoryNumber = rand()%numberOfTips; 

  fprintf(new_file,"%s",tips[aleatoryNumber]);

  fclose(new_file);
}

void printPath(char *path) // printPath recebe uma string com o caminho e o imprime na tela junto com o pid e o ppid.
{
  printf("\t%d [label=\"%s, %d\"];\n",getpid(), path, getpid());
  printf("\t%d -> %d;\n", getppid(), getpid());
}

void travelThroughFolders(char **tips, int numberOfTips, char *path) // travelThroughFolders recebe uma matriz com dicas, o número de dicas contidas na matriz e o caminho do diretório a ser percorrido. A função percorre todos os arquivos dentro do diretório recebido e quando identifica pastas realiza um fork. O pai continua percorrendo o diretório atual enquanto o filho chama a função novamente enviando o caminho da pasta encontrada. Esse processo ocorre até que não se encontrem mais pastas.
{
  DIR *dp;

  struct dirent *ep;

  dp = opendir(path);

  if (dp != NULL){
    while (ep = readdir (dp)){
      if(strcmp(ep->d_name, ".") && strcmp(ep->d_name, "..") && ep->d_type == DT_DIR){
        //se for filho
        if(fork() == 0){
          srand((unsigned)time(NULL) ^ getpid() << 16); // Gera uma seed nova para o filho utilizando OU o tempo do processador OU o pid do filho shiftado 16 vezes para a esquerda. Isto é feito pois alguns filhos são gerados no mesmo segundo, ficando assim com a mesma seed e diminuindo a aleatoriedade.
          strcat(path, "/");
          strcat(path, ep->d_name); // concatena o diretório com o caminho
          
          printPath(path); // imprime o novo caminho
          createTip(tips, numberOfTips, path); // cria a dica
          travelThroughFolders(tips, numberOfTips, path); // chama a função novamente
          break; 
        }
        //se for pai
        else{
          wait(NULL);
        }
      }
    }
    (void) closedir (dp);
  }
  else
    perror ("Couldn't open the directory");
}
