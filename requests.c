#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <stdio.h>
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"

char *compute_get_request(char *host, const char *url, char *url_params, const char *authorization) {
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));
    
    /*
        Scriem numele metodei, calea, parametri din url (daca exista) si tipul protocolului
    */
    if (url_params != NULL) {
        sprintf(line, "GET %s?%s HTTP/1.1", url, url_params);
    } else {
        sprintf(line, "GET %s HTTP/1.1", url);
    }
   
    compute_message(message, line);

    /*
        Adaugam host-ul
    */
    sprintf(line, "%s", host);
    compute_message(message, line);

    /*
        Daca exista un token de autorizatie il adaugam
    */
    if (authorization != NULL) {
       sprintf(line, "Authorization: Bearer %s", authorization);
       compute_message(message, line);
    }

    return message;
}

char *compute_post_request(char *host, const char *url, char *form_data, const char *type) {

    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));
    /*
        Scriem numele metodei, calea si tipul protocolului
    */
    sprintf(line, "POST %s HTTP/1.1", url);
    compute_message(message, line);

    /*
        Adaugam host-ul
    */

    sprintf(line, "%s", host);
    compute_message(message, line);
    
    /*  
        Adaugam tipul
    */
    sprintf(line, "Content-Type: %s", type);
    compute_message(message, line);

    /*
        Adaugam lungimea
    */

    int length = strlen(form_data);
    sprintf(line, "Content-Length: %d", length);
    compute_message(message, line);

    return message;
}