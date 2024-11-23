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
            P = std::stoi(argv[i + 1]);
        }
        else if (std::string(argv[i]) == "-N")
        {
            N = std::stoi(argv[i + 1]);
        }
    }

    cout << "P = " << P << ". N = " << N << "." << endl;

    int numtasks, rank, rc;

    // rc = MPI_Init(&argc, &argv);

    // if (rc != MPI_SUCCESS)
    // {
    //     printf("Error starting MPI program. Terminating.\n");
    //     MPI_Abort(MPI_COMM_WORLD, rc);
    // }

    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    printf("Number of tasks= %d My rank= %d\n", numtasks, rank);

    return 0;
}
