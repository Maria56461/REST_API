#include <stdio.h>      
#include <stdlib.h>     
#include <unistd.h>     
#include <string.h>    
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <netdb.h>      
#include <arpa/inet.h>
#include "helpers.c"
#include <math.h>
#define BUF 4096
#define LINE 1000

char *compute_register_post(char *username, char *password) {

	  char *message = calloc(BUF, sizeof(char));
    char *line = calloc(LINE, sizeof(char));

    compute_message(message, "POST /api/v1/tema/auth/register HTTP/1.1");
    compute_message(message, "Host: 34.118.48.238");
    compute_message(message, "Content-Type: application/json");
    int length = 29 + strlen(username) + strlen(password);
    sprintf(line, "Content-Length: %d", length);
    compute_message(message, line); 
    compute_message(message, "");
    sprintf(line, "{\"username\":\"%s\",\"password\":\"%s\"}", username, password);
    compute_message(message, line); 

return message;
}

char *compute_login_post(char *username, char *password) {

	  char *message = calloc(BUF, sizeof(char));
    char *line = calloc(LINE, sizeof(char));

    compute_message(message, "POST /api/v1/tema/auth/login HTTP/1.1");
    compute_message(message, "Host: 34.118.48.238");
    compute_message(message, "Content-Type: application/json");
    int length = 29 + strlen(username) + strlen(password);
    sprintf(line, "Content-Length: %d", length);
    compute_message(message, line); 
    compute_message(message, "");
    sprintf(line, "{\"username\":\"%s\",\"password\":\"%s\"}", username, password);
    compute_message(message, line); 

return message;
}

char *compute_enter_library_get(char *cookie) {

  char *message = calloc(BUF, sizeof(char));
  char *line = calloc(LINE, sizeof(char));
  compute_message(message, "GET /api/v1/tema/library/access HTTP/1.1");
  compute_message(message, "Host: 34.118.48.238:8080");
  sprintf(line, "Cookie: connect.sid=%s\n", cookie);
  compute_message(message, line); 
  
  return message;
}

char *compute_get_books(char *cookie, char* jwt) {

  char *message = calloc(BUF, sizeof(char));
  char *line = calloc(LINE, sizeof(char));
  compute_message(message, "GET /api/v1/tema/library/books HTTP/1.1");
  compute_message(message, "Host: 34.118.48.238:8080");
  sprintf(line, "Authorization: Bearer %s", jwt);
  compute_message(message, line);
  sprintf(line, "Cookie: connect.sid=%s\n", cookie);
  compute_message(message, line); 
  
  return message;
}

char *compute_get_book(char* jwt, int id) {

  char *message = calloc(BUF, sizeof(char));
  char *line = calloc(LINE, sizeof(char));
  sprintf(line, "GET /api/v1/tema/library/books/%d HTTP/1.1", id);
  compute_message(message, line);
  compute_message(message, "Host: 34.118.48.238");
  sprintf(line, "Authorization: Bearer %s\n", jwt);
  compute_message(message, line);
  
  return message;
}

char *compute_add_book_post(char* jwt, char* title, char* author, char* genre,
  int page_count, char* publisher) {

  char *message = calloc(BUF, sizeof(char));
  char *line = calloc(LINE, sizeof(char));
  compute_message(message, "POST /api/v1/tema/library/books/ HTTP/1.1");
  compute_message(message, "Host: 34.118.48.238");
  sprintf(line, "Authorization: Bearer %s", jwt);
  compute_message(message, line);
  compute_message(message, "Content-Type: application/json");
  int length = 65 + strlen(title) + strlen(author) + strlen(genre) + strlen(publisher) + floor(log10(abs(page_count)));
  sprintf(line, "Content-Length: %d", length);
  compute_message(message, line); 
  compute_message(message, "");
  int length_title = strlen(title);
  for (int i = 0; i < length_title; i++) {
    if (title[i] == '\n') {
      title[i] = '\0';
    }
  }
  int length_author = strlen(author);
  for (int i = 0; i < length_author; i++) {
    if (author[i] == '\n') {
      author[i] = '\0';
    }
  }
  int length_publisher = strlen(publisher);
  for (int i = 0; i < length_publisher; i++) {
    if (publisher[i] == '\n') {
      publisher[i] = '\0';
    }
  }
  int length_genre = strlen(genre);
  for (int i = 0; i < length_genre; i++) {
    if (genre[i] == '\n') {
      genre[i] = '\0';
    }
  }
  sprintf(line, "{\"title\":\"%s\",\"author\":\"%s\", \"genre\":\"%s\", \"page_count\":%d, \"publisher\":\"%s\"}",
    title, author, genre, page_count, publisher);
  compute_message(message, line); 
  
  return message;
}

char *compute_delete_book(char* jwt, int id) {
  
  char *message = calloc(BUF, sizeof(char));
  char *line = calloc(LINE, sizeof(char));
  sprintf(line, "DELETE /api/v1/tema/library/books/%d HTTP/1.1", id);
  compute_message(message, line);
  compute_message(message, "Host: 34.118.48.238");
  sprintf(line, "Authorization: Bearer %s\n", jwt);
  compute_message(message, line);
  
  return message;
}

int main() {
    
    char type[100]; 
    char *cookie;
    char *jwt = NULL;
    scanf("%s", type);
    while (strcmp(type, "exit") != 0) {
      int sockfd = open_connection("34.118.48.238", 8080, AF_INET, SOCK_STREAM, 0);
      if (strcmp(type, "register") == 0) {
    	 printf("username=");
    	 char username[100];
    	 scanf("%s", username);
    	 printf("password=");
    	 char password[100];
    	 scanf("%s", password);
    	 char *message = compute_register_post(username, password);
		 send_to_server(sockfd, message);
    	 char* response = receive_from_server(sockfd);
    	 char* token = strtok(response, "\n");
    	 int index = 0; 
   		 while( token != NULL ) {
   			index++; 
   			if (index == 19) {
   			  printf( " %s\n", token);
   			}
      	token = strtok(NULL, "\n");
   		 }
   		 if (index < 19) {
   		   printf("Inregistrarea s-a realizat cu succes!\n");
   		 }
      }
      else if (strcmp(type, "login") == 0) { 
    	 printf("username=");
    	 char username[100];
    	 scanf("%s", username);
    	 printf("password=");
    	 char password[100];
    	 scanf("%s", password);
    	 char *message = compute_login_post(username, password);
		 send_to_server(sockfd, message);
    	 char* response = receive_from_server(sockfd);
    	 char* copy = malloc((strlen(response) + 1) * sizeof(char));
       	 strcpy(copy, response);
    	 char* token = strtok(response, " \n");
    	 int index = 0; 
   		 while (token != NULL ) {
   		   index++; 
   		   if ((index == 2) && (strcmp(token, "200") == 0)) {
   				// intoarce cookie de sesiune
   			  char* token1 = strtok(copy, " \n=;");
   				int index1 = 0; 
   				while (token1 != NULL) {
            index1++;
   					if (index1 == 24) {
   						printf("Autentificare realizata cu succes! Cookie-ul de sesiune este:\n%s\n", token1);
              cookie = malloc(strlen(token1) * sizeof(char));
              strcpy(cookie, token1);
   						break;
   					}
            token1 = strtok(NULL, " \n;=");
   				}
   		   }
         else if ((index == 40) && (strcmp(token, "{\"error\":\"Credentials") == 0)) {
          printf("Eroare! Parola incorecta!!!\n");
         }
         else if ((index == 40) && (strcmp(token, "{\"error\":\"No") == 0)) {
          printf("Eroare! Nu exista cont cu acest username!\n");
         }
      	 token = strtok(NULL, " \n");
   		 }
      }
      else if (strcmp(type, "enter_library") == 0){
        char *message1 = compute_enter_library_get(cookie);
        send_to_server(sockfd, message1);
        char* response1 = receive_from_server(sockfd);
        char* token = strtok(response1, " \n\"");
        int index = 0; 
        while (token != NULL) {
          index++;
          if ((index == 2) && (strcmp(token, "400") == 0)) {
            printf("Eroare: Nu te-ai logat la server!\n");
          }
          else if (index == 44) {
            jwt = malloc(strlen(token) * sizeof(char));
            strcpy(jwt, token);
            printf("Cerere realizata cu succes! Tockenul pentru accesul in biblioteca este: %s\n", jwt);
          }
          token = strtok(NULL, " \n\"");
        } 
      }
      else if (strcmp(type, "get_books") == 0) {
        char *message = compute_get_books(cookie, jwt);
        send_to_server(sockfd, message);
        char* response = receive_from_server(sockfd);
        char* token = strtok(response, " \n{}");
        int index = 0; 
        while (token != NULL) {
          index++;
          if ((index == 2) && (strcmp(token, "400") == 0)) {
            printf("Eroare: Nu te-ai logat la server!\n");
          }
          else if (index == 38) {
            printf("Succes! Cartile din biblioteca sunt:\n");
          }
          else if (index >= 39) {
            printf("%s ", token);
          }
          token = strtok(NULL, " \n{}");
        }
        printf("\n");
      }
      else if (strcmp(type, "get_book") == 0) {
        printf("id=");
        int id;
        scanf("%d", &id);
        char *message = compute_get_book(jwt, id);
        send_to_server(sockfd, message);
        char* response = receive_from_server(sockfd);
        int index = 0; 
        char* token = strtok(response, " \n{}");
        while (token != NULL) {
          index++;
          if ((index == 2) && ((strcmp(token, "400") == 0) ||
             (strcmp(token, "403") == 0) || (strcmp(token, "500") == 0))) {
            printf("Eroare: Nu ai acces la biblioteca!\n");
            break;
          }
          else if ((index == 2) && (strcmp(token, "404") == 0)) {
            printf("Eroare: Nu exista aceasta carte!\n");
            break;
          }
          else if (index == 38) {
            printf("Succes! Iata niste informatii despre cartea cautata:\n");
          }
          else if (index >= 39) {
            printf("%s ", token);
          }
          token = strtok(NULL, " \n{}");
        }
        printf("\n");
      }
      else if (strcmp(type, "add_book") == 0) {
        printf("title=");
        char title[50];
        char first[2];
        char author[50];
        fgets(first, 2, stdin);
        fgets(title, 50, stdin);
        printf("author=");
        fgets(author, 50, stdin);
        printf("genre=");
        char genre[50];
        fgets(genre, 50, stdin);
        printf("publisher=");
        char publisher[50];
        fgets(publisher, 50, stdin);
        printf("page_count=");
        int page_count;
        scanf("%d", &page_count);
        char *message = compute_add_book_post(jwt, title, author, genre, page_count, publisher);
        send_to_server(sockfd, message);
        char* response = receive_from_server(sockfd);
        int index = 0; 
        char* token = strtok(response, " \n");
        if (jwt == NULL) {
        	printf("Eroare: Nu aveti acces in biblioteca!\n");
        }
        else {
        	while (token != NULL) {
          		index++;
          		if ((index == 2) && ((strcmp(token, "400") == 0))) {
            	printf("Eroare! Informatiile introduse sunt incomplete/ nu respecta formatarea!\n");
            	break;
          		} 
          		else if ((index == 2) && (strcmp(token, "403") == 0)) {
            		printf("Eroare: Nu aveti acces in biblioteca!\n");
            		break;
          		}
          		else if ((index == 2) && (strcmp(token, "429") == 0)){
            		printf("Eroare! Prea multe cereri intr-un timp scurt!\n");
            		break;
          		}
          		else if ((index == 2) && (strcmp(token, "200") == 0)){
            		printf("Succes! Cartea a fost adaugata!\n");
            		break;
          		}
          		token = strtok(NULL, " \n");
        	}
    	}
      }
      else if (strcmp(type, "delete_book") == 0) {
        printf("id=");
        int id;
        scanf("%d", &id);
         if (jwt == NULL) {
        	printf("Eroare: Nu ai acces la biblioteca!\n");
        	break;
        }
        char *message = compute_delete_book(jwt, id);
        //puts(message);
        send_to_server(sockfd, message);
        char* response = receive_from_server(sockfd);
        //puts(response);
        int index = 0; 
        if (jwt != NULL) {
        	char* token = strtok(response, " \n");
        	while (token != NULL) {
          		index++;
          		if ((index == 2) && (strcmp(token, "404") == 0)) {
            		printf("Eroare: Nu exista aceasta carte!\n");
            		break;
          		}
          		else if (index == 2){
            		printf("Succes! Cartea a fost stearsa!\n");
            		break;
          		}
        		token = strtok(NULL, " \n");
        	}
        }
      }
      scanf("%s", type);
      close_connection(sockfd);
    }

	return 0;
}














