#include <stdio.h>
#include <string.h>
#include "mpi.h" 

//incluimos funciones comunes
#include "utiles_ping.c" 

int main(int argc, char* argv[]){
  int rank, size;         
  int repeticiones = 5;
  int tamanio=100;
  
  //Esta funcion se encarga de completar los parametros repeticiones y tamaño
  parsear_parametros_cliente(argc, argv, &repeticiones, &tamanio);  

  MPI_Init(&argc, &argv); 
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  
  //buscamos el nombre del host para no afectar las mediciones más adelante
  char * hostname = nombre_host();

  printf("Rank: %d Cant. servidores: %d Nombre servidor: %s\n", rank, size, hostname);

  //preparamos el mensaje con el tamaño ingresado por el usuario
  char * mensaje;
  int largo_mensaje;
  mensaje = generar_paquete(tamanio);
  largo_mensaje = tamanio;

  int r;
  for (r = 0 ; r < repeticiones; r++)
  {  
    if (rank == 0)
    {             
      iniciar_temporizador();

      //Enviamos el mensaje a todos los hosts del anillo
      MPI_Bcast((void *)mensaje, largo_mensaje, MPI_CHAR, rank, MPI_COMM_WORLD);  
      printf("Se envio el mensaje %d desde el host %s y rank: %d\n",r, hostname, rank);

      //Esperamos la respuesta de cada host
      int i;
      for (i=1;i<size;i++)
      {
        MPI_Bcast(mensaje, largo_mensaje, MPI_CHAR, i, MPI_COMM_WORLD);  
        printf("%s - Se recibio el mensaje desde el rank: %d\n", hostname, i);        
      }         

      //Imprimimos RTT de esta repeticion
      printf("RTT flooding %d: %d uSeg\n", r+1,finalizar_temporizador());    
    }
    else 
    {
        //Esperamos mensaje desde el proceso 0
        MPI_Bcast(mensaje, largo_mensaje, MPI_CHAR, 0, MPI_COMM_WORLD);  
        printf("%s - Se recibio notificacion desde el rank: %d\n", hostname, 0);        
        //Respondemos con un broadcast a todos los hosts del grupo
        MPI_Bcast( (void *) mensaje, largo_mensaje, MPI_CHAR, rank, MPI_COMM_WORLD);  
        printf("Se envio el mensaje %d desde el host %s y rank: %d\n",r, hostname, rank);
    }
  }
  if (rank==0)
  {
    //Imprimimos los resultados finales 
    imprimir_resultados();
  }

  MPI_Finalize();         
  return 0;
 }
