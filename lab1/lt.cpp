#include <bits/stdc++.h>


using namespace std;
using ll = int64_t;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using graph = vector< vector<int> >;

#define all(x) x.begin(), x.end()

const int MOD = 1e9 + 7;
const ll INF = 1e18;

std::string simplifyPath(const std::string &path)
{
    std::string s;
    std::cin >> s;
    
    std::vector<std::string> result;
    
    int n = s.size();
    for(int i = 0; i < n; ++i) {
        
        if(s[i] == '/') {
            ++i;
            std::string tmp = "";
            while(s[i] != '/') {
                tmp += s[i];
                ++i;
            }
            --i;
            cout << tmp << endl;
            if(tmp == "" or tmp == "." or tmp == "..") {
                continue;
            } else {
                result.push_back(tmp);
            }
        }   
    }
    
    std::string r = "/";
    for(const auto& el : result) {
        r += el;
        r += '/';
    }
    
    return r;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);


    // int t;
    // cin >> t;

    cout << simplifyPath("/home/users/../bobo/.//cat/");

}