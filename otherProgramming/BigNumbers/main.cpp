#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

using namespace std;


struct bigNumber
{
    int dotAt;
    vector<int> integer;
    vector<int> real;
};


string clear(string* s)
{
    string l_s = *s;
    int i = 0, j = l_s.size(); j--;
    while (l_s[i] == '0' && l_s[i+1] != '.') i++;
    while (l_s[j] == '0' && l_s[j-1] != '.') j--;

    return l_s.substr(i, j - i + 1);
}


void fillBigNumber(string* s, bigNumber* N) {
    int dotPos = s->find('.');
    int t = 0;
    string tempString;
    bool writeToInteger = false;
    stringstream sstr;
    sstr << oct;

    if(dotPos % 4 != 0) {
        s->insert(0, 4 - dotPos % 4, '0');
        dotPos += 4 - dotPos % 4;
    }

    if(s->length() - dotPos - 1 % 4 != 0) {
        int t = (s->length() - dotPos - 1);
        for(int i = 0; i < 4 - t % 4; i++) {
            s->push_back('0');
        }
    }
    for(int i = s->length(); i > 0; i -= 4) {
        tempString = s->substr(i - 4, 4);
        if(tempString[3] == '.') {
            i += 3;
            writeToInteger = true;
            continue;
        }
        sstr << tempString;
        sstr >> oct >> t;
        sstr.clear();
        if(writeToInteger)
            N->integer.push_back(t);
        else
            N->real.push_back(t);
    }
}


int compare(bigNumber* A, bigNumber* B) {
    if(A->dotAt > B->dotAt) return 2;
    if(A->dotAt < B->dotAt) return 0;
    //integer parts are equal length
    for(int i = A->integer.size() - 1; i >= 0; i--) {
        if(A->integer[i] > B->integer[i]) return 2;
        if(A->integer[i] < B->integer[i]) return 0;
    }
    for(int i = A->real.size() - 1; i >= 0; i--) {
        if(A->real[i] > B->real[i]) return 2;
        if(A->real[i] < B->real[i]) return 0;
    }
    return 1;
}


bool sanitize(string* s)
{
    bool dotDetected = false;
    for(int i = 0; i < s->length(); i++) {
        if(s->c_str()[i] > '7' or s->c_str()[i] < '0') {
            if(s->c_str()[i] == '.') {
                if(dotDetected) return false;
                dotDetected = true;
                if(i+1 == s->length()) return false;
                if(s->c_str()[i+1] > '7' or s->c_str()[i+1] < '0') return false;
            }
            else {
                return false;
            }
        }
    }
    if(dotDetected == false) return false;
    return true;
}


void doAlign(bigNumber* A, bigNumber* B)
{
    //integer
    if(A->integer.size() < B->integer.size()) {
        for(int i = 0; i < B->integer.size() - A->integer.size(); i++)
            A->integer.push_back(0);
    }
    else if (A->integer.size() < B->integer.size()) {
        for(int i = 0; i < A->integer.size() - B->integer.size(); i++)
            B->integer.push_back(0);
    }
    //real
    if(A->real.size() < B->real.size()) {
        A->real.insert(A->real.begin(), B->real.size() - A->real.size(), 0);
    }
    else if (B->real.size() < A->real.size()) {
        B->real.insert(B->real.begin(), A->real.size() - B->real.size(), 0);
    }
}

void printBigNumber(bigNumber* B)
{
    for(int i = B->integer.size() - 1; i >= 0; i--)
        cout << oct << B->integer[i];
    cout << '.';
    for(int i = B->real.size() - 1; i >= 0; i--)
        cout << setfill('0') << setw(4) << oct <<  B->real[i];
    cout << '\n';
}


void add(bigNumber* A, bigNumber* B)
{
    bigNumber* result = new bigNumber;
    //real parts first
    for(int i = 0; i < A->real.size(); i++)
    {
        result->real.push_back(A->real.size());
    }
    //then integer
    for(int i = 0; i < A->integer.size(); i++)
    {
        result->integer.push_back(A->integer[i]);
    }
    result->integer.push_back(0);

    int carry = 0;

    for(int i = 0; i < A->real.size(); i++)
    {
        result->real[i] = ((A->real[i] + B->real[i]) & 0xFFF) + carry;
        carry = (A->real[i] + B->real[i]) >> 12;
    }

    for(int i = 0; i < A->integer.size(); i++)
    {
        result->integer[i] = ((A->integer[i] + B->integer[i]) & 0xFFF) + carry;
        carry = (A->integer[i] + B->integer[i]) >> 12;
    }

    printBigNumber(result);
}

void substract(bigNumber* A, bigNumber* B)
{
    bigNumber* result = new bigNumber;
    //real parts first
    for(int i = 0; i < A->real.size(); i++)
    {
        result->real.push_back(A->real.size());
    }
    //then integer
    for(int i = 0; i < A->integer.size(); i++)
    {
        result->integer.push_back(A->integer[i]);
    }

    int carry = 0;

    for(int i = 0; i < A->real.size(); i++)
    {
        result->real[i] = (A->real[i] - B->real[i]);
        if(result->real[i] < 0) {
            result->real[i] += 0x1000;
            if(i < A->real.size() - 1) {
                result->real[i+1]--;
            }
            else
                A->integer[0]--;
        }
    }

    for(int i = 0; i < A->integer.size(); i++)
    {
        result->integer[i] = (A->integer[i] - B->integer[i]);
        if(result->integer[i] < 0) {
            result->integer[i] += 0x1000;
            result->integer[i+1]--;
        }
    }

    printBigNumber(result);
}

int main()
{
    string sA, sB, sSum, sDiff;
    bigNumber *A = new bigNumber;
    bigNumber *B = new bigNumber;
    bigNumber *T;

    ifstream in("in.txt");
    if (in.is_open()) {
        getline(in, sA);
        getline(in, sB);
    }

    if(!sanitize(&sA) or !sanitize(&sB)) {
        cout << "Bad input. Quitting...\n";
        return -1;
    }

    sA = clear(&sA);
    sB = clear(&sB);

    fillBigNumber(&sA, A);
    fillBigNumber(&sB, B);

    doAlign(A, B);

    printBigNumber(A);
    printBigNumber(B);


    //cout << sA << " || " << sB << '\n';

    if(sA.compare(sB) == 0) cout << "The numbers are equal, by the way\n";


    if(compare(A, B) == 0) {
        cout << "A is less than B!\n";
        T = A;
        A = B;
        B = T;
    }

    add(A, B);
    substract(A, B);

    return 0;
}
