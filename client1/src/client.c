#include "client_actions.h"

#define RECEIVE_TEXT 1
#define RECEIVE_SCRIPT 2
#define EXIT 3


void receive_text_command(clientParam *cp, int clientID)
{
    printf("You will receive the file soon\n");

    char commandWithId[100];
    strcpy(commandWithId, "receive text");
    char idText[10];

    sprintf(idText, "!%d", clientID);
    strcat(commandWithId, idText);

    GError *gerror = NULL;
    if (g_socket_send(cp->socket, commandWithId, 100, 0, gerror) == -1)
        printf("error");

    printf("command sent\n");
    receive_text(cp);
}

void receive_script_command(clientParam *cp, int clientID)
{
    printf("You will receive the script soon\n");

    char commandWithId[100];
    strcpy(commandWithId, "receive script");
    char idText[10];

    sprintf(idText, "!%d", clientID);
    strcat(commandWithId, idText);

    GError *gerror = NULL;
    if (g_socket_send(cp->socket, commandWithId, 100, 0, gerror) == -1)
        printf("error");

    printf("command sent\n");
    receive_script(cp);
}

void *command_menu(void *arg)
{

    clientParam *cp = (clientParam *)arg;

    while (1)
    {

        int param1;
        int param2;
        char command_number[10];
        int command;

        printf("Enter command number:\n");
        printf("1.receive from client_id (you have to insert it) \n");
        printf("2.receive script client_id\n");
        printf("3.exit\n");

        gets(command_number);
        command = sscanf(command_number, "%d %d", &param1, &param2);

        if (command == 1)
        {
            printf("ok\n");
        }

        printf("command: %d\n", param1);
        printf("user id: %d\n", param2);
        switch (param1)
        {
            case RECEIVE_TEXT:

                receive_text_command(cp, param2);
                break;

            case RECEIVE_SCRIPT:

                receive_script_command(cp, param2);
                break;

            case EXIT:

                g_socket_send(cp->socket, "exit", 100, 0, 0);
                exit(1);

            default:
                printf("Can't solve the request\n");
                break;
        }
    }
}

void *respond(void *arg)
{
    clientParam *cp = (clientParam *)arg;

    while (1)
    {
        char command[100];
        int debug;
        bzero(command, 100);

        g_socket_receive(cp->socket, command, 100, 0, 0);

        printf("received:\n %s\n", command);
        printf("from %d\n", cp->socket);
        if (strcmp(command, "send text") == 0)
        {
            send_text(cp);
        }
    }
}

int main(int argc, char **argv)
{
    printf("This is a shared library test...\n");
    foo();

    int port = atoi(argv[1]);
    struct sockaddr_in server;

    server.sin_port = htons(port);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    GSocket *socket = g_socket_new(G_SOCKET_FAMILY_IPV4, G_SOCKET_TYPE_STREAM, G_SOCKET_PROTOCOL_TCP, NULL);
    GSocketAddress *address = g_socket_address_new_from_native(&server, sizeof(server));

    if (g_socket_connect(socket, address, 0, 0) == 0)
    {
        printf("Connecting error\n");
        exit(1);
    }

    printf("Connected to server\n");

    //test_compressed();
    //test_hash();
    //test_extract_string();

    int clientType = 1;
    g_socket_send(socket, &clientType, 4, 0, 0); //send the type of the client

    g_socket_send(socket, "active", 100, 0, 0); //send the part of the client

    int client_id;
    g_socket_receive(socket, (gchar *)&client_id, sizeof(int), 0, 0); //receive the client id
    printf("I am client number %d\n", client_id);

    GThread *tid1;
    GThread *tid2;

    clientParam cP;
    cP.socket = socket;
    cP.address = address;
    cP.clientId = client_id;

    tid1 = g_thread_new(0, command_menu, &cP);
    tid2 = g_thread_new(0, respond, &cP);

    g_thread_join(tid1);
    g_thread_join(tid2);

    return 0;
}