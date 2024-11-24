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

    // * Print out the command line arguments
    cout << "P = " << P << ". N = " << N << "." << endl;

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

    printf("Number of tasks= %d My rank= %d\n", numtasks, rank);

    srand(time(0) + rank);

    int samples_per_processor;

    if (rank == 0)
    {
        // Number of samples each processor should handle
        samples_per_processor = N / numtasks;
        // Broadcast the number of samples to all processors
        MPI_Bcast(&samples_per_processor, 1, MPI_INT, 0, MPI_COMM_WORLD);
    }

    // Estimate the integral locally
    double local_estimate = 0.0;
    if (P == 1)
    {
        local_estimate = estimate_integral_1(samples_per_processor);
    }
    else if (P == 2)
    {
        local_estimate = estimate_integral_2(samples_per_processor);
    }

    // Reduce all local estimates to the root processor (rank 0)
    double global_estimate = 0.0;
    MPI_Reduce(&local_estimate, &global_estimate, 1, MPI_DOUBLE, MPI_SUM, 0,
               MPI_COMM_WORLD);

    // The root processor prints the result
    if (rank == 0)
    {
        global_estimate /= numtasks; // Average the results from all processors
        if (P == 1)
        {
            std::cout << "Estimated value of integral 1: " << global_estimate
                      << std::endl;
            std::cout << "Expected value: " << 1.0 / 3.0 << std::endl;
        }
        else if (P == 2)
        {
            std::cout << "Estimated value of integral 2: " << global_estimate
                      << std::endl;
            std::cout << "Expected value: 0.7468" << std::endl;
        }
    }

    // Finalize MPI
    MPI_Finalize();

    return 0;
}
