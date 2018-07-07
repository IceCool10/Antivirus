#include <iostream>
#include <string>
using namespace std;

class UPX {

    private:
        const unsigned PRIME_BASE;
        const unsigned PRIME_MOD;

        const unsigned PRIME_BASE_2;
        const unsigned PRIME_MOD_2;

    public:
        UPX():PRIME_BASE(257),PRIME_BASE_2(283),PRIME_MOD(1000000007),PRIME_MOD_2(179426549) {};
        unsigned    hashStr1(const string& s);
        unsigned    hashStr2(const string& s);
        int         rollingHash(const string& string_to_search, const string& destination_string);
        bool        isUPXFile(char* file);
};
