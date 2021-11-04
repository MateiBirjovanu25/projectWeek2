#include "../include.h"

//#define PORT 8080

void sendText(GSocket* sockfd)
{
    printf("Fisier text trimis catre server...\n");
        printf("Introduceti numele fisierului: \n");
        char fileName[100];
        scanf("%s",fileName);
        int fd = open(fileName,O_RDONLY);
        char buffer1[1024];
        read(fd,buffer1,1024);

        g_socket_send(sockfd,buffer1,1024,0,0);

        //VARIANTA PENTRU MAI TARZIU.........
        /*FILE *fp = fopen("/home/calinnc/Documents/Exercises/Client2/misc/Mesaj", "r");
        while (fgets(buffer1, sizeof(buffer1), fp)) 
            g_socket_send(sockfd, buffer1, 1024, 0, 0);
        fclose(fp);*/ 
}

void receiveText(GSocket* sockfd)
{
        char buffer2[1024];
        g_socket_receive(sockfd, buffer2, 1024, 0, 0);
        printf("Textul primit: \n");
        printf("%s\n", buffer2); 
}

int main(int argc,char** argv)
{
    struct sockaddr_in serveraddr;
    int PORT = atoi(argv[1]);

    serveraddr.sin_port=htons(PORT);
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    //creare socket si verificare
    GSocket* socket = g_socket_new(G_SOCKET_FAMILY_IPV4,G_SOCKET_TYPE_STREAM,G_SOCKET_PROTOCOL_TCP, NULL);
    GSocketAddress* address = g_socket_address_new_from_native(&serveraddr,sizeof(serveraddr));

    if(socket == 0)
        printf("Eroare la crearea socket-ului!\n");
    else
        printf("Socket creat cu succes!\n");

    if(g_socket_connect(socket,address, 0, 0) == 0)
    {
        printf("Conectare esuata! \n");
        exit(1);
    }
    else
        printf("Conectare reusita!\n");

    int client = 2;
    g_socket_send(socket, (gchar*)&client, 4, 0, 0);

    char msg[100];
    printf("Urmatoarea actiune send/receive:\n");
    scanf("%s", msg);
    g_socket_send(socket,msg,100,0,0);

    //Raspuns de la server
    char message[100];
    g_socket_receive(socket, message,100,0,0);

    if(strcmp(message, "success") == 0)
    {
        
        if(strcmp(request,"send") == 0)
            sendText(socket);
        else
            receiveText(socket);

    }
    else
        printf("Request failed!\n");

    return 0;
}
