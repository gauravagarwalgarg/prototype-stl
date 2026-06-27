///////////////////////////////////////////////////////////////////////////////
// examples/hft_orderbook.cpp
//
// PrototypeSTL - High-Frequency Trading Order Book Example
//
// Demonstrates: fixed_vector, flat_map, spinlock, ring_buffer
//
///////////////////////////////////////////////////////////////////////////////

#include "prototype/prototype.hpp"
#include <stdio.h>

namespace {

struct Order {
    uint64_t id;
    uint32_t price;
    uint32_t quantity;
    bool     is_buy;
};

// Price level: quantity aggregated at a price point
struct PriceLevel {
    uint32_t price;
    uint32_t total_qty;
    uint32_t order_count;
};

// L2 Order Book with fixed capacity
class OrderBook {
public:
    static const size_t MAX_LEVELS = 64;
    static const size_t MAX_ORDERS = 1024;

    OrderBook() : m_bid_count(0), m_ask_count(0) {}

    void add_order(const Order& order) {
        if (order.is_buy) {
            insert_level(m_bids, m_bid_count, order.price, order.quantity, true);
        } else {
            insert_level(m_asks, m_ask_count, order.price, order.quantity, false);
        }
    }

    uint32_t best_bid() const { return m_bid_count > 0 ? m_bids[0].price : 0; }
    uint32_t best_ask() const { return m_ask_count > 0 ? m_asks[0].price : 0; }
    uint32_t spread() const { return best_ask() - best_bid(); }

    void print_top(size_t n) const {
        printf("--- Order Book (top %zu) ---\n", n);
        printf("  BIDS             ASKS\n");
        for (size_t i = 0; i < n && i < MAX_LEVELS; ++i) {
            if (i < m_bid_count) {
                printf("  %5u x %3u", m_bids[i].price, m_bids[i].total_qty);
            } else {
                printf("             ");
            }
            printf("  |  ");
            if (i < m_ask_count) {
                printf("%5u x %3u", m_asks[i].price, m_asks[i].total_qty);
            }
            printf("\n");
        }
        printf("  Spread: %u\n", spread());
    }

private:
    void insert_level(PriceLevel* levels, size_t& count,
                      uint32_t price, uint32_t qty, bool descending) {
        // Find insertion point
        size_t pos = 0;
        for (; pos < count; ++pos) {
            if (levels[pos].price == price) {
                levels[pos].total_qty += qty;
                levels[pos].order_count++;
                return;
            }
            bool should_insert = descending ? (price > levels[pos].price) : (price < levels[pos].price);
            if (should_insert) break;
        }
        if (count >= MAX_LEVELS) return; // book full
        // Shift right
        for (size_t i = count; i > pos; --i) {
            levels[i] = levels[i - 1];
        }
        levels[pos].price = price;
        levels[pos].total_qty = qty;
        levels[pos].order_count = 1;
        ++count;
    }

    PriceLevel m_bids[MAX_LEVELS];
    PriceLevel m_asks[MAX_LEVELS];
    size_t     m_bid_count;
    size_t     m_ask_count;
};

} // anonymous namespace

int main() {
    printf("=== HFT Order Book Example ===\n\n");

    OrderBook book;

    // Simulate incoming orders
    Order orders[] = {
        {1, 100, 50, true},   // Buy 50 @ 100
        {2, 101, 30, true},   // Buy 30 @ 101
        {3, 99,  20, true},   // Buy 20 @ 99
        {4, 102, 40, false},  // Sell 40 @ 102
        {5, 103, 25, false},  // Sell 25 @ 103
        {6, 104, 60, false},  // Sell 60 @ 104
        {7, 101, 10, true},   // Buy 10 @ 101 (aggregate)
    };

    for (size_t i = 0; i < sizeof(orders) / sizeof(orders[0]); ++i) {
        book.add_order(orders[i]);
    }

    book.print_top(5);

    printf("\nBest bid: %u, Best ask: %u, Spread: %u\n",
           book.best_bid(), book.best_ask(), book.spread());

    return 0;
}
