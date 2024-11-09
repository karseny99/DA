#include <iostream>
#include <vector>
#include <deque>


int main() {
    std::ios::sync_with_stdio(false);
    size_t m , n;
    std::cin >> n >> m;

    std::vector<size_t> w(n + 1);
    std::vector<size_t> c(n + 1);

    for(size_t i{1}; i <= n; ++i) {
        std::cin >> w[i] >> c[i];
    }

    /* 

        dp[i][j][k] = {
            i - set of items from 1..i
            j - num of taken items from 1..i 
            k - weights are less or equal k
        }

        dp[i][j][k] = max(
            dp[i - 1][j][k], 
            (dp[i - 1][j - 1][k - w[i]] / (j - 1) + c[i]) * j
        )

    */

    std::vector<std::vector<std::vector<size_t>>> dp(n + 1, std::vector<std::vector<size_t>>(n + 1, std::vector<size_t>(m + 1, 0)));

    for(size_t i{1}; i <= n; ++i) {
        for(size_t k{1}; k <= m; ++k) {
            dp[i][1][k] = dp[i - 1][1][k];

            if(k < w[i]) continue;
            
            if(dp[i][1][k] < dp[i - 1][1][k - w[i]] + c[i]) {
                dp[i][1][k] = dp[i - 1][1][k - w[i]] + c[i];
            }
        }
    }

    for(size_t i{2}; i <= n; ++i) {
        for(size_t j{2}; j <= i; ++j) {
            for(size_t k{1}; k <= m; ++k) {

                dp[i][j][k] = dp[i - 1][j][k];

                if(k < w[i]) continue; 
                // can't take such items together
                if(dp[i - 1][j - 1][k - w[i]] == 0) continue;
                
                if(dp[i][j][k] < (dp[i - 1][j - 1][k - w[i]] / (j - 1) + c[i]) * j) {
                    dp[i][j][k] = (dp[i - 1][j - 1][k - w[i]] / (j - 1) + c[i]) * j;
                }

            }
        }
    }
    
    size_t itemsAmount = 0;
    for(size_t j{1}; j <= n; ++j) {
        if(dp[n][j][m] > dp[n][itemsAmount][m]) 
            itemsAmount = j;
    }

    std::deque<size_t> path;
    size_t i = n, j = itemsAmount, k = m;
    while(dp[i][j][k] > 0) {
        if(dp[i][j][k] == dp[i - 1][j][k]) {
            --i;
        } else {
            path.emplace_front(i);
            k -= w[i];
            --j;
            --i;
        }
    }

    std::cout << dp[n][itemsAmount][m] << '\n';
    for(size_t e : path)
        std::cout << e << ' ';
    std::cout << '\n';
}