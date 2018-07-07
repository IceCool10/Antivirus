#!/bin/bash
g++ -o antiv main.cpp Crypter.cpp Database.cpp Parser.cpp UPX.cpp -lmysqlcppconn
