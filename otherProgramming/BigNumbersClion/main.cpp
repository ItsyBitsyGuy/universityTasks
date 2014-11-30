 #include <iostream>
#include <fstream>
#include <string>
using namespace std;

string clean(string* s)
{
    string l_s = *s;
    int i = 0, j = l_s.size(); j--;
    while (l_s[i] == '0' && l_s[i+1] != '.') i++;
    while (l_s[j] == '0' && l_s[j-1] != '.') j--;

    return l_s.substr(i, j - i + 1);
}


int main()
{
    string sA, sB;

    ifstream in("in.txt");
    if (in.is_open())
    {
        getline(in, sA);
        getline(in, sB);
    }

    cout << sA << " || " << sB << '\n';

    sA = clean(&sA);
    sB = clean(&sB);

    cout << sA << " || " << sB << '\n';

    return 0;
}