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
        executeFilter(request, filters.begin());
    }
}

void FilterChain::executeFilter(const ClientRequest& request, list<unique_ptr<IFilter>>::iterator it) {
    if (it != filters.end()) {
        auto nextIt = next(it);
        try {
            (*it)->doFilter(request, (nextIt != filters.end()) ? nextIt->get() : nullptr);
        } catch (const exception& e) {
            string filterName = typeid(**it).name();
            string errorMsg = "Filter [" + filterName + "] failed: " + e.what();
            
            Logger::error(errorMsg);
            throw FilterException(filterName, e.what());
        }
    }
}

bool FilterChain::isEmpty() const {
    return filters.empty();
}

// 특정 필터 앞에 추가
void FilterChain::addFilterBefore(const string& targetFilterName, unique_ptr<IFilter> filter) {
    auto it = findFilter(targetFilterName);
    if (it == filters.end()) {
        throw FilterException(targetFilterName, "Target filter not found.");
    }
    filters.insert(it, move(filter));
}

// 특정 필터 뒤에 추가
void FilterChain::addFilterAfter(const string& targetFilterName, unique_ptr<IFilter> filter) {
    auto it = findFilter(targetFilterName);
    if (it == filters.end()) {
        throw FilterException(targetFilterName, "Target filter not found.");
    }
    filters.insert(next(it), move(filter));
}

// 특정 필터 제거
void FilterChain::removeFilter(const string& filterName) {
    auto it = findFilter(filterName);
    if (it == filters.end()) {
        throw FilterException(filterName, "Filter not found.");
    }
    filters.erase(it);
}

// 특정 필터의 iterator 찾기
list<unique_ptr<IFilter>>::iterator FilterChain::findFilter(const string& filterName) {
    for (auto it = filters.begin(); it != filters.end(); ++it) {
        if (typeid(**it).name() == filterName) {
            return it;
        }
    }
    return filters.end();
}
