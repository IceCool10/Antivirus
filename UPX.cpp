#include "UPX.h"
#include <string>
#include <iostream>
using namespace std;

unsigned UPX::hashStr1(const string& s) {
    long long ret = 0;
    for (int i = 0; i < s.size(); i++)
    {
        ret = ret*PRIME_BASE + s[i];
        ret %= PRIME_MOD;
    }
    return ret;
}

unsigned UPX::hashStr2(const string& s)
{
    long long ret = 0;
    for (int i = 0; i < s.size(); i++)
    {
        ret = ret*PRIME_BASE_2 + s[i];
        ret %= PRIME_MOD_2;
    }
    return ret;
}

int UPX::rollingHash(const string& string_to_search, const string& destination_string)
{
    long long hash1_1 = hashStr1(string_to_search);
    long long hash2_1 = 0;

    long long hash1_2 = hashStr2(string_to_search);
    long long hash2_2 = 0;

    long long power = 1;
    long long power2 = 1;
    for (int i = 0; i < string_to_search.size(); i++)
        power = (power * PRIME_BASE) % PRIME_MOD;
    for (int i = 0; i < string_to_search.size(); i++)
        power2 = (power2 * PRIME_BASE_2) % PRIME_MOD_2;

    for (int i = 0; i < destination_string.size(); i++)
    {
        hash2_1 = hash2_1*PRIME_BASE + destination_string[i];
        hash2_1 %= PRIME_MOD;

        if (i >= string_to_search.size())
        {
            hash2_1 -= power * destination_string[i-string_to_search.size()] % PRIME_MOD;
            if (hash2_1 < 0)
                hash2_1 += PRIME_MOD;
        }

        hash2_2 = hash2_2*PRIME_BASE_2 + destination_string[i];
        hash2_2 %= PRIME_MOD_2;

        if (i >= string_to_search.size())
        {
            hash2_2 -= power2 * destination_string[i-string_to_search.size()] % PRIME_MOD_2;
            if (hash2_2 < 0)
                hash2_2 += PRIME_MOD_2;
        }

        if (i >= string_to_search.size()-1 && hash1_1 == hash2_1 && hash1_2 == hash2_2)
            return i - (string_to_search.size()-1);
    }

    return -1;
}

bool UPX::isUPXFile(char* file) {

    system((string("upx -d -o ") + string(file) + string(".unpacked ") + string(file) + string(" > /dev/null 2>&1")).c_str());
    if (FILE *f = fopen((string(file)+".unpacked").c_str(), "r")) {
        fclose(f);
        
        return true;
    }
    else {
        //cout << "UPX not found!\n";
        return false;
    }
}
