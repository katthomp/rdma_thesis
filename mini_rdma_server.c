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

//pretty sure that poll_completion should go into here
//only because the server is always going to be running this function, right?
//TODO
static int
poll_completion (struct resources *res)
{   
    struct ibv_wc wc;
    unsigned long start_time_msec;
    unsigned long cur_time_msec;
    struct timeval cur_time;
    int poll_result;
    int rc = 0;
    /* poll the completion for a while before giving up of doing it .. */
    gettimeofday (&cur_time, NULL);
    start_time_msec = (cur_time.tv_sec * 1000) + (cur_time.tv_usec / 1000);
    do
    {   
        poll_result = ibv_poll_cq (res->cq, 1, &wc);
        gettimeofday (&cur_time, NULL);
        cur_time_msec = (cur_time.tv_sec * 1000) + (cur_time.tv_usec / 1000);
    }
    while ((poll_result == 0)
            && ((cur_time_msec - start_time_msec) < MAX_POLL_CQ_TIMEOUT));
    if (poll_result < 0)
    {   
        /* poll CQ failed */
        fprintf (stderr, "poll CQ failed\n");
        rc = 1;
    }
    else if (poll_result == 0)
    {   
        /* the CQ is empty */
        fprintf (stderr, "completion wasn't found in the CQ after timeout\n");
        rc = 1;
    }
    else
    {   
        /* CQE found */
        fprintf (stdout, "completion was found in CQ with status 0x%x\n",
                wc.status);
        /* check the completion status (here we don't care about the completion opcode */
        if (wc.status != IBV_WC_SUCCESS)
        {   
            fprintf (stderr,
                    "got bad completion with status: 0x%x, vendor syndrome: 0x%x\n",
                    wc.status, wc.vendor_err);
            rc = 1;
        }
    }
    return rc;
}


//server will only be receiving.
//TODO
static int
post_receive (struct resources *res)
{
    struct ibv_recv_wr rr;
    struct ibv_sge sge;
    struct ibv_recv_wr *bad_wr;
    int rc;
    /* prepare the scatter/gather entry */
    memset (&sge, 0, sizeof (sge));
    sge.addr = (uintptr_t) res->buf;
    sge.length = MSG_SIZE;
    sge.lkey = res->mr->lkey;
    /* prepare the receive work request */
    memset (&rr, 0, sizeof (rr));
    rr.next = NULL;
    rr.wr_id = 0;
    rr.sg_list = &sge;
    rr.num_sge = 1;
    /* post the Receive Request to the RQ */
    rc = ibv_post_recv (res->qp, &rr, &bad_wr);
    if (rc)
        fprintf (stderr, "failed to post RR\n");
    else
        fprintf (stdout, "Receive Request was posted\n");
    return rc;
}



