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
    while (l_s[j] == '0' && l_s[i-1] != '.') j--;

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
        if(A->dotAt < B->dotAt)
        {
            A->integer.insert(A->integer.end(), B->dotAt - A->dotAt, (short int)0);
            addToA = B->dotAt - A->dotAt;
        }
        else
        {
            B->integer.insert(B->integer.end(), A->dotAt - B->dotAt, (short int)0);
            addToB = A->dotAt - B->dotAt;
        }
    }

    if(A->integer.size() - addToA - A->dotAt != B->integer.size() - addToB - B->dotAt)
    {
        if(A->integer.size() - addToA - A->dotAt < B->integer.size() - B->dotAt)
        {
            A->integer.insert(A->integer.begin(), (B->integer.size() -addToB - B->dotAt) - (A->integer.size() -addToA - A->dotAt), (short int)0);
        }
        else
        {
            B->integer.insert(B->integer.begin(), (A->integer.size() - addToA - A->dotAt) - (B->integer.size() - addToB - B->dotAt), (short int)0);
        }
    }
}

bigNumber* sum(bigNumber* A, bigNumber* B)
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
    result->dotAt = A->dotAt; + bool(result->integer[result->integer.size() - 1]);
    cout << "Sum is ... ";

    for(int i = result->integer.size() - 1; i >= 0; i--)
    {
        cout << result->integer[i];
    }
    cout << '\n';
    return result;
}

bigNumber* diffirence(bigNumber* A, bigNumber* B)
{
    bigNumber* result = new bigNumber;
    result->integer.insert(result->integer.begin(), A->integer.size(), 0);
    for(int i = 0; i < A->integer.size(); i++) result->integer[i] = A->integer[i];

    for(int i = 0; i < A->integer.size(); i++)
    {
        if(result->integer[i] >= B->integer[i])
        {
            result->integer[i] = result->integer[i] - B->integer[i];
        }
        else
        {
            result->integer[i] = (result->integer[i] + 10 - B->integer[i]);
            result->integer[i+1]--;
            if(result->integer[i+1] < 0) result->integer[i+1] += 10;
        }
    }
    result->dotAt = A->dotAt + bool(result->integer[0]);
    cout << "Diff is ... ";
    for(int i = result->integer.size() - 1; i >= 0; i--)
    {
        cout << result->integer[i];
        //if(result->integer.size() + 1 - i == result->dotAt) cout << '.';
    }
    cout << '\n';
}


int main()
{
    string sA, sB;
    bigNumber *A = new bigNumber;
    bigNumber *B = new bigNumber;

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

    sum(A, B);

    diffirence(A, B);

    return 0;
}
