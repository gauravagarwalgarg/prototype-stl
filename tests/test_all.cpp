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
// Hash Map
///////////////////////////////////////////////////////////////////////////////
void test_hash_map() {
    TEST_SECTION("hash_map");
    prototype::fixed_hash_map<int, int, 32> hm;
    TEST_ASSERT(hm.empty());
    TEST_ASSERT(hm.size() == 0);

    hm.insert(1, 100);
    hm.insert(2, 200);
    hm.insert(3, 300);
    TEST_ASSERT(hm.size() == 3);
    TEST_ASSERT(hm.contains(1));
    TEST_ASSERT(hm.contains(2));
    TEST_ASSERT(!hm.contains(99));

    int* val = hm.find(2);
    TEST_ASSERT(val != nullptr && *val == 200);

    // Update existing key
    hm.insert(2, 999);
    TEST_ASSERT(*hm.find(2) == 999);

    // Erase
    TEST_ASSERT(hm.erase(1));
    TEST_ASSERT(!hm.contains(1));
    TEST_ASSERT(hm.size() == 2);

    // operator[]
    hm[10] = 42;
    TEST_ASSERT(hm.contains(10));
    TEST_ASSERT(*hm.find(10) == 42);

    hm.clear();
    TEST_ASSERT(hm.empty());
}

///////////////////////////////////////////////////////////////////////////////
// Hash Set
///////////////////////////////////////////////////////////////////////////////
void test_hash_set() {
    TEST_SECTION("hash_set");
    prototype::fixed_hash_set<int, 32> hs;
    TEST_ASSERT(hs.empty());

    TEST_ASSERT(hs.insert(5));
    TEST_ASSERT(hs.insert(10));
    TEST_ASSERT(hs.insert(15));
    TEST_ASSERT(!hs.insert(5)); // duplicate
    TEST_ASSERT(hs.size() == 3);

    TEST_ASSERT(hs.contains(5));
    TEST_ASSERT(hs.contains(10));
    TEST_ASSERT(!hs.contains(7));

    TEST_ASSERT(hs.erase(10));
    TEST_ASSERT(!hs.contains(10));
    TEST_ASSERT(hs.size() == 2);

    hs.clear();
    TEST_ASSERT(hs.empty());
}

///////////////////////////////////////////////////////////////////////////////
// Disjoint Set
///////////////////////////////////////////////////////////////////////////////
void test_disjoint_set() {
    TEST_SECTION("disjoint_set");
    prototype::fixed_disjoint_set<10> ds;
    TEST_ASSERT(ds.count_sets() == 10);

    ds.unite(0, 1);
    TEST_ASSERT(ds.connected(0, 1));
    TEST_ASSERT(!ds.connected(0, 2));
    TEST_ASSERT(ds.count_sets() == 9);

    ds.unite(2, 3);
    ds.unite(0, 2);
    TEST_ASSERT(ds.connected(0, 3));
    TEST_ASSERT(ds.connected(1, 2));
    TEST_ASSERT(ds.count_sets() == 7);

    // Verify find with path compression
    TEST_ASSERT(ds.find(0) == ds.find(3));
}

///////////////////////////////////////////////////////////////////////////////
// Graph
///////////////////////////////////////////////////////////////////////////////
void test_graph() {
    TEST_SECTION("graph");

    // Undirected graph
    prototype::fixed_graph<8, 8, false> g;
    g.set_node_count(5);
    g.add_edge(0, 1);
    g.add_edge(0, 2);
    g.add_edge(1, 3);
    g.add_edge(2, 4);

    TEST_ASSERT(g.node_count() == 5);
    TEST_ASSERT(g.edge_count() == 4);
    TEST_ASSERT(g.has_edge(0, 1));
    TEST_ASSERT(g.has_edge(1, 0)); // undirected
    TEST_ASSERT(!g.has_edge(0, 3));

    // BFS from node 0
    prototype::fixed_graph<8, 8, false>::bfs_iterator bfs = g.bfs(0);
    size_t bfs_order[5];
    size_t bfs_count = 0;
    while (bfs.has_next()) {
        bfs_order[bfs_count++] = bfs.next();
    }
    TEST_ASSERT(bfs_count == 5);
    TEST_ASSERT(bfs_order[0] == 0); // starts from 0

    // Directed graph with topological sort
    prototype::fixed_graph<8, 8, true> dag;
    dag.set_node_count(4);
    dag.add_edge(0, 1);
    dag.add_edge(0, 2);
    dag.add_edge(1, 3);
    dag.add_edge(2, 3);

    size_t topo[4];
    size_t topo_count = dag.topological_sort(topo, 4);
    TEST_ASSERT(topo_count == 4);
    TEST_ASSERT(topo[0] == 0); // 0 has no incoming edges
}

///////////////////////////////////////////////////////////////////////////////
// Bloom Filter
///////////////////////////////////////////////////////////////////////////////
void test_bloom_filter() {
    TEST_SECTION("bloom_filter");
    prototype::bloom_filter<256, 3> bf;

    bf.insert(42);
    bf.insert(100);
    bf.insert(7);

    TEST_ASSERT(bf.possibly_contains(42));
    TEST_ASSERT(bf.possibly_contains(100));
    TEST_ASSERT(bf.possibly_contains(7));
    TEST_ASSERT(bf.item_count() == 3);

    // False positive rate should be low with few items
    TEST_ASSERT(bf.false_positive_rate() < 0.5);

    bf.clear();
    TEST_ASSERT(bf.item_count() == 0);
    TEST_ASSERT(bf.count() == 0);
}

///////////////////////////////////////////////////////////////////////////////
// Trie
///////////////////////////////////////////////////////////////////////////////
void test_trie() {
    TEST_SECTION("trie");
    prototype::fixed_trie<128, 26> trie;

    trie.insert("hello", 5);
    trie.insert("help", 4);
    trie.insert("world", 5);

    TEST_ASSERT(trie.search("hello", 5));
    TEST_ASSERT(trie.search("help", 4));
    TEST_ASSERT(trie.search("world", 5));
    TEST_ASSERT(!trie.search("hell", 4));
    TEST_ASSERT(!trie.search("worlds", 6));

    TEST_ASSERT(trie.starts_with("hel", 3));
    TEST_ASSERT(trie.starts_with("wor", 3));
    TEST_ASSERT(!trie.starts_with("xyz", 3));

    TEST_ASSERT(trie.erase("help", 4));
    TEST_ASSERT(!trie.search("help", 4));
    TEST_ASSERT(trie.search("hello", 5)); // should still be there
}

///////////////////////////////////////////////////////////////////////////////
// LRU Cache
///////////////////////////////////////////////////////////////////////////////
void test_lru_cache() {
    TEST_SECTION("lru_cache");
    prototype::fixed_lru_cache<int, int, 3> cache;

    cache.put(1, 10);
    cache.put(2, 20);
    cache.put(3, 30);
    TEST_ASSERT(cache.size() == 3);

    prototype::optional<int> v = cache.get(1);
    TEST_ASSERT(v.has_value() && *v == 10);

    // Adding 4th item should evict LRU (which is 2, since we just accessed 1)
    cache.put(4, 40);
    TEST_ASSERT(cache.size() == 3);
    TEST_ASSERT(!cache.contains(2)); // 2 was evicted
    TEST_ASSERT(cache.contains(1));
    TEST_ASSERT(cache.contains(3));
    TEST_ASSERT(cache.contains(4));

    // Update existing
    cache.put(1, 100);
    v = cache.get(1);
    TEST_ASSERT(v.has_value() && *v == 100);
}

///////////////////////////////////////////////////////////////////////////////
// Priority Queue
///////////////////////////////////////////////////////////////////////////////
void test_priority_queue() {
    TEST_SECTION("priority_queue");
    prototype::fixed_priority_queue<int, 16> pq;
    TEST_ASSERT(pq.empty());

    pq.push(5);
    pq.push(10);
    pq.push(3);
    pq.push(8);
    TEST_ASSERT(pq.size() == 4);
    TEST_ASSERT(pq.top() == 10); // max-heap by default

    pq.pop();
    TEST_ASSERT(pq.top() == 8);

    pq.pop();
    TEST_ASSERT(pq.top() == 5);

    pq.pop();
    TEST_ASSERT(pq.top() == 3);

    pq.pop();
    TEST_ASSERT(pq.empty());
}

///////////////////////////////////////////////////////////////////////////////
// Tuple
///////////////////////////////////////////////////////////////////////////////
void test_tuple() {
    TEST_SECTION("tuple");
    prototype::tuple<int, double, char> t(42, 3.14, 'x');

    TEST_ASSERT(prototype::get<0>(t) == 42);
    TEST_ASSERT(prototype::get<2>(t) == 'x');

    // tuple_size
    TEST_ASSERT((prototype::tuple_size<prototype::tuple<int, double, char>>::value == 3));

    // make_tuple
    auto t2 = prototype::make_tuple(1, 2, 3);
    TEST_ASSERT(prototype::get<0>(t2) == 1);
    TEST_ASSERT(prototype::get<1>(t2) == 2);
    TEST_ASSERT(prototype::get<2>(t2) == 3);

    // tie
    int a = 0, b = 0;
    prototype::tuple<int&, int&> tied = prototype::tie(a, b);
    prototype::get<0>(tied) = 10;
    prototype::get<1>(tied) = 20;
    TEST_ASSERT(a == 10);
    TEST_ASSERT(b == 20);
}

///////////////////////////////////////////////////////////////////////////////
// Expected
///////////////////////////////////////////////////////////////////////////////
void test_expected() {
    TEST_SECTION("expected");

    // Success case
    prototype::expected<int, const char*> success(42);
    TEST_ASSERT(success.has_value());
    TEST_ASSERT(success.value() == 42);
    TEST_ASSERT(*success == 42);
    TEST_ASSERT(success.value_or(0) == 42);

    // Error case
    prototype::expected<int, const char*> err(prototype::unexpect, "failed");
    TEST_ASSERT(!err.has_value());
    TEST_ASSERT(err.value_or(99) == 99);

    // Copy
    prototype::expected<int, const char*> copy = success;
    TEST_ASSERT(copy.has_value());
    TEST_ASSERT(copy.value() == 42);
}

///////////////////////////////////////////////////////////////////////////////
// String View
///////////////////////////////////////////////////////////////////////////////
void test_string_view() {
    TEST_SECTION("string_view");
    prototype::string_view sv("hello world");
    TEST_ASSERT(sv.size() == 11);
    TEST_ASSERT(sv[0] == 'h');
    TEST_ASSERT(sv[10] == 'd');

    TEST_ASSERT(sv.starts_with(prototype::string_view("hello", 5)));
    TEST_ASSERT(sv.ends_with(prototype::string_view("world", 5)));
    TEST_ASSERT(!sv.starts_with(prototype::string_view("world", 5)));

    TEST_ASSERT(sv.find('w') == 6);
    TEST_ASSERT(sv.find('z') == prototype::string_view::npos);
    TEST_ASSERT(sv.rfind('l') == 9);

    prototype::string_view sub = sv.substr(6, 5);
    TEST_ASSERT(sub.size() == 5);
    TEST_ASSERT(sub == prototype::string_view("world", 5));

    // compare
    prototype::string_view a("abc", 3);
    prototype::string_view b("abd", 3);
    TEST_ASSERT(a < b);
    TEST_ASSERT(a != b);

    // remove_prefix / remove_suffix
    prototype::string_view rs("hello", 5);
    rs.remove_prefix(2);
    TEST_ASSERT(rs == prototype::string_view("llo", 3));
    rs.remove_suffix(1);
    TEST_ASSERT(rs == prototype::string_view("ll", 2));
}

///////////////////////////////////////////////////////////////////////////////
// Binary Search Algorithms
///////////////////////////////////////////////////////////////////////////////
void test_binary_search() {
    TEST_SECTION("binary_search");
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // lower_bound
    int* lb = prototype::lower_bound(arr, arr + 10, 5);
    TEST_ASSERT(lb != arr + 10 && *lb == 5);

    lb = prototype::lower_bound(arr, arr + 10, 0);
    TEST_ASSERT(lb == arr); // first element

    // upper_bound
    int* ub = prototype::upper_bound(arr, arr + 10, 5);
    TEST_ASSERT(ub != arr + 10 && *ub == 6);

    // binary_search
    TEST_ASSERT(prototype::binary_search(arr, arr + 10, 7));
    TEST_ASSERT(!prototype::binary_search(arr, arr + 10, 11));

    // equal_range
    int arr2[] = {1, 2, 2, 2, 3, 4, 5};
    prototype::pair<int*, int*> er = prototype::equal_range(arr2, arr2 + 7, 2);
    TEST_ASSERT(*er.first == 2);
    TEST_ASSERT(er.second - er.first == 3); // three 2's

    // set_operations
    TEST_SECTION("set_operations");
    int s1[] = {1, 2, 3, 4, 5};
    int s2[] = {3, 4, 5, 6, 7};
    int out[10];

    // set_intersection
    int* end_it = prototype::set_intersection(s1, s1 + 5, s2, s2 + 5, out);
    TEST_ASSERT(end_it - out == 3); // {3, 4, 5}
    TEST_ASSERT(out[0] == 3 && out[1] == 4 && out[2] == 5);

    // set_union
    end_it = prototype::set_union(s1, s1 + 5, s2, s2 + 5, out);
    TEST_ASSERT(end_it - out == 7); // {1,2,3,4,5,6,7}

    // set_difference
    end_it = prototype::set_difference(s1, s1 + 5, s2, s2 + 5, out);
    TEST_ASSERT(end_it - out == 2); // {1, 2}
    TEST_ASSERT(out[0] == 1 && out[1] == 2);

    // includes
    int subset[] = {2, 3};
    TEST_ASSERT(prototype::includes(s1, s1 + 5, subset, subset + 2));
    int not_subset[] = {2, 6};
    TEST_ASSERT(!prototype::includes(s1, s1 + 5, not_subset, not_subset + 2));

    // minmax
    TEST_SECTION("minmax");
    int mm[] = {5, 2, 8, 1, 9, 3};
    int* min_it = prototype::min_element(mm, mm + 6);
    int* max_it = prototype::max_element(mm, mm + 6);
    TEST_ASSERT(*min_it == 1);
    TEST_ASSERT(*max_it == 9);

    prototype::pair<int*, int*> mmr = prototype::minmax_element(mm, mm + 6);
    TEST_ASSERT(*mmr.first == 1);
    TEST_ASSERT(*mmr.second == 9);
}

///////////////////////////////////////////////////////////////////////////////
// Circular Buffer
///////////////////////////////////////////////////////////////////////////////
void test_circular_buffer() {
    TEST_SECTION("circular_buffer");
    prototype::fixed_circular_buffer<int, 3> cb;
    TEST_ASSERT(cb.empty());

    cb.push(1);
    cb.push(2);
    cb.push(3);
    TEST_ASSERT(cb.full());
    TEST_ASSERT(cb.size() == 3);
    TEST_ASSERT(cb.peek() == 1);

    // Overwrite oldest
    cb.push(4);
    TEST_ASSERT(cb.size() == 3);
    TEST_ASSERT(cb.peek() == 2); // 1 was overwritten

    prototype::optional<int> v = cb.pop();
    TEST_ASSERT(v.has_value() && *v == 2);
    TEST_ASSERT(cb.size() == 2);
}

///////////////////////////////////////////////////////////////////////////////
// Skip List
///////////////////////////////////////////////////////////////////////////////
void test_skip_list() {
    TEST_SECTION("skip_list");
    prototype::fixed_skip_list<int, 32> sl;
    TEST_ASSERT(sl.empty());

    TEST_ASSERT(sl.insert(5));
    TEST_ASSERT(sl.insert(3));
    TEST_ASSERT(sl.insert(7));
    TEST_ASSERT(sl.insert(1));
    TEST_ASSERT(!sl.insert(5)); // duplicate
    TEST_ASSERT(sl.size() == 4);

    TEST_ASSERT(sl.find(5));
    TEST_ASSERT(sl.find(1));
    TEST_ASSERT(!sl.find(4));

    // lower_bound
    prototype::optional<int> lb = sl.lower_bound(4);
    TEST_ASSERT(lb.has_value() && *lb == 5);

    lb = sl.lower_bound(1);
    TEST_ASSERT(lb.has_value() && *lb == 1);

    // erase
    TEST_ASSERT(sl.erase(3));
    TEST_ASSERT(!sl.find(3));
    TEST_ASSERT(sl.size() == 3);
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
    test_hash_map();
    test_hash_set();
    test_disjoint_set();
    test_graph();
    test_bloom_filter();
    test_trie();
    test_lru_cache();
    test_priority_queue();
    test_tuple();
    test_expected();
    test_string_view();
    test_binary_search();
    test_circular_buffer();
    test_skip_list();

    printf("\n=== Results: %d passed, %d failed ===\n",
           g_tests_passed, g_tests_failed);

    return g_tests_failed > 0 ? 1 : 0;
}
