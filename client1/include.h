#pragma once
#include <stdio.h>
#include <glib.h>
#include <sys/types.h>
#include <unistd.h>
#include <gio/gio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sodium.h>
#define CLIENT_ID 1
#define _OPEN_SYS_ITOA_EXT