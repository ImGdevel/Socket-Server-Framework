#ifndef IFILTER_H
#define IFILTER_H

#include "ClientRequest.h"

class IFilter {
public:
    virtual ~IFilter() = default;
    virtual void doFilter(const ClientRequest& request, IFilter* nextFilter) = 0;
};

#endif
