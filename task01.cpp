#include <iostream>
#include <iomanip>
#include <stdio.h>
using namespace std;
int main()
{
    long double PI, x, sum = 0, rez, denomiN;
    int i;
    int n;
    cout << "Введите N: \n";
    cin >> n;
    denomiN = 1 / ((double)n-1);
    for (int i = 0; i < n-1; i++)
    {
        x = ((double)i + 0.5)*denomiN;
        PI = 4 / (1 + x * x);
        sum += PI;
    }
    rez = sum * denomiN;
    cout << "PI = " << setprecision(n)<< rez << endl;
}


