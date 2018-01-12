#ifndef SQLINTERFACE_H
#define SQLINTERFACE_H

#include <string>
#include <mysql/mysql.h>

// SQL server-specific information - enter the server URL, username and password here
#define SQL_HOST ""
#define SQL_PORT 3306
#define SQL_USERNAME ""
#define SQL_PASSWORD ""

class SQLInterface {
public:
    SQLInterface();
    ~SQLInterface();
    int checkUser(std::string, std::string);
    bool checkUsername(std::string);
    void newUser(std::string, std::string);
    bool removeUser(std::string);
    void updateScore(std::string, int, int, int);
    std::vector<int> getScores(std::string, int);
    std::vector<int> getAllScores(std::string);
    std::vector<std::vector<int>> getReport();

private:
    bool runQuery(std::string);
    void getLevelScore(std::string, int, std::vector<int>&);
    MYSQL *connection, mysql;
};

#endif // SQLINTERFACE_H
