#include <stdio.h>
#include <arpa/inet.h>
#include <signal.h>
#include "OT_proto.h"
#include "funciones.h"

#define PORTNO 102
#define queueSIZE 5 

int parentsk;

struct ind_ap gen_apl_proto()
{
    struct ind_ap apl_proto;
    apl_proto.type = 1;
    apl_proto.option = 0;
    apl_proto.err = 0;
    apl_proto.mem_addr = 0;
    apl_proto.value = 0;
    return apl_proto;
}

struct ind_tp gen_trans_proto()
{
    struct ind_tp trans_proto;
    trans_proto.ID_proto = 7;
    strcpy(trans_proto.ID_source, "plc");
    strcpy(trans_proto.ID_dest, "gtw");
    trans_proto.credits = 0;
    trans_proto.tpdu_type = 1;
    return trans_proto;
}

void error(char *msg) { 
  perror(msg);
  exit(0);
}

void signal_handler (int signo){

	if (signo == SIGINT){ 

    Py_Finalize();
		shutdown(parentsk,SHUT_RDWR);
		close(parentsk);
		exit(EXIT_SUCCESS);
	}
}

void handle_connection (int childsk, struct sockaddr_in clientaddr){ //Función a ejecutar para cada conexión

  struct ind_tp tp_proto;
  struct ind_ap apl_resp = (struct ind_ap)gen_apl_proto();
  struct ind_tp trans_resp = (struct ind_tp)gen_trans_proto();

  if (recv(childsk, &tp_proto, sizeof(struct ind_tp),0) == -1) 
    error("recv()");                          

  if (tp_proto.ID_proto==7 && strcmp(tp_proto.ID_source,"gtw")==0 && strcmp(tp_proto.ID_dest,"plc")==0 && tp_proto.tpdu_type==0){
    
    int credits = tp_proto.credits;

    for (int i=0; i<credits; i++){
      if (recv(childsk, &tp_proto, sizeof(struct ind_tp),0) == -1)
        error("recv()");

      if (tp_proto.ID_proto==7 && strcmp(tp_proto.ID_source,"gtw")==0 && strcmp(tp_proto.ID_dest,"plc")==0 &&  
        tp_proto.tpdu_type==1 && tp_proto.ap_proto.type==0 && tp_proto.ap_proto.err==0){
        
        if (tp_proto.ap_proto.option==1){
          apl_resp.option = 1;

          if (tp_proto.ap_proto.mem_addr==500){
            apl_resp.mem_addr = 500;

            int energia = (int) calcularEnergia();
            if (energia>=0){
              apl_resp.value = energia;
            }else
              apl_resp.err = 1;
            

          }else if (tp_proto.ap_proto.mem_addr==1000){
            apl_resp.mem_addr = 1000;

            int status = getStatus();
            if (status>=0)
              apl_resp.value = status;
            else
              apl_resp.err = 1;
            

          }else if (tp_proto.ap_proto.mem_addr==1500){
            apl_resp.mem_addr = 1500;

            int angulo = getAnguloAct();
            if (angulo>=0)
              apl_resp.value = angulo;
            else
              apl_resp.err = 1;

          }else
            apl_resp.err = 1; 
          
        }else if (tp_proto.ap_proto.option==2){
          apl_resp.option = 2;
          
          if (tp_proto.ap_proto.mem_addr==1000){
            apl_resp.mem_addr = 1000;
                      
            int res = setStatus(tp_proto.ap_proto.value);
            if (res<0)
              apl_resp.err = 1;
          
          }else if (tp_proto.ap_proto.mem_addr==1500){
            apl_resp.mem_addr = 1500;
            
            int res = moverPlaca(tp_proto.ap_proto.value);
            if (res<0)
              apl_resp.err = 1;
          
          }else
            apl_resp.err = 1;
          
      
        }else if (tp_proto.ap_proto.option==3){
          apl_resp.option = 3;

          int res = reset(tp_proto.ap_proto.mem_addr);
          if (res<0)
            apl_resp.err = 1;
            
        }else
           apl_resp.err = 1;
        
      }else
        apl_resp.err = 1;
    }

  }else
    apl_resp.err = 1;

  trans_resp.ap_proto = apl_resp;
  send(childsk, &trans_resp, sizeof(trans_resp), 0);
  close(childsk); 
}

int main(int argc, char **argv) {

  setenv("PYTHONPATH",".",1);
  Py_Initialize();
  int childsk, pid; 
  socklen_t slen;
  struct sockaddr_in serveraddr, clientaddr; 

  if(signal(SIGINT, signal_handler)== SIG_ERR) 
      error("signal()");                      

  if ((parentsk = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
    error("socket()");

  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serveraddr.sin_port = htons(PORTNO);

  if (bind(parentsk, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) == -1)
    error("bind()");

  if (listen(parentsk, queueSIZE) == -1)
    error("listen()");

  slen = sizeof(clientaddr);

  printf("Servidor corriendo...\n\n");
  
  while(1) {

    if((childsk = accept(parentsk, (struct sockaddr *) &clientaddr, &slen)) == -1) 
      error("accept()");      

    if ((pid = fork()) == -1) 
      error("fork()");

    if (pid==0)
      handle_connection(childsk, clientaddr);
  }
}