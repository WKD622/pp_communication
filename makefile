build:
	mpicc -o measure_c measure.c 	
	mpicc -o measure_value_c measure_value.c 	
	mpiexec -machinefile ./allnodes	-np 2 ./measure_c
	mpiexec -machinefile ./allnodes	-np 2 ./measure_value_c	
