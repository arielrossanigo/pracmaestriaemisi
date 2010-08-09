#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

//completa puerto de acuerdo a los parametros de entrada
int parsear_parametros_servidor(int argc, char* argv[], int *puerto)
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

//completa IP, puerto, repeticiones y tamanio de acuerdo a los parametros de entrada
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

//Genera un string con los digitos del 0 al 9, de tamanio caracteres
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

//Variables utilizadas en los temporizadores
struct timeval start;      
struct timeval stop;       
struct timezone tz;       

//Calcula la diferencia entre dos valores de tiempo y la almacena en out
void tvsub( out, in )
struct timeval *out, *in;
{
  if( (out->tv_usec -= in->tv_usec) < 0 )   
  {
    out->tv_sec--;
    out->tv_usec += 1000000;
  }
  out->tv_sec -= in->tv_sec;
}

struct ResultadosTemporizador 
{
  int Minimo;
  int Maximo;
  int Total;
  int Cantidad;
  float Promedio;
};

//variable para llevar los resultados acumulados del todas las mediciones
struct ResultadosTemporizador res;

//inicializamos res
void iniciar_resultados_temporizador()
{
  res.Minimo = 0;
  res.Maximo = -1;
  res.Cantidad= 0;
  res.Total = 0;
}

//Iniciamos una medicion, colocamos en start la hora del dia
void iniciar_temporizador()
{
  gettimeofday( &start, &tz );    
}

//finalizamos una medicion, calculamos la diferencia entre la hora del dia y start
//actualizamos los valores acumulados
int finalizar_temporizador()
{
  int tiempo;
  gettimeofday( &stop, &tz );    
  tvsub( &stop, &start );    
  tiempo =  stop.tv_sec * 1000000 + stop.tv_usec;
  if (res.Minimo > tiempo || res.Cantidad==0) res.Minimo = tiempo;
  if (res.Maximo < tiempo || res.Cantidad==0) res.Maximo = tiempo;
  res.Total += tiempo;
  res.Cantidad++;
  return tiempo;
}

//Obtenemos los resultados acumulados de todas las mediciones
struct ResultadosTemporizador obtener_resultados_temporizador()
{
    if (res.Cantidad!= 0)
        res.Promedio = (float)res.Total/res.Cantidad;
    else
        res.Promedio = 0;
    return res;
}

//Imprimimos los resultados acumulados
void imprimir_resultados_ping(char *IP)
{
    struct ResultadosTemporizador r= obtener_resultados_temporizador();
    printf("========================================================\n");
    printf("Ping a: %s\n", IP);
    printf("RTT min: %d uSeg, max: %d uSeg, prom: %.2f uSeg\n", r.Minimo, r.Maximo, r.Promedio);
    printf("========================================================\n");
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
