#include <cstdlib>
#include <stdio.h>
#include <omp.h>
#include <iostream>
#include <ctime>
using namespace std;

double **ProcessInitialization(int l, int m)
{
    double **a;
    a = new double *[l];
    cout << "Введите элементы матрицы" << endl;

    for (int i = 0; i < l; i++)
    {
        a[i] = new double[m];
        for (int j = 0; j < m; j++)
        {
            cout << "a[" << i << "][" << j << "]= ";
            cin >> a[i][j];
        }
    }

    return a;
}


//генерация случайных чисел
double **ProcessInitializationT(int l, int m)
{
    srand(time(NULL));
    double **a;
    a = new double *[l];
    for (int i = 0; i < l; i++)
    {
        a[i] = new double[m];
        for (int j = 0; j < m; j++)
        {
            a[i][j] = rand() % 10 + 1;
        }
    }

    for (int i = 0; i < l; i++)
    {
        for (int j = 0; j < m; j++)
        {
            cout << a[i][j] << "\t";
        }
        cout << endl;
    }
    return a;
}

int main()
{
    int rowA, rowB, colA, colB;
    double **a, **b, **c;
    system("chcp 1251");
    system("cls");

    cout << "Введите количество строк A: ";
    cin >> rowA;
    cout << "Введите количество столбцов A: ";
    cin >> colA;
    cout << "Введите количество строк B: ";
    cin >> rowB;
    cout << "Введите количество столбцов B: ";
    cin >> colB;

    if (colA != rowB)
    {
        cout << "Умножение невозможно";
        cin.get();
        cin.get();
        return 0;
    }

    int W;
    cout << "Задайте матрицы: вручную - 1, рандомно - 2:";
    cin >> W;

    if (W == 1)
    {
        a = ProcessInitialization(rowA, colA);
        b = ProcessInitialization(rowB, colB);
    }
    else
    {
        a = ProcessInitializationT(rowA, colA);
        b = ProcessInitializationT(rowB, colB);
    }
    
    int i, j, k;

    c = new double *[rowA];
    for (int i = 0; i < rowA; ++i)
    {
        c[i] = new double[colB];
    }

    
#pragma omp parallel for collapse(2) schedule(static) shared(a, b)

    for (int i = 0; i < rowB; i++)
    {
        for (int j = 0; j < colB; j++)
        {
            for (int k = 0; k < colA; k++)
            {
#pragma omp critical
                {
                    c[i][j] += a[i][k] * b[k][j];
                }
            }
        }
    }

    cout << "Произведение матриц" << endl;
    for (int i = 0; i < rowA; i++)
    {
        for (int j = 0; j < colB; j++)
            cout << c[i][j] << " ";
        cout << endl;
    }
    return 0;
}




//// первый варик

    c = new double *[rowA];
    
//Shared описывает переменные, существующие в одном экземпляре и доступные всем потокам
//Private описывает переменные, существующие для каждого потока отдельно, каждый поток>
#pragma omp parallel for shared(a, b, c) private(i, j, k) 

    for (int i = 0; i < rowB; i++)
    {
        c[i] = new double[colB];
        for (int j = 0; j < colB; j++)
        {
            c[i][j] = 0;
            for (int k = 0; k < colA; k++)
                c[i][j] += a[i][k] * b[k][j];
        }
    }
    
    cout << "Произведение матриц" << endl;
    for (int i = 0; i < rowA; i++)
    {
        for (int j = 0; j < colB; j++)
            cout << c[i][j] << " ";
        cout << endl;
    }
    return 0;
} 