#include "pingRPC.h"
#include "utiles_ping.c"

void date_prog_1(char *host, int tamanio, int repeticiones)
{
  CLIENT *clnt;
  char * *result_1;
  char *msg = generar_paquete(tamanio);

  clnt = clnt_create (host, DATE_PROG, DATE_VERS, "udp");
  if (clnt == NULL) {
    clnt_pcreateerror (host);
    exit (1);
  }

  int i;
  for (i=0; i< repeticiones;i++)
  {
    iniciar_temporizador();
    result_1 = ping_1(&msg, clnt);
    if (result_1 == (char **) NULL) {clnt_perror (clnt, "call failed");}
        
    printf("RTT paquete %d: %d uSeg\n", i+1,finalizar_temporizador());    
    
  }
  imprimir_resultados_ping(host);
  clnt_destroy (clnt);
}


int main (int argc, char *argv[])
{
  char *ip_server;
  int puerto;
  int repeticiones = 5;
  int tamanio=100;

  if (parsear_parametros_cliente(argc, argv, &ip_server, &puerto, &repeticiones, &tamanio) < 0)
  {
    return -1;
  }

  date_prog_1 (ip_server, tamanio, repeticiones);
  exit (0);
}
