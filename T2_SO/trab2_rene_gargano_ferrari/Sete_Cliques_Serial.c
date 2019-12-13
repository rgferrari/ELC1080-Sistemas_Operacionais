#include "curl_code.h"

// Para rodar este código:
// ./Sete_Cliques_Serial <número de jogadas>

// A função searchWord recebe um link, uma variável inteira que servirá de contador
// e uma string contendo a palavra que será buscada. 
// A função irá procurar a palavra dentro do link recebido e caso não encontre 
// irá chamar a si mesma enviando um novo link aleatório encontrado na página atual.
// Caso ela encontre a palavra irá retornar true.
// Se após 7 chamadas a função não encontrar a palavra que está sendo buscada ela retornará false.
bool searchWord(char *link, int iterations, char* targetWord){
	CURL *curl_handle;

	// page_content recebe o conteudo da pagina de partida: Sistema_operativo
	char *page_content = download_page(curl_handle, link);

	// links_readed: variavel onde o numero de links lidos eh salvo.
	int links_readed;
	// links: lista de links lidos. A funcao find_links ira tentar ler 50
	// links dentro da pagina.
	char **links = find_links(curl_handle, page_content, 50, &links_readed);

	int random;

	// Caso tenha encontrado a palavra na página
	if(strstr(page_content, targetWord)){
		curl_global_cleanup();

		printf(COLOR_GREEN"Found the word at %s\n\n"COLOR_RESET, link);

		return true;
	}

	// Caso não tenha encontrado a palavra e ainda não tenha feito 7 iterações
	else if(iterations <= 7){
		// Sorteia um dos links para ser acessado	
		while(random = rand() % links_readed){
			// Checa se o link sorteado não está na lista de links quebrados
			if(strstr(links[random], "Carregar_ficheiro") || strstr(links[random], "Especial:Aleatória") || strstr(links[random], "Wikipédia:Manutenção") || strstr(links[random], "Especial:Pesquisar") ||
            strstr(links[random], "Especial:GlobalRenameRequest"))
            	continue;
        	break;
		}
		
		printf("Click %d - "COLOR_BLUE"%s\n"COLOR_RESET, iterations, links[random]);

		return searchWord(links[random], ++iterations, targetWord);
	}

	// Se não encontrar a palavra procurada depois de clicar em 7 liks, retorna false
	else{
		curl_global_cleanup();

		printf(COLOR_RED "Didn't find it ;(\n\n" COLOR_RESET);
		
		return false;
	}
}

// A main armazena em argc o número de argumentos inseridos no terminal e em argv um vetor com os argumentos inseridos em forma de string
int main(int argc, char **argv){
	srand(time(NULL));

	struct timeval start, end;

	//Transforma o número de jogadas para int
	int numberOfRuns = (*argv[1]) - '0';
	
	// Inicializa o clock
	gettimeofday(&start, NULL);

	// Chama a função o número de vezes que o jogador tiver escolhido
	for(int i = 0; i < numberOfRuns; i++){
		printf(COLOR_YELLOW "Run %d:\n" COLOR_RESET, i+1);
		searchWord("https://pt.wikipedia.org/wiki/Sistema_operativo", 1, "Python");
	}

	// Finaliza o clock
	gettimeofday(&end, NULL);

	long double totalTimeInSeconds = (long double)((end.tv_sec - start.tv_sec)*1000000 + end.tv_usec - start.tv_usec)/1000000;

	printf("Total execution time: %.3Lf seconds\n", totalTimeInSeconds);
}
