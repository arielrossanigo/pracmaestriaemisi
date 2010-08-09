#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include "utiles_ping.c"

int main(int argc, char** argv)
{
  int puerto= 2222;

  if (parsear_parametros_servidor(argc, argv, &puerto)<0) return -1;
  
  int socket_tcp;
  struct sockaddr_in direccion;

  direccion.sin_family = AF_INET;
  direccion.sin_port = htons(puerto);
  direccion.sin_addr.s_addr = INADDR_ANY;

  //abrimos un socket
  socket_tcp = socket (AF_INET, SOCK_STREAM, 0);
  if (socket_tcp == -1) 
  {
    printf("Error, no se pudo crear el socket\n");
    return -1;
  }  
  if (bind (socket_tcp, (struct sockaddr *)&direccion, sizeof (direccion)) == -1)
  {
    close (socket_tcp);
    printf("Error, no funciono el bind\n");
    return -1;
  }
  
  // Se avisa al sistema que comience a atender llamadas de clientes
  if (listen (socket_tcp, 1) == -1)
  {
    close (socket_tcp);
    printf("Error, no pudo escuchar\n");
    return -1;
  }

  struct sockaddr_in cliente;
  int socket_retorno;
  socklen_t largo = sizeof(struct sockaddr_in);

  printf("Servidor a la espera de paquetes. Escuchando en puerto %d\n", puerto);
  
  while(1)
  {
    //llamamos al aceptar para esperar una coneccion
    socket_retorno= accept(socket_tcp, (struct sockaddr *) &cliente, &largo);
    if (socket_retorno == -1)
    {
      close(socket_tcp);
      printf("Error en el aceptar\n");
      return -1;
    }
    
    char *leido;
    int leido_l;
    leido = malloc(10002*sizeof(char));
    leido_l= read(socket_retorno, leido, 10002);
    printf("Cliente: %s, TS: %s, Datos: %s \n",  inet_ntoa( cliente.sin_addr), str_hora_actual(), leido);
    write(socket_retorno, leido, leido_l);

    close(socket_retorno);
  }
  close(socket_tcp);
  return 0;
}
