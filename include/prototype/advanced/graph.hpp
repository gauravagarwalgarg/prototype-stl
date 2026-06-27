///////////////////////////////////////////////////////////////////////////////
// include/prototype/advanced/graph.hpp
//
// PrototypeSTL - Fixed-capacity adjacency-list graph with BFS/DFS/Topo sort
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTOTYPE_ADVANCED_GRAPH_HPP
#define PROTOTYPE_ADVANCED_GRAPH_HPP

#include "../config.hpp"
#include "../containers/span.hpp"

namespace prototype {

template<size_t MaxNodes, size_t MaxEdgesPerNode, bool Directed = false>
class fixed_graph {
public:
    typedef size_t size_type;
    typedef size_t node_type;

    fixed_graph() : node_count_(0), edge_count_(0) {
        for (size_t i = 0; i < MaxNodes; ++i) {
            adj_count_[i] = 0;
        }
    }

    /// Add a node. Returns its index.
    node_type add_node() {
        PROTOTYPE_ASSERT(node_count_ < MaxNodes);
        return node_count_++;
    }

    /// Ensure at least n nodes exist
    void set_node_count(size_t n) {
        PROTOTYPE_ASSERT(n <= MaxNodes);
        if (n > node_count_) node_count_ = n;
    }

    /// Add edge from u to v
    void add_edge(node_type u, node_type v) {
        PROTOTYPE_ASSERT(u < node_count_ && v < node_count_);
        PROTOTYPE_ASSERT(adj_count_[u] < MaxEdgesPerNode);
        adj_[u][adj_count_[u]++] = v;
        ++edge_count_;
        if (!Directed) {
            PROTOTYPE_ASSERT(adj_count_[v] < MaxEdgesPerNode);
            adj_[v][adj_count_[v]++] = u;
        }
    }

    /// Remove edge from u to v (first occurrence)
    bool remove_edge(node_type u, node_type v) {
        bool removed = remove_directed_edge(u, v);
        if (removed && !Directed) {
            remove_directed_edge(v, u);
        }
        if (removed) --edge_count_;
        return removed;
    }

    /// Check if edge u->v exists
    bool has_edge(node_type u, node_type v) const {
        PROTOTYPE_ASSERT(u < node_count_);
        for (size_t i = 0; i < adj_count_[u]; ++i) {
            if (adj_[u][i] == v) return true;
        }
        return false;
    }

    /// Get neighbors of a node
    span<const node_type> neighbors(node_type u) const {
        PROTOTYPE_ASSERT(u < node_count_);
        return span<const node_type>(adj_[u], adj_count_[u]);
    }

    size_type node_count() const { return node_count_; }
    size_type edge_count() const { return edge_count_; }

    ///////////////////////////////////////////////////////////////////////////
    // BFS iterator
    ///////////////////////////////////////////////////////////////////////////
    class bfs_iterator {
    public:
        bfs_iterator(const fixed_graph& g, node_type start)
            : graph_(g), front_(0), back_(0) {
            for (size_t i = 0; i < MaxNodes; ++i) visited_[i] = false;
            if (start < g.node_count_) {
                queue_[back_++] = start;
                visited_[start] = true;
            }
        }

        bool has_next() const { return front_ < back_; }

        node_type next() {
            PROTOTYPE_ASSERT(has_next());
            node_type cur = queue_[front_++];
            span<const node_type> nbrs = graph_.neighbors(cur);
            for (size_t i = 0; i < nbrs.size(); ++i) {
                node_type n = nbrs[i];
                if (!visited_[n]) {
                    visited_[n] = true;
                    PROTOTYPE_ASSERT(back_ < MaxNodes);
                    queue_[back_++] = n;
                }
            }
            return cur;
        }

    private:
        const fixed_graph& graph_;
        node_type queue_[MaxNodes];
        bool      visited_[MaxNodes];
        size_t    front_;
        size_t    back_;
    };

    bfs_iterator bfs(node_type start) const {
        return bfs_iterator(*this, start);
    }

    ///////////////////////////////////////////////////////////////////////////
    // DFS iterator
    ///////////////////////////////////////////////////////////////////////////
    class dfs_iterator {
    public:
        dfs_iterator(const fixed_graph& g, node_type start)
            : graph_(g), top_(0) {
            for (size_t i = 0; i < MaxNodes; ++i) visited_[i] = false;
            if (start < g.node_count_) {
                stack_[top_++] = start;
            }
        }

        bool has_next() const { return top_ > 0; }

        node_type next() {
            PROTOTYPE_ASSERT(has_next());
            while (top_ > 0) {
                node_type cur = stack_[--top_];
                if (visited_[cur]) continue;
                visited_[cur] = true;
                span<const node_type> nbrs = graph_.neighbors(cur);
                for (size_t i = nbrs.size(); i > 0; --i) {
                    node_type n = nbrs[i - 1];
                    if (!visited_[n]) {
                        PROTOTYPE_ASSERT(top_ < MaxNodes * MaxEdgesPerNode);
                        stack_[top_++] = n;
                    }
                }
                return cur;
            }
            PROTOTYPE_PANIC("DFS exhausted");
            return 0;
        }

    private:
        const fixed_graph& graph_;
        node_type stack_[MaxNodes * MaxEdgesPerNode];
        bool      visited_[MaxNodes];
        size_t    top_;
    };

    dfs_iterator dfs(node_type start) const {
        return dfs_iterator(*this, start);
    }

    ///////////////////////////////////////////////////////////////////////////
    // Topological sort (Kahn's algorithm, for directed graphs)
    ///////////////////////////////////////////////////////////////////////////
    /// Returns number of nodes written to output. If less than node_count, cycle exists.
    size_t topological_sort(node_type* output, size_t output_capacity) const {
        PROTOTYPE_ASSERT(Directed);
        PROTOTYPE_ASSERT(output_capacity >= node_count_);

        size_t in_degree[MaxNodes];
        for (size_t i = 0; i < node_count_; ++i) in_degree[i] = 0;

        for (size_t u = 0; u < node_count_; ++u) {
            for (size_t e = 0; e < adj_count_[u]; ++e) {
                in_degree[adj_[u][e]]++;
            }
        }

        node_type queue[MaxNodes];
        size_t qfront = 0, qback = 0;
        for (size_t i = 0; i < node_count_; ++i) {
            if (in_degree[i] == 0) queue[qback++] = i;
        }

        size_t count = 0;
        while (qfront < qback) {
            node_type u = queue[qfront++];
            output[count++] = u;
            for (size_t e = 0; e < adj_count_[u]; ++e) {
                node_type v = adj_[u][e];
                if (--in_degree[v] == 0) {
                    queue[qback++] = v;
                }
            }
        }
        return count;
    }

private:
    bool remove_directed_edge(node_type u, node_type v) {
        for (size_t i = 0; i < adj_count_[u]; ++i) {
            if (adj_[u][i] == v) {
                adj_[u][i] = adj_[u][adj_count_[u] - 1];
                --adj_count_[u];
                return true;
            }
        }
        return false;
    }

    node_type adj_[MaxNodes][MaxEdgesPerNode];
    size_t    adj_count_[MaxNodes];
    size_type node_count_;
    size_type edge_count_;
};

} // namespace prototype

#endif // PROTOTYPE_ADVANCED_GRAPH_HPP
