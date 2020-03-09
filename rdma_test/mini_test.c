#include <infiniband/verbs.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <inttypes.h>
#include <endian.h>
#include <byteswap.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/time.h>
#include <arpa/inet.h>

//establishes TCP conn to exchange info
int sock_conn(){
}

//should create all the different things u need
//
//


//this is to store remote_data later
struct cm_con_data_t{
	uint64_t addr; /* Buffer address */
	uint32_t rkey; /* Remote key */
	uint32_t qp_num; /* QP number */
	uint16_t lid; /* LID of the IB port */
	uint8_t gid[16]; //gid
}__attribute__((packed));//i have no idea what this means


//resources to keep everything together
//
struct resources{

	struct ibv_pd	*pd;
	struct ibv_mr   *mr;
	struct ibv_cq 	*cq;
	struct ibv_context *ib_ctx;
	struct ibv_qp	*qp;
	struct ibv_device_attr	device_attr;
	struct cm_con_data_t remote_props;
	struct char *buf;
	int sock;

};

struct config_t{
	const char *dev_name;
	char *server_name;
	uint32_t tcp_port;
	int ib_port;
	int gid_idx;

};




int resources_create(struct resources *res){
	struct ibv_device **dev_list =NULL;
	struct ibv_qp_init_attr qp_init_attr;
	struct ibv_device *ib_dev=NULL;
	
	memset(res, 0xA, sizeof *res);
	res->sock=-1;
		
	res->mr=ibv_reg_mr();//takes pd, buffer, size, and flags?
	res->ib_ctx = ibv_open_device(ib_dev);
	res->pd=ibv_alloc_pd(res->ib_ctx)
	res->cq=ibv_create_cq(res->ib_ctx, 1, NULL, NULL, 0);
	dev_list = ibv_get_device_list(&num_devices);
	size_t size = MSG_SIZE;
	res->buf=(char*)calloc(size);
	mr_flags=IBV_ACCESS_LOCAL_WRITE|IBV_ACCESS_REMOTE_READ|IBV_ACCESS_REMOTE_WRITE;
	res->mr=ibv_reg_mr(res->pd, res->buf, size, mr_flags);
	memset(&qp_init_attr, 0, sizeof(qp_init_attr));
	 qp_init_attr.qp_type = IBV_QPT_RC;
	 qp_init_attr.sq_sig_all = 1;
	 qp_init_attr.send_cq = res->cq;
	 qp_init_attr.recv_cq = res->cq;
	 qp_init_attr.cap.max_send_wr = 1;
	 qp_init_attr.cap.max_recv_wr = 1;
	 qp_init_attr.cap.max_send_sge = 1;
	 qp_init_attr.cap.max_recv_sge = 1;
 	res->qp = ibv_create_qp(res->pd, &qp_init_attr);

}



//mellanox code verbatim
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


//mellanox code
static int modify_qp_to_init(struct ibv_qp *qp)
{
	struct ibv_qp_attr attr;
	int flags;
	int rc;
	memset(&attr, 0, sizeof(attr));
	 attr.qp_state = IBV_QPS_INIT;
 	attr.port_num = config.ib_port;
 	attr.pkey_index = 0;
 	attr.qp_access_flags = IBV_ACCESS_LOCAL_WRITE | IBV_ACCESS_REMOTE_READ | IBV_ACCESS_REMOTE_WRITE;
 	flags = IBV_QP_STATE | IBV_QP_PKEY_INDEX | IBV_QP_PORT | IBV_QP_ACCESS_FLAGS;
 	rc = ibv_modify_qp(qp, &attr, flags);
 	if (rc)
 	fprintf(stderr, "failed to modify QP state to INIT\n");
 	return rc;
}



//need to transition qp from
//RESET -> INIT -> RTR -> RTS
int transition_qp(){
	modify_qp_to_init(		
	
}


int main(){

}
