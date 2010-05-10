#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

tvsub( out, in )
struct timeval *out, *in;
{
	if( (out->tv_usec -= in->tv_usec) < 0 )   {
		out->tv_sec--;
		out->tv_usec += 1000000;
	}
	out->tv_sec -= in->tv_sec;
}

int parsear_parametros_servidor(int argc, char* argv[],   int *puerto)
{
	int c;
	while ((c= getopt(argc, argv, "p::")) != -1)
	{
		switch(c)
		{
			case 'p':
			{
				*puerto = atoi(optarg);
				if (*puerto <1024 || *puerto > 65535)
				{
					printf ("El puerto debe estar comprendido entre 1024 y 65535\n");
					return -1;
				}
				break;
			}			
		}
	}
    return 0;
}

int parsear_parametros_cliente(int argc, char* argv[],  char **ip, int *puerto, int *repeticiones, int *tamanio)
{
	if (argc < 2)
	{
		printf ("Error. Uso: %s <IPservidor> [-p <puerto>] [-r <repeticiones>] [-s <tamaño>]\n", argv[0]);
		return -1;
	}
	*ip= argv[1];
	
	int c;
	while ((c= getopt(argc, argv, "r::p::s::")) != -1)
	{
		switch(c)
		{
			case 'r':
			{
				*repeticiones = atoi(optarg);
				if (*repeticiones < 1 || *repeticiones > 101)
				{
					printf ("El nro de repeticiones debe estar comprendido entre 1 y 101\n");
					return -1;
				}
				break;
			}			
			case 's':
			{
				*tamanio = atoi(optarg);
				if (*tamanio <1 || *tamanio > 10001)
				{
					printf ("El tamano debe estar comprendido entre 1 y 10001\n");
					return -1;
				}
				break;
			}			
			case 'p':
			{
				*puerto = atoi(optarg);
				if (*puerto <1024 || *puerto > 65535)
				{
					printf ("El puerto debe estar comprendido entre 1024 y 65535\n");
					return -1;
				}
				break;
			}			
		}
	}
	return 0;	
}

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

struct timeval start;      /* for timing */
struct timeval stop;       /* for timing */
struct timezone tz;       /* for timing */
struct ResultadosTemporizador {
    int Minimo;
    int Maximo;
    int Total;
    int Cantidad;
    float Promedio;
};

struct ResultadosTemporizador res;

void iniciar_resultados_temporizador()
{
    res.Minimo = 10000000;
    res.Maximo = -1;
    res.Cantidad= 0;
    res.Total = 0;
}

void iniciar_temporizador()
{
		gettimeofday( &start, &tz );    
}

int finalizar_temporizador()
{
        int tiempo;
		gettimeofday( &stop, &tz );		
		tvsub( &stop, &start );		
		tiempo =  stop.tv_sec * 1000000 + stop.tv_usec;
		if (res.Minimo > tiempo)
		    res.Minimo = tiempo;
		if (res.Maximo < tiempo)
		    res.Maximo = tiempo;
		res.Total += tiempo;
		res.Cantidad++;
		return tiempo;
}

struct ResultadosTemporizador obtener_resultados_temporizador()
{
    if (res.Cantidad!= 0)
        res.Promedio = (float)res.Total/res.Cantidad;
    else
        res.Promedio = 0;
    return res;
}


