TP1: 
	gcc -o ../obj/tcpping ../src/ClienteTcp.c
	gcc -o ../obj/tcppingd ../src/ServidorTcp.c
	gcc -o ../obj/udppingd ../src/ServidorUdp.c
	gcc -o ../obj/udpping ../src/ClienteUdp.c
	
clean: 
	rm ../obj/*
	
copy:	
	if test -d /home/sod/2010/tp1/Zenobi-DIorio-Rossanigo; then echo "el path ya existe"; else mkdir /home/sod/2010/tp1/Zenobi-DIorio-Rossanigo; fi
	cp -R -f -u ../* /home/sod/2010/tp1/Zenobi-DIorio-Rossanigo
	
