#ifndef DB_INTERFACE_H
#define DB_INTERFACE_H

#include <string>
#include <vector>

class DBInterface {
public:
    virtual ~DBInterface() = default;

    // 데이터베이스 연결 설정
    virtual void connect(const std::string& host, const std::string& user, const std::string& password, const std::string& database) = 0;

    // 쿼리 실행
    virtual void execute(const std::string& query) = 0;

    // 데이터 조회
    virtual std::vector<std::vector<std::string>> fetch(const std::string& query) = 0;
};

#endif
