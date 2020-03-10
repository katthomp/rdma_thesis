#include "mini_rdma.h"


//unsure what inputs this takes right now
//TODO
static int close_everything(){
	 if (listenfd)
        close (listenfd);
    if (resolved_addr)
        freeaddrinfo (resolved_addr);
    if (sockfd < 0)
    {
        if (servername)
            fprintf (stderr, "Couldn't connect to %s:%d\n", servername, port);
        else
        {
            perror ("server accept");
            fprintf (stderr, "accept() failed\n");
        }
    }

}


static int sock_conn(const char *servername, int port){
	struct addrinfo *resolved_addr = NULL;
   	 struct addrinfo *iterator;
   	 char service[6];
   	 int sockfd = -1;
   	 int listenfd = 0;
   	 int tmp;
   	 struct addrinfo hints = {
        .ai_flags = AI_PASSIVE,
        .ai_family = AF_INET,
        .ai_socktype = SOCK_STREAM
   	 };
	sockfd = getaddrinfo (servername, service, &hints, &resolved_addr);
	//getaddrinfo is a library function?
	if (sockfd < 0){
        fprintf (stderr, "%s for %s:%d\n", gai_strerror (sockfd), servername,
                port);
	//close_everything()
	//wrapper around destruction so it's not a Fucking goto	
	}

}
