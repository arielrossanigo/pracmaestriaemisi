TP2: 
	gcc -o ../obj/rpcping ../src/clienteRPC.c ../src/pingRPC_clnt.c
	gcc -o ../obj/rpcpingd ../src/servidorRPC.c ../src/pingRPC_svc.c
	
clean: 
	rm ../obj/*

copy:	
	if test -d /home/sod/2010/tp2/Zenobi-DIorio-Rossanigo; then echo "el path ya existe"; else mkdir /home/sod/2010/tp2/Zenobi-DIorio-Rossanigo; fi
	cp -R -f -u ../* /home/sod/2010/tp2/Zenobi-DIorio-Rossanigo
	
	
	
