#ifndef FILTEREXCEPTION_H
#define FILTEREXCEPTION_H

#include <exception>
#include <string>

using namespace std;

class FilterException : public exception {
private:
    string message;

public:
    FilterException(const string& filterName, const string& error)
        : message("Filter [" + filterName + "] failed: " + error) {}

    const char* what() const noexcept override {
        return message.c_str();
    }
};

#endif
