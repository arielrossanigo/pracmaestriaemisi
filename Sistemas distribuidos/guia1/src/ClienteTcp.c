#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>

char* generar_paquete(int tamanio)
{
	char* res = malloc(tamanio*sizeof(char));
	int i;
	for (i = 0; i<tamanio; i++)
	{
		res[i] = (char)(i%10 + 48);
	}
	return res;
}


tvsub( out, in )
struct timeval *out, *in;
{
	if( (out->tv_usec -= in->tv_usec) < 0 )   {
		out->tv_sec--;
		out->tv_usec += 1000000;
	}
	out->tv_sec -= in->tv_sec;
}


int main(int argc, char* argv[])
{			
	char *ip_server;
	int puerto = 2222;
	int repeticiones = 5;
	int tamanio=100;
	
	if (argc < 2)
	{
		printf ("Error. Uso: %s <IPservidor> [-p <puerto>] [-r <repeticiones>] [-s <tamaño>]\n", argv[0]);
		return -1;
	}
	ip_server= argv[1];
	
	
	int c;
	while ((c= getopt(argc, argv, "r::p::s::")) != -1)
	{
		switch(c)
		{
			case 'r':
			{
				repeticiones = atoi(optarg);
				if (repeticiones < 1 || repeticiones > 101)
				{
					printf ("El nro de repeticiones debe estar comprendido entre 1 y 101\n");
					return -1;
				}
				break;
			}			
			case 's':
			{
				tamanio = atoi(optarg);
				if (tamanio <1 || tamanio > 10001)
				{
					printf ("El tamano debe estar comprendido entre 1 y 10001\n");
					return -1;
				}
				break;
			}			
			case 'p':
			{
				puerto = atoi(optarg);
				if (puerto <1024 || puerto > 65535)
				{
					printf ("El puerto debe estar comprendido entre 1024 y 65535\n");
					return -1;
				}
				break;
			}			
		}
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
	lee= malloc(100 * sizeof(char));
	int i;
	
	struct timeval start;      /* for timing */
	struct timeval stop;       /* for timing */
	struct timezone tz;       /* for timing */
	
	
	int mayor=0;
	int menor=1000000000;
	int total=0;
	int tiempo =0;
	
	
	
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

		gettimeofday( &start, &tz );
		write(socket_tcp, msg, l_msg);		
		read(socket_tcp, lee, 100);
		gettimeofday( &stop, &tz );		
		tvsub( &stop, &start );
		tiempo = stop.tv_sec * 1000000 + stop.tv_usec;
		printf("RTT paquete %d: %d uSeg\n", i+1,tiempo);
		if (tiempo> mayor) mayor = tiempo;
		if (tiempo< menor) menor = tiempo;
		total+= tiempo;
		
		close(socket_tcp);
	}
	printf("===========================================\n");
	printf("IP del servidor: %s\n", ip_server);
	printf("RTT minimo:   %d uSeg\n", menor);
	printf("RTT maximo:   %d uSeg\n", mayor);
	printf("RTT promedio: %0.2f uSeg\n", (float)total/repeticiones);
	
	
	
	return 0;
}

