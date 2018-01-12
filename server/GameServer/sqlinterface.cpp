#include <iostream>
#include <vector>
#include <string.h>
#include "sqlinterface.h"

// some SQL library syntax/help from http://blog.davenicholas.me.uk/2013/04/how-to-c-mysql-mac-osx.html

/* constructor */
SQLInterface::SQLInterface() {
    mysql_init(&mysql);
    connection = mysql_real_connect(&mysql, SQL_HOST, SQL_USERNAME, SQL_PASSWORD, "ScrummyBears", SQL_PORT, 0, CLIENT_MULTI_STATEMENTS);
    if(connection == NULL) { // no connection
        std::cout << "Error connecting to MySQL database: " << mysql_error(&mysql) << std::endl;
    }

    // test table listing - if we get more than 0 tables, the connection is successful
    std::vector<std::string> tablelist;
    int state = mysql_query(connection, "SHOW TABLES");
    if(state) {
        std::cout << "Error running queries on SQL database: " << mysql_error(&mysql) << std::endl;
    }

    MYSQL_RES *result = mysql_store_result(connection);
    if(mysql_num_rows(result) > 0) {
        std::cout << "Connected to SQL database." << std::endl;
    }

    mysql_free_result(result);
}

/* Destructor */
SQLInterface::~SQLInterface() {
    mysql_close(connection);
}

/* Wrapper to run a MySQL query. If the server has gone away, automatically reconnect
 *  and try again
 */
bool SQLInterface::runQuery(std::string query) {
    if(connection == NULL)
        return false;

    int state = mysql_query(connection, query.c_str());
    if(state) {
        const char *error = mysql_error(&mysql);
        if(!strcmp(error, "MySQL server has gone away") || !strcmp(error, "Commands out of sync; you can't run this command now")) {
            std::cout << "Reconnecting to MySQL server..." << std::endl;
            if(!strcmp(error, "Commands out of sync; you can't run this command now")) {
                mysql_close(connection);
            }
            // reinitiate connection and try again
            connection = mysql_real_connect(&mysql, SQL_HOST, SQL_USERNAME, SQL_PASSWORD, "ScrummyBears", SQL_PORT, 0, 0);
            if(connection == NULL) {
                std::cout << "Error reconnecting to MySQL database: " << mysql_error(&mysql) << std::endl;
                return false;
            }

            state = mysql_query(connection, query.c_str());
            if(state) {
                std::cout << "Error running queries on SQL database: " << mysql_error(&mysql) << std::endl;
                return false;
            }
        } else {
            std::cout << "Error running queries on SQL database: " << error << std::endl;
            return false;
        }
    }

    return true;
}

/* Check the database for the given username/password combo. If the password
 * is correct, return the most recent level - otherwise return 0.
 */
int SQLInterface::checkUser(std::string username, std::string password) {
    // run a query for a password for the given student ID
    std::string query = "SELECT password FROM ScrummyBears.students WHERE stud_id = " + username;
    if(!runQuery(query))
        return 0;

    // get result
    MYSQL_RES *result = mysql_store_result(connection);
    MYSQL_ROW row = mysql_fetch_row(result);
    // nothing returned - ID does not exist in database
    if(row == NULL)
        return 0;

    mysql_free_result(result); // all done with result now

    std::string db_pass = row[0];
    // compare password with database password
    if(password.compare(db_pass) == 0)
        // TODO: return recent/last level value
        return 1;
    else
        return 0;
}

/* Check the database to see if a username is already in use. If so, return true.
 * Otherwise (no entry) return false.
 */
bool SQLInterface::checkUsername(std::string username) {
    // run a query for the given student ID
    std::string query = "SELECT stud_id FROM ScrummyBears.students WHERE stud_id = " + username;
    if(!runQuery(query))
        return false;

    // get result
    MYSQL_RES *result = mysql_store_result(connection);
    MYSQL_ROW row = mysql_fetch_row(result);
    mysql_free_result(result); // all done with result

    if(row == NULL) // nothing returned - ID does not exist in database
        return false;
    else
        return true;
}

/* Add a new user to the database. This adds rows to the students, level1, level2
 * and level3 tables.
 */
void SQLInterface::newUser(std::string username, std::string password) {
    std::string query;

    // start a transaction - get ready to insert values
    if(!runQuery("START TRANSACTION"))
        return;

    // insert new user into students table
    query = "INSERT INTO ScrummyBears.students (stud_id, password) values (" + username + ", \"" + password + "\")";
    if(!runQuery(query))
        return;

    // insert new user into the level tables
    for(int i=1; i <= 3; i++) {
        query = "INSERT INTO ScrummyBears.level" + std::to_string(i) + " (stud_id) values (" + username + ")";
        if(!runQuery(query))
            return;
    }

    // finally, commit the transaction
    query = "COMMIT";
    if(!runQuery(query))
        return;
}

bool SQLInterface::removeUser(std::string username) {
    std::string query = "CALL delete_user(" + username + ")";

    if(checkUsername(username)) {
        if(!runQuery(query))
            return false;
        else
            return true;
    } else
        return false;
}

void SQLInterface::updateScore(std::string username, int level, int round, int score) {
    std::string query = ("CALL update_score(\"level" + std::to_string(level) + "\", \"r" + std::to_string(round) +
                         "_score\", \"" + std::to_string(score) + "\", \"" + username + "\")");
    if(!runQuery(query))
        return;
}

/* Private function - get scores for a particular user and level. Note that this changes the
 * vector of ints passed in, instead of returning anything
 */
void SQLInterface::getLevelScore(std::string username, int level, std::vector<int> &levelinfo) {
    // run a query for the given student ID
    std::string query = "SELECT * FROM ScrummyBears.level" + std::to_string(level) + " WHERE stud_id = " + username;
    if(!runQuery(query))
        return;

    MYSQL_RES *result = mysql_store_result(connection);
    MYSQL_ROW row = mysql_fetch_row(result);

    if(row == NULL) {// nothing returned - ID does not exist in database
        mysql_free_result(result); // all done with result
        return;
    }

    // add items into vector - skip the first one because that's the student ID
    for(unsigned int i=1; i < mysql_num_fields(result); i++) {
        levelinfo.push_back(atoi(row[i]));
    }

    mysql_free_result(result); // all done with result
}

/* Get scores for a particular user and level. This returns a vector of level scores (as ints). */
std::vector<int> SQLInterface::getScores(std::string username, int level) {
    std::vector<int> levelinfo;
    getLevelScore(username, level, levelinfo);
    return levelinfo;
}

/* get all scores for a particular user */
std::vector<int> SQLInterface::getAllScores(std::string username) {
    std::vector<int> levelinfo;
    for(int i=1; i <= 3; i++) {
        getLevelScore(username, i, levelinfo);
    }
    return levelinfo;
}

std::vector<std::vector<int>> SQLInterface::getReport() {
    std::vector<std::vector<int>> report;
    // run a query for the given student ID
    std::string query = "CALL load_report()";
    if(!runQuery(query))
        return report;

    // get result
    MYSQL_RES *result = mysql_store_result(connection);

    for(unsigned int i=0; i<mysql_num_rows(result); i++) {
        MYSQL_ROW row = mysql_fetch_row(result);
        if(row == NULL) // safety
            break;

        std::vector<int> student;
        for(unsigned int j=0; j<mysql_num_fields(result); j++) {
            student.push_back(atoi(row[j]));
        }
        report.push_back(student);
    }

    mysql_free_result(result); // all done with result
    return report;
}
