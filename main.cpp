#include <iostream>
#include <string.h>
#include "Parser.h"
#include <string>
using namespace std;

int main(int argc, char** argv) {

    string banner    =  "===========================\n";
    banner          +=  "= Cosmin's Virus Detector =\n";
    banner          +=  "===========================\n";
    banner          +=  "\nUsage: ./antiv [ OPTION ] [ FILE ]\n\n";
    banner          +=  "\tOPTIONS :\n\n";
    banner          +=  "\t\tscan\t\tScan File\n";
    banner          +=  "\t\tsign\t\tSign File\n";   
    banner          +=  "\t\tscanfolder\tScan Folder\n";   
    Parser p;
    if(argc < 2) {
        cout << banner;
    }
    else {
        if(!strcmp(argv[1],"-h") || !strcmp(argv[1],"--help") || !strcmp(argv[1],"help")) {
            cout << banner;
        }
        else if (!strcmp(argv[1],"scan")){
    //    p.signFile("../tests/elf");
    //    p.signFile("../../virus_samples/wanncry.exe");
    //    p.scanFile("../../virus_samples/wanncry.exe");

    //    p.signFile("viruses/cerber.exe");
    //    p.scanFile("viruses/cerber.exe");

    //    p.searchHexSignature("viruses/131.exe");
    //    p.searchHexSignature("viruses/wanncry.exe");
        //p.signFile("viruses/cerber_upxed.exe");

        //p.scanFile("viruses/cerber_upxed.exe");
           p.scanFile(argv[2]); 
           p.searchHexSignature(argv[2]);
        }
        else if (!strcmp(argv[1],"sign")) {
            p.signFile(argv[2]);
        }
        else if (!strcmp(argv[1],"scanfolder")) {
            p.scanFolder(argv[2]);
        }
    }
    return 0;
}
