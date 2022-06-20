struct ind_ap
{
    unsigned int type: 1;
    unsigned int option: 2;
    unsigned int err: 1;
    unsigned int mem_addr: 20;
    unsigned int value: 10;
};

struct ind_tp
{
    unsigned int ID_proto: 3;
    char ID_source[5];
    char ID_dest[5];
    unsigned int tpdu_type: 1; 
    unsigned int credits: 2; 
    struct ind_ap ap_proto;
};