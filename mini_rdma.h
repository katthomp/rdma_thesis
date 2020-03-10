//put in all the libs that u pasted in

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <inttypes.h>
#include <endian.h>
#include <byteswap.h>
#include <getopt.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <infiniband/verbs.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>


struct config_t
{
    const char *dev_name;         /* IB device name */
    char *server_name;            /* server host name */
    u_int32_t tcp_port;           /* server TCP port */
    int ib_port;                  /* local IB port to work with */
    int gid_idx;                  /* gid index to use */
};

struct cm_con_data_t
{
    uint64_t addr;                /* Buffer address */
    uint32_t rkey;                /* Remote key */
    uint32_t qp_num;              /* QP number */
    uint16_t lid;                 /* LID of the IB port */
    uint8_t gid[16];              /* gid */
} __attribute__ ((packed));
//last line just means no byte padding

struct resources
{
   	 struct ibv_device_attr device_attr;
   	 struct ibv_port_attr port_attr;       /* IB port attributes */
   	 struct con_data_t remote_props;    /* values to connect to remote side */
   	 struct ibv_context *ib_ctx;   /* device handle */
   	 struct ibv_pd *pd;            /* PD handle */
   	 struct ibv_cq *cq;            /* CQ handle */
   	 struct ibv_qp *qp;            /* QP handle */
   	 struct ibv_mr *mr;            /* MR handle for buf */
   	 char *buf;                    /* memory buffer pointer, used for RDMA and send
                                     ops */
    int sock;                     /* TCP socket file descriptor */
};





