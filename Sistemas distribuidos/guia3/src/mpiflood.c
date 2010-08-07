/*
 * MPI version of HelloWorld.c compilation: mpicc -o hello hello.c
 * mpirun -np number_of_processes hello
 */

 #include <stdio.h>
 #include <string.h>
 #include "utiles_ping.c" 
 #include "mpi.h" 

int main(int argc, char* argv[]){
  int rank, size;         //process rank and number of processes
	int repeticiones = 5;
	int tamanio=100;
	
	parsear_parametros_cliente(argc, argv, &repeticiones, &tamanio);  

  MPI_Init(&argc, &argv); //MPI initialize        
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        
  printf("Rank: %d Cant. servidores: %d Nombre servidor: %s\n", rank, size, nombre_host());

	int r;
	char * mensaje;
	char * hostname = nombre_host();
	int largo_mensaje;
	mensaje = generar_paquete(tamanio);
	largo_mensaje = tamanio;
	for (r = 0 ; r < repeticiones; r++)
	{	
		if (rank == 0)
		{			
			iniciar_temporizador();

			MPI_Bcast( (void *) mensaje, largo_mensaje, MPI_CHAR, rank, MPI_COMM_WORLD);	
			printf("Se envio el mensaje %d desde el host %s y rank: %d\n",r, hostname, rank);

			int i;
			for (i=1;i<size;i++)
			{
				MPI_Bcast(mensaje, largo_mensaje, MPI_CHAR, i, MPI_COMM_WORLD);	
				printf("%s - Se recibio el mensaje desde el rank: %d\n", hostname, i);				
			}     		
		  printf("RTT flooding %d: %d uSeg\n", r+1,finalizar_temporizador());		
		}
		else 
		{
				MPI_Bcast(mensaje, largo_mensaje, MPI_CHAR, 0, MPI_COMM_WORLD);	
				printf("%s - Se recibio notificacion desde el rank: %d\n", hostname, 0);				
				MPI_Bcast( (void *) mensaje, largo_mensaje, MPI_CHAR, rank, MPI_COMM_WORLD);	
				printf("Se envio el mensaje %d desde el host %s y rank: %d\n",r, hostname, rank);
		}
	}
	if (rank==0)
	{
		imprimir_resultados();
	}

  MPI_Finalize();         //MPI finalize
  return 0;
 }
