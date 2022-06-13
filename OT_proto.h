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

void printProto(struct ind_tp tp_proto){
  printf("--PROTOCOLO DE TRANSPORTE--\n");
  printf("ID_proto: %d \n", tp_proto.ID_proto);
  printf("ID_source: %s \n", tp_proto.ID_source);
  printf("ID_dest %s \n", tp_proto.ID_dest);
  printf("tpdu_type: %d \n", tp_proto.tpdu_type);
  printf("credits: %d \n", tp_proto.credits);  
  printf("--PROTOCOLO DE APLICACIÓN--\n");
  printf("type: %d\n", tp_proto.ap_proto.type);
  printf("option: %d\n", tp_proto.ap_proto.option);
  printf("err: %d\n", tp_proto.ap_proto.err);
  printf("mem_addr: %d\n", tp_proto.ap_proto.mem_addr);
  printf("value: %d\n\n", tp_proto.ap_proto.value);     
}
