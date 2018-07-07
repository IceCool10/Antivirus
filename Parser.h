#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <elf.h>

#include "Database.h"
#include "Crypter.h"
#include "UPX.h"

using namespace std;

class Parser {
    
    private:
        Database    *db;
        Crypter     *crypt;
        UPX         *upx;
        char cwd[1024];

	public:
		Parser();
        void 		read_elf_header(int32_t fd, Elf32_Ehdr *elf_header);
		bool 		is_ELF(Elf32_Ehdr eh);
        bool        is_PE(char* fd);
		Elf64_Addr 	get_entry_point(Elf32_Ehdr elf_header);
		//Needs to be changed	--->	full path for file
		void 		signFile(char* file);
		void        insertSignature(string signature);
        bool        checkSignature(const char* file);
        bool        scanFile(char* file);
        void        signing(const char* file);
        bool        searchHexSignature(char* file);

        bool        isDir(const char *name);
        void        scanFolder(const char *name);

        ~Parser();
};
