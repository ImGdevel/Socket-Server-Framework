#include "MySQLDB.h"
#include "Logger.h"
#include <mysql_driver.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <stdexcept>

using namespace std;

MySQLDB::~MySQLDB() {
    if (connection) {
        try {
            connection->close();
            Logger::info("MySQL connection closed.");
        } catch (const sql::SQLException& e) {
            string errorMsg = "Error closing MySQL connection: " + std::string(e.what());
            Logger::error(errorMsg);
            throw runtime_error(errorMsg);
        }
    }
}


void MySQLDB::connect(const string& host, const string& user, const string& password, const string& database) {
    try {
        Logger::debug("Attempting to connect to MySQL database...");
        sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();
        connection = make_shared<sql::Connection>(driver->connect("tcp://" + host, user, password));
        connection->setSchema(database);
        Logger::info("MySQL Connection established to database: " + database);
    } catch (sql::SQLException& e) {
        string errorMsg = "MySQL Connection Error: " + string(e.what());
        throw runtime_error(errorMsg);
    }
}

void MySQLDB::execute(const string& query) {
    try {
        Logger::debug("Executing query: " + query);
        shared_ptr<sql::Statement> stmt(connection->createStatement());
        stmt->execute(query);
        Logger::debug("Query executed successfully.");
    } catch (sql::SQLException& e) {
        string errorMsg = "MySQL Query Error: " + string(e.what()) + " | Query: " + query;
        throw runtime_error(errorMsg);
    }
}

vector<vector<string>> MySQLDB::fetch(const string& query) {
    vector<vector<string>> results;
    try {
        Logger::debug("Fetching results for query: " + query);
        shared_ptr<sql::Statement> stmt(connection->createStatement());
        shared_ptr<sql::ResultSet> res(stmt->executeQuery(query));
        while (res->next()) {
            vector<string> row;
            for (unsigned int i = 1; i <= res->getMetaData()->getColumnCount(); i++) {
                row.push_back(res->getString(i));
            }
            results.push_back(row);
        }
        Logger::debug("Query fetched successfully. Rows returned: " + to_string(results.size()));
    } catch (sql::SQLException& e) {
        string errorMsg = "MySQL Fetch Error: " + string(e.what()) + " | Query: " + query;
        throw runtime_error(errorMsg);
    }
    return results;
}
