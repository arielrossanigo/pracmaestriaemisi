/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "pingRPC.h"
#include "utiles_ping.c"


char **
ping_1_svc(char **argp, struct svc_req *rqstp)
{
	static char * result;
	
	printf("Se recibio peticion en el puerto %d, desde la IP: %s\n", 
		(svc_getcaller(rqstp->rq_xprt))->sin_port, 
		inet_ntoa((svc_getcaller(rqstp->rq_xprt))->sin_addr));
	printf("TS: %s,  Datos:%s \n", str_hora_actual(), *argp);
	
	result= *argp;	
	return &result;
}
