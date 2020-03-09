//need ibv_post_send() to initiate communications
//
//
//
// Goal: small example to transfer a small area of memory


struct config_t{
	const char *dev_name;
	char *server_name;
	uint32_t tcp_port;
	int ib_port;
	int gid_idx;
};

struct cm_con_data_t{
	uint64_t addr;
	uint32_t rkey;
	uint32_t qp_num;
	uint16_t lid;
	uint8_t gid[16];
}__attribute((packed));



//straight from mellanox
struct resources{
	struct ibv_device_attr
	device_attr;
			/* Device attributes */
	struct ibv_port_attr port_attr; /* IB port attributes */
	struct cm_con_data_t remote_props; /* values to connect to remote side */
	struct ibv_context *ib_ctx; /* device handle */
	struct ibv_pd *pd; /* PD handle */
	struct ibv_cq *cq; /* CQ handle */
	struct ibv_qp *qp; /* QP handle */
	struct ibv_mr *mr; /* MR handle for buf */
	char *buf; /* memory buffer pointer, used for RDMA and send
								ops */
	int sock; /* TCP socket file descriptor */

};

static int socket_conn(const char *servername, int port){
		
}


//straight from mellanox code manual
int sock_sync_data(int sock, int xfer_size, char *local_data, char *remote_data)
{
 int rc;
 int read_bytes = 0;
 int total_read_bytes = 0;
 rc = write(sock, local_data, xfer_size);
 if(rc < xfer_size)
 fprintf(stderr, "Failed writing data during sock_sync_data\n");
 else
 rc = 0;

 while(!rc && total_read_bytes < xfer_size)
 {
 read_bytes = read(sock, remote_data, xfer_size);
 if(read_bytes > 0)
 total_read_bytes += read_bytes;
 else
 rc = read_bytes;
 }
 return rc;
}





int main(){

	
	//gets available list of devices (returns ptr ls)	
	ibv_get_device_list()
	//opens specified device using pointer
	ibv_open_device()
	//allocates protection domain for memory transfer
	ibv_alloc_pd()
	//registers a memory region
	ibv_reg_mr()
	//alloc qp
	ibv_create_qp()
	//bring up qp through to RTS (RESET->INIT->RTR->RTS)
	ibv_modify_qp()
	

	ibv_post_send()
}

