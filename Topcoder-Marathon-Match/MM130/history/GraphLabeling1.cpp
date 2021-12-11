#pragma GCC optimize("Ofast")
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
#include <bitset>

using namespace std;

using ll = long long;
using P = pair<int, int>;
template <class T>using V = vector<T>;
template <class T>using VV = V<V<T>>;
using PQ = priority_queue<pair<P, V<int>>>;

template <class T, class U>bool chmin(T& t, const U& u) { if (t > u) { t = u; return 1; } else return 0; }
template <class T, class U>bool chmax(T& t, const U& u) { if (t < u) { t = u; return 1; } else return 0; }

#define REP(i,n) for(int i=0;i<int(n);i++)
#define FOR(i,a,b) for(int i=int(a);i<=int(b);i++)
#define FORD(i,a,b) for(int i=int(a);i>=int(b);i--)
#define MP make_pair
#define SZ(x) int(x.size())
#define ALL(x) x.begin(),x.end()
#define CINF 1ll<<30
#define INF 10001000
#define endl "\n"


chrono::system_clock::time_point t_start;
const double TIME_LIMIT = 9950;
double last_update = 0, t_diff;
double start_temp, end_temp;

mt19937 rng;
uniform_real_distribution<> dist(0.0, 1.0);
uniform_int_distribution<> dist_int(0, 3);

//V<int> x_vec = { -1, 0,1, 0 };
//V<int> y_vec = { 0, 1,0, -1 };

/*
double prob = exp((next_score-current_score) / get_temp());
if (prob > dist(rng)) {
*/

int N, E;
V<int> nodes;
V<P> edges;
VV<int> graph;
V<int> pool;
V<bool> check_node;
V<bool> check_edge;


void get_time() {
    auto t_now = chrono::system_clock::now();
    t_diff = chrono::duration_cast<chrono::milliseconds>(t_now - t_start).count();
}

double get_temp() {
    start_temp = 10000; end_temp = 0.1;
    return start_temp + (end_temp - start_temp) * (t_diff / TIME_LIMIT);
}

void init() {
    cin.tie(0);
    ios::sync_with_stdio(false);

    cin >> N >> E;
    nodes = V<int>(N, -1);
    edges = V<P>(E);
    graph = VV<int>(N);
    check_node = V<bool>(2100000);
    check_edge = V<bool>(2100000);
    REP(i, E) {
        int a, b; cin >> a >> b;
        if (a > b)swap(a, b);
        edges[i] = MP(a, b);
        graph[a].emplace_back(b);
        graph[b].emplace_back(a);
    }
}

void greedy() {
    queue<int> q;
    q.push(0);
    int min_node = 0;
    while (!q.empty()) {
        int x = q.front();
        q.pop();
        if (nodes[x] != -1)continue;
        FOR(p, min_node, CINF) {
            bool flag = true;
            for (int i : graph[x]) {
                if (nodes[i] == -1)continue;
                if (check_edge[p - nodes[i]]) {
                    while (check_edge[p - nodes[i]])p++;
                    p--;
                    flag = false;
                    break;
                }
            }
            if (!flag)continue;
            for (int i : graph[x]) {
                if (nodes[i] == -1)continue;
                check_edge[abs(nodes[i] - p)] = true;
            }
            nodes[x] = p;
            check_node[p] = true;
            min_node = p + 1;
            break;
        }
        for (int i : graph[x]) {
            if (nodes[i] != -1)continue;
            q.push(i);
        }
    }
}

signed main() {
    cin.tie(0);
    ios::sync_with_stdio(false);
    //cout << fixed << setprecision(15);
    t_start = chrono::system_clock::now();

    init();
    greedy();
    

    REP(i, N)cout << nodes[i] << " ";
    cout << endl;

    //sort(ALL(nodes));
    //REP(i, N-1)cerr << nodes[i+1]-nodes[i] << endl;

    get_time();

    cerr << "Node: " << N << endl;
    cerr << "Edge: " << E << endl;
    //cerr << "time:  " << t_diff << endl;
    //cerr << "score: " << score - price[SZ(machine)] << endl;
    //cerr << "last:  " << last_update << endl;
    //cerr << "cnt:   " << cnt << endl;
    //cerr << "update:" << update_cnt << endl;
    return 0;
}
