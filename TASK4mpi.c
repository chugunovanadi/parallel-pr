/*
   компиляция: mpicc task4.c -o task4
   запуск: job4.sh: printf  "12\n1" | mpirun -np 2  task4
*/
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

void swap(int *v, int i, int j)
{
    int t = v[i];
    v[i] = v[j];
    v[j] = t;
}
/*
 Сортировка фрагмента массива v, половина начинается с s и имеет длину n,
 nвыбираем pivot и меняем местами с первым элементом, применяем рекурсивно к обеим частям.
*/
void quicksort(int *v, int s, int n)
{
    int pivot, j, i;
    if (n <= 1)
        return;
    pivot = v[s + n / 2];
    swap(v, s, s + n / 2);
    j = s;
    for (i = s + 1; i < s + n; i++)
        if (v[i] < pivot)
        {
            j++;
            swap(v, i, j);
        }
    swap(v, s, j);
    quicksort(v, s, j - s);
    quicksort(v, j + 1, s + n - j - 1);
}
// Объединение двух отсортированных массивов v1, v2
int *merge(int *v1, int n1, int *v2, int n2)
{
    int *rez = (int *)malloc((n1 + n2) * sizeof(int));
    int i = 0;
    int j = 0;
    int k;
    for (k = 0; k < n1 + n2; k++)
    {
        if (i >= n1)
        {
            rez[k] = v2[j];
            j++;
        }
        else if (j >= n2)
        {
            rez[k] = v1[i];
            i++;
        }
        else if (v1[i] < v2[j])
        {
            rez[k] = v1[i];
            i++;
        }
        else
        {
            rez[k] = v2[j];
            j++;
        }
    }
    return rez;
}
void printArray(int arr[], int size)
{
    int i;
    for (i = 0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

int main(int argc, char **argv)
{
    int n, i=0, nranks, rank, *arr, *firstPart, *secondPart, step, o, c, s;
    char autoGen;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nranks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0)
    {
        printf("Введите длину массива: ");
        scanf("%d", &n);
        c = (n % nranks != 0) ? n / nranks + 1 : n / nranks;
        arr = (int *)malloc(nranks * c * sizeof(int));
        printf("Задайте массив: сгенерировать - 1, иначе - введите вручную  ");
        scanf(" %c", &autoGen);
        if (autoGen == '1')
        {
            for (int i = 0; i < n; i++)
            {
                arr[i] = rand() % 100;
            }
            printf("Сгенерированный массив: ");
            printArray(arr, n);
        }
        else
        {
            for (int i = 0; i < n; i++)
            {
                printf("arr[%d]= ", i);
                scanf("%d", &arr[i]);
            }
      
        }
            
    }
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    c = (n % nranks != 0) ? n / nranks + 1 : n / nranks;
    firstPart = (int *)malloc(c * sizeof(int));
    // Распространение данных
    MPI_Scatter(arr, c, MPI_INT, firstPart, c, MPI_INT, 0, MPI_COMM_WORLD);
    free(arr);
    arr = NULL;
    // вычисление размера первой части и ее сортировка
    s = (n >= c * (rank + 1)) ? c : n - c * rank;
    quicksort(firstPart, 0, s);
    for (step = 1; step < nranks; step = 2 * step)
    {
        // отправить часть на rank-step и выйти из цикла
        if (rank % (2 * step) != 0)
        {
            MPI_Send(firstPart, s, MPI_INT, rank - step, 0, MPI_COMM_WORLD);
            break;
        }
        // получить другую часть, объединить части и освободить память
        if (rank + step < nranks)
        { 
            //размер другой принимаемой части
            o = (n >= c * (rank + 2 * step)) ? c * step : n - c * (rank + step);
            secondPart = (int *)malloc(o * sizeof(int));
            MPI_Recv(secondPart, o, MPI_INT, rank + step, 0, MPI_COMM_WORLD, &status);
            arr = merge(firstPart, s, secondPart, o);
            free(firstPart);
            free(secondPart);
            firstPart = arr;
            s = s + o;
        }
    }
    if (rank == 0)
    {
        printf("Отсортированный массив: ");
        printArray(arr, n);
    }
    MPI_Finalize();
    return 0;
}