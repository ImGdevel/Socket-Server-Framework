#ifndef FILTERCHAIN_H
#define FILTERCHAIN_H

#include "IFilter.h"
#include <vector>
#include <memory>

class FilterChain {
public:
    void addFilter(std::unique_ptr<IFilter> filter);
    void doFilter(const ClientRequest& request);
    bool isEmpty() const;

private:
    std::vector<std::unique_ptr<IFilter>> filters;
    void executeFilter(const ClientRequest& request, size_t index);
};

#endif 