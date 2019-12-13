#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#define CAPACIDADE_MAX_SALA 5
#define COLOR_RED "\x1b[31m"
#define COLOR_GREEN "\x1b[32m"
#define COLOR_YELLOW "\x1b[33m"
#define COLOR_BLUE "\x1b[34m"
#define COLOR_CYAN "\x1b[36m"
#define COLOR_RESET "\x1b[0m"

// Aluno : René Gargano Ferrari
// Matrícula: 201812031
// Turma: Ciência da Computação

// Para compilar: gcc -o confeccao_de_carteirinhas confeccao_de_carteirinhas.c -pthread

int alunosNaSala = 0;
int IDsAlunosNaSala[CAPACIDADE_MAX_SALA];
sem_t mutex;
sem_t *s;

void *criaFicha(void *arg){
	int threadID = *((int*)arg); //Transforma o índice da thread de void* para int

	printf(COLOR_GREEN"Ficha %d inicializada.\n"COLOR_RESET, threadID);

  while(true){
    sem_wait(&mutex); //Entra se a porta do DCE estiver aberta e a fecha em seguida
    IDsAlunosNaSala[alunosNaSala] = threadID; //Guarda o ID do aluno que entrou em um vetor
    alunosNaSala++; //Incrementa o número de alunos na sala
    printf(COLOR_CYAN"Ficha %d: Entrei. Estou esperando o bolsista chegar.\n"COLOR_RESET, threadID);
    if(alunosNaSala < CAPACIDADE_MAX_SALA){ //Se a sala não estiver cheia
      sem_post(&mutex); //Abre a porta para mais alunos entrarem
      sem_wait(&s[threadID]); //Aluno espera bolsista entrar
    }else{ //Se estiver cheia
      sem_post(&s[0]); //Libera o bolsista
      sem_wait(&s[threadID]); //Aluno espera bolsista entrar
    }
    printf(COLOR_CYAN"Ficha %d: Aqui estão meus documentos.\n"COLOR_RESET, threadID);
    sem_post(&s[0]); //Libera o bolsista para analisar os documentos
    sem_wait(&s[threadID]); //Aluno espera o bolsista analisar os documentos
    printf(COLOR_CYAN"Ficha %d: Recebi minha carteirinha. Estou indo embora.\n"COLOR_RESET, threadID);
    alunosNaSala--; //Decrementa o número de alunos na sala
    if(alunosNaSala == 0) //Se a sala estiver vazia
      sem_post(&s[0]); //Libera o bolsista para sair da sala
    sleep(2 + rand()%8); //Ficha espera de 2 a 10 segundos para ser usada por outro aluno
  }
}

void *criaBolsista(void *arg){
  int threadID = *((int*)arg);

  printf(COLOR_GREEN"Bolsista inicializado.\n"COLOR_RESET);

  while(true){
    sem_wait(&s[threadID]); // Bolsista está esperando a sala ficar cheia
    printf(COLOR_BLUE"Bolsista: Voltei. Vou trabalhar...\n"COLOR_RESET);
    for(int i = 0; i < CAPACIDADE_MAX_SALA; i++){
      sem_post(&s[IDsAlunosNaSala[i]]); //Libera o aluno para entregar os documentos
      sem_wait(&s[threadID]); //Espera o aluno entregar os documentos
      printf(COLOR_BLUE"Bolsista: checando documentos da ficha %d...\n"COLOR_RESET, IDsAlunosNaSala[i]);
    }
    for(int i = 0; i < CAPACIDADE_MAX_SALA; i++){
      printf(COLOR_BLUE"Bolsista: Tudo certo com os documentos. Entregando carteirinha da ficha %d.\n"COLOR_RESET, IDsAlunosNaSala[i]);
      sem_post(&s[IDsAlunosNaSala[i]]); //Libera o aluno
    }
    sem_wait(&s[threadID]); //Bolsista espera todos os alunos saírem
    printf(COLOR_BLUE"Bolsista: Terminei. Vou tomar um ar e já volto...\n");
    sem_post(&mutex); //Bolsista abre a porta do DCE para que mais alunos entrem
  }
}

void main(int argc, char **argv){
  if(argc != 2 || atoi(argv[1]) < 5){ //Checa se foi inserido apenas 1 argumento com valor igual ou maior que 5
    printf(COLOR_RED"Erro nos argumentos!\n"COLOR_RESET);
    exit(-1);
  }

  srand(time(NULL));

  int numeroDeAlunos = atoi(argv[1]), *threadsID, numeroDeThreads;
  pthread_t *t;

  numeroDeThreads = numeroDeAlunos + 1; //Numero de threads = numero de alunos mais 1 bolsista

  s = (sem_t*)malloc(numeroDeThreads*sizeof(sem_t));

  
  sem_init(&mutex, 0, 1); //Inicializa o semáforo de mutual exclusion com valor 1
  for(int i = 0; i < numeroDeThreads; i++){ 
    sem_init(&s[i], 0, 0); //Inicializa os semáforos dos alunos e do bolsista com valor 0
  }
  
	threadsID = (int*)malloc(numeroDeThreads * sizeof(int)); //Aloca um vetor do tamanho do número de alunos

	t = (pthread_t*)malloc(numeroDeThreads * sizeof(pthread_t)); //Cria threads equivalentes ao número de alunos
	
	for(int i = 0; i < numeroDeThreads; i++){ //Inicia todas as threads e atribui um ID a cada uma delas
		threadsID[i] = i;
    if(i == 0)
      pthread_create(&t[i], NULL, criaBolsista, &threadsID[i]);
    else
		  pthread_create(&t[i], NULL, criaFicha, &threadsID[i]);
	}

	for(int i = 0; i < numeroDeThreads; i++){ //Finaliza todas as threads
		pthread_join(t[i], NULL);
		printf(COLOR_YELLOW"Thread %d finalizada\n"COLOR_RESET, i);
	}
}