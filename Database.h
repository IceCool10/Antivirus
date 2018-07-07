#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>


#include <string>
using namespace std;

class Database {
    
    private:
        static Database* instance;
        Database();
        sql::Driver *driver;
        sql::Connection *con;
        char cwd[1024];
        bool connected;
        
    public:
        static  Database*   getInstance();
        void    connect();
        int     insertSignature(string signature);
        bool    checkSignature(string signature);
        bool    checkHexSignature(char* file);
        ~Database();
};
