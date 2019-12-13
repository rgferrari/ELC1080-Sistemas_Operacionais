#include "curl_code.h"
#include <pthread.h>

// Para rodar este código:
// ./Sete_Cliques_Threads <número de jogadas>

// A função searchWord recebe um link, uma variável inteira que servirá de contador,
// um identificador da thread atual e uma string contendo a palavra que será buscada. 
// A função irá procurar a palavra dentro do link recebido e caso não encontre 
// irá chamar a si mesma enviando um novo link aleatório encontrado na página atual.
// Caso ela encontre a palavra irá retornar true.
// Se após 7 chamadas a função não encontrar a palavra que está sendo buscada ela retornará false.
bool searchWord(char *link, int iterations, char* targetWord, int threadID){
	CURL *curl_handle;

	// page_content recebe o conteudo da pagina de partida: Sistema_operativo
	char *page_content = download_page(curl_handle, link);

	// links_readed: variavel onde o numero de links lidos eh salvo.
	int links_readed;
	// links: lista de links lidos. A funcao find_links ira tentar ler 50
	// links dentro da pagina.
	char **links = find_links(curl_handle, page_content, 50, &links_readed);

	int random;

	// Caso tenha encontrado a palavra na página retorna true
	if(strstr(page_content, targetWord)){
		curl_global_cleanup();

		printf(COLOR_YELLOW "Thread %d:"COLOR_GREEN" Found the word at %s\n" COLOR_RESET, threadID, link);

		return true;
	}

	// Caso não tenha encontrado a palavra e ainda não tenha feito 7 iterações
	else if(iterations <= 7){
		//Sorteia um dos links para ser acessado 	
		while(random = rand() % links_readed){
			//Checa se o link sorteado não está na lista de links quebrados
			if(strstr(links[random], "Carregar_ficheiro") || strstr(links[random], "Especial:Aleatória") || strstr(links[random], "Wikipédia:Manutenção") || strstr(links[random], "Especial:Pesquisar") ||
            strstr(links[random], "Especial:GlobalRenameRequest"))
            	continue;
        	break;
		}
		
		printf(COLOR_YELLOW "Thread %d:"COLOR_RESET" Click %d - "COLOR_BLUE"%s\n"COLOR_RESET, threadID, iterations, links[random]);

		return searchWord(links[random], ++iterations, targetWord, threadID);
	}

	// Se não encontrar a palavra procurada depois de clicar em 7 liks, retorna false
	else{
		curl_global_cleanup();

		printf(COLOR_YELLOW "Thread %d:"COLOR_RED" Didn't find it :(\n" COLOR_RESET, threadID);
		
		return false;
	}
}

void * thread(void *arg){
	// Transforma o índice da thread de void* para int
	int threadID = *((int*)arg);

	printf(COLOR_YELLOW"Thread %d:"COLOR_GREEN" Started\n"COLOR_RESET, threadID);

	// Chama a função
	searchWord("https://pt.wikipedia.org/wiki/Sistema_operativo", 1, "Python", threadID);
}

// A main armazena em argc o número de argumentos inseridos no terminal e em argv um vetor com os argumentos inseridos em forma de string
int main(int argc, char **argv){
	srand(time(NULL));

	struct timeval start, end;

	//Transforma o número de jogadas para int
	int numberOfRuns = (*argv[1]) - '0';

	int *arguments;

	pthread_t *t;

	//Cria um vetor do tamanho do número do número de jogadas que irá armazenar o índice de cada thread
	arguments = (int*)malloc(numberOfRuns*sizeof(int));

	//Cria threads equivalentes ao número de jogadas
	t = (pthread_t*)malloc(numberOfRuns * sizeof(pthread_t));

	//Inicializa o clock
	gettimeofday(&start, NULL);
	
	//Inicia todas as threads
	for(int i = 0; i < numberOfRuns; i++){
		arguments[i] = i;
		pthread_create(&t[i], NULL, thread, &arguments[i]);
	}
	
	//Finaliza todas as threads
	for(int i = 0; i < numberOfRuns; i++){
		pthread_join(t[i], NULL);
		printf(COLOR_YELLOW"Thread %d:"COLOR_RED" Finished\n"COLOR_RESET, i);
	}

	//Finaliza o clock
	gettimeofday(&end, NULL);

	long double totalTimeInSeconds = (long double)((end.tv_sec - start.tv_sec)*1000000 + end.tv_usec - start.tv_usec)/1000000;

	printf("Total execution time: %.3Lf seconds\n", totalTimeInSeconds);
}
