///////////////////////////////////////////////////////////////////////////////
// tests/test_all.cpp
//
// PrototypeSTL - Comprehensive test suite (no external test framework)
//
// Compile: g++ -std=c++11 -fno-exceptions -fno-rtti -I../include -o test_all test_all.cpp
//
///////////////////////////////////////////////////////////////////////////////

#include "prototype/prototype.hpp"
#include <stdio.h>

static int g_tests_passed = 0;
static int g_tests_failed = 0;

#define TEST_ASSERT(cond) do { \
    if (!(cond)) { \
        printf("FAIL: %s at %s:%d\n", #cond, __FILE__, __LINE__); \
        ++g_tests_failed; \
    } else { \
        ++g_tests_passed; \
    } \
} while(0)

#define TEST_SECTION(name) printf("--- %s ---\n", name)

///////////////////////////////////////////////////////////////////////////////
// Type Traits
///////////////////////////////////////////////////////////////////////////////
void test_type_traits() {
    TEST_SECTION("type_traits");
    TEST_ASSERT(prototype::is_integral<int>::value == true);
    TEST_ASSERT(prototype::is_integral<double>::value == false);
    TEST_ASSERT(prototype::is_floating_point<float>::value == true);
    TEST_ASSERT(prototype::is_pointer<int*>::value == true);
    TEST_ASSERT(prototype::is_pointer<int>::value == false);
    TEST_ASSERT((prototype::is_same<int, int>::value == true));
    TEST_ASSERT((prototype::is_same<int, long>::value == false));
    TEST_ASSERT(prototype::is_trivially_copyable<int>::value == true);
}

///////////////////////////////////////////////////////////////////////////////
// Utility
///////////////////////////////////////////////////////////////////////////////
void test_utility() {
    TEST_SECTION("utility");
    int a = 10, b = 20;
    prototype::swap(a, b);
    TEST_ASSERT(a == 20 && b == 10);

    prototype::pair<int, double> p(1, 2.5);
    TEST_ASSERT(p.first == 1);
    TEST_ASSERT(p.second == 2.5);

    prototype::pair<int, int> p1 = prototype::make_pair(3, 4);
    TEST_ASSERT(p1.first == 3 && p1.second == 4);
}

///////////////////////////////////////////////////////////////////////////////
// Array
///////////////////////////////////////////////////////////////////////////////
void test_array() {
    TEST_SECTION("array");
    prototype::array<int, 5> arr = {{1, 2, 3, 4, 5}};
    TEST_ASSERT(arr.size() == 5);
    TEST_ASSERT(arr[0] == 1);
    TEST_ASSERT(arr[4] == 5);
    TEST_ASSERT(arr.front() == 1);
    TEST_ASSERT(arr.back() == 5);

    arr.fill(42);
    TEST_ASSERT(arr[2] == 42);
}

///////////////////////////////////////////////////////////////////////////////
// Vector
///////////////////////////////////////////////////////////////////////////////
void test_vector() {
    TEST_SECTION("vector");
    prototype::vector<int> v;
    TEST_ASSERT(v.empty());
    v.push_back(10);
    v.push_back(20);
    v.push_back(30);
    TEST_ASSERT(v.size() == 3);
    TEST_ASSERT(v[0] == 10);
    TEST_ASSERT(v[2] == 30);

    v.pop_back();
    TEST_ASSERT(v.size() == 2);

    v.clear();
    TEST_ASSERT(v.empty());
}

///////////////////////////////////////////////////////////////////////////////
// Fixed Deque
///////////////////////////////////////////////////////////////////////////////
void test_deque() {
    TEST_SECTION("fixed_deque");
    prototype::fixed_deque<int, 8> dq;
    dq.push_back(1);
    dq.push_back(2);
    dq.push_front(0);
    TEST_ASSERT(dq.size() == 3);
    TEST_ASSERT(dq.front() == 0);
    TEST_ASSERT(dq.back() == 2);

    dq.pop_front();
    TEST_ASSERT(dq.front() == 1);
    dq.pop_back();
    TEST_ASSERT(dq.back() == 1);
}

///////////////////////////////////////////////////////////////////////////////
// String
///////////////////////////////////////////////////////////////////////////////
void test_string() {
    TEST_SECTION("fixed_string");
    prototype::fixed_string<64> s("hello");
    TEST_ASSERT(s.size() == 5);
    TEST_ASSERT(s[0] == 'h');
    s.append(" world");
    TEST_ASSERT(s.size() == 11);
    TEST_ASSERT(s.find('w') == 6);
}

///////////////////////////////////////////////////////////////////////////////
// Optional
///////////////////////////////////////////////////////////////////////////////
void test_optional() {
    TEST_SECTION("optional");
    prototype::optional<int> empty;
    TEST_ASSERT(!empty.has_value());

    prototype::optional<int> val(42);
    TEST_ASSERT(val.has_value());
    TEST_ASSERT(*val == 42);
    TEST_ASSERT(val.value_or(0) == 42);
    TEST_ASSERT(empty.value_or(99) == 99);

    val.reset();
    TEST_ASSERT(!val.has_value());
}

///////////////////////////////////////////////////////////////////////////////
// Span
///////////////////////////////////////////////////////////////////////////////
void test_span() {
    TEST_SECTION("span");
    int arr[] = {10, 20, 30, 40, 50};
    prototype::span<int> s(arr, 5);
    TEST_ASSERT(s.size() == 5);
    TEST_ASSERT(s[0] == 10);
    TEST_ASSERT(s[4] == 50);

    prototype::span<int> sub = s.first(3);
    TEST_ASSERT(sub.size() == 3);
    TEST_ASSERT(sub[2] == 30);
}

///////////////////////////////////////////////////////////////////////////////
// Bitset
///////////////////////////////////////////////////////////////////////////////
void test_bitset() {
    TEST_SECTION("bitset");
    prototype::bitset<32> bs;
    TEST_ASSERT(bs.none());
    bs.set(0);
    bs.set(31);
    TEST_ASSERT(bs.count() == 2);
    TEST_ASSERT(bs.test(0));
    TEST_ASSERT(bs.test(31));
    TEST_ASSERT(!bs.test(1));
    bs.flip();
    TEST_ASSERT(bs.count() == 30);
}

///////////////////////////////////////////////////////////////////////////////
// List
///////////////////////////////////////////////////////////////////////////////
void test_list() {
    TEST_SECTION("list");
    prototype::fixed_list<int, 16> lst;
    lst.push_back(1);
    lst.push_back(2);
    lst.push_front(0);
    TEST_ASSERT(lst.size() == 3);
    TEST_ASSERT(lst.front() == 0);
    TEST_ASSERT(lst.back() == 2);
    lst.pop_front();
    TEST_ASSERT(lst.front() == 1);
}

///////////////////////////////////////////////////////////////////////////////
// Algorithms
///////////////////////////////////////////////////////////////////////////////
void test_algorithms() {
    TEST_SECTION("algorithms");
    int arr[] = {5, 3, 1, 4, 2};

    // sort
    prototype::sort(arr, arr + 5);
    TEST_ASSERT(arr[0] == 1);
    TEST_ASSERT(arr[4] == 5);

    // find
    int* it = prototype::find(arr, arr + 5, 3);
    TEST_ASSERT(it != arr + 5 && *it == 3);

    // count
    int arr2[] = {1, 2, 2, 3, 2};
    TEST_ASSERT(prototype::count(arr2, arr2 + 5, 2) == 3);

    // equal
    int a[] = {1, 2, 3};
    int b[] = {1, 2, 3};
    TEST_ASSERT(prototype::equal(a, a + 3, b));
}

///////////////////////////////////////////////////////////////////////////////
// Numeric
///////////////////////////////////////////////////////////////////////////////
void test_numeric() {
    TEST_SECTION("numeric");
    int arr[] = {1, 2, 3, 4, 5};
    TEST_ASSERT(prototype::accumulate(arr, arr + 5, 0) == 15);
    TEST_ASSERT(prototype::gcd(12, 8) == 4);
    TEST_ASSERT(prototype::lcm(4, 6) == 12);
}

///////////////////////////////////////////////////////////////////////////////
// Flat Map/Set
///////////////////////////////////////////////////////////////////////////////
void test_flat_containers() {
    TEST_SECTION("flat_set");
    prototype::flat_set<int, 16> fs;
    fs.insert(5);
    fs.insert(3);
    fs.insert(7);
    fs.insert(3); // duplicate
    TEST_ASSERT(fs.size() == 3);
    TEST_ASSERT(fs.contains(5));
    TEST_ASSERT(!fs.contains(4));

    TEST_SECTION("flat_map");
    prototype::flat_map<int, int, 16> fm;
    fm.insert(prototype::make_pair(1, 100));
    fm.insert(prototype::make_pair(3, 300));
    fm.insert(prototype::make_pair(2, 200));
    TEST_ASSERT(fm.size() == 3);
    TEST_ASSERT(fm.find(2) != fm.end());
    TEST_ASSERT(fm.find(2)->second == 200);
}

///////////////////////////////////////////////////////////////////////////////
// Atomic
///////////////////////////////////////////////////////////////////////////////
void test_atomic() {
    TEST_SECTION("atomic");
    prototype::atomic<int> a(0);
    TEST_ASSERT(a.load() == 0);
    a.store(42);
    TEST_ASSERT(a.load() == 42);
    TEST_ASSERT(a.fetch_add(8) == 42);
    TEST_ASSERT(a.load() == 50);

    int expected = 50;
    TEST_ASSERT(a.compare_exchange_strong(expected, 100));
    TEST_ASSERT(a.load() == 100);
}

///////////////////////////////////////////////////////////////////////////////
// Stack / Queue
///////////////////////////////////////////////////////////////////////////////
void test_adaptors() {
    TEST_SECTION("stack");
    prototype::stack<int, 16> stk;
    stk.push(1); stk.push(2); stk.push(3);
    TEST_ASSERT(stk.size() == 3);
    TEST_ASSERT(stk.top() == 3);
    stk.pop();
    TEST_ASSERT(stk.top() == 2);

    TEST_SECTION("queue");
    prototype::queue<int, 16> q;
    q.push(10); q.push(20); q.push(30);
    TEST_ASSERT(q.front() == 10);
    TEST_ASSERT(q.back() == 30);
    q.pop();
    TEST_ASSERT(q.front() == 20);
}

///////////////////////////////////////////////////////////////////////////////
// Main
///////////////////////////////////////////////////////////////////////////////
int main() {
    printf("=== PrototypeSTL Test Suite ===\n\n");

    test_type_traits();
    test_utility();
    test_array();
    test_vector();
    test_deque();
    test_string();
    test_optional();
    test_span();
    test_bitset();
    test_list();
    test_algorithms();
    test_numeric();
    test_flat_containers();
    test_atomic();
    test_adaptors();

    printf("\n=== Results: %d passed, %d failed ===\n",
           g_tests_passed, g_tests_failed);

    return g_tests_failed > 0 ? 1 : 0;
}
