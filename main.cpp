/*
Author: Da Teng
Class: ECE6122
Last Date Modified: 11/24/2024
Description: This is the main file for the Lab
*/

#include "mpi.h"
#include "utils.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

int main(int argc, char* argv[])
{
    int P = 1, N = 1000000;
    // * P determines which function to use.
    // * N is the number of random samples to generate.

    // * Load the command line arguments
    for (int i = 1; i < argc; i++)
    {
        if (std::string(argv[i]) == "-P")
        {
            try
            {
                P = std::stoi(argv[i + 1]); // Convert P to integer
            }
            catch (const std::exception& e)
            {
                std::cerr
                    << "Error: Invalid value for -P. It must be an integer."
                    << std::endl;
                MPI_Finalize();
                return 1;
            }
            // Check if P is either 1 or 2
            if (P != 1 && P != 2)
            {
                std::cerr << "Error: -P must be either 1 or 2." << std::endl;
                MPI_Finalize();
                return 1;
            }
        }
        else if (std::string(argv[i]) == "-N")
        {
            try
            {
                N = std::stoi(argv[i + 1]); // Convert N to integer
            }
            catch (const std::exception& e)
            {
                std::cerr
                    << "Error: Invalid value for -N. It must be an integer."
                    << std::endl;
                MPI_Finalize();
                return 1;
            }
            // Check if N is greater than 0
            if (N <= 0)
            {
                std::cerr << "Error: -N must be greater than 0." << std::endl;
                MPI_Finalize();
                return 1;
            }
        }
    }

    int numtasks, rank, rc;

    // * Disable warning
    setenv("MV2_SUPPRESS_JOB_STARTUP_PERFORMANCE_WARNING", "1", 1);
    setenv("MV2_USE_RDMA_CM", "0", 1);

    rc = MPI_Init(&argc, &argv);

    if (rc != MPI_SUCCESS)
    {
        printf("Error starting MPI program. Terminating.\n");
        MPI_Abort(MPI_COMM_WORLD, rc);
    }

    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Status status;

    srand(time(0) + rank);
    int samplesPerProcessor;
    if (rank == 0)
    { // Number of samples each processor should handle
        samplesPerProcessor = N / numtasks;
    }

    MPI_Bcast(&samplesPerProcessor, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == numtasks - 1)
    {
        samplesPerProcessor += N % numtasks;
    }

    // Estimate the integral locally
    double localEstimate = 0.0;
    if (P == 1)
    {
        localEstimate = estimateIntegral1(samplesPerProcessor);
    }
    else if (P == 2)
    {
        localEstimate = estimateIntegral2(samplesPerProcessor);
    }

    // Reduce all local estimates to the root processor (rank 0)
    double globalEstimate = 0.0;
    MPI_Reduce(&localEstimate, &globalEstimate, 1, MPI_DOUBLE, MPI_SUM, 0,
               MPI_COMM_WORLD);

    // The root processor prints the result
    if (rank == 0)
    {
        globalEstimate /= numtasks; // Average the results from all processors
        if (P == 1)
        {
            std::cout << "The estimate for integral 1 is " << globalEstimate
                      << std::endl;
        }
        else if (P == 2)
        {
            std::cout << "The estimate for integral 2 is " << globalEstimate
                      << std::endl;
        }
    }

    // Finalize MPI
    MPI_Finalize();

    if (rank == 0)
    {
        cout << "Bye!" << endl;
    }

    return 0;
}
