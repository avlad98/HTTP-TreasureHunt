#pragma GCC diagnostic ignored "-Wwrite-strings"

#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <sys/types.h>
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"
#include <queue>
#include <string>
#include <sstream>
#include <iostream>
#include "parson.h"

using namespace std;

#define SERVER_IP "185.118.200.35"
#define SERVER_PORT 8081

string getCookieFrom(string token) {
    token.erase(0, strlen("Set-Cookie: "));
    return token;
}

int main(int argc, char *argv[])
{
    /* *********************** Etapa I *********************** */

    char *message;
    char* response;
    int sockfd;

    /* Fac prima cerere catre server */

    char nextURL[URL_LEN];
    sprintf(nextURL, "%s", "/task1/start");

    sockfd = open_connection(SERVER_IP, SERVER_PORT, AF_INET, SOCK_STREAM, 0);
    message = compute_get_request(SERVER_IP, nextURL, NULL, NULL, NULL);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    close_connection(sockfd); 


    printf("****************** RASPUNS ETAPA 1: ******************\n%s\n\n\n", response);
    free(message);
    free(response);



    /* *********************** Etapa II *********************** */

    // deplasament = nr de octeti pana la inceputul cookie-ului
    size_t deplasament = strlen("Set-Cookie: ");

    char *temp;
    char *cookie;
    char *cursor;

    // cookies = bufferul in care alcatuiesc cookie-urile sub forma:
    // "Cookie: cookie1=val1; cookie2=val2; ..."
    char cookies[BUFFSIZE];
    memset(cookies, 0, sizeof(cookies));

    // vectorul in care tin liniile care contin cookie-urile
    vector<char *> cookies_arr;

    // memorie alocata pentru a stoca json-ul
    char *json_txt;

    // parsez fiecare linie
    temp = strtok(response, NEWLINE);
    while((temp = strtok(NULL, NEWLINE)) != NULL) {;

        // verific daca este linie cu cookie
        if((cursor = strstr(temp, "Set-Cookie")) != NULL) {
            cursor += deplasament;
            char *sfarsit = strstr(cursor, ";");
            cookie = (char*)calloc(1, sfarsit - cursor + 1);
            if(!cookie){break;}
            memcpy(cookie, cursor, sfarsit - cursor);
            cookies_arr.push_back(cookie);
        }

        // verific daca este linia cu json
        if((cursor = strstr(temp, "{")) != NULL) {
            json_txt = (char*)calloc(1, strlen(temp)+1);
            if(!json_txt){break;}
            sprintf(json_txt, "%s", temp);
        }
    }

    // Scrie in format corespunzator cookies
    strcat(cookies, "Cookie: ");
    //extrag cookie-ul din vector
    while(!cookies_arr.empty()) {
        // concatenez cookie-ul curent la cele adaugate deja
        strcat(cookies, *cookies_arr.begin());
        strcat(cookies, "; ");

        // si eliberez memoria
        free(*cookies_arr.begin());
        cookies_arr.erase(cookies_arr.begin());
    }
    strcat(cookies, "\r\n");


    // parsez json-ul si extrag campurile care ma intereseaza
    JSON_Value *json = json_parse_string(json_txt);
    JSON_Object *obj = json_value_get_object(json);

    char *url = (char*)json_object_get_string(obj, "url");
    char *content_type = (char*)json_object_get_string(obj, "type");
    char *username = (char*)json_object_dotget_string(obj, "data.username");
    char *password = (char*)json_object_dotget_string(obj, "data.password");

    // alcatuiesc username-ul si parola in formatul corespunzator
    char form_data[1000];
    sprintf(form_data, "username=%s&password=%s", username, password);

    // redeschid socketul si compun mesajul cu ajutorul functiei din laborator
    sockfd = open_connection(SERVER_IP, SERVER_PORT, AF_INET, SOCK_STREAM, 0);
    message = compute_post_request(SERVER_IP, url, content_type, form_data, cookies);

    json_value_free(json);
    free(json_txt);

    // trimit mesajul si afisez raspunsul
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);

    //cand termin cu serverul pentru o etapa inchid si socket-ul
    close_connection(sockfd); 

    printf("****************** RASPUNS ETAPA 2: ******************\n%s\n\n\n", response);
    free(message);
    free(response);

    /* *********************** Etapa III *********************** */

    // resetez zona de memorie in care stochez cookie-urile si fac aceiasi pasi ca
    // si pana acum
    memset(cookies, 0, sizeof(cookies));
    temp = strtok(response, NEWLINE);
    while((temp = strtok(NULL, NEWLINE)) != NULL) {;

        if((cursor = strstr(temp, "Set-Cookie")) != NULL) {
            cursor += deplasament;
            char *sfarsit = strstr(cursor, ";");
            cookie = (char*)calloc(1, sfarsit - cursor + 1);
            if(!cookie){break;}
            memcpy(cookie, cursor, sfarsit - cursor);
            cookies_arr.push_back(cookie);
        }

        if((cursor = strstr(temp, "{")) != NULL) {
            json_txt = (char*)calloc(1, strlen(temp)+1);
            if(!json_txt){break;}
            sprintf(json_txt, "%s", temp);
        }
    }

    // Scrie in format corespunzator cookies
    strcat(cookies, "Cookie: ");
    while(!cookies_arr.empty()) {
        strcat(cookies, *cookies_arr.begin());
        strcat(cookies, "; ");

        free(*cookies_arr.begin());
        cookies_arr.erase(cookies_arr.begin());
    }
    strcat(cookies, "\r\n");

    json = json_parse_string(json_txt);
    obj = json_value_get_object(json);

    char *url_params = (char*)calloc(1, 2048);
    if(!url_params){return -1;}

    // obtin datele necesare
    char *token = (char*)json_object_dotget_string(obj, "data.token");
    char *id = (char*)json_object_dotget_string(obj, "data.queryParams.id");
    url = (char*)json_object_get_string(obj, "url");

    // si formez header-ul de autentificare pentru JWT
    char *authorization = (char*)calloc(1, 2048);
    if(!authorization){return -1;}

    sprintf(authorization, "Authorization: Bearer %s", token);
    sprintf(url_params, "raspuns1=omul&raspuns2=numele&id=%s", id);

    sockfd = open_connection(SERVER_IP, SERVER_PORT, AF_INET, SOCK_STREAM, 0);
    message = compute_get_request(SERVER_IP, url, url_params, authorization, cookies);   
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    close_connection(sockfd); 

    printf("****************** RASPUNS ETAPA 3: ******************\n%s\n\n\n", response);
    json_value_free(json);
    free(json_txt);
    free(url_params); 
    free(response);
    free(message);


    /* *********************** Etapa IV *********************** */
    memset(cookies, 0, sizeof(cookies));
    temp = strtok(response, NEWLINE);
    while((temp = strtok(NULL, NEWLINE)) != NULL) {;

        if((cursor = strstr(temp, "Set-Cookie")) != NULL) {
            cursor += deplasament;
            char *sfarsit = strstr(cursor, ";");
            cookie = (char*)calloc(1, sfarsit - cursor + 1);
            if(!cookie){break;}
            memcpy(cookie, cursor, sfarsit - cursor);
            cookies_arr.push_back(cookie);
        }

        if((cursor = strstr(temp, "{")) != NULL) {
            json_txt = (char*)calloc(1, strlen(temp)+1);
            if(!json_txt){break;}
            sprintf(json_txt, "%s", temp);
        }
    }

    // Scrie in format corespunzator cookies
    strcat(cookies, "Cookie: ");
    while(!cookies_arr.empty()) {
        strcat(cookies, *cookies_arr.begin());
        strcat(cookies, "; ");

        free(*cookies_arr.begin());
        cookies_arr.erase(cookies_arr.begin());
    }
    strcat(cookies, "\r\n");

    json = json_parse_string(json_txt);
    obj = json_value_get_object(json);

    url = (char*)json_object_get_string(obj, "url");
    url_params = NULL;

    sockfd = open_connection(SERVER_IP, SERVER_PORT, AF_INET, SOCK_STREAM, 0);
    message = compute_get_request(SERVER_IP, url, url_params, authorization, cookies);   
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    close_connection(sockfd); 
    printf("****************** RASPUNS ETAPA 4: ******************\n%s\n\n\n", response);
    json_value_free(json);
    free(json_txt);
    free(response);
    free(message);


    /* *********************** Etapa V *********************** */
    memset(cookies, 0, sizeof(cookies));
    temp = strtok(response, NEWLINE);
    while((temp = strtok(NULL, NEWLINE)) != NULL) {;

        if((cursor = strstr(temp, "Set-Cookie")) != NULL) {
            cursor += deplasament;
            char *sfarsit = strstr(cursor, ";");
            cookie = (char*)calloc(1, sfarsit - cursor + 1);
            if(!cookie){break;}
            memcpy(cookie, cursor, sfarsit - cursor);
            cookies_arr.push_back(cookie);
        }

        if((cursor = strstr(temp, "{")) != NULL) {
            json_txt = (char*)calloc(1, strlen(temp)+1);
            if(!json_txt){break;}
            sprintf(json_txt, "%s", temp);
        }
    }

    // Scrie in format corespunzator cookies
    strcat(cookies, "Cookie: ");
    while(!cookies_arr.empty()) {
        strcat(cookies, *cookies_arr.begin());
        strcat(cookies, "; ");

        free(*cookies_arr.begin());
        cookies_arr.erase(cookies_arr.begin());
    }
    strcat(cookies, "\r\n");

    json = json_parse_string(json_txt);
    obj = json_value_get_object(json);

    url = (char*)json_object_get_string(obj, "url");
    content_type = (char*)json_object_get_string(obj, "type");
    url_params = NULL;

    // Extrag informatiile necesare pentru a interoga serverul openweather.com
    char *weather_url = (char*)json_object_dotget_string(obj, "data.url");
    char *city = (char*)json_object_dotget_string(obj, "data.queryParams.q");
    char *appid = (char*)json_object_dotget_string(obj, "data.queryParams.APPID");

    // fac cerere dns pentru adresa IP a site-ului
    char tempURL[100];
    sprintf(tempURL, "%s", weather_url);
    char *tempName = strtok(tempURL, "/");
    
	struct addrinfo hints, *res, *p;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	int errcode = getaddrinfo(tempName, NULL, &hints, &res);
	if(errcode != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(errcode));
		return -1;
	}

    // din toate adresele returnate o folosesc doar pe prima si o convertesc in
    // format ASCII
	p = res;
	void *ptr =  &((struct sockaddr_in *) p->ai_addr)->sin_addr;
	char weather_ip[100];
	inet_ntop(p->ai_family, ptr, weather_ip, sizeof(weather_ip));
    freeaddrinfo(res);

    url_params = (char*)calloc(1, 500);
    if(!url_params){return -1;}

    sprintf(url_params, "q=%s&APPID=%s", city, appid);

    /* *********************** OPENWEATHER *********************** */
    // deschid conexiunea cu serverul openweather
    int weather_sock = open_connection(weather_ip, 80, AF_INET, SOCK_STREAM, 0);
    if(weather_sock < 0){printf("Nu s-a deschis socket-ul cu openweather\n"); return -1;}
    
    // ii scriu mesajul si il trimit
    message = compute_get_request(weather_ip, strstr(weather_url, "/"), url_params, NULL, NULL);       
    send_to_server(weather_sock, message);    
    response = receive_from_server(weather_sock);
    close_connection(weather_sock);
    printf("****************** OPENWEATHER: ******************\n%s\n\n\n", response);

    // din mesajul primit extrag json-ul (verific daca linia curenta contine "{")
    char *weather_json;

    temp = strtok(response, NEWLINE);
    while((temp = strtok(NULL, NEWLINE)) != NULL) {;
        if((cursor = strstr(temp, "{")) != NULL) {
            weather_json = (char*)calloc(1, strlen(temp)+1);
            if(!weather_json){break;}
            sprintf(weather_json, "%s", temp);
        }
    }

    // si compun mesajul care trebuie trimis la serverul temei
    char *headers = (char*)calloc(1, 4000);
    if(!headers) {return -1;}

    sprintf(headers, "%s\r\n%s", authorization, cookies);

    // redeschid conexiunea cu acesta (ca pana acum)
    sockfd = open_connection(SERVER_IP, SERVER_PORT, AF_INET, SOCK_STREAM, 0);
    message = compute_post_request(SERVER_IP, url, content_type, weather_json, headers);   
    // si dupa ce ii alcatuiesc mesajul i-l trimit
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    close_connection(sockfd); 
    printf("****************** RASPUNS ETAPA 5: ******************\n%s\n\n\n", response);

    // la final eliberez toata memoria alocata de mine si de API-ul pentru json
    json_value_free(json);
    free(json_txt);
    free(url_params); 
    free(response);
    free(message);
    free(headers);
    free(weather_json);

    return 0;
}