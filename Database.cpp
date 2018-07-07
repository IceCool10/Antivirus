#include "Database.h"
#include <time.h>
#include "UPX.h"
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


Database* Database::instance    =   NULL;

Database* Database::getInstance() {
    
    if(instance == 0) {
        instance = new Database();
    }

    return instance;

}

Database::Database() {
    
    this->connected =   false;
    getcwd(this->cwd,sizeof(this->cwd));
    try {
        this->driver  =   get_driver_instance();
        this->connect();
    }
    catch (sql::SQLException &e) {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " 
             << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() <<
                " )" << endl;
    }

}

void Database::connect() {
    try {
    this->con     =   driver->connect("tcp://127.0.0.1:3306","maco","Pass/123");
    this->con->setSchema("Antivirus");
    this->connected =   true;
    }
    catch (sql::SQLException &e) {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " 
             << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() <<
                " )" << endl;
    }

}

bool Database::checkSignature(string signature) {
    if(this->connected) {
    int success         =   0;
    sql::PreparedStatement* pstmt;
    sql::ResultSet* res;
    try {
        pstmt     =   this->con->prepareStatement("SELECT * FROM Signs WHERE signature = ?");
        pstmt->setString(1,signature);
        res       =   pstmt->executeQuery();
        cout << "First method detection : \n";
        if(res->rowsCount() > 0) {
            res->next();
            cout << "\033[1;31mINFECTED FILE\033[0m: ( " << res->getString("name") << " )" << "\n";
            pstmt->close();
            delete res;
            delete pstmt;
            return true;
        }
        else {
            pstmt->close();
        }
        delete res;
        delete pstmt;
        cout << "\033[1;32mClean file!\033[0m\n";
        return false;
    }
    catch (sql::SQLException &e) {
        cout << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() <<
                " )" << endl;
    }


    }
} 

bool Database::checkHexSignature(char* file) {
    if(this->connected) {
    int success         =   0;
    sql::ResultSet* res;
    try {
        //this->pstmt     =   this->con->prepareStatement("SELECT * FROM hexSigns");
        sql::Statement* stmt = this->con->createStatement();
        res       =   stmt->executeQuery("SELECT * FROM hexSigns");
        string file_name(file);
        size_t found    =   file_name.find_last_of("/");
        system((string("python3 ") + string(this->cwd) + string("/bin2op.py -f ") + string(file) + string(" -s > ") + file_name.substr(found+1) + string(".opcodes") ).c_str());
        FILE* fp    =   fopen((file_name.substr(found+1) + ".opcodes").c_str(),"r");

        //report




        cout << "\nSecond method detection : \n";
        while(res->next()) {
            rewind(fp);
            string signature    =   res->getString("signature");
            while(!feof(fp)) {
                char*   file_content    = new char[signature.size() + 1];
                char*   file_content2   = new char[signature.size() + 1];
                int nr_bytes = fread(file_content,1,signature.size(),fp);
                int nr_bytes2 = fread(file_content2,1,signature.size(),fp);
                UPX u;
                if(nr_bytes > 0) {

                    if(u.rollingHash(signature,file_content) != -1 || u.rollingHash(signature,file_content2) != -1) {
                        cout << "\n\n\033[1;31mINFECTED FILE\033[0m : ( " << res->getString("category") << " )  \nSignature : " << signature.c_str() << "\n";
                        remove((file_name.substr(found+1) + ".opcodes").c_str());
                        //cout << "Signature in file : " << signature << "\n\n";
                        delete [] file_content;
                        delete [] file_content2;
                        delete stmt;
                        delete res;
                        fclose(fp);
                        return true;
                        
                    }
                    else {
                        char* middle    =   new char[2*signature.size() + 2];
                        strncpy(middle,file_content, signature.size() + 1);
                        strncat(middle,file_content2, signature.size() + 1);
                        if(u.rollingHash(signature,middle) != -1) {
                            cout << "\033[1;31mINFECTED FILE\033[0m : ( " << res->getString("category") << " ) \nSignature : " << signature.c_str() << "\n";
                            remove((file_name.substr(found+1) + ".opcodes").c_str());

                            //cout << "Signature in file : " << signature << "\n\n";
                            delete [] file_content;
                            delete [] file_content2;
                            delete [] middle;
                            delete stmt;
                            delete res;
                            fclose(fp);
                            return true;
                            
                        }
                        else {
                            delete [] middle;
                            fseek(fp,-nr_bytes2,SEEK_CUR);
                        }
                    }
                } 
            }
            fseek(fp,0,SEEK_SET);
        }
        cout << "\033[1;32mClean file!\033[0m\n";
        //cout << "Clean file!\n";
        //remove((string(file) + ".opcodes").c_str());
        delete stmt;
        remove((file_name.substr(found+1) + ".opcodes").c_str());
        return false;
    }
    catch (sql::SQLException &e) {
        cout << e.what();
    }

    }
}

int Database::insertSignature(string signature) {
    int success         =   0;
    sql::PreparedStatement* pstmt;
    try {
        pstmt     =   this->con->prepareStatement("INSERT INTO Signs(signature) VALUES (?)");
        pstmt->setString(1, signature);

        success     =   pstmt->executeUpdate();
    }
    catch (sql::SQLException &e) {
        cout << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << 
                            " )" << endl;
        success =   0;
    }
    pstmt->close();
    delete pstmt;
    return success;
}

Database::~Database() {
    delete this->con;
}
