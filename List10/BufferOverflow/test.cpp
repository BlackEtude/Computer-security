//
// Created by pandemic on 06.01.18.
//
#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <fstream>
#include <iostream>

int main(int argc, char* argv[]) {
    char buff[15];
    int pass = 0;

    printf("\n Enter the password : \n");
    gets(buff);

    if(strcmp(buff, argv[1])) {
        std::cout << "Wrong password.\nAccess denied." << std::endl;
    }
    else {
        std::cout << "Correct password." << std::endl;
        pass = 1;
    }

    if(pass) {
        std::cout << "Admin privileges given to the user" << std::endl;
        system("./bserver sign");
    }
}

