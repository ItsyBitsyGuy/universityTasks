#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
using namespace std;

struct cosher_string
{
    char* block;
    int spaces;
};

int iColumns;
int iSize;
char* cContent, cFinal;
vector <cosher_string> strings;

char delims [] = {' ', '\0'};

int col_width = 0;
int col_number = 0;
int col_space =  4;

char* strrchr_c (char* str)
{
    char* max = 0x1;
    char temp;
    temp = *(str + col_width - 1);
    *(str + col_width - 1) = '\0';

    for (int i = 0; i < sizeof (delims) - 1; i++)
    {
        char* t = strrchr (str, delims [i]);
        if (t > max) max = t;
    }

    *(str + col_width - 1) = temp;

    return max;
}

char* strchr_c (char* str)
{
    char* min = 0x7fffffff;

    for (int i = 0; i < sizeof (delims) - 1; i++)
    {
        char* t = strchr (str, delims [i]);
        if (t < min && t != 0) min = t;
    }

    return min;
}

void do_init(char* block)
{
    block [col_width] = '\0';
    for (int i = 0; i < col_width; i++) block [i] = ' ';
}

int main(int argc, char** argv) {
	if (argc < 4) {cout << "NewspapeIt.exe %input %column_width %#of_columns"; return -1;}

    ifstream fFile (argv [1]);
    if (fFile.is_open())
    {
        std::string sContent((std::istreambuf_iterator<char>(fFile)), std::istreambuf_iterator<char>());
        iSize = sContent.size();
        cContent = new char [iSize];
        sContent.copy(cContent, iSize, 0);
        fFile.close();
    }
    else {cout << "Can't open file ya gimme\n"; return (-1);}

    col_width = atoi (argv [2]);//fixit
    col_number = atoi (argv [3]);

    if (col_width <= 0 || col_number <= 0)
    {
        exit(-1);
    }

    for (int i = 0; i < iSize; i++)
    {
        if (cContent [i] == '\n') cContent [i] = ' ';
    }

    bool eof = false;
    bool last_space = false;
    char* it = cContent;
    cosher_string* block;


    while (!eof)
    {
        last_space = false;
        if (it + col_width >= cContent + iSize)
        {
            block = new cosher_string;
            block->block = new char [col_width + 1];
            do_init(block->block);
            memcpy(block->block, it, cContent + iSize - it);
            block->spaces = col_width - (cContent + iSize - it);
            strings.push_back(*block);
            eof = true;
        }
        else
        {
            char* min = strchr_c (it + col_width - 1);
            char* max = strrchr_c (it);

            if (max == 1 && min > it + col_width)
            {
                cout << "Too big\n";
                return (-1);
            }

            if (max == 1) max = it - 1;

            //can we fit it?
            if (min - it - last_space <= col_width)
            {
                //yeah, sure
                block = new cosher_string;
                block->block = new char [col_width + 1];
                do_init(block->block);
                memcpy(block->block, it, min - it);
                block->spaces = col_width + it - min;
                strings.push_back(*block);
                it += (min - it) + 1;
            }
            else
            {
                //nope -_- let's cut a piece from string
                block = new cosher_string;
                block->block = new char [col_width + 1];
                do_init(block->block);
                memcpy(block->block, it, max - it);
                block->spaces = col_width + it - max;
                strings.push_back(*block);
                it += (max - it) + 1;
            }
        }
    }

    for (int i = 0; i < strings.size(); i++)
    {
        int spaces = 0;
        int offset = 0;
        string sBlock;

        //let's find number of words in the line
        for (int j = 0; j < col_width; j++)
        {
            if (isspace (strings [i].block [j])) spaces++;
        }
        spaces -= strings [i].spaces; //got the real num of spaces;

        if (spaces == 0 && strings [i].block [col_width - 1] != ' ') { //the whole word fits column size
            continue;
        }

        if (spaces == 1) {
            sBlock = strings [i].block;
            string second_word = sBlock.substr(sBlock.find(' ', 0) + 1,
                                sBlock.find (' ', sBlock.find(' ', 0) + 1) - sBlock.find(' ', 0) - 1);
            for (int i = sBlock.find (' ', 0); i < sBlock.size(); i++)
            {
                sBlock [i] = ' ';
            }
            sBlock.insert(sBlock.size() - second_word.size(), second_word);
            memcpy (strings [i].block, sBlock.c_str(), sBlock.size());
            strings [i].block [col_width] = '\0';
            continue;
        }

        if (spaces == 0 && isspace (strings [i].block [col_width - 1]))//just one word in the very start
        {
            sBlock = strings [i].block;
            string word = sBlock.substr (0, sBlock.find(' ', 0));
            for (int j = 0; j < col_width; j++) {sBlock [j] = ' ';}
            sBlock.insert (sBlock.size () - word.size(), word);
            memcpy (strings [i].block, sBlock.c_str(), sBlock.size());
            strings [i].block [col_width] = '\0';
        }
        else //we have multiple words in string with space (s) at final
        {
            sBlock = strings [i].block;
            string typical = "";
            string last = "";
            offset = 0;

            for (int j = 0; j < strings [i].spaces / spaces; j++) typical += " ";
            last += typical;
            for (int j = 0; j < strings [i].spaces % spaces; j++) last += " ";

            //if (typical == "") continue;

            for (int i = 0; i < spaces; i++)
            {
                if (i == spaces - 1) {
                    sBlock.insert (sBlock.find (' ', offset + typical.size()), last);
                    break;
                }
                sBlock.insert (sBlock.find(' ', offset), typical);
                offset = sBlock.find(' ', offset + typical.size()) + typical.size ();
            }
            memcpy (strings [i].block, sBlock.c_str(), sBlock.size());
            strings [i].block [col_width] = '\0';
        }
        strings [i].block [col_width] = '\0';
    }

    ofstream fOut ("out.txt");


    for (int row = 0; row < strings.size() / col_number + strings.size() % col_number; row++)
    {
        string sString = "";

        for (int col = 0; col < col_number; col++)
        {
          if (row + col * (strings.size() / col_number + strings.size() % col_number) < strings.size())
          {
                sString += strings [row + col * (strings.size() / col_number + strings.size() % col_number)].block;
                sString += "    ";
          }
        }
        fOut << sString << '\n';
    }
    cout << "Newspaped it\n";

    fOut.close();

    return 0;
}
