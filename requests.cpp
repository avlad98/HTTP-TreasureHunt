#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"

// Am modificat cele doua functii astfel incat sa primeasca ca parametrii
// si headerele + cookie-urile
char *compute_get_request(char *host, char *url, char *url_params,
                        char *headers, char *cookies)
{
    char *message = (char*)calloc(BUFLEN, sizeof(char));
    char *line = (char*)calloc(LINELEN, sizeof(char));

    if (url_params != NULL) {
        sprintf(line, "GET %s?%s HTTP/1.1", url, url_params);
    }else {
        sprintf(line, "GET %s HTTP/1.1", url);
    }

    compute_message(message, line);

    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    // daca headerul nu exista atunci nu se adauga
    if(headers != NULL)
        compute_message(message, headers);

    // la fel si cu cookie-urile (desi nu va fi cazul in aceasta tema)
    if(cookies != NULL)
        compute_message(message, cookies);

    compute_message(message, "");
    free(line);
    return message;
}

// la fel si aceasta functie (in loc de header avem sectiune de form_data)
char *compute_post_request(char *host, char *url, char *content_type, 
                        char *form_data, char *cookies)
{
    char *message = (char*)calloc(BUFLEN, sizeof(char));
    char *line = (char*)calloc(LINELEN, sizeof(char));

    sprintf(line, "POST %s HTTP/1.1", url);
    compute_message(message, line);
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    sprintf(line, "Content-Type: %s", content_type);
    compute_message(message, line);
    sprintf(line, "Content-Length: %ld", strlen(form_data));
    compute_message(message, line);

    if(cookies)
        compute_message(message, cookies);

    strcat(message, form_data);
    compute_message(message, "");

    free(line);
    return message;
}