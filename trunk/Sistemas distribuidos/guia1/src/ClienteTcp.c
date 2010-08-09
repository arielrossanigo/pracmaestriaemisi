#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include "utiles_ping.c"

int main(int argc, char* argv[])
{      
  char *ip_server;
  int puerto = 2222;
  int repeticiones = 5;
  int tamanio=100;

  iniciar_resultados_temporizador();
  
  if (parsear_parametros_cliente(argc, argv, &ip_server, &puerto, &repeticiones, &tamanio) < 0)
  {
    return -1;
  }
    
  int socket_tcp;
  struct sockaddr_in direccion;
  //generamos IP del server
  direccion.sin_family = AF_INET;
  direccion.sin_port = htons(puerto);
  direccion.sin_addr.s_addr = inet_addr(ip_server);

  //generamos el paquete a enviar
  char *msg = generar_paquete(tamanio);
  int l_msg= strlen(msg);

  //creamos buffer de lectura
  char *lee;  
  lee= malloc(tamanio * sizeof(char));

  printf("========================================================\n");    
  int i;
  for (i =0; i<repeticiones;i++)
  {
    //abrimos un socket
    socket_tcp = socket (AF_INET, SOCK_STREAM, 0);
    if (socket_tcp == -1) 
    {
      printf("Error, no se pudo crear el socket\n");
      return -1;
    }
    //establecemos la conexion
    if (connect (socket_tcp, (struct sockaddr *) &direccion, sizeof (direccion)) == -1)
    {
      printf("Error, no se pudo conectar con el server\n");
      return -1;
    }
    
    //iniciamos medicion
    iniciar_temporizador();
    //escribimos el msg y esperamos la respuesta
    write(socket_tcp, msg, l_msg);    
    read(socket_tcp, lee, tamanio);
    //finalizamos la medicion e imprimimos el RTT
    printf("RTT paquete %d: %d uSeg\n", i+1,finalizar_temporizador());
    
    close(socket_tcp);
  }
  //imprimimos resultados acumulados
  imprimir_resultados_ping(ip_server);    
  return 0;
}

