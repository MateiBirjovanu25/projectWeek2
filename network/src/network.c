
#include "../network.h"

#define MYPROTO NETLINK_ROUTE
#define MYMGRP RTMGRP_IPV4_ROUTE



void foo(){
    printf("Hello this is from network library\n");
}

int open_netlink()
{
    int sock = socket(AF_NETLINK,SOCK_RAW,MYPROTO);
    struct sockaddr_nl addr;

    memset((void *)&addr, 0, sizeof(addr));

    if (sock<0)
        return sock;

    addr.nl_family = AF_NETLINK;
    addr.nl_pid = getpid();
    addr.nl_groups = RTMGRP_LINK|RTMGRP_IPV4_IFADDR|RTMGRP_IPV6_IFADDR;

    if (bind(sock,(struct sockaddr *)&addr,sizeof(addr))<0)
        return -1;

    return sock;
}


int read_event(int sockint, int (*msg_handler)(struct sockaddr_nl *,
                                               struct nlmsghdr *))
{
    int status;
    int ret = 0;
    char buf[4096];
    struct iovec iov = { buf, sizeof buf };
    struct sockaddr_nl snl;
    struct msghdr msg = { (void*)&snl, sizeof snl, &iov, 1, NULL, 0, 0};
    struct nlmsghdr *h;

    status = recvmsg(sockint, &msg, 0);

    if(status < 0)
    {
        /* Socket non-blocking so bail out once we have read everything */
        if (errno == EWOULDBLOCK || errno == EAGAIN)
            return ret;

        /* Anything else is an error */
        printf("read_netlink: Error recvmsg: %d\n", status);
        perror("read_netlink: Error: ");
        return status;
    }

    if(status == 0)
    {
        printf("read_netlink: EOF\n");
    }

    /* We need to handle more than one message per 'recvmsg' */
    for(h = (struct nlmsghdr *) buf; NLMSG_OK (h, (unsigned int)status); 
    h = NLMSG_NEXT (h, status))
    {
        /* Finish reading */
        if (h->nlmsg_type == NLMSG_DONE)
            return ret;

        /* Message is some kind of error */
        if (h->nlmsg_type == NLMSG_ERROR)
        {
            printf("read_netlink: Message is an error - decode TBD\n");
            return -1; // Error
        }

        /* Call message handler */
        if(msg_handler)
        {
            ret = (*msg_handler)(&snl, h);
            if(ret < 0)
            {
                printf("read_netlink: Message hander error %d\n", ret);
                return ret;
            }
        }
        else
        {
            printf("read_netlink: Error NULL message handler\n");
            return -1;
        }
    }



    return ret;
}

static int netlink_link_state(struct sockaddr_nl *nl, struct nlmsghdr *msg)
{
    //int len;
    struct ifinfomsg *ifi;

    //nl = nl;

    ifi = NLMSG_DATA(msg);
    char ifname[1024];
    if_indextoname(ifi->ifi_index,ifname);

    printf("netlink_link_state: Link %s %s\n", 
            //ifname,(ifi->ifi_flags & IFF_RUNNING)?"Up":"Down");
        ifname,(ifi->ifi_flags & IFF_UP)?"Up":"Down");
    return 0;
}

static int msg_handler(struct sockaddr_nl *nl, struct nlmsghdr *msg)
{
    struct ifinfomsg *ifi=NLMSG_DATA(msg);
    struct ifaddrmsg *ifa=NLMSG_DATA(msg);
    char ifname[1024];
    switch (msg->nlmsg_type)
    {
        case RTM_NEWADDR:
            if_indextoname(ifi->ifi_index,ifname);
            printf("msg_handler: RTM_NEWADDR - connected: %s\n",ifname);
            //on = 1;
            return 0;
            break;
        case RTM_DELADDR:
            if_indextoname(ifi->ifi_index,ifname);
            printf("msg_handler: RTM_DELADDR - disconnected : %s\n",ifname);
            //on = 0;
            return 0;
            break;
        case RTM_NEWLINK:
            if_indextoname(ifa->ifa_index,ifname);
            printf("msg_handler: RTM_NEWLINK\n");
            netlink_link_state(nl, msg);
            return 0;
            break;
        case RTM_DELLINK:
            if_indextoname(ifa->ifa_index,ifname);
            printf("msg_handler: RTM_DELLINK : %s\n",ifname);
            return 0;
            break;
        default:
            printf("msg_handler: Unknown netlink nlmsg_type %d\n",
                    msg->nlmsg_type);
            return 0;
            break;
    }
    return 0;
}

void say_net()
{
    system("nload");
} 
/*
void monitoring(int seconds)
{
    struct event ev;
    struct timeval tv;

    tv.tv_sec = seconds;
    tv.tv_usec = 0;

    event_init();
    evtimer_set(&ev, say_net, NULL);
    evtimer_add(&ev, &tv);
    event_dispatch();
}
*/

void check_connection(){

    int nls = open_netlink();
    //printf("Started watching:\n");
    if (nls<0) {
        printf("Open Error!");
    }
    while (1)
        read_event(nls, msg_handler);
}

int main(int argc, char *argv[])
{
    int nls = open_netlink();
    //monitoring(4);
    //say_net();
    printf("Started watching:\n");
    if (nls<0) {
        printf("Open Error!");
    }
    while (1)
        read_event(nls, msg_handler);
    return 0;
}