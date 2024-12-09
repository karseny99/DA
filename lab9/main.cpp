#include <vector>
#include <iostream>
#include <queue>
#include <limits>

namespace task {

using ll = int64_t;
struct wedge {
    int u, v;
    ll w;

    wedge (int _u, int _v, ll _w) : u(_u), v(_v), w(_w) {}
};

using wgraph = std::vector<std::vector<wedge>>;

using item = std::pair<ll, int>;  


void dijkstra(int u, const wgraph& g, std::vector<ll>& d) {
    int n = g.size();

    d[u] = 0;

    std::priority_queue<item, std::vector<item>, std::greater<item>> pq; 
    pq.push(std::make_pair(d[u], u));

    std::vector<bool> visited(n, false);
    
    while(!pq.empty()) {
        item cur = pq.top();
        pq.pop();
        u = cur.second;
        if(visited[u]) continue;

        visited[u] = true;

        for(wedge uv : g[u]) {
            int v = uv.v;
            ll w = uv.w;

            if(d[u] + w < d[v]) {
                d[v] = d[u] + w;
                pq.push(std::make_pair(d[v], v)); 
            }

        }
    }

}

};

int main() {
    std::ios::sync_with_stdio(false);

    task::ll n, m, start, finish;
    std::cin >> n >> m >> start >> finish;
    --start; --finish;

    task::wgraph g(n);

    for(int i{0}; i < m; ++i) {
        task::ll u, v, w;
        std::cin >> u >> v >> w;
        --u; --v;

        g[u].push_back({u, v, w});
        g[v].push_back({v, u, w});
    }

    std::vector<task::ll> d(n, std::numeric_limits<task::ll>::max());
    task::dijkstra(start, g, d);

    if(d[finish] != std::numeric_limits<task::ll>::max()) 
        std::cout << d[finish] << std::endl;
    else
        std::cout << "No solution\n";
}