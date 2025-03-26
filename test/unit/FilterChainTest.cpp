#include "FilterChain.h"
#include "IFilter.h"
#include "ClientRequest.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using ::testing::StrictMock;
using ::testing::Invoke;

class MockFilter : public IFilter {
public:
    MOCK_METHOD(void, doFilter, (const ClientRequest& request, IFilter* nextFilter), (override));
};

// 초기 상태 테스트
TEST(FilterChainTest, InitiallyEmpty) {
    FilterChain chain;
    EXPECT_TRUE(chain.isEmpty()); // 처음에는 비어 있어야 한다.
}

// 필터 추가 후 비어있지 않은지 확인
TEST(FilterChainTest, AddFilterMakesItNonEmpty) {
    FilterChain chain;
    chain.addFilter(std::make_unique<MockFilter>());
    EXPECT_FALSE(chain.isEmpty()); // 필터 추가 후에는 비어 있지 않아야 한다.
}

// 단일 필터 실행 테스트
TEST(FilterChainTest, SingleFilterExecution) {
    FilterChain chain;
    auto mockFilter = std::make_unique<StrictMock<MockFilter>>();
    MockFilter* mockPtr = mockFilter.get();
    
    ClientRequest request;
    EXPECT_CALL(*mockPtr, doFilter(testing::_, nullptr)) // 다음 필터가 없으므로 nullptr
        .Times(1);
    
    chain.addFilter(std::move(mockFilter));
    chain.doFilter(request);
}

// 여러 개의 필터가 올바른 순서로 실행되는지 확인
TEST(FilterChainTest, MultipleFiltersExecutionInOrder) {
    FilterChain chain;
    
    auto mockFilter1 = std::make_unique<StrictMock<MockFilter>>();
    auto mockFilter2 = std::make_unique<StrictMock<MockFilter>>();
    MockFilter* filter1Ptr = mockFilter1.get();
    MockFilter* filter2Ptr = mockFilter2.get();
    
    ClientRequest request;
    EXPECT_CALL(*filter1Ptr, doFilter(testing::_, filter2Ptr)) // 첫 번째 필터가 두 번째 필터를 호출해야 함
        .Times(1);
    EXPECT_CALL(*filter2Ptr, doFilter(testing::_, nullptr)) // 두 번째 필터는 마지막 필터이므로 nullptr
        .Times(1);
    
    chain.addFilter(std::move(mockFilter1));
    chain.addFilter(std::move(mockFilter2));
    chain.doFilter(request);
}
