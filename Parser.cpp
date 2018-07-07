#include "Parser.h"
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <fcntl.h>
#include <sys/stat.h>
using namespace std;

Parser::Parser() {
    this->db    =   Database::getInstance();
    this->crypt =   new Crypter();
    this->upx   =   new UPX();
    getcwd(this->cwd,sizeof(this->cwd));
}

void Parser::read_elf_header(int32_t fd, Elf32_Ehdr *elf_header)
{
	assert(elf_header != NULL);
	assert(lseek(fd, (off_t)0, SEEK_SET) == (off_t)0);
	assert(read(fd, (void *)elf_header, sizeof(Elf32_Ehdr)) == sizeof(Elf32_Ehdr));
}

bool Parser::is_ELF(Elf32_Ehdr eh)
{
	if(!strncmp((char*)eh.e_ident, "\177ELF", 4)) {
		//printf("ELFMAGIC \t= ELF\n");
		
		return 1;
	} 
    else {
		//printf("ELFMAGIC mismatch!\n");
		
		return 0;
	}
}

bool Parser::is_PE(char* fd) {
    char* pe_magic = (char*) malloc(3 * sizeof(char));
    FILE* f = fopen(fd,"rb");
    fread(pe_magic,1,2,f);
    pe_magic[2] = '\0';
    if(!strcmp(pe_magic,"MZ")) {
        free(pe_magic);
        return true;
    }
    else {
        free(pe_magic);
        return false;
    }
}

Elf64_Addr Parser::get_entry_point(Elf32_Ehdr elf_header) {
    return elf_header.e_entry;
}

bool Parser::checkSignature(const char* file) {
    int32_t fd;
	Elf32_Ehdr eh;		
	Elf32_Shdr* sh_tbl;

	fd = open(file, O_RDONLY | O_SYNC);
	char* pe_file = (char*) malloc(sizeof(char) * strlen(file));
    strcpy(pe_file,file);
    if(fd<0) {
		printf("Error %d Unable to open %s\n", fd, file);
        return false;
	}

    this->read_elf_header(fd, &eh);
    if(is_ELF(eh)) {
        Elf64_Addr entry_point  =   this->get_entry_point(eh);
        
        lseek(fd, entry_point, SEEK_SET);
        char buffer[1025];
        bool isVirus = false;
        int bytes_number    =   read(fd, buffer, 1024);
        
        if(bytes_number > 0) {
            string signature    =   this->crypt->print_md5(buffer,1024);
            isVirus        =   this->db->checkSignature(signature);
            if(isVirus == true) {
                    
                cout << "[*] Signature hash : " << signature << "\n";
                   
            }
        }
        return isVirus;
    }
    else if(is_PE(pe_file)) {
        FILE *fpipe;
        char command[] = "python ";
        strcat(command,this->cwd);
        strcat(command,"/peparser.py '");
        strcat(command,file);
        strcat(command,"'");
        char* c = (char*)malloc(30*sizeof(char));

        if (0 == (fpipe = (FILE*)popen(command, "r")))
        {
            perror("popen() failed.");
            exit(1);
        }

        fread(c, sizeof(char), 20, fpipe);
        
        int pe_entry_point = atoi(c);
        //cout << "pe_entry_point : " << pe_entry_point << "\n";
        pclose(fpipe);
        
        lseek(fd, pe_entry_point, SEEK_SET);
        char buffer[1025];
        bool isVirus = false;
        /*
        int offset = 1;
        while(int bytes_number    =   read(fd, buffer, 1024) > 0) {
            
            if(bytes_number > 0) {
                string signature    =   this->crypt->print_md5(buffer,1024);
                cout << "[*] Signature hash : " << signature << "\n";
                isVirus        =   this->db->checkSignature(signature);
                if(isVirus == true)
                   break;
            }
            lseek(fd, pe_entry_point+offset, SEEK_SET);
            offset++;
        }
        return isVirus;
        */
        lseek(fd, pe_entry_point, SEEK_SET);
        int bytes_number    =   read(fd, buffer, 1024);
        
        if(bytes_number > 0) {
            string signature    =   this->crypt->print_md5(buffer,1024);
            bool isVirus        =   this->db->checkSignature(signature);
            return isVirus;
        }
        
    }
    else {
        cout    <<  "NOT AN EXECUTABLE file!\n";
        return false;
    }
}

bool Parser::scanFile(char* file) {
    if(this->upx->isUPXFile(file)) {
        cout << "UPX found\n";
        bool virus = this->checkSignature((string(file)+".unpacked").c_str());
        remove((string(file)+".unpacked").c_str());
        return virus;
    }
    else {
        return this->checkSignature(file); 
    }
}

bool Parser::isDir(const char* name) {
    DIR* dir    =   opendir(name);
    if(dir) {
        closedir(dir);
        return false;
    }
    return true;
}

void Parser::scanFolder(const char *name) {
    DIR* dir;
    struct dirent *entry;
    cout << "SCANNING " << name << " FOLDER\n\n";
    if(!(dir = opendir(name))) {
        cout << "\n" << name << " is NOT a directory\n";
        return;
    }
    chdir(name);

    while((entry = readdir(dir)) != NULL) {
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;
        else if((isDir(entry->d_name) == true)) {
            cout << "\n[*] Scaning " << entry->d_name << "...\n\n";
            this->scanFile(entry->d_name);
            this->searchHexSignature(entry->d_name);
        }
        if(isDir(entry->d_name) == false) {
            scanFolder(entry->d_name);
        }
    }
    chdir("..");
    closedir(dir);
}

bool Parser::searchHexSignature(char* file) {
    int32_t fd;
	Elf32_Ehdr eh;		
	Elf32_Shdr* sh_tbl;

	fd = open(file, O_RDONLY | O_SYNC);
    if(fd<0) {
		printf("Error %d Unable to open %s\n", fd, file);
        return false;
	}
    this->read_elf_header(fd, &eh);
    if(is_ELF(eh) || is_PE(file)) {
    if(this->upx->isUPXFile(file)) {
        cout << "UPX found\n";
        bool virus = this->db->checkHexSignature((char*)(string(file)+".unpacked").c_str());
        remove((string(file)+".unpacked").c_str());
        return virus;
    }
    else {
        return this->db->checkHexSignature(file);
    }
    }
}

void Parser::signing(const char* file) {
        int32_t fd;
        Elf32_Ehdr eh;		
        Elf32_Shdr* sh_tbl;

        fd = open(file, O_RDONLY | O_SYNC);
        if(fd<0) {
            printf("Error %d Unable to open %s\n", fd, file);
        }
    
        this->read_elf_header(fd, &eh);
        if(is_ELF(eh)) {
            Elf64_Addr entry_point  =   this->get_entry_point(eh);
            
            lseek(fd, entry_point, SEEK_SET);
            char* buffer        =   (char*) malloc(1025 * sizeof(char));
            int bytes_number    =   read(fd, buffer, 1024);
            if(bytes_number > 0) {
                string signature    =   this->crypt->print_md5(buffer,1024);
                this->insertSignature(signature);
            }
            free(buffer);
            close(fd);
        }
        else if(is_PE((char*)file)) {
            
            FILE *fpipe;
            char command[] = "python ";
            strcat(command,this->cwd);
            strcat(command," /parser.py '");
            strcat(command,file);
            strcat(command,"'");
            char* c = (char*)malloc(30*sizeof(char));

            if (0 == (fpipe = (FILE*)popen(command, "r")))
            {
                perror("popen() failed.");
                exit(1);
            }

            fread(c, sizeof(char), 20, fpipe);
            
            int pe_entry_point = atoi(c);
            pclose(fpipe);
            
            lseek(fd, pe_entry_point, SEEK_SET);
            char* buffer        =   (char*) malloc(1025 * sizeof(char));
            int bytes_number    =   read(fd, buffer, 1024);
            if(bytes_number > 0) {
                string signature    =   this->crypt->print_md5(buffer,1024);
                this->insertSignature(signature);
                
            }
            free(buffer);
            close(fd);
        }
        else {
            cout    <<  "NOT an executable file!\n";
            return;
        }
}

void Parser::signFile(char* file) {
    if(this->upx->isUPXFile(file)) {
        cout << "UPX found\n";
        this->signing((string(file)+".unpacked").c_str());
        remove((string(file)+".unpacked").c_str());
        return;
    }
    else {
        this->signing(file);
        

    }
}

void Parser::insertSignature(string signature) {
    int success = this->db->insertSignature(signature);
    if(success > 0) {
        cout    <<  "[+] Signature inserted successfully!\n";
    }
    else {
        cout    <<  "[-] Failed to insert signature!\n";
    }
}

Parser::~Parser() {
    delete db;
    delete crypt;
    delete upx;
}
