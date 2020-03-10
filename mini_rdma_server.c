//Goal: take mellanox code for server and make it Small
//
// 3-9-2020
//step 1: get device list
//step 2: open device
//step 3: query device
//step 4: alloc pd
//step 5: register mem
//step 6: create qp and cq
//step 7: transition qp (RESET->INIT->RTR->RTS)
//step 8: actually use it
//step 9: destroy objects in REVERSE order of creation
#include "mini_rdma_cmn.h"



//may need to return res to make sure things worked
int sock_conn_server(struct resources *res){
	strcpy (res->buf, MSG);
        fprintf (stdout, "going to send the message: '%s'\n", res->buf);
}


int resources_create_server(struct resources *res){
	fprintf (stdout, "waiting on port %d for TCP connection\n",
                config.tcp_port);
        res->sock = sock_connect (NULL, config.tcp_port);
        if (res->sock < 0)
        {
            fprintf (stderr,
                    "failed to establish TCP connection with client on port %d\n",
                    config.tcp_port);
            rc = -1;
            close_resources(res, rc); //link up with the def in mini_rdma_cmn.c
        }


}

