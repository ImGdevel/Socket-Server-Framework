#ifndef DEFAULTFILTERX_H
#define DEFAULTFILTERX_H

#include "IFilter.h"
#include "Logger.h"

class DefaultFilterX : public IFilter {
public:
    void doFilter(const ClientRequest& request, IFilter* nextFilter) override {
        Logger::info("DefaultFilterX: Passing request to the next filter.");
        if (nextFilter) {
            nextFilter->doFilter(request, nullptr);
        }
    }
};

#endif
