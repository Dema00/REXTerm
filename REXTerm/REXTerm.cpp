#include <stdio.h>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[]){

    string str = "./cp437 ./GondTerm";
    for(int i = 1; i<argc; i++)
    {
        str = str + " " + argv[i];
    }

    const char * command = str.c_str();
    system(command);

}
