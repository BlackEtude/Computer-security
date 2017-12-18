#!/bin/bash

g++ -std=c++11 bserver.cpp -o bserver -lssl -lcrypto
g++ -std=c++11 bclient.cpp -o bclient -lssl -lcrypto
g++ -std=c++11 sign_test.cpp -o test -lssl -lcrypto