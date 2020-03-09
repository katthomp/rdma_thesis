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


int resources_create(){
	
	pd=ibv_alloc_pd()
	mr=ibv_reg_mr()//takes pd, buffer, size, and flags?
	cq=ibv_create_cq()
}

//need to transition qp from
//RESET -> INIT -> RTR -> RTS
int transition_qp(){
	
	
}


int main(){

}
