#include <asm/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <net/if.h>
#include <netinet/in.h>

#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <sys/time.h>
#include <event.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdlib.h>

extern void check_connection();
extern int onNet();

//#ifndef foo_h__
//#define foo_h__
 
extern void foo();
extern void say_net();
 
//#endif  // network_h__