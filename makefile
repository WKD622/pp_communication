build:
	mpicc -o measure_c measure.c 	
	mpiexec -machinefile ./allnodes	-np 2 ./measure_c	

