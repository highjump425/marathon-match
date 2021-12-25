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

class UnionFind {
public:
    vector<ll> parent;
    vector<ll> siz;
    map<ll, vector<ll> > group;
    ll n;

    UnionFind(ll n_) :n(n_), parent(n_), siz(n_, 1) {
        for (ll i = 0; i < n; i++) { parent[i] = i; }
    }

    ll root(ll x) {
        if (parent[x] == x) return x;
        return parent[x] = root(parent[x]);
    }

    bool unite(ll x, ll y) {
        ll rx = root(x);
        ll ry = root(y);
        if (rx == ry) return false;
        if (siz[rx] < siz[ry]) swap(rx, ry);
        siz[rx] += siz[ry];
        parent[ry] = rx;
        return true;
    }

    bool same(ll x, ll y) {
        return root(x) == root(y);
    }

    ll size(ll x) {
        return siz[root(x)];
    }
};


void get_time() {
    auto t_now = chrono::system_clock::now();
    t_diff = chrono::duration_cast<chrono::milliseconds>(t_now - t_start).count();
}

double get_temp() {
    start_temp = 10000; end_temp = 0.1;
    return start_temp + (end_temp - start_temp) * (t_diff /TIME_LIMIT);
}

class Edge {
public:
    int id = -1;
    int length = 0;
    int left = 0;
    int right = 0;

    Edge() {};
    Edge(int i,int n, int x, int y) :id(i),length(n), left(x), right(y) {};

    bool operator <(const Edge& r)const {
        if (length != r.length)return length < r.length;
        if (left != r.left)return left < r.left;
        if (right != r.right)return right < r.right;
        return id < r.id;
    }
};

int N = 400;
int M = 1995;
VV<P> graph(N);
V<P> vertex(N);
V<Edge> edges(M);
V<Edge> adjust(M);
V<Edge> real(M);
V<int> check(M);

int get_distance(int u, int v) {
    return round(sqrt((vertex[u].first - vertex[v].first) * (vertex[u].first - vertex[v].first) +
        (vertex[u].second - vertex[v].second) * (vertex[u].second - vertex[v].second)));
}

uni_real dist_real(0,1.0);

int get_tree(UnionFind u,V<Edge> edges,int turn,int temp) {
    int res = temp;
    check = V<int>(M);
    sort(ALL(edges));
    for (Edge e : edges) {
        if (e.id < turn)continue;
        if (u.same(e.left, e.right))continue;
        //if (dist_real(rng) > 0.999)continue;
        check[e.id] = 1;
        u.unite(e.left, e.right);
        res += e.length;
    }
    return res;
}


int get_ans(UnionFind &u, V<Edge> &edges) {
    int res = 0;
    sort(ALL(edges));
    for (Edge e : edges) {
        if (u.same(e.left, e.right))continue;
        u.unite(e.left, e.right);
        res += e.length;
    }
    return res;
}

bool can_skip(UnionFind u, V<Edge> &edges,int turn) {
    FOR(i, turn+1, M-1) {
        //if (u.same(edges[i].left, edges[i].right))continue;
        u.unite(edges[i].left, edges[i].right);
    }
    return u.size(0) == N;
}

int cnt;
double par0 = 1.7,par1 = 1.9;
double get_pal(int turn) {
    return  par0 + (par1-par0) * cnt/N;
    //return par0 + dist_real(rng);
}

signed main() {
    cin.tie(0);
    ios::sync_with_stdio(false);
    cout << fixed << setprecision(15);
    cerr << fixed << setprecision(15);
    t_start = chrono::system_clock::now();

    get_time();

    REP(i, N) {
        cin >> vertex[i].first >> vertex[i].second;
    }
    REP(i, M) {
        int u, v; cin >> u >> v;
        if (u > v)swap(u, v);
        int dis = get_distance(u, v);
        //cerr << dis << endl;
        graph[u].emplace_back(dis,v);
        graph[v].emplace_back(dis,u);
        edges[i] = Edge(i,int(double(dis)), u, v);
    }
    real = edges;
    adjust = edges;
    UnionFind u(N);

    int score = 0;
    int expect=get_tree(u, edges,0,0);
    cerr << expect << endl;
    REP(i, M) {
        int ans; cin >> ans;
        int len = edges[i].length;
        real[i].length = ans;
        edges[i].length = ans;

        if (u.same(edges[i].left, edges[i].right)) {
            cout << 0 << endl;
            cout.flush();
            continue;
        }
        if (!can_skip(u,edges,i)) {
            cout << 1 << endl;
            cout.flush();
            score += ans;
            cnt++;
            u.unite(edges[i].left, edges[i].right);
            continue;
        }

        adjust[i].length = ans;
        FOR(j, i + 1, M - 1)adjust[j].length = int(double(edges[j].length) * get_pal(i));
        get_tree(u, adjust, i, score);

        if (check[i]) {
            cout << 1 << endl;
            cout.flush();
            score += ans;
            cnt++;
            u.unite(edges[i].left, edges[i].right);
            continue;
        }
        else {
            cout << 0 << endl;
            cout.flush();
        }
    }
    get_time();
    UnionFind f(N);
    int ideal = get_ans(f, real);

    cerr << ideal << endl;

    cerr << "time:  " << int(t_diff) << " ms" << endl;
    cerr << "length: " << score << endl;
    cerr << "Score : " << int(round(double(ideal)/score*100000000)) << endl;

    //cerr << "last:  " << last_update << endl;
    //cerr << "cnt:   " << cnt << endl;
    //cerr << "update:" << update_cnt << endl;
    return 0;
}
