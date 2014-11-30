#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct bigNumber
{
    int dotAt;
    vector<short int> integer;
};

string clear(string* s)
{
    string l_s = *s;
    int i = 0, j = l_s.size(); j--;
    while (l_s[i] == '0' && l_s[i+1] != '.') i++;
    while (l_s[j] == '0' && l_s[j-1] != '.') j--;

    return l_s.substr(i, j - i + 1);
}


void fillBigNumber(string* s, bigNumber* N)
{
    for(int i = s->length() - 1; i >= 0; i--)
    {
        if(s->c_str()[i] != '.')
        {
            N->integer.push_back((short int)(s->c_str()[i]) - 0x30);
        }
        else N->dotAt = i;
    }
}


void doAlign(bigNumber* A, bigNumber* B)
{
    //integer parts firstly
    int addToA = 0;
    int addToB = 0;

    if (A->dotAt != B->dotAt)
    {
        B->integer.insert(B->integer.end(), A->dotAt - B->dotAt, (short int)0);
        addToB = A->dotAt - B->dotAt;
    }

    if(A->integer.size() - A->dotAt != B->integer.size() + addToB - B->dotAt - addToB)
    {
        if(A->integer.size() - A->dotAt < B->integer.size() - B->dotAt - addToB)
        {
            A->integer.insert(A->integer.begin(), (B->integer.size() - B->dotAt - addToB) - (A->integer.size() - A->dotAt), (short int)0);
        }
        else
        {
            B->integer.insert(B->integer.begin(), (A->integer.size() - A->dotAt) - (B->integer.size() - addToB - B->dotAt), (short int)0);
        }
    }
}


void sum(bigNumber* A, bigNumber* B, string* sResult)
{
    bigNumber* result = new bigNumber;
    result->integer.insert(result->integer.begin(), A->integer.size() + 1, 0);
    short int t = 0;

    for(int i = 0; i < A->integer.size(); i++)result->integer[i] = A->integer[i];

    for(int i = 0; i < A->integer.size(); i++)
    {
        result->integer[i] += B->integer[i];
        result->integer[i+1] += result->integer[i] / 10;
        result->integer[i] %= 10;
    }
    result->dotAt = A->dotAt + 1;

    for(int i = result->integer.size() - 1; i >= 0; i--)
    {
        if(i == result->integer.size() - 1 - result->dotAt) sResult->push_back('.');
        sResult->push_back(result->integer[i] + 0x30);
    }

}


void diffirence(bigNumber* A, bigNumber* B, string* sResult)
{
    bigNumber* result = new bigNumber;
    result->integer.insert(result->integer.begin(), A->integer.size(), 0);
    for(int i = 0; i < A->integer.size(); i++) result->integer[i] = A->integer[i];

    for(int i = 0; i < A->integer.size(); i++)
    {
        result->integer[i] = result->integer[i] - B->integer[i];
        if(result->integer[i] < 0) {
            result->integer[i] += 10;
            result->integer[i+1] -= 1;
        }
    }
    result->dotAt = A->dotAt;

    for(int i = result->integer.size() - 1; i >= 0; i--)
    {
        if(i == result->integer.size() - 1 - result->dotAt) sResult->push_back('.');
        sResult->push_back(result->integer[i] + 0x30);
    }
}

int compare(bigNumber* A, bigNumber* B)
{
    if(A->dotAt > B->dotAt) return 2;
    if(A->dotAt < B->dotAt) return 0;
    //integer parts are equal length
    for(int i = A->integer.size() - 1; i >= 0; i--)
    {
        if(A->integer[i] > B->integer[i]) return 2;
        if(A->integer[i] < B->integer[i]) return 0;
    }
    return 1;
}

int main()
{
    string sA, sB, sSum, sDiff;
    bigNumber *A = new bigNumber;
    bigNumber *B = new bigNumber;
    bigNumber *T;

    ifstream in("in.txt");
    if (in.is_open())
    {
        getline(in, sA);
        getline(in, sB);
    }

    sA = clear(&sA);
    sB = clear(&sB);

    cout << sA << " || " << sB << '\n';

    if(sA.compare(sB) == 0) cout << "The numbers are equal, by the way\n";

    fillBigNumber(&sA, A);
    fillBigNumber(&sB, B);

    doAlign(A, B);

    if(compare(A, B) == 0)
    {
        cout << "A is less than B!\n";
        T = A;
        A = B;
        B = T;
    }

    sum(A, B, &sSum);

    diffirence(A, B, &sDiff);

    sSum = clear(&sSum);
    sDiff = clear(&sDiff);

    cout << '\n' << sSum << " : " << sDiff << '\n';

    return 0;
}
