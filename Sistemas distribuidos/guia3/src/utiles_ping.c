#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

char * nombre_host()
{
	size_t len = 126;
	char * hostname = malloc(128);
	gethostname (hostname, len);
	return hostname;
}

void tvsub( out, in )
struct timeval *out, *in;
{
	if( (out->tv_usec -= in->tv_usec) < 0 )   {
		out->tv_sec--;
		out->tv_usec += 1000000;
	}
	out->tv_sec -= in->tv_sec;
}


int parsear_parametros_cliente(int argc, char* argv[], int *repeticiones, int *tamanio)
{
	int c;
	while ((c= getopt(argc, argv, "r::s::")) != -1)
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
		}
	}
	return 0;	
}

char* generar_paquete(int tamanio)
{
	char* res = malloc((tamanio+1)*sizeof(char));
	int i;
	for (i = 0; i<tamanio; i++)
	{
		res[i] = (char)(i%10 + 48);
	}
	res[tamanio]='\0';
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
    res.Minimo = 0;
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
		if (res.Minimo > tiempo || res.Cantidad==0)
		    res.Minimo = tiempo;
		if (res.Maximo < tiempo || res.Cantidad==0)
		    res.Maximo = tiempo;
		res.Total += tiempo;
		res.Cantidad++;
		return tiempo;
}

char * str_hora_actual()
{
	struct tm *ptr;
    time_t lt;
    //obtenemos tiempo actual
    lt = time(NULL);
    //obtenemos tiempo local
    ptr = localtime(&lt);
    //obtenemos un string formateado con el tiempo local
    char * res= malloc(9*sizeof(char));
    strftime(res, 9, "%H:%M:%S", ptr);
    return res;
}

struct ResultadosTemporizador obtener_resultados_temporizador()
{
    if (res.Cantidad!= 0)
        res.Promedio = (float)res.Total/res.Cantidad;
    else
        res.Promedio = 0;
    return res;
}

void imprimir_resultados()
{
    struct ResultadosTemporizador r= obtener_resultados_temporizador();
    printf("========================================================\n");
    printf("RTT min: %d uSeg, max: %d uSeg, prom: %.2f uSeg\n", r.Minimo, r.Maximo, r.Promedio);
    printf("========================================================\n");
}






