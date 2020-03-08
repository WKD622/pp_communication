#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

void initialize_table(int *t, int number_of_elements)
{
  int i;
  for (i = 0; i < number_of_elements; i++)
  {
    t[i] = i;
  }
}

void print_table(int *t, int number_of_elements)
{
  int i;
  for (i = 0; i < number_of_elements; i++)
  {
    printf("%i ", t[i]);
  }
}

double byte_to_mega_bits(int number_of_elements)
{
  return ((double)number_of_elements) * 8 / 1000000;
}

double mega_bits_per_seconds(double mega_bits, double seconds)
{
  return mega_bits * 1.0 / seconds;
}

int main(int argc, char **argv)
{
  MPI_Init(NULL, NULL);
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  long int data_size = 1000000;
  int *data = malloc(data_size * sizeof(int));
  initialize_table(data, data_size);
  int number_of_attempts = 1000;
  int average_speed = 0;
  int j;
  for (j = 0; j < number_of_attempts; j++)
  {
    if (world_size < 2)
    {
      fprintf(stderr, "World size must be greater than 1 for %s\n", argv[0]);
      MPI_Abort(MPI_COMM_WORLD, 1);
    }
    int number;
    if (world_rank == 0)
    {
      MPI_Barrier(MPI_COMM_WORLD);

      number = -1;

      struct timeval tval_before, tval_after, tval_result;
      gettimeofday(&tval_before, NULL);

      MPI_Send(data, data_size, MPI_INT, 1, 0, MPI_COMM_WORLD);
      MPI_Recv(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

      gettimeofday(&tval_after, NULL);
      timersub(&tval_after, &tval_before, &tval_result);

      double time_passed_in_seconds = (double)tval_result.tv_sec + (double)tval_result.tv_usec / 1000000;
      double speed = mega_bits_per_seconds(byte_to_mega_bits(data_size), time_passed_in_seconds);
      printf("Speed: %f Mb/s\n", speed);
      if (j == 1){
        average_speed = speed
      } else {
        average_speed = average_speed - average_speed / (double)j
        average_speed = average_speed + speed / (double)j
      }
    }
    else if (world_rank == 1)
    {
      MPI_Barrier(MPI_COMM_WORLD);
      int *received_data = malloc(data_size * sizeof(int));
      int response_number = 2;
      MPI_Recv(received_data, data_size, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      printf("Process 1 received array.\n");
      MPI_Send(&response_number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
  }
  MPI_Finalize();
  printf('average: %f', average_speed)
  return 0;
}
