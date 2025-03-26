#ifndef DEFAULTFILTER_H
#define DEFAULTFILTER_H

#include "IFilter.h"
#include "Logger.h"

class DefaultFilter : public IFilter {
public:
    void doFilter(const ClientRequest& request, IFilter* nextFilter) override {
        Logger::info("DefaultFilter: Passing request to the next filter.");
        if (nextFilter) {
            nextFilter->doFilter(request, nullptr);
        }
    }
};

#endif
