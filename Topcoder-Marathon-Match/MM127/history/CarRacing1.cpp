//#pragma GCC optimize("Ofast")
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <iomanip>
#include <cctype>
#include <sstream>
#include <stack>
#include <deque>
#include <queue>
#include <list>
#include <set>
#include <map>
#include <chrono>
#include <random>
#include <unordered_map>  

using namespace std;

using ll = long long;
using P = pair<int, int>;
template <class T>using V = vector<T>;
template <class T>using VV = V<V<T>>;
template <class T, class U>bool chmin(T& t, const U& u) { if (t > u) { t = u; return 1; } else return 0; }
template <class T, class U>bool chmax(T& t, const U& u) { if (t < u) { t = u; return 1; } else return 0; }

#define REP(i,n) for(int i=0;i<int(n);i++)
#define FOR(i,a,b) for(int i=int(a);i<=int(b);i++)
#define FORD(i,a,b) for(int i=int(a);i>=int(b);i--)
#define MP make_pair
#define SZ(x) int(x.size())
#define ALL(x) x.begin(),x.end()
#define INF 10001000
#define endl "\n"

mt19937 rng;
V<ll> ans;
string result;
int N, K, cnt = 0;


signed main() {
    cin.tie(0);
    ios::sync_with_stdio(false);
    cout << fixed << setprecision(15);

    cin >> N>>K;
    cerr << N << " " << K << endl;

    string out = "";
    getline(cin, result); // read the endline

    ans = V<ll>(N);
    REP(i, N)ans[i] = i;

    //selection sort
    REP(i,N){
        int pos = i;
        int zan = i;
        int next = i + 1;
        while (next < N) {
            int num = min(N - 1, next + K - 2) - next + 2;
            string race = to_string(ans[zan]) + " ";
            FOR(j, next, min(N - 1, next + K - 2)) {
                race += to_string(ans[j]) + " ";
            }
            cout << race << endl;
            cout.flush();
            cnt++;
            getline(cin, result);
            stringstream res{ result };
            string rank;
            int j = 0;
            while (getline(res, rank, ' ')) {
                if (rank == "0") {
                    if (j == 0)break;
                    zan = next + j - 1;
                    break;
                }
                j++;
            }
            //cerr << zan << " " << next << " " << cnt << endl;
            next = min(N - 1, next + K - 2) + 1;
        }

        if (pos != zan)swap(ans[pos], ans[zan]);
    }
    REP(i, N)cout << ans[i] << " ";
    cout << endl;
    cout.flush();

}
