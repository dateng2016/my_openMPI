#include "mpi.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

int main(int argc, char* argv[])
{
    int P = 1, N = 1000000;
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

    return 0;
}
