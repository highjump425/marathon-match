#pragma GCC optimize("Ofast")
#include <iostream>
#include <algorithm>
#include <string>
#include <cstdlib>
#include <cmath>
#include <iomanip>
#include <cctype>
#include <sstream>
#include <vector>
#include <stack>
#include <deque>
#include <queue>
#include <list>
#include <set>
#include <map>
#include <unordered_map>  
#include <chrono>
#include <random>

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

chrono::system_clock::time_point t_start;
const double TIME_LIMIT1 = 500, TIME_LIMIT = 2930;
double last_update = 0, t_diff;
double start_temp, end_temp;

mt19937 rng;
using uni_int = uniform_int_distribution<>;
using uni_real = uniform_real_distribution<>;

V<ll> y_vec = { 0, 1,0, -1 };
V<ll> x_vec = { -1, 0,1, 0 };

/*
double prob = exp((next_score-current_score) / get_temp());
if (prob > dist(rng)) {
*/

void get_time() {
    auto t_now = chrono::system_clock::now();
    t_diff = chrono::duration_cast<chrono::milliseconds>(t_now - t_start).count();
}

double get_temp() {
    start_temp = 10000; end_temp = 0.1;
    return start_temp + (end_temp - start_temp) * (t_diff /TIME_LIMIT);
}

signed main() {
    cin.tie(0);
    ios::sync_with_stdio(false);
    cout << fixed << setprecision(15);
    cerr << fixed << setprecision(15);
    t_start = chrono::system_clock::now();

    get_time();

    cerr << "time:  " << int(t_diff) << " ms" << endl;
    //cerr << "score: " << score << endl;
    //cerr << "last:  " << last_update << endl;
    //cerr << "cnt:   " << cnt << endl;
    //cerr << "update:" << update_cnt << endl;
    return 0;
}
