#include "chat_headers.h"


//参考代码
//https://github.com/shineyr/Socket
//https://blog.csdn.net/google19890102/article/details/60871829?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522167024225316782428637363%2522%252C%2522scm%2522%253A%252220140713.130102334..%2522%257D&request_id=167024225316782428637363&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~sobaiduend~default-1-60871829-null-null.142^v67^control,201^v4^add_ask,213^v2^t3_control2&utm_term=linux%20c%20%E5%A4%9A%E7%BA%BF%E7%A8%8B&spm=1018.2226.3001.4187
//https://baike.baidu.com/item/pthread_create/5139072?fr=aladdin thread
//https://blog.csdn.net/qq_39736982/article/details/82348672?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522167029293716782429744599%2522%252C%2522scm%2522%253A%252220140713.130102334..%2522%257D&request_id=167029293716782429744599&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~top_positive~default-1-82348672-null-null.142^v67^control,201^v4^add_ask,213^v2^t3_control2&utm_term=mutex&spm=1018.2226.3001.4187


/**
 * 接受用户的连接
 * @param server_socket the server socket id
*/
void *recieve_conn(void* server_socket)
{
    int client_socket;
    struct sockaddr_in client_addr;
    client client_struct;
    printf("%s", "listening client...\n");
    while (1)
    {
        while(isFull(&client_list)) sleep(50);
        if((client_socket = accept(*((int*)server_socket) , (struct sockaddr *)&client_addr , &sockaddrin_len)) < 0 )// return a client socket id;
        {
            perror("accept error\n");
            //exit(1);
        }else{
            client_struct.client_addr=client_addr;
            client_struct.socket_id=client_socket;
            cl_add_client(&client_list, client_struct);
        }
    }
}

int main(int argc, char** argv)
{    
    init();
    if (argc!=2)
    {
        printf("usage: chat (ip) %s",argv[1]);
        exit(1);
    }else{
        ip=argv[1];
        printf("%s:%d",argv[1],port);
    }

    printf("program starting ... \n");
    pthread_t *pt_recieve_conn = (pthread_t *)malloc(sizeof(pthread_t));
    struct sockaddr_in server_addr;// thre server addr
    memset(&server_addr, 0, sizeof(struct sockaddr_in));//clear the server addr
    server_addr.sin_family = AF_INET;//set protocol family
    server_addr.sin_port = htons(port);//set port
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);//set ip addr, the availiable ips 123is all in use
    
    //bind the socket and port
    if(bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr))<0)
    {
        printf("bind addr error!\n");
        exit(1);
    }

    //listen the clients' connections
    if (listen(server_socket, connection_count)<0)
    {
        printf("listen error\n");
        exit(1);
    }
    
    //启动一个新的线程来接受用户的连接
    if (pthread_create(pt_recieve_conn, NULL, recieve_conn, &server_socket) != 0)
    {
        printf("accpet thread create error\n");
    }

    int i;

    char send_buf[4096];
    char rev_buf[4096];    
    while (1)
    {
        printf("input the msg:");
        scanf("%s",&send_buf);
        if (send_buf[0]=='q'||send_buf[0]=='Q')
            return;
        //向所有连接的电脑发送字符串
        for ( i = 0; i < client_list.client_count; i++)
            send(client_list.clients[i].socket_id, send_buf, sizeof(send_buf), 0); 
    }
    
    
    
    //kill the accept thread
    if(pthread_cancel(*pt_recieve_conn)!=0)
    {
        printf("accpet thread cancel error!");
        exit(1);
    }
    free(pt_recieve_conn);

    //close all the connections
    for ( i = 0; i < client_list.client_count; i++)
        cl_delete_client(&client_list,&client_list.clients[i]);

    close(server_socket);//close the server
    closeLog();//关闭日志
    printf("%s","program over!\n");
    system("pause");
}