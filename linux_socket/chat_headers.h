#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <pthread.h>


int connection_count=10;//define the connection count
char* ip="192.168.81.129";
uint16_t port=8080;
int server_socket;

socklen_t sockaddrin_len=sizeof(struct sockaddr_in);
FILE* fileptr;//日志文件

typedef struct client_thread{
    pthread_t *pid;//process id
    int socket_id;
    struct sockaddr_in client_addr;
}client,*ClientPtr;

typedef struct data_exchange
{
    int client_count;//the default value is 0
    int max_count;
    client* clients;
}data_exchange, *DataExchangePtr;


data_exchange client_list;


void init()
{
    server_socket = socket(AF_INET,SOCK_STREAM,0);/*create a socket*/
    fileptr=fopen("./log.txt","w");
    client_list.max_count=10;
    client_list.client_count=0;
    client_list.clients=malloc(sizeof(client)*client_list.max_count);
}


/**
 * stop a client thread
 * @param c the client to stop
 * @return  0 for success, 1 for error;
*/
int stop_a_client_process(ClientPtr c)
{
    //pthread_cancel(*c->pid);
    //free(&c->pid);
    close(c->socket_id);
    return 0;
}

/**
 * add a client id to the end of the list;
 * @param c the client to added
 * @return the number of the changed element;
*/
int cl_add_client(DataExchangePtr list, client c)
{
    if (list->max_count<=list->client_count)// the list is full
    {
        return 0;
    }else{
        list->clients[list->client_count]=c;
        list->client_count++;
        //start_a_client_process(&list->clients[list->client_count-1]);//start process
        fprintf(fileptr,"%d connected",c.client_addr.sin_addr);
        fflush(fileptr);
        return 1;
    }
}


/**
 * delete a client id from the list ;
 * @param client the client to deleted
 * @return the number of the changed element;
*/
int cl_delete_client(DataExchangePtr list, ClientPtr c)
{
    fprintf(fileptr,"%d disconnected", c->client_addr.sin_addr);
    int i;
    for (i = 0; i < list->client_count; i++)
    {
        if (&list->clients[i]==c)
            break;
    }

    if (i==list->client_count)
        return 0;

    stop_a_client_process(&list->clients[i]);
    while (i<list->client_count)
    {
        list->clients[i]=list->clients[i+1];
        i++;
    }
    list->client_count--;
    
    fflush(fileptr);
    return 1;    
}

int isFull(DataExchangePtr list)
{
    return list->max_count<=list->client_count;
}

int closeLog(){
    fflush(fileptr);
    return fclose(fileptr);
}

