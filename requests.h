#ifndef _REQUESTS_
#define _REQUESTS_

#define BUFFSIZE 2000
#define URL_LEN 256
#define NEWLINE "\n"

char *compute_get_request(char *host, char *url, char *url_params,
                        char *form_data, char *cookies);
char *compute_post_request(char *host, char *url, char *content_type, 
                        char *form_data, char *cookies);
#endif