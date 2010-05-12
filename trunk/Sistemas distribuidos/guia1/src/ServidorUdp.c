#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include "utiles_ping.c"

int main(int argc, char** argv)
{
	
	int puerto= 2222;

	if (parsear_parametros_servidor(argc, argv, &puerto)<0)
	    return -1;
	

	int socket_udp;
	struct sockaddr_in direccion;
	
	direccion.sin_family = AF_INET;
	direccion.sin_port = htons(puerto);
	direccion.sin_addr.s_addr = INADDR_ANY;

	// Crear socket
	socket_udp = socket (AF_INET, SOCK_DGRAM, 0);
	if (socket_udp == -1) 
	{
		printf("Error, no se pudo crear el socket\n");
		return -1;
	}

	if (bind (socket_udp, (struct sockaddr *)&direccion, sizeof (direccion)) == -1)
	{
		close (socket_udp);
		printf("Error, no funciono el bind\n");
		return -1;
	}
	
	//direccion del cliente
	struct sockaddr_in cliente;
	int bytes_leidos;
	int longitud_cliente = sizeof  (cliente);
	
	printf("Servidor a la espera de paquetes. Escuchando en puerto %d\n", puerto);

    //buffer para la lectura
	char *leido;
	leido = malloc(10002*sizeof(char));
	int leido_l;
	
	while(1)
	{	
		bytes_leidos= recvfrom(socket_udp, leido, 10002, 0, (struct sockaddr*) &cliente, &longitud_cliente);
		if (bytes_leidos == -1)
		{
			close(socket_udp);
			printf("Error en la lectura\n");
			return -1;
		}		
		printf("Cliente: %s, TS: %s, Datos: %s \n",  inet_ntoa( cliente.sin_addr), str_hora_actual(), leido);		
		sendto(socket_udp, leido, bytes_leidos, 0, (struct sockaddr*) &cliente,  longitud_cliente);

	}
	close(socket_udp);
	return 0;
}
