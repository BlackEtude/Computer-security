#!/bin/bash

g++ -std=c++11 reader.cpp -o serverstart
g++ -fno-stack-protector -std=c++11 test.cpp -o test
g++ -fno-stack-protector -std=c++11 bserver.cpp -o bserver -lssl -lcrypto
g++ -std=c++11 bclient.cpp -o bclient -lssl -lcrypto
