#include <iostream>
#include "BigNumbers.h"

using namespace std;

int main()
{
    string sA, sB;
    BigNumber nSum, nDiff;

    if(!ReadFile("in.txt", &sA, &sB))
    {
        cout << "Can't open file\n";
        return(-1);
    }
    cout << sA << " | " << sB << '\n';

    Add(&sA, &sB, &nSum);
    Substract(&sA, &sB, &nDiff);

    cout << "Test\n" << BigNumberToString(&nSum) << '\n' << BigNumberToString(&nDiff) << '\n';

    if(!WriteFile("out.txt", &nSum, &nDiff)) {
        cout << "Can't write file\n";
    }
    return 0;
}
