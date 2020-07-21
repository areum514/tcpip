#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling(char *message);

int main(int argc,char* argv[]){

    int serv_sock;
    int clnt_socket;

    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_adr;
    socklen_t clnt_adr_sz;
    int str_len,i;

    char message[]="Hello World!";
    if (argc!=2){
        printf("Usage:%s<port>\n",argv[0]);
        exit(1);
    }

    serv_sock=socket(PF_INET,SOCK_STREAM,0);
    if(serv_sock==-1){
        error_handling("socket() errpr");
    }

    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_addr.sin_port=htons(atoi(argv[1]));

    if(bind(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1)
        error_handling("bind() error");
    
    if(listen(serv_sock,5)==-1)
        error_handling("listen() error");
    
    clnt_adr_sz=sizeof(clnt_adr);

    for(i=0;i<5;i++){
        clnt_socket=accept(serv_sock,(struct sockaddr*)&clnt_adr,&clnt_adr_sz);
        if(clnt_socket==-1)
            error_handling("accept() error");
        else
            printf("Connected client %d \n",i+1);
        while((str_len=read(clnt_socket,message,BUF_SIZE))!=0)
            write(clnt_socket,message,str_len);
        close(clnt_socket);
    }
    close(serv_sock);
    return 0;    
}

void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}