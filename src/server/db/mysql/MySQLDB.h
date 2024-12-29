#ifndef MYSQL_DB_H
#define MYSQL_DB_H

#include "../interface/DBInterface.h"
#include <mysql_connection.h>
#include <memory>
#include <string>
#include <vector>

class MySQLDB : public DBInterface {
private:
    std::shared_ptr<sql::Connection> connection;

public:
    ~MySQLDB();
    void connect(const std::string& host, const std::string& user, const std::string& password, const std::string& database) override;
    void execute(const std::string& query) override;
    std::vector<std::vector<std::string>> fetch(const std::string& query) override;
};

#endif