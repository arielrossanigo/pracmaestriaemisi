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
    struct ResultadosTemporizador r;
    iniciar_resultados_temporizador();
	int tiempo =0;

	
	if (parsear_parametros_cliente(argc, argv, &ip_server, &puerto, &repeticiones, &tamanio) < 0)
	{
		return -1;
	}
		
	// Crear socket
	int socket_tcp;
	struct sockaddr_in direccion;
	
	direccion.sin_family = AF_INET;
	direccion.sin_port = htons(puerto);
	direccion.sin_addr.s_addr = inet_addr(ip_server);


	char *msg = generar_paquete(tamanio);

	int l_msg= strlen(msg);
	char *lee;	
	lee= malloc(tamanio * sizeof(char));
	int i;
		
	for (i =0; i<repeticiones;i++)
	{
		socket_tcp = socket (AF_INET, SOCK_STREAM, 0);
		if (socket_tcp == -1) 
		{
			printf("Error, no se pudo crear el socket\n");
			return -1;
		}

		if (connect (socket_tcp, (struct sockaddr *) &direccion, sizeof (direccion)) == -1)
		{
			printf("Error, no se pudo conectar con el server\n");
			return -1;
		}

		iniciar_temporizador();
		
		write(socket_tcp, msg, l_msg);		
		read(socket_tcp, lee, tamanio);

		tiempo = finalizar_temporizador();
		
		printf("RTT paquete %d: %d uSeg\n", i+1,tiempo);
		
		close(socket_tcp);
	}
	printf("===========================================\n");
	
	r= obtener_resultados_temporizador();
	printf("IP del servidor: %s\n", ip_server);
	printf("RTT minimo:   %d uSeg\n", r.Minimo);
	printf("RTT maximo:   %d uSeg\n", r.Maximo);
	printf("RTT promedio: %0.2f uSeg\n", r.Promedio);
	
		
	return 0;
}

