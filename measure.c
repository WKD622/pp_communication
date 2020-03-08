#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

void initialize_table(int *t,int number_of_elements){
  int i;
  for (i = 0; i < number_of_elements; i++){
      t[i] = i;
  }
}

void print_table(int *t, int number_of_elements){
  int i;
  for (i = 0; i < number_of_elements; i++){
      printf("%i ", t[i]);
  }
}

int main(int argc, char** argv) {
  MPI_Init(NULL, NULL);
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  int data_size = 10;
  int data[data_size];
  initialize_table(data, data_size);
  print_table(data, data_size);

  // We are assuming at least 2 processes for this task
  if (world_size < 2) {
    fprintf(stderr, "World size must be greater than 1 for %s\n", argv[0]);
    MPI_Abort(MPI_COMM_WORLD, 1);
  }
  int number;
  if (world_rank == 0) {
    MPI_Barrier(MPI_COMM_WORLD);

    // If we are rank 0, set the number to -1 and send it to process 1
    number = -1;

    struct timeval tval_before, tval_after, tval_result;

    gettimeofday(&tval_before, NULL);

    // Some code you want to time, for example:
    MPI_Send(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);

    MPI_Recv(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    gettimeofday(&tval_after, NULL);
    timersub(&tval_after, &tval_before, &tval_result);

    printf("Time elapsed: %ld.%06ld\n", (long int)tval_result.tv_sec, (long int)tval_result.tv_usec);
    printf("Process 0 received number %d from process 1\n", number);


} else if (world_rank == 1) {
    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("Process 1 received number %d from process 0\n", number);

    MPI_Send(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

}
  MPI_Finalize();
  return 0;
}
