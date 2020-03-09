build:
	mpicc -o lab_c lab.c 	
	mpiexec -machinefile ./allnodes	-np 2 ./lab_c 3
