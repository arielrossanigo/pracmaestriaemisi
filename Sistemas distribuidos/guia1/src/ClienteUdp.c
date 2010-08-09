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
    
  int socket_udp;
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
  lee= malloc((tamanio + 1) * sizeof(char));
  
  int i;
  int longitud_direccion = sizeof(direccion);  
  int bytes_leidos;
  //abrimos un socket
  socket_udp = socket (AF_INET, SOCK_DGRAM, 0);
  if (socket_udp == -1) 
  {
    printf("Error, no se pudo crear el socket\n");
    return -1;
  }

  printf("========================================================\n");
  for (i =0; i<repeticiones;i++)
  {
    //iniciamos medicion
    iniciar_temporizador();
    //escribimos el msg y esperamos la respuesta
    sendto (socket_udp, msg, l_msg+1, 0,  (struct sockaddr*) &direccion, longitud_direccion);    
    bytes_leidos= recvfrom(socket_udp, lee, 10002, 0, (struct sockaddr*) &direccion, &longitud_direccion);

    if (bytes_leidos == -1)
    {
      close(socket_udp);
      printf("Error en la lectura\n");
      return -1;
    }    
    //finalizamos la medicion e imprimimos el RTT
    printf("RTT paquete %d: %d uSeg\n", i+1,finalizar_temporizador());    
  }
  close(socket_udp);
  //imprimimos resultados acumulados
  imprimir_resultados_ping(ip_server);
  return 0;
}

