#include <iostream>
#include <mpi.h>
#include <iomanip>
#include <chrono>
using namespace std;
int main(int argc, char **argv)
{
    int i, n, size, rank;
    double x, pi, myPI, rez, denomiN, t1, t2;
    double sum = 0.0;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0)
    {
        n = 1000000;
        t1 = MPI_Wtime();
    }
    denomiN = 1 / ((double)n-1);
    for (i = rank; i < n-1; i += size)
    {
        x = ((double)i + 0.5)*denomiN;
        myPI = 4 / (1 + x * x);
        sum += myPI;
    }
    rez = sum * denomiN;

    MPI_Reduce(&rez, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    t2 = MPI_Wtime();
    if (rank == 0)
    {
        std::cout << "pi: " << std::fixed << std::setprecision(8) << pi << std::endl;
        std::cout << "time: " << t2 - t1 << std::endl;
    }
    MPI_Finalize();
    return 0;
}
