/*
   компиляция: mpicc task3.c -o task3
   запуск: job3.sh: echo "1" | mpirun -np 2 task3
*/
#include <mpi.h>
#include <stdio.h>
#include <time.h>
#define SIZE 4

void fill_matrix(int m[SIZE][SIZE])
{
    printf("Введите элементы матрицы \n");
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            printf("M[%d][%d]= ", i, j);
            scanf("%d", &m[i][j]);
        }
    }
}

void generate_matrix(int m[SIZE][SIZE])
{
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            m[i][j] = rand() % 100;
        }
    }
}

void print_matrix(int m[SIZE][SIZE])
{
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
            printf("%d\t", m[i][j]);
        printf("\n");
    }
}

int main(int argc, char *argv[])
{
    int rank, nranks, from, to, i, j, k;
    int a[SIZE][SIZE], b[SIZE][SIZE], c[SIZE][SIZE];
    int tag = 1;
    char autoGen;

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nranks);

/* Чтобы использовать эти варианты MPI_Gather и MPI_Scatter,
 вводим, что SIZE должен делиться на количество процессоров */
    if (SIZE % nranks != 0)
    {
        if (rank == 0)
           printf("Размер матрицы должен делиться на количество процессоров\n");
           MPI_Finalize();
           exit(-1);
    }
    if (rank == 0)
    {
        printf("Задайте матрицы: сгенерировать - 1, иначе - заполнить вручную ");
        scanf(" %c", &autoGen);
        if (autoGen == '1')
        {
            generate_matrix(a);
            generate_matrix(b);
        }
        else
        {
            fill_matrix(a);
            fill_matrix(b);
        }
    }

    from = rank * SIZE / nranks;
    to = (rank + 1) * SIZE / nranks;
    
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Bcast(b, SIZE * SIZE, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(a, SIZE * SIZE / nranks, MPI_INT, a[from], SIZE * SIZE / nranks, MPI_INT, 0, MPI_COMM_WORLD);

    for (i = from; i < to; i++)
        for (j = 0; j < SIZE; j++)
        {
            c[i][j] = 0;
            for (k = 0; k < SIZE; k++)
                c[i][j] += a[i][k] * b[k][j];
        }

    MPI_Gather(c[from], SIZE * SIZE / nranks, MPI_INT, c, SIZE * SIZE / nranks, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank == 0)
    {
        printf("Произведение матриц A и B: \n");
        printf("A: \n");
        print_matrix(a);
        printf("B: \n");
        print_matrix(b);
        printf("C: \n");
        print_matrix(c);
    }
    MPI_Finalize();
    return 0;
}