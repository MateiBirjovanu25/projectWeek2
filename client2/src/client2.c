#include "client2_func.h"
#include "../include.h"
int clientID;
GMutex* mutex;

void* commands(void* arg)
{
    clientParam* cp = (clientParam*) arg;
    int targetID;
    
    while(1)
    {
    int param1, param2, command;
    char commandNr[15];
    printf("Enter a command:\n");
    printf("1 = Receive from a client specified by ID \n");
    printf("2 = Command not implemented yet...\n");
    printf("3 = Exit.\n");
    
    gets(commandNr);
    command = sscanf(commandNr, "%d %d", &param1, &param2);
    GError* gerror=NULL;
    
    g_mutex_lock(&mutex); 

    switch (param1){
        case 1:
            printf("Receiving text...\n");
            char command_ID[100];
            strcpy(command_ID, "receive text");
            char ID[10];
            sprintf(ID, "!%d", param2);
            strcat(command_ID, ID);
            if(g_socket_send(cp->socket,command_ID,100,0,gerror) == -1)
                printf("Error..\n");
            printf("Command sent!\n");
            receiveText(cp);
            break;
        case 2:
            //...
            break;
        case 3:
            printf("The connection will be closed!\n");
            g_socket_send(cp->socket, "exit", 100, 0, 0);
            break;
        default:
            printf("Command not found!\n");
            break; }
    }
    g_mutex_unlock(&mutex); 
}

void* respond(void* arg)
{
    clientParam* cp = (clientParam*) arg;
    while(1)
    {
        g_mutex_lock(&mutex);
        
        char cmd[100];
        bzero(cmd, 20);
        g_socket_receive(cp->socket, cmd, 100, 0, 0);
        printf("Command received: %s from %d \n", cmd, cp->socket);
        if(strcmp(cmd, "send text") == 0)
            sendText(cp);
        else
            if(strcmp(cmd, "send script") == 0)
                sendScript(cp);
        
        g_mutex_unlock(&mutex);
    }
}

int main(int argc,char** argv)
{
    struct sockaddr_in serveraddr;
    int PORT = atoi(argv[1]);

    serveraddr.sin_port=htons(PORT);
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    GSocket* socket = g_socket_new(G_SOCKET_FAMILY_IPV4,G_SOCKET_TYPE_STREAM,G_SOCKET_PROTOCOL_TCP, NULL);
    GSocketAddress* address = g_socket_address_new_from_native(&serveraddr,sizeof(serveraddr));

    //Initializing the logger   
    //=============================================

    logger_reset_state();
    logger_set_log_file("../LOGGER/LOGGER.txt");
    log_status("Client2 is working!");

    //=============================================

    if(socket == 0){
        log_error("Error creating the socket!\n");

        exit(1);
    }
    else
        log_status("Socket created successfully!\n");

    if(g_socket_connect(socket,address, 0, 0) == 0)
    {
        log_error("Connecting error! \n");
        exit(1);
    }
    else
        log_status("Connected to server!\n");

    int client = 2;
    char clientType;
    g_socket_send(socket, (gchar*)&client, 4, 0, 0);
    g_socket_send(socket, "active", 100, 0, 0);

    g_socket_receive(socket, (gchar*)&clientType, sizeof(int), 0,0);
    log_status("I am client nr %d\n", clientType);

    log_status("Script sent to the server...\n");
    char scriptName[100] = "misc/script.sh";
    int fd = open(scriptName,O_RDONLY);
    char originalBuff[1024],compressedBuff[1024], originalHash[256],compressedHash[256];
    bzero(originalBuff, 1024);
    if(fd < 0)
    {
        log_error("Wrong path or script doesn t exist!\n");
    }
    else
        read(fd, originalBuff, 1024);


    g_mutex_init(&mutex);
    GThread* t1;
    GThread* t2;

    clientParam cp;
    cp.socket = socket;
    cp.addr = address;
    cp.clientID = clientType;

    t1 = g_thread_new(0,commands, &cp);
    t2 = g_thread_new(0,respond, &cp);

    g_thread_join(t1);
    g_thread_join(t2);  
    g_mutex_clear(&mutex);

    return 0;
}