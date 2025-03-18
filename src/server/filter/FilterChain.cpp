#include "FilterChain.h"

void FilterChain::addFilter(std::unique_ptr<IFilter> filter) {
    filters.push_back(std::move(filter));
}

void FilterChain::doFilter(const ClientRequest& request) {
    if (!filters.empty()) {
        executeFilter(request, 0);
    }
}

void FilterChain::executeFilter(const ClientRequest& request, size_t index) {
    if (index < filters.size()) {
        IFilter* nextFilter = (index + 1 < filters.size()) ? filters[index + 1].get() : nullptr;
        filters[index]->doFilter(request, nextFilter);
    }
}

bool FilterChain::isEmpty() const {
    return filters.empty();
} 