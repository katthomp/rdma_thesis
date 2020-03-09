#include <infiniband/verbs.h> //unsure if this is the Exact right library


//establishes TCP conn to exchange info
int sock_conn(){
}

//should create all the different things u need
//
//
//
struct resources{
	pd;
	mr;

}
int resources_create(){
	pd=ibv_alloc_pd(context)
	mr=ibv_reg_mr()//takes pd, buffer, size, and flags?
	cq=ibv_create_cq()
}

//need to transition qp from
//RESET -> INIT -> RTR -> RTS
int transition_qp(){
}


int main(){

}
