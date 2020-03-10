//client side mini rdma test
//

#include "mini_rdma_cmn.h"

static int sock_conn_client(int tmp, int sockfd, struct addrinfo *iterator){
	        if ((tmp =  
                      connect (sockfd, iterator->ai_addr, iterator->ai_addrlen)))
                {   
                    fprintf (stdout, "failed connect \n");
                    close (sockfd);
                    sockfd = -1;
                }
}



static int resources_create_client(struct resources *res){
	res->sock = sock_connect (config.server_name, config.tcp_port);
        if (res->sock < 0)
        {
            fprintf (stderr,
                    "failed to establish TCP connection to server %s, port %d\n",
                    config.server_name, config.tcp_port);
            rc = -1;
            goto resources_create_exit;
        }

}
