#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <pthread.h>



int main(int argc, char** argv)
{
    int i;
    char rev_buf[4096];  
    int client_socket_fd=socket(AF_INET,SOCK_STREAM,0);/*create a socket*/
    char* server_ip="192.168.81.129";
    uint16_t server_port=8080;

    if (argc!=2)
    {
        printf("usage: client (ip) %s",argv[1]);
        exit(1);
    }else{
        server_ip=argv[1];
        printf("%s:%d",argv[1],server_port);
    }


    printf("program starting ... \n");
    struct sockaddr_in server_addr;// thre server addr
    memset(&server_addr, 0, sizeof(struct sockaddr_in));//clear the server addr
    
    //设置服务器的地址
    server_addr.sin_family = AF_INET;//set protocol family
    server_addr.sin_port = htons(server_port);//set port
    if(inet_pton(AF_INET,server_ip,&server_addr.sin_addr)<0)
    {
        printf("ip addr error: %s\n",server_ip);
        exit(1);
    }//将ip字符串转化成32位字节写入server_addr
    
    //连接服务器
    if(connect(client_socket_fd,(struct sockaddr*)&server_addr, sizeof(server_addr))<0)
    {
        printf("connect error!");
        exit(1);
    }else{
        printf("connect success\n");
    }

      
    while (1)
    {
        if(recv(client_socket_fd, rev_buf, sizeof(rev_buf), 0)==0)
            break;
        printf("%s\n",rev_buf);//打印接收到的数据，字符串'\0'结束
        // printf("exit?(q):");
        // if (getchar()=='q')
        //     break;        
    }

    close(client_socket_fd);//close the server
    printf("\n%s","program over!\n");
    system("pause");
}