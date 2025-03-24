#ifndef FILTERCHAIN_H
#define FILTERCHAIN_H

#include "IFilter.h"
#include <list>
#include <memory>
#include <string>

class FilterChain {
public:
    void addFilter(std::unique_ptr<IFilter> filter);
    void doFilter(const ClientRequest& request);
    bool isEmpty() const;

    void addFilterBefore(const std::string& targetFilterName, std::unique_ptr<IFilter> filter);
    void addFilterAfter(const std::string& targetFilterName, std::unique_ptr<IFilter> filter);
    void removeFilter(const std::string& filterName); 

private:
    std::list<std::unique_ptr<IFilter>> filters;
    void executeFilter(const ClientRequest& request, std::list<std::unique_ptr<IFilter>>::iterator it);
    std::list<std::unique_ptr<IFilter>>::iterator findFilter(const std::string& filterName);
};

#endif
