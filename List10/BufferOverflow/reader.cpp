//
// Created by pandemic on 06.01.18.
//

#include <iostream>
#include <fstream>

int main(int argc, char*argv[]) {
    //sign port path/to/private/key and then ask for password

    std::string server_pass;
    std::ifstream myfile("server_pass");
    if (myfile.is_open()) {
        while (std::getline(myfile, server_pass));
        myfile.close();
    }

    std::string command = "./test " + server_pass;
    system(command.c_str());

    return 0;
}
