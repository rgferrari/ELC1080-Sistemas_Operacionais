all:
	gcc -c curl_code.c -lcurl
	gcc -o Sete_Cliques_Serial curl_code.o Sete_Cliques_Serial.c -lcurl
	gcc -o Sete_Cliques_Threads curl_code.o Sete_Cliques_Threads.c -lcurl -pthread
clean:
	rm curl_code.o
	rm Sete_Cliques_Serial
	rm Sete_Cliques_Threads
