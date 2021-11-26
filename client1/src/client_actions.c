#include "client_actions.h"
#include "update_agent.h"

void send_text(clientParam *cp)
{

    char fileName[100] = "misc/text.in";
    //g_socket_receive(socket,fileName,100,0,0);

    int fd = open(fileName, O_RDONLY);
    char text[100];
    bzero(text, 100);
    if (fd < 0)
    {
        strcpy(text, "Wrong path or file doesn't exist\n");
        printf("%s\n", text);
    }
    else
    {
        read(fd, text, 100);
        printf("read: %s\n", text);
    }

    GSocket *secondSocket = g_socket_new(G_SOCKET_FAMILY_IPV4, G_SOCKET_TYPE_STREAM, G_SOCKET_PROTOCOL_TCP, NULL);

    if (g_socket_connect(secondSocket, cp->address, 0, 0) == 0)
    {
        printf("Connecting error\n");
        exit(1);
    }

    int clientType = 1;
    g_socket_send(secondSocket, &clientType, 4, 0, 0); //send the type of the client

    g_socket_send(secondSocket, "passive", 100, 0, 0); //send the part of the client

    g_socket_send(secondSocket, &cp->clientId, 4, 0, 0); //send the id of the client

    g_socket_send(secondSocket, text, 100, 0, 0);

    system("clear");
    printf("Enter command number:\n");
    printf("1.receive from clientId (you have to insert it) \n");
    printf("2.receive script client_id\n");
    printf("3.exit\n");

    //g_socket_close(secondSocket,0);
}
void receive_text(clientParam *cp)
{

    GSocket *secondSocket = g_socket_new(G_SOCKET_FAMILY_IPV4, G_SOCKET_TYPE_STREAM, G_SOCKET_PROTOCOL_TCP, NULL);

    if (g_socket_connect(secondSocket, cp->address, 0, 0) == 0)
    {
        printf("Connecting error\n");
        exit(1);
    }

    int clientType = 1;
    g_socket_send(secondSocket, &clientType, 4, 0, 0); //send the type of the client

    g_socket_send(secondSocket, "passive", 100, 0, 0); //send the part of the client

    g_socket_send(secondSocket, &cp->clientId, 4, 0, 0); //send the id of the client

    char text[100];
    bzero(text, 100);
    printf("started receiving\n");
    g_socket_receive(secondSocket, text, 100, 0, 0);

    system("clear");
    printf("text received: \n");
    printf("%s\n", text);
}

void generate_hash(char *text, char *hash)
{
    unsigned char generatedHash[crypto_hash_sha256_BYTES];
    bzero(hash, 1024);
    crypto_hash_sha256(generatedHash, text, sizeof(text));
    strcpy(hash, generatedHash);
    for (int i = 0; i < 32; i++)
    {
        if (hash[i] == '!')
            hash[i] = '7';
    }
    hash[32] = 0;
    printf("hash: %s\n", hash);
}




void receive_script(clientParam *cp)
{

    GSocket *secondSocket = g_socket_new(G_SOCKET_FAMILY_IPV4, G_SOCKET_TYPE_STREAM, G_SOCKET_PROTOCOL_TCP, NULL);

    if (g_socket_connect(secondSocket, cp->address, 0, 0) == 0)
    {
        printf("Connecting error\n");
        exit(1);
    }

    int clientType = 1;
    g_socket_send(secondSocket, &clientType, 4, 0, 0); //send the type of the client

    g_socket_send(secondSocket, "passive", 100, 0, 0); //send the part of the client

    g_socket_send(secondSocket, &cp->clientId, 4, 0, 0); //send the id of the client

    char compressed_script_hash[1024];
    bzero(compressed_script_hash, 1024);
    char hashNum[1024],compressed[1024];

    printf("started receiving\n");
    g_socket_receive(secondSocket, compressed, 1024, 0, 0);//received compressed file
    printf("received compressed\n");
    g_socket_receive(secondSocket, hashNum, 1024, 0, 0);//receives hashes and length
    

    char hashcode_compressed[1024];
    char hashcode_decompressed[1024];
    char hash[1024];
    char file_size[1024];
    bzero(hash, 1024);
    bzero(hashcode_compressed, 1024);
    bzero(hashcode_decompressed, 1024);

    extract_strings(hashNum, hashcode_compressed, hashcode_decompressed, file_size);

    int n = atoi(file_size);
    printf("compressed: \n");
    for(int i=0;i<9;i++)
    {
        printf("%c",compressed[i]);
    }
    printf("\n");
    generate_hash(compressed, hash);
    printf("generated: \n");
    printf("%s\n", hash);
    printf("recv: \n");
    printf("%s\n", hashcode_compressed);

    if (strcmp(hash, hashcode_compressed) != 0)
    {
        printf("An error has occured while sending the script - diffrent hash for the compressed file.\n");
    }
    else
    {
        printf("n = %d \n", n);
        // o primesc de la celalalt client, e dimensiunea scriputlui necompressat
        
        char hash[1024];
        char decompressed[1024];
        bzero(decompressed,1024);
        bzero(hash, 1024);
        printf("COMPRESSED: \n");
        printf("compressed: \n");
        for(int i=0;i<9;i++)
        {
            printf("%c",compressed[i]);
        }
        printf("\n");

        //bzero(decompressed, 1024);
        //decompress(n+1, decompressed, compressed);

        uLong compSize = compressBound(n); 
        uLong ucompSize = n + 1;
        int errorMessage = uncompress((Bytef*)decompressed,&ucompSize,(Bytef*) compressed,compSize);
        printf("error message: %d\n",errorMessage);
        
        for(int i=0;i<n;i++)
            printf("%c",decompressed[i]);
        printf("\n");

        generate_hash(decompressed, hash);

        //compar hash de pe file decompresat
        if (strcmp(hash, hashcode_decompressed) != 0)
        {
            printf("An error has occured while sending the script - diffrent hash for the uncompressed.\n");
        }
        else
        {
            printf("The file will be sent to the update agent\n");
            printf("%s ALO \n", decompressed);
            //updateAgent.c apleaza o functie(param locatia fisierului) care face update
            FILE *fp;
            char file[100] = "misc/data.sh";
            fp = fopen(file, "w");

            

            fprintf(fp, "%s", decompressed);
            fclose(fp);

            update(file);
        }
    }

    //system("clear");
    printf("script received: \n");

    return NULL;
}


void decompress(int x, char *decompressed, char *compressed)
{
    printf("%s\n", compressed);
    printf("%d\n", x);

    z_stream infstream;
    infstream.zalloc = Z_NULL;
    infstream.zfree = Z_NULL;
    infstream.opaque = Z_NULL;
    // setup "b" as the input and "c" as the compressed output
    infstream.avail_in = (uInt)(strlen(compressed)+1);                   // size of input
    infstream.next_in = (Bytef *)compressed;          // input char array
    infstream.avail_out = (uInt)(x); // size of output
    infstream.next_out = (Bytef *)(decompressed);     // output char array


    
    // the actual DE-compression work.
    inflateInit(&infstream);
    inflate(&infstream, Z_NO_FLUSH);
    inflateEnd(&infstream);

    decompressed[x] = 0;
    printf("size: %d\n",infstream.avail_out);
    for(int i=0;i<infstream.avail_out;i++)
        printf("%c",infstream.next_out[i]);
    printf("\n");

    printf("Uncompressed size is: %d\n", strlen(decompressed));
    printf("Uncompressed string is: %s\n", decompressed);
}

void test_compressed()
{
    char a[1024] = "Hello Hello Hello Hello Hello Hello!";

    // placeholder for the compressed (deflated) version of "a"
    char b[1024];
    char c[1024];

    int x = strlen(a);
    printf("Uncompressed size is: %lu\n", strlen(a));
    printf("Uncompressed string is: %s\n", a);

    printf("\n----------\n\n");

    // STEP 1.
    // deflate a into b. (that is, compress a into b)

    // zlib struct
    z_stream defstream;
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;
    // setup "a" as the input and "b" as the compressed output
    defstream.avail_in = (uInt)strlen(a) + 1; // size of input, string + terminator
    defstream.next_in = (Bytef *)a;           // input char array
    defstream.avail_out = (uInt)sizeof(b);    // size of output
    defstream.next_out = (Bytef *)b;          // output char array

    // the actual compression work.
    deflateInit(&defstream, Z_BEST_COMPRESSION);
    deflate(&defstream, Z_FINISH);
    deflateEnd(&defstream);

    // This is one way of getting the size of the output
    printf("Compressed size is: %lu\n", strlen(b));
    printf("Compressed string is: %s\n", b);

    printf("\n----------\n\n");

    /*
    // STEP 2.
    // inflate b into c
    // zlib struct
    z_stream infstream;
    infstream.zalloc = Z_NULL;
    infstream.zfree = Z_NULL;
    infstream.opaque = Z_NULL;
    // setup "b" as the input and "c" as the compressed output
    infstream.avail_in = (uInt)(x); // size of input
    infstream.next_in = (Bytef *)b; // input char array
    infstream.avail_out = (uInt)sizeof(c); // size of output
    infstream.next_out = (Bytef *)c; // output char array
     
    // the actual DE-compression work.
    inflateInit(&infstream);
    inflate(&infstream, Z_NO_FLUSH);
    inflateEnd(&infstream);*/

    // placeholder for the UNcompressed (inflated) version of "b"

    bzero(c, 1024);
    printf("compressed123: %s\n",b);
    decompress(x, c, b);

    //printf("Uncompressed size is: %d\n", strlen(c));
    //printf("Uncompressed string is: %s\n", c);

    // make sure uncompressed is exactly equal to original.
    if (strcmp(a, c) == 0)
        printf("MERGE OK\n");
}

void extract_strings(char *source, char *a, char *b, char *c)
{
    //compressed!hashcomp!hashdecomp!file_size
    const char s[2] = "!";
    char *token;

    token = strtok(source, s);
    int i = 0;
    while (token != NULL)
    {
        //printf( " %s\n", token );
        if (i == 0)
        {
            strcpy(a, token);
            i++;
        }
        else if (i == 1)
        {
            strcpy(b, token);
            i++;
        }
        else if (i == 2)
        {
            strcpy(c, token);
            i++;
        }
        token = strtok(NULL, s);
    }
}

void extract_string_2()
{

    char message[100] = "ANAA";
    char hashcode_compressed[100];
    char compressed[100];
    char hashcode_decompressed[100];

    int n = strlen(message) - 2 * (crypto_generichash_BYTES) + 1;
    int mid = n + crypto_generichash_BYTES;
    int k = 0;
    int l = 0;

    strncpy(compressed, message, n);

    for (int i = n; i < strlen(message); i++)
    {
        if (i < mid)
        {
            hashcode_compressed[k] = message[i];
            k++;
        }
        else
        {
            hashcode_decompressed[l] = message[i];
            l++;
        }
    }
}

void test_hash()
{
    char script[100] = "ANAAAfvrv";
    char script2[100] = "ANAAvrvrvrevA";
    //update(script);

    unsigned char hash[crypto_hash_sha256_BYTES];
    unsigned char hash2[crypto_hash_sha256_BYTES];

    crypto_hash_sha256(hash, script, sizeof(script));

    printf("%s %d %d\n", hash, strlen(hash), crypto_hash_sha256_BYTES);

    crypto_hash_sha256(hash2, script2, sizeof(script2));

    printf("%d\n", strlen(hash));
    printf("%d\n", strlen(hash2));
}