
// Author: Wes Kendall
// Copyright 2012 www.mpitutorial.com
// This code is provided freely with the tutorials on mpitutorial.com. Feel
// free to modify it for your own use. Any distribution of the code must
// either provide a link to www.mpitutorial.com or keep this header intact.
//
// Program that computes the average of an array of elements in parallel using
// MPI_Scatter and MPI_Allgather
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <assert.h>

void initialize_table(int *t, int number_of_elements)
{
    int i;
    for (i = 0; i < number_of_elements; i++)
    {
        t[i] = 1.0;
    }
}

void print_table(int *t, int number_of_elements)
{
    int i;
    for (i = 0; i < number_of_elements; i++)
    {
        printf("%f ", t[i]);
    }
}

// Computes the average of an array of numbers
void compute_multiply(float *array, int num_elements, int rank)
{
    int i;
    for (i = 0; i < num_elements; i++)
    {
        array[i] = array[i] * rank;
    }
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: avg num_elements_per_proc\n");
        exit(1);
    }

    srand(time(NULL));
    MPI_Init(NULL, NULL);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int number_of_nodes = 3;
    float *array = malloc(sizeof(float) * number_of_nodes);
    float *rand_nums = NULL;
    
    if (world_rank == 0)
    {   
        initialize_table(array, number_of_nodes);
        print_table(array, number_of_nodes);
    }

    float *response = (float *)malloc(sizeof(float) * number_of_nodes);

    MPI_Scatter(rand_nums, number_of_nodes, MPI_FLOAT, response,
                number_of_nodes, MPI_FLOAT, 0, MPI_COMM_WORLD);

    compute_multiply(response, number_of_nodes, world_rank);

    // Gather all partial averages down to all the processes
    // float *sub_avgs = (float *)malloc(sizeof(float) * world_size);
    // assert(sub_avgs != NULL);
    // MPI_Allgather(&sub_avg, 1, MPI_FLOAT, sub_avgs, 1, MPI_FLOAT, MPI_COMM_WORLD);

    // Now that we have all of the partial averages, compute the
    // total average of all numbers. Since we are assuming each process computed
    // an average across an equal amount of elements, this computation will
    // produce the correct answer.
    // float avg = compute_avg(sub_avgs, world_size);
    // printf("Avg of all elements from proc %d is %f\n", world_rank, avg);

    // Clean up
    if (world_rank == 0)
    {
        free(rand_nums);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    return 0;
}