#ifndef CURL_CODE
#define CURL_CODE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <time.h>
#include <sys/time.h>
#include <stdbool.h>
#define COLOR_RED "\x1b[31m"
#define COLOR_GREEN "\x1b[32m"
#define COLOR_YELLOW "\x1b[33m"
#define COLOR_BLUE "\x1b[34m"
#define COLOR_RESET "\x1b[0m"


static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);

// Funcao que faz o download do conteudo de uma pagina web e retorna esse conteudo
// na variavel de retorno char *.
char* download_page(CURL *curl_handle, const char* url);

// Funcao que extrai os links internos de uma pagina do wikipedia pt-br. A Funcao
// recebe como argumento a struct CURL, o conteudo de uma pagina web, o numero
// maximo de links que devem ser lidos e um inteiro onde a funcao salva o numero
// de links lidos. A funcao retorna uma lista de links, o tamanho alocado para a
// lista é igual ao tamanho informado em max_of_links.
char** find_links(CURL *curl_handle, char *str, int max_of_links, int *links_readed);

#endif
