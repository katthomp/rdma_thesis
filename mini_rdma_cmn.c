#include "mini_rdma.h"


//unsure what inputs this takes right now
//TODO
static int close_sock(){
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

static int close_resources(struct resources *res, int rc){
	if (rc) {
        /* Error encountered, cleanup */
        if (res->qp)
        {
            ibv_destroy_qp (res->qp);
            res->qp = NULL;
        }
        if (res->mr)
        {
            ibv_dereg_mr (res->mr);
            res->mr = NULL;
        }
        if (res->buf)
        {
            free (res->buf);
            res->buf = NULL;
        }
        if (res->cq)
        {
            ibv_destroy_cq (res->cq);
            res->cq = NULL;
        }
        if (res->pd)
        {
            ibv_dealloc_pd (res->pd);
            res->pd = NULL;
        }
        if (res->ib_ctx)
        {
            ibv_close_device (res->ib_ctx);
            res->ib_ctx = NULL;
        }
        if (dev_list)
        {
            ibv_free_device_list (dev_list);
            dev_list = NULL;
        }
        if (res->sock >= 0)
        {
            if (close (res->sock))
                fprintf (stderr, "failed to close socket\n");
            res->sock = -1;
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

static int resources_create(struct resources *res){
	 struct ibv_device **dev_list = NULL;
   	 struct ibv_qp_init_attr qp_init_attr;
   	 struct ibv_device *ib_dev = NULL;
   	 size_t size;
   	 int i;
   	 int mr_flags = 0;
   	 int cq_size = 0;
   	 int num_devices;
   	 int rc = 0;
	//TODO:
	//compare ib_dev to mlnx5_0, if so, continue, else fault
	/* get device handle */
   	 res->ib_ctx = ibv_open_device (ib_dev);
   	 if (!res->ib_ctx)
   	 {
       		fprintf (stderr, "failed to open device %s\n", config.dev_name);
        	rc = 1;
 		close_resources(res, rc);       	
   	 }
	 if (ibv_query_port (res->ib_ctx, config.ib_port, &res->port_attr))
	{
        	fprintf (stderr, "ibv_query_port on port %u failed\n", config.ib_port);
        	rc = 1;
        	close_resources(res, rc);
    	}
   	 /* allocate Protection Domain */
   	 res->pd = ibv_alloc_pd (res->ib_ctx);
   	 if (!res->pd)
   	{
       		fprintf (stderr, "ibv_alloc_pd failed\n");
        	rc = 1;
        	close_resources(res, rc);
    	}
   	 /* each side will send only one WR, so Completion Queue with 1 entry is enough */
   	 cq_size = 1;
   	 res->cq = ibv_create_cq (res->ib_ctx, cq_size, NULL, NULL, 0);
   	 if (!res->cq)
   	 {
       		 fprintf (stderr, "failed to create CQ with %u entries\n", cq_size);
       		 rc = 1;
       		 close_resources(res, rc);
   	 }
   	 /* allocate the memory buffer that will hold the data */
   	 size = MSG_SIZE;
   	 res->buf = (char *) malloc (size);
   	 if (!res->buf)
   	 {
       		 fprintf (stderr, "failed to malloc %Zu bytes to memory buffer\n", size);
        	 rc = 1;
       		 close_resources(res, rc);
   	 }
   	 memset (res->buf, 0, size);
	 mr_flags = IBV_ACCESS_LOCAL_WRITE | IBV_ACCESS_REMOTE_READ |
       	 IBV_ACCESS_REMOTE_WRITE;
    	res->mr = ibv_reg_mr (res->pd, res->buf, size, mr_flags);
   	 if (!res->mr)
   	 {
        	fprintf (stderr, "ibv_reg_mr failed with mr_flags=0x%x\n", mr_flags);
        	rc = 1;
        	close_resources(res, rc);
    	 }
    	fprintf (stdout,
            "MR was registered with addr=%p, lkey=0x%x, rkey=0x%x, flags=0x%x\n",
            res->buf, res->mr->lkey, res->mr->rkey, mr_flags);
    	/* create the Queue Pair */
    	memset (&qp_init_attr, 0, sizeof (qp_init_attr));
    	qp_init_attr.qp_type = IBV_QPT_RC;
    	qp_init_attr.sq_sig_all = 1;
    	qp_init_attr.send_cq = res->cq;
    	qp_init_attr.recv_cq = res->cq;
    	qp_init_attr.cap.max_send_wr = 1;
    	qp_init_attr.cap.max_recv_wr = 1;
    	qp_init_attr.cap.max_send_sge = 1;
    	qp_init_attr.cap.max_recv_sge = 1;
    	res->qp = ibv_create_qp (res->pd, &qp_init_attr);
    	if (!res->qp)
    	{
        	fprintf (stderr, "failed to create QP\n");
        	rc = 1;
        	close_resources(res, rc);
    	}
    	fprintf (stdout, "QP was created, QP number=0x%x\n", res->qp->qp_num);
	return rc;
}


static int
modify_qp_to_init (struct ibv_qp *qp)
{   
    struct ibv_qp_attr attr;
    int flags;
    int rc;
    memset (&attr, 0, sizeof (attr));
    attr.qp_state = IBV_QPS_INIT;
    attr.port_num = config.ib_port;
    attr.pkey_index = 0; 
    attr.qp_access_flags = IBV_ACCESS_LOCAL_WRITE | IBV_ACCESS_REMOTE_READ |
        IBV_ACCESS_REMOTE_WRITE;
    flags =
        IBV_QP_STATE | IBV_QP_PKEY_INDEX | IBV_QP_PORT | IBV_QP_ACCESS_FLAGS;
    rc = ibv_modify_qp (qp, &attr, flags);
    if (rc)
        fprintf (stderr, "failed to modify QP state to INIT\n");
    return rc;
}

static int
modify_qp_to_rtr (struct ibv_qp *qp, uint32_t remote_qpn, uint16_t dlid,
        uint8_t * dgid)
{
    struct ibv_qp_attr attr;
    int flags;
    int rc;
    memset (&attr, 0, sizeof (attr));
    attr.qp_state = IBV_QPS_RTR;
    attr.path_mtu = IBV_MTU_256;
    attr.dest_qp_num = remote_qpn;
    attr.rq_psn = 0;
    attr.max_dest_rd_atomic = 1;
    attr.min_rnr_timer = 0x12;
    attr.ah_attr.is_global = 0;
    attr.ah_attr.dlid = dlid;
    attr.ah_attr.sl = 0;
    attr.ah_attr.src_path_bits = 0;
    attr.ah_attr.port_num = config.ib_port;
    if (config.gid_idx >= 0)
    {
        attr.ah_attr.is_global = 1;
        attr.ah_attr.port_num = 1;
        memcpy (&attr.ah_attr.grh.dgid, dgid, 16);
        attr.ah_attr.grh.flow_label = 0;
        attr.ah_attr.grh.hop_limit = 1;
        attr.ah_attr.grh.sgid_index = config.gid_idx;
        attr.ah_attr.grh.traffic_class = 0;
    }
    flags = IBV_QP_STATE | IBV_QP_AV | IBV_QP_PATH_MTU | IBV_QP_DEST_QPN |
        IBV_QP_RQ_PSN | IBV_QP_MAX_DEST_RD_ATOMIC | IBV_QP_MIN_RNR_TIMER;
    rc = ibv_modify_qp (qp, &attr, flags);
    if (rc)
        fprintf (stderr, "failed to modify QP state to RTR\n");
    return rc;
}


static int
modify_qp_to_rts (struct ibv_qp *qp)
{
    struct ibv_qp_attr attr;
    int flags;
    int rc;
    memset (&attr, 0, sizeof (attr));
    attr.qp_state = IBV_QPS_RTS;
    attr.timeout = 0x12;
    attr.retry_cnt = 6;
    attr.rnr_retry = 0;
    attr.sq_psn = 0;
    attr.max_rd_atomic = 1;
    flags = IBV_QP_STATE | IBV_QP_TIMEOUT | IBV_QP_RETRY_CNT |
        IBV_QP_RNR_RETRY | IBV_QP_SQ_PSN | IBV_QP_MAX_QP_RD_ATOMIC;
    rc = ibv_modify_qp (qp, &attr, flags);
    if (rc)
        fprintf (stderr, "failed to modify QP state to RTS\n");
    return rc;
}




