#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

using namespace std;

struct BigNumber {
    vector<int> integer;
    vector<int> real;
};


string Clear(string* s) {
    string l_s = *s;
    int i = 0, j = l_s.size(); j--;
    while (l_s[i] == '0' && l_s[i+1] != '.') i++;
    while (l_s[j] == '0' && l_s[j-1] != '.') j--;

    return l_s.substr(i, j - i + 1);
}


bool Sanitize(string* s) {
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


void FillBigNumber(string* s, BigNumber* N) {
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


bool ReadFile(string filename, string* number1, string* number2) {
    ifstream in(filename);
    if (in.is_open()) {
        getline(in, *number1);
        getline(in, *number2);
        if(!Sanitize(number1) or !Sanitize(number2)) {
            cout << "Bad input. Quitting...\n";
            return false;
        }
        *number1 = Clear(number1);
        *number2 = Clear(number2);
        return true;
    }
    return false;
}


void DoAlign(BigNumber* A, BigNumber* B) {
    //integer
    if(A->integer.size() < B->integer.size()) {
        for(int i = 0; i < B->integer.size() - A->integer.size(); i++)
            A->integer.push_back(0);
    }
    else if (B->integer.size() < A->integer.size()) {
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


string getZeroes(int num)
{
    stringstream t;
    string result;
    t << oct << num;
    for(int i = 0; i < 4 - t.str().length(); i++)
    {
        result += '0';
    }
    return result;
}


string BigNumberToString(BigNumber* A) {
    stringstream temp;
    for(int i = A->integer.size() - 1; i >= 0; i--) {
        temp << oct << getZeroes(A->integer[i]) << A->integer[i];
    }
    temp << '.';
    for(int i = A->real.size() - 1; i >= 0; i--)
        temp << getZeroes(A->real[i]) << oct << A->real[i];
    return Clear(&temp.str());
    //return temp.str();
}


bool WriteFile(string filename, BigNumber* number1, BigNumber* number2) {
    ofstream out(filename);
    if (out.is_open()) {
        out << BigNumberToString(number1) << '\n' << BigNumberToString(number2);
        return true;
    }
    return false;
}


int Compare(BigNumber* A, BigNumber* B) {
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


void Add(string* A, string* B, BigNumber* result) {
    BigNumber* nA = new BigNumber;
    BigNumber* nB = new BigNumber;
    BigNumber* nT;

    FillBigNumber(A, nA);
    FillBigNumber(B, nB);

    DoAlign(nA, nB);

    if(Compare(nA, nB) == 0) {
        nT = nA;
        nA = nB;
        nB = nT;
    }

    //cout << "HOLA\n" << BigNumberToString(nA) << '\n' << BigNumberToString(nB) << "\nHOLA\n";

    //real parts first
    for(int i = 0; i < nA->real.size(); i++)
    {
        result->real.push_back(nA->real.size());
    }
    //then integer
    for(int i = 0; i < nA->integer.size(); i++)
    {
        result->integer.push_back(nA->integer[i]);
    }
    result->integer.push_back(0);

    int carry = 0;

    for(int i = 0; i < nA->real.size(); i++)
    {
        result->real[i] = ((nA->real[i] + nB->real[i]) & 0xFFF) + carry;
        carry = (nA->real[i] + nB->real[i]) >> 12;
    }

    for(int i = 0; i < nA->integer.size(); i++)
    {
        result->integer[i] = ((nA->integer[i] + nB->integer[i]) & 0xFFF) + carry;
        carry = (nA->integer[i] + nB->integer[i]) >> 12;
    }
}


void Substract(string* A, string* B, BigNumber* result) {
    BigNumber* nA = new BigNumber;
    BigNumber* nB = new BigNumber;
    BigNumber* nT;

    FillBigNumber(A, nA);
    FillBigNumber(B, nB);

    DoAlign(nA, nB);

    if(Compare(nA, nB) == 0) {
        nT = nA;
        nA = nB;
        nB = nT;
    }

    //cout << "HOLA\n" << BigNumberToString(nA) << '\n' << BigNumberToString(nB) << "\nHOLA\n";

    //real parts first
    for(int i = 0; i < nA->real.size(); i++)
    {
        result->real.push_back(nA->real.size());
    }
    //then integer
    for(int i = 0; i < nA->integer.size(); i++)
    {
        result->integer.push_back(nA->integer[i]);
    }

    for(int i = 0; i < nA->real.size(); i++)
    {
        result->real[i] = (nA->real[i] - nB->real[i]);
        if(result->real[i] < 0) {
            result->real[i] += 0x1000;
            if(i < nA->real.size() - 1) {
                result->real[i+1]--;
            }
            else
                nA->integer[0]--;
        }
    }

    for(int i = 0; i < nA->integer.size(); i++)
    {
        result->integer[i] = (nA->integer[i] - nB->integer[i]);
        if(result->integer[i] < 0) {
            result->integer[i] += 0x1000;
            result->integer[i+1]--;
        }
    }
}


int Compare(string* A, string* B) {
    BigNumber* nA = new BigNumber;
    BigNumber* nB = new BigNumber;

    FillBigNumber(A, nA);
    FillBigNumber(B, nB);

    DoAlign(nA, nB);

    return Compare(nA, nB);
}
