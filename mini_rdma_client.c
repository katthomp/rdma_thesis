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


//client will only be sending wr, correct?
//TODO
static int
post_send (struct resources *res, int opcode)
{  
 struct ibv_sge sge;
    struct ibv_send_wr *bad_wr = NULL;
    int rc;
    /* prepare the scatter/gather entry */
    memset (&sge, 0, sizeof (sge));
    sge.addr = (uintptr_t) res->buf;
    sge.length = MSG_SIZE;
    sge.lkey = res->mr->lkey;
    /* prepare the send work request */
    memset (&sr, 0, sizeof (sr));
    sr.next = NULL;
    sr.wr_id = 0;
    sr.sg_list = &sge;
    sr.num_sge = 1;
    sr.opcode = opcode;
    sr.send_flags = IBV_SEND_SIGNALED;
    if (opcode != IBV_WR_SEND)
    {   
        sr.wr.rdma.remote_addr = res->remote_props.addr;
        sr.wr.rdma.rkey = res->remote_props.rkey;
    }
    /* there is a Receive Request in the responder side, so we won't get any into RNR flow */ 
    rc = ibv_post_send (res->qp, &sr, &bad_wr);
    if (rc)
        fprintf (stderr, "failed to post SR\n");
    else
    {   
        switch (opcode)
        {   
            case IBV_WR_SEND:
                fprintf (stdout, "Send Request was posted\n");
                break;
            case IBV_WR_RDMA_READ:
                fprintf (stdout, "RDMA Read Request was posted\n");
                break;
            case IBV_WR_RDMA_WRITE:
                fprintf (stdout, "RDMA Write Request was posted\n");
                break;
            default:
                fprintf (stdout, "Unknown Request was posted\n");
                break;
        }
    }
    return rc;
}

