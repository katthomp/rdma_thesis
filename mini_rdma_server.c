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

int main(){

}
