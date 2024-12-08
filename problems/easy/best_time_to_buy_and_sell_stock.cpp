// ------------------------------------------------------------------------------------------------
// Title: Best time to buy and sell stock.
// Category: Easy.
// Author: Luiz G. Mugnaini A. <luizmugnaini@gmail.com>
// ------------------------------------------------------------------------------------------------
// Description:
//
// You are given an integer array prices where prices[i] is the price of Y on the ith day.
//
// You may choose a single day to buy one Y and choose a different day in the future to
// sell it.
//
// Return the maximum profit you can achieve. You may choose to not make any transactions, in
// which case the profit would be 0.
// ------------------------------------------------------------------------------------------------

#include <common.hpp>
#include <type_traits>

static int32_t max_profit(std::vector<int32_t> const& stock_prices) {
    size_t stock_count = stock_prices.size();
    if (stock_count == 0) {
        return {};
    }

    int32_t max_profit = 0;
    for (size_t buy_idx = 0; buy_idx < stock_count; ++buy_idx) {
        for (size_t sell_idx = buy_idx + 1; sell_idx < stock_count; ++sell_idx) {
            int32_t current_profit = stock_prices[sell_idx] - stock_prices[buy_idx];
            max_profit             = max_value(max_profit, current_profit);
        }
    }

    return max_profit;
}

int main() {
    assert_eq(max_profit({10, 1, 5, 6, 7, 1}), 6);
    assert_eq(max_profit({10, 8, 7, 2, 12}), 10);
    assert_eq(max_profit({10, 8, 7, 5, 2}), 0);
    report_success();
    return 0;
}
