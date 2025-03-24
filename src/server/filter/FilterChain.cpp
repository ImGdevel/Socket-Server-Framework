#include "FilterChain.h"
#include "FilterException.h"
#include "Logger.h"
#include <typeinfo>

using namespace std;

void FilterChain::addFilter(unique_ptr<IFilter> filter) {
    filters.push_back(move(filter));
}

void FilterChain::doFilter(const ClientRequest& request) {
    if (!filters.empty()) {
        executeFilter(request, 0);
    }
}

void FilterChain::executeFilter(const ClientRequest& request, size_t index) {
    if (index < filters.size()) {
        IFilter* nextFilter = (index + 1 < filters.size()) ? filters[index + 1].get() : nullptr;

        try {
            filters[index]->doFilter(request, nextFilter);
        } catch (const exception& e) {
            string filterName = typeid(*filters[index]).name();
            string errorMsg = "Filter [" + filterName + "] failed: " + e.what();
            
            Logger::error(errorMsg);
            throw FilterException(filterName, e.what());
        }
    }
}

bool FilterChain::isEmpty() const {
    return filters.empty();
} 