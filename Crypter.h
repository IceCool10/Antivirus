#include <stdio.h>
#include <sstream>
#include <iostream>
#include <string.h>
#include <stdint.h>
using namespace std;

#define LEFTROTATE(x, c) (((x) << (c)) | ((x) >> (32 - (c))))

class Crypter {

    private:
        uint32_t h0, h1, h2, h3;

    public:
        Crypter();
        void    md5(uint8_t *initial_msg, size_t initial_len);
        string  print_md5(char* msg, size_t length);
};
