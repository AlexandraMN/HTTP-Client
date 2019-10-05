#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"
#include "json-c/json.h" /* biblioteca pentru parsare JSON */ 

int main(int argc, char *argv[]) {
    
    char *host_ip = "185.118.200.35";
    int portno = 8081;

    // task1
    char *message;
    char *response;
    int sockfd;

    sockfd = open_connection(host_ip, portno, AF_INET, SOCK_STREAM, 0);
    message = compute_get_request("Host: 185.118.200.35", "/task1/start", NULL, NULL);
    compute_message(message, "");
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    close_connection(sockfd);
    printf("Task1\n");
    printf("%s\n", message);
    printf("%s\n", response);
    printf("-----------------------------------------------------------------------\n");

    // task2
    // extrag textul JSON
    char *text = strstr(response, "{");
    struct json_object *parsed_json = json_tokener_parse(text);
    //parsez elementele de care o sa am nevoie in cerere
    struct json_object *url;
    json_object_object_get_ex(parsed_json, "url", &url);
    struct json_object *method;
    json_object_object_get_ex(parsed_json, "method", &method);
    struct json_object *type;
    json_object_object_get_ex(parsed_json, "type", &type);
    struct json_object *data;
    json_object_object_get_ex(parsed_json, "data", &data);
    struct json_object *username;
    json_object_object_get_ex(data, "username", &username);
    struct json_object *password;
    json_object_object_get_ex(data, "password", &password);

    char *message2;
    char *response2;
    int sockfd2;
    // creez campul necesar pentru logare
    char form_data[FORM_DATA_LEN];
    strcat(form_data, "username:");
    strcat(form_data, json_object_get_string(username));
    strcat(form_data, "&password:");
    strcat(form_data, json_object_get_string(password));

    sockfd2 = open_connection(host_ip, portno, AF_INET, SOCK_STREAM, 0);
    const char *url_string = json_object_get_string(url);
    if (strcmp(json_object_get_string(method), "POST") == 0) {
        message2 = compute_post_request("Host: 185.118.200.35", url_string, form_data, json_object_get_string(type));
    } else {
        message2 = compute_get_request("Host: 185.118.200.35", url_string, form_data, NULL);
    }
    compute_message(message2, "");
    compute_message(message2, form_data);
    send_to_server(sockfd2, message2);
    response2 = receive_from_server(sockfd2);
    close_connection(sockfd2);
    printf("Task2\n");
    printf("%s\n", message2);
    printf("%s\n", response2); 
    printf("-----------------------------------------------------------------------\n");    

    // task3
    char *text2 = strstr(response2, "{");
    struct json_object *parsed_json2 = json_tokener_parse(text2);

    struct json_object *url2;
    json_object_object_get_ex(parsed_json2, "url", &url2);
    struct json_object *method2;
    json_object_object_get_ex(parsed_json2, "method", &method2);
     struct json_object *type2;
    json_object_object_get_ex(parsed_json2, "type", &type2);
    struct json_object *data2;
    json_object_object_get_ex(parsed_json2, "data", &data2);

    struct json_object *token;
    json_object_object_get_ex(data2, "token", &token);
    struct json_object *queryParams;
    json_object_object_get_ex(data2, "queryParams", &queryParams);

    struct json_object *id;
    json_object_object_get_ex(queryParams, "id", &id);
    // fac campul de raspunsuri
    char my_response[TEXTLEN] = "";
    strcat(my_response, json_object_get_string(url2));
    strcat(my_response, "?raspuns1=omul&raspuns2=numele&id=");
    strcat(my_response, json_object_get_string(id));
    // creez campul pentru cookie-uri
    char cookie[TEXTLEN] = "";
    char *cookie_pos = strstr(response2, "Cookie:");
    char *cookie_fin = strstr(response2, "; path=/;");
    memcpy(&cookie, cookie_pos, strlen(cookie_pos) - strlen(cookie_fin));

    char cookie2[TEXTLEN] = "";
    char *cookie_pos2 = strstr(cookie_pos, "Set-Cookie:");
    char *cookie_fin2 = strstr(cookie_pos2, "; p");
    memcpy(&cookie2, cookie_pos2 + 11, strlen(cookie_pos2) - strlen(cookie_fin2) - 11);
    strcat(cookie, "; ");
    strcat(cookie, cookie2);

    char *message3;
    char *response3;
    int sockfd3;

    sockfd3 = open_connection(host_ip, portno, AF_INET, SOCK_STREAM, 0);
    if (strcmp(json_object_get_string(method2), "GET") == 0) {
        message3 = compute_get_request("Host: 185.118.200.35", my_response, NULL, json_object_get_string(token));
    } else {
        message3 = compute_post_request("Host: 185.118.200.35", json_object_get_string(url2), my_response, 
                                        json_object_get_string(type2));
    }
    compute_message(message3, cookie);
    compute_message(message3, "");
    send_to_server(sockfd3, message3);
    response3 = receive_from_server(sockfd3);
    close_connection(sockfd3);
    printf("Task3\n");
    printf("%s\n", message3);
    printf("%s\n", response3);
    printf("-----------------------------------------------------------------------\n");

    // task4
    char *text3 = strstr(response3, "{");
    struct json_object *parsed_json3 = json_tokener_parse(text3);

    struct json_object *url3;
    json_object_object_get_ex(parsed_json3, "url", &url3);
    struct json_object *method3;
    json_object_object_get_ex(parsed_json3, "method", &method3);
    struct json_object *type3;
    json_object_object_get_ex(parsed_json3, "type", &type3);
  
    char cookie3[TEXTLEN] = "";
    char *cookie_pos3 = strstr(response3, "Cookie:");
    char *cookie_fin3 = strstr(response3, "; path=/;");
    memcpy(&cookie3, cookie_pos3, strlen(cookie_pos3) - strlen(cookie_fin3));

    char cookie4[TEXTLEN] = "";
    char *cookie_pos4 = strstr(cookie_pos3, "Set-Cookie:");
    char *cookie_fin4 = strstr(cookie_pos4, "; path=/;"); 
    memcpy(&cookie4, cookie_pos4 + 11, strlen(cookie_pos4) - strlen(cookie_fin4) - 11);
    strcat(cookie3, "; ");
    strcat(cookie3, cookie4);

    char *message4;
    char *response4;
    int sockfd4;

    sockfd4 = open_connection(host_ip, portno, AF_INET, SOCK_STREAM, 0);
    if (strcmp(json_object_get_string(method3), "GET") == 0) {
        message4 = compute_get_request("Host: 185.118.200.35", json_object_get_string(url3), 
                                    NULL, json_object_get_string(token));
    } else {
        message4 = compute_get_request("Host: 185.118.200.35", json_object_get_string(url3), 
                                    NULL, json_object_get_string(type3));
    }
    
    compute_message(message4, cookie3);
    compute_message(message4, "");
    send_to_server(sockfd4, message4);
    response4 = receive_from_server(sockfd4);
    close_connection(sockfd4);
    printf("Task4\n");
    printf("%s\n", message4);
    printf("%s\n", response4);
    printf("-----------------------------------------------------------------------\n");    

    char *text4 = strstr(response4, "{");
    struct json_object *parsed_json4 = json_tokener_parse(text4);

    struct json_object *url4;
    json_object_object_get_ex(parsed_json4, "url", &url4);
    struct json_object *method4;
    json_object_object_get_ex(parsed_json4, "method", &method4);
    struct json_object *type4;
    json_object_object_get_ex(parsed_json4, "type", &type4);
    struct json_object *data4;
    json_object_object_get_ex(parsed_json4, "data", &data4);
    struct json_object *url_data;
    json_object_object_get_ex(data4, "url", &url_data);
    struct json_object *queryParams4;
    json_object_object_get_ex(data4, "queryParams", &queryParams4);
    struct json_object *q;
    json_object_object_get_ex(queryParams4, "q", &q);
    struct json_object *APPID;
    json_object_object_get_ex(queryParams4, "APPID", &APPID);
    struct json_object *method_data;
    json_object_object_get_ex(data4, "method", &method_data);
  
    char cookie5[TEXTLEN] = "";
    char *cookie_pos5 = strstr(response4, "Cookie:");
    char *cookie_fin5 = strstr(response4, "; path=/;");
    memcpy(&cookie5, cookie_pos5, strlen(cookie_pos5) - strlen(cookie_fin5));

    char cookie6[TEXTLEN] = "";
    char *cookie_pos6 = strstr(cookie_pos5, "Set-Cookie:");
    char *cookie_fin6 = strstr(cookie_pos6, "; path=/;"); 
    memcpy(&cookie6, cookie_pos6 + 11, strlen(cookie_pos6) - strlen(cookie_fin6) - 11);
    strcat(cookie5, "; ");
    strcat(cookie5, cookie6);

    char my_response2[LEN] = "";
    strcat(my_response2, "q=");
    strcat(my_response2, json_object_get_string(q));
    strcat(my_response2, "&APPID=");
    strcat(my_response2, json_object_get_string(APPID));

    char addr_ip[LEN];
    const char *url_data_string = json_object_get_string(url_data);
    char *url_position = strstr(url_data_string, "/");
    memcpy(addr_ip, url_data_string, strlen(url_data_string) - strlen(url_position));
    char *url_data_ip = strstr(url_data_string, "/");
    
    // determinare ip (functie laborator dns)
    int ret;
	struct addrinfo hints, *result, *p;

	// set hints
	memset(&hints, 0, sizeof(hints));
	char aux[LEN];

	// get addresses
	hints.ai_flags = AI_PASSIVE | AI_CANONNAME;
	hints.ai_family = AF_UNSPEC;
	ret = getaddrinfo(addr_ip, NULL, &hints, &result);
    if (ret < 0) {
        printf("Eroare getaddrinfo!");
    }

	// iterate through addresses
	for (p = result; p != NULL; p = p->ai_next) {
		if (p->ai_family == AF_INET) {
			struct sockaddr_in* addr = (struct sockaddr_in *) p->ai_addr;
			inet_ntop(p->ai_family, &addr->sin_addr, aux, sizeof(aux));
		} else if (p->ai_family == AF_INET6) {
			struct sockaddr_in6* addr = (struct sockaddr_in6 *) p->ai_addr;
			inet_ntop(p->ai_family, &addr->sin6_addr, aux, sizeof(aux));
		}
		
	}
	// free allocated data
	freeaddrinfo(result);

    char *message5;
    char *response5;
    int sockfd5;

    int portno_weather = 80;
    char host[LEN] = "Host: ";
    strcat(host, aux);

    sockfd5 = open_connection(aux, portno_weather, AF_INET, SOCK_STREAM, 0);
    if (strcmp(json_object_get_string(method_data), "GET") == 0) {
        message5 = compute_get_request(host, url_data_ip, my_response2, json_object_get_string(token));
    } else {
        message5 = compute_post_request(host, url_data_ip, my_response2, json_object_get_string(type4));
    }
    compute_message(message5, cookie5);
    compute_message(message5, "");
    send_to_server(sockfd5, message5);
    response5 = receive_from_server(sockfd5);
    close_connection(sockfd5);
    printf("Task5\n");
    printf("%s\n", message5);
    printf("%s\n", response5);
    printf("\n");

    char *form_data2 = strstr(response5, "{");

    char authorization[LINELEN] = "Authorization: Bearer ";
    strcat(authorization, json_object_get_string(token));

    char *message6;
    char *response6;
    int sockfd6;

    sockfd6 = open_connection(host_ip, portno, AF_INET, SOCK_STREAM, 0);
    if (strcmp(json_object_get_string(method4), "POST") == 0) {
        message6 = compute_post_request("Host: 185.118.200.35", json_object_get_string(url4), 
                                        form_data2, json_object_get_string(type4));    
    } else {
        message6 = compute_post_request("Host: 185.118.200.35", json_object_get_string(url4), 
                                        form_data2, json_object_get_string(token));
    }
    compute_message(message6, authorization);
    compute_message(message6, cookie5);
    compute_message(message6, "");
    compute_message(message6, form_data2);
    send_to_server(sockfd6, message6);
    response6 = receive_from_server(sockfd6);
    printf("%s\n", message6);
    printf("%s\n", response6);
    printf("\n");
    close_connection(sockfd6);

    return 0;
}