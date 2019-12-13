#include "curl_code.h"


struct MemoryStruct {
	char *memory;
	size_t size;
};

static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	size_t realsize = size * nmemb;
	struct MemoryStruct *mem = (struct MemoryStruct *)userp;

	char *ptr = realloc(mem->memory, mem->size + realsize + 1);
	if(ptr == NULL) {
	/* out of memory! */
	printf("not enough memory (realloc returned NULL)\n");
	return 0;
	}

	mem->memory = ptr;
	memcpy(&(mem->memory[mem->size]), contents, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;

	return realsize;
}

char* download_page(CURL *curl_handle, const char* url){

	CURLcode res;

        struct MemoryStruct chunk;

        chunk.memory = malloc(1);  /* will be grown as needed by the realloc above */
        chunk.size = 0;    /* no data at this point */

        curl_global_init(CURL_GLOBAL_ALL);

        /* init the curl session */
        curl_handle = curl_easy_init();

        /* specify URL to get */
        curl_easy_setopt(curl_handle, CURLOPT_URL, url);

        /* send all data to this function  */
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

        /* we pass our 'chunk' struct to the callback function */
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);

        /* some servers don't like requests that are made without a user-agent
           field, so we provide one */
        curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

        /* get it! */
        res = curl_easy_perform(curl_handle);

        /* check for errors */
        if(res != CURLE_OK) {
          fprintf(stderr, "curl_easy_perform() failed: %s\n",
                  curl_easy_strerror(res));
        }
        else {
          /*
           * Now, our chunk.memory points to a memory block that is chunk.size
           * bytes big and contains the remote file.
           *
           * Do something nice with it!
           */

          // printf("%lu bytes retrieved\n", (unsigned long)chunk.size);
          // puts(chunk.memory);
	  /* cleanup curl stuff */
          curl_easy_cleanup(curl_handle);

          /* we're done with libcurl, so clean it up */
          // curl_global_cleanup();
	  return chunk.memory;

        }

	/* cleanup curl stuff */
        curl_easy_cleanup(curl_handle);


	return NULL;
}


char** find_links(CURL *curl_handle, char *str, int max_of_links, int *links_readed){

	char** links_list = (char**) malloc(max_of_links*sizeof(char*));
	char *new;
	int it = 0;

	do{
		new = strstr(str, "href=\"/wiki/");
		if(new == NULL || it + 1 > max_of_links){
			break;
		}
		char* pos = strchr(new+12, '"');

		int int_pos = pos - new + 1;

		char *out = (char*)malloc((int_pos-6) * sizeof(char));
		memcpy(out, new+6, int_pos-7);
		out[int_pos-7] = '\0';

		char url[int_pos-5 + 24];
		strcpy (url,"https://pt.wikipedia.org");
 		strcat (url,out);

		int int_pos0 = new - str + 1;
		str += int_pos + int_pos0;

		char *decoded;
		int tam;
		decoded = curl_easy_unescape(curl_handle, url, strlen(url), &tam);

		links_list[it] = decoded;
		it++;
		// puts(decoded);

	}while(new != NULL);

	*links_readed = it;
	return links_list;

}
