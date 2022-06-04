#include <iostream>
using namespace std;
int main()
{
    double PI, x, sum = 0, rez, denomiN;
    int n;  
    int i;    

    cout << "Enter N: \n";
    cin >> n;

    denomiN = 1 / ((double)n-1);   
    #pragma omp parallel for num_threads(1) private(i)         
    for (int i = 0; i < n-1; i++) 
    {
        x = ((double)i + 0.5)*denomiN;
        PI = 4 / (1 + x * x);
        sum += PI;
    }

    rez = sum * denomiN;
    cout << "PI = " << rez << endl;
}