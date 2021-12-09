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

extern int check_connection();
//#ifndef foo_h__
//#define foo_h__
 
extern void foo();
 
//#endif  // network_h__