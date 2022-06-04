#include <cstdlib>
#include <stdio.h>
#include <omp.h>
#include <iostream>
#include <ctime>
#include <fstream>
#include <bits/stdc++.h>
using namespace std;

int *ProcessInitialization(int l)
{
    int *a;
    a = new int[l];
    cout << "Введите элементы массива" << endl;
    for (int i = 0; i < l; i++)
    {
        cin >> a[i];
    }
    return a;
}

int *ProcessInitializationT(int l)
{
    srand(time(NULL));
    int *a;
    a = new int[l];
    for (int i = 0; i < l; i++)
    {
        a[i] = rand() % 100 + 1;
    }

    cout << "Сгенерированный массив: \n";
    for (int i = 0; i < l; i++)
    {
        cout << a[i] << " ";
    }
    return a;
}

void swap(int *a, int *b)
{
    int t = *a;
    *a = *b;
    *b = t;
}
// Разбиение массива
int partition(int arr[], int start, int end)
{
    int pivot = arr[end];
    int i = (start - 1);

// Переупорядочивание массива
    for (int j = start; j <= end - 1; j++)
    {
        if (arr[j] < pivot)
        {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[end]);

    return (i + 1);
}

void quicksort(int arr[], int start, int end)
{
    int index;

    if (start < end)
    {
        index = partition(arr, start, end);

//sections определяет разделы кода, которые должны быть разделены между всеми потоками
//каждый раздел выполняется один раз потоком в команде
#pragma omp parallel sections
        {
#pragma omp section
            {
                quicksort(arr, start, index - 1);
            }
#pragma omp section
            {
                quicksort(arr, index + 1, end);
            }
        }
    }
}

int main()
{
    int row1;
    int *a, *b;
    cout << "Укажите количество элементов массива: ";
    cin >> row1;

    int W;
    cout << "Задайте массив: вручную - 1, рандомно - 2: ";
    cin >> W;
    if (W == 1)
    {
        a = ProcessInitialization(row1);
    }
    else
    {
        a = ProcessInitializationT(row1);
    }
    quicksort(a, 0, row1 - 1);

    cout << "Результат сортивровки: \n";

    for (int i = 0; i < row1; i++)
    {
        cout << a[i] << " ";
    }
    return 0;
}
