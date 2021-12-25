<<<<<<< HEAD
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
const double TIME_LIMIT = 9950,TIME_LIMIT2=9500;
double last_update = 0, t_diff;
double start_temp, end_temp;
int cnt = 0;

mt19937 rng;
uniform_real_distribution<> dist(0.0, 1.0);
uniform_int_distribution<> dist_int(0, 3);

//V<int> x_vec = { -1, 0,1, 0 };
//V<int> y_vec = { 0, 1,0, -1 };

/*
double prob = exp((next_score-current_score) / get_temp());
if (prob > dist(rng)) {
*/

int mode = 0;
VV<int> graph, new_graph;

class state {
public:
    int now = 0;
    int id = 0;
    state() {}
    bool operator<(const state& l) const{
        if (mode == 0) {
            if (double(this->now) / double(SZ(graph[this->id])) != double(l.now) / double(SZ(graph[l.id]))) {
                return double(this->now) / double(SZ(graph[this->id])) < double(l.now) / double(SZ(graph[l.id]));
            }
            else {
                if (SZ(graph[this->id]) != SZ(graph[l.id]))return SZ(graph[this->id]) > SZ(graph[l.id]);
                else {
                    if (this->now != l.now)return this->now < l.now;
                    else {
                        return this->id < l.id;
                    }
                }
            }
        }
        else if (mode == 1) {
            if (double(this->now) / double(SZ(graph[this->id])) != double(l.now) / double(SZ(graph[l.id]))) {
                return double(this->now) / double(SZ(graph[this->id])) < double(l.now) / double(SZ(graph[l.id]));
            }
            else {
                if (SZ(graph[this->id]) != SZ(graph[l.id]))return SZ(graph[this->id]) < SZ(graph[l.id]);
                else {
                    if (this->now != l.now)return this->now < l.now;
                    else {
                        return this->id < l.id;
                    }
                }
            }
        }
        else if (mode == 2) {
            if (this->now != l.now)return this->now < l.now;
            else {
                if (SZ(graph[this->id]) != SZ(graph[l.id]))return SZ(graph[this->id]) > SZ(graph[l.id]);
                else {
                    return this->id < l.id;
                }
            }
        }
        else if (mode == 3){
            if (this->now != l.now)return this->now < l.now;
            else {
                if (SZ(graph[this->id]) != SZ(graph[l.id]))return SZ(graph[this->id]) < SZ(graph[l.id]);
                else {
                    return this->id < l.id;
                }
            }
        }
    }
};

int N, E;
int ans = INF, best_ans = INF;
V<int> nodes,best_nodes,order, best_order;
V<P> edges;
V<int> pool;
V<bool> check_node;
V<bool> check_edge;
V<bool> edge_pool;
V<state> states;
V<int> edge_num;

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
    best_nodes = V<int>(N, -1);
    order = V<int>(N);
    edges = V<P>(E);
    graph = VV<int>(N);
    check_node = V<bool>(2100000);
    check_edge = V<bool>(2100000);
    edge_pool = V<bool>(2100000);
    REP(i, E) {
        int a, b; cin >> a >> b;
        if (a > b)swap(a, b);
        edges[i] = MP(a, b);
        graph[a].emplace_back(b);
        graph[b].emplace_back(a);
    }
    REP(i, N)order[i] = i;
    states = V<state>(N);
    edge_num = V<int>(N);
    REP(i, N) {
        states[i].id = i;
    }
}

bool validate() {
    edge_pool = V<bool>(2100000);
    REP(i, E) {
        if (edge_pool[abs(nodes[edges[i].first] - nodes[edges[i].second])])return false;
        else edge_pool[abs(nodes[edges[i].first] - nodes[edges[i].second])] = true;
    }
    return true;
}

void greedy(int start) {
    queue<int> q;
    q.push(start);
    int min_node = 0;
    while (!q.empty()) {
        int x = q.front();
        q.pop();
        if (nodes[x] != -1)continue;
        FOR(p, min_node, CINF) {
            bool flag = true;
            for (int i : graph[x]) {
                if (nodes[i] == -1)continue;
                if (check_edge[abs(p - nodes[i])]) {
                    while (check_edge[abs(p - nodes[i])])p++;
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

bool greedy2() {
    new_graph = VV<int>(N);
    int min_node = 0;
    for(int x:order){  
        if (nodes[x] != -1) {
            for (int i : graph[x]) {
                if (nodes[i] == -1)new_graph[i].emplace_back(x);
                else check_edge[abs(nodes[i] - nodes[x])] = true;
            }
            min_node = nodes[x] + 1;
            continue;
        }
        FOR(p, min_node, CINF) {
            bool flag = true;
            if (p > best_ans) {
                nodes[x] = 2099999;
                return false;
            }
            FORD(k, SZ(new_graph[x]) - 1, 0) {
                int i = new_graph[x][k];
                if (check_edge[abs(p - nodes[i])]) {
                    while (check_edge[abs(p - nodes[i])])p++;
                    p--;
                    flag = false;
                    break;
                }
            }
            if (!flag)continue;
            for (int i : graph[x]) {
                if (nodes[i] == -1)new_graph[i].emplace_back(x);
                else check_edge[abs(nodes[i] - p)] = true;
            }
            nodes[x] = p;
            min_node = p + 1;
            break;
        }
    }
    return true;
}

void greedy3() {
    new_graph = VV<int>(N);
    REP(i, N)if (edge_num[i] != 0)edge_num[i] = 0;
    REP(i, N)if (states[i].now != 0)states[i].now = 0;
    
    sort(ALL(states));
    bool fast = false;
    REP(p, CINF) {
        if (p % 100000 == 0) {
            get_time();
            if (t_diff > TIME_LIMIT2)fast = true;
        }
        bool flag = false;
        REP(i, N) {
            int x = states[i].id;
            if (nodes[x] != -1)break;
            bool flag2 = true;
            FORD(k, SZ(new_graph[x]) - 1, 0) {
                int j = new_graph[x][k];
                if (check_edge[abs(p - nodes[j])]) {
                    flag2 = false;
                    break;
                }
            }
            if (!flag2) {
                if (!fast)continue;
                else break;
            }
            for (int j : graph[x]) {
                if (nodes[j] == -1) {
                    edge_num[j]++;
                    new_graph[j].emplace_back(x);
                }
                else check_edge[abs(nodes[j] - p)] = true;
            }

            edge_num[x] = N;
            nodes[x] = p;
            //check_node[p] = true;
            flag = true;
            break;
        }
        if (!flag)continue;
        REP(i, N) {
            if (states[i].now >= N)break;
            states[i].now = edge_num[states[i].id];
        }
        sort(ALL(states));

        if (states[0].now >= N)break;
    }
}

void greedy4() {
    mode = 2;
    new_graph = VV<int>(N);
    REP(i, N)if (edge_num[i] != 0)edge_num[i] = 0;
    REP(i, N)if (states[i].now != 0)states[i].now = 0;
    REP(i, N)nodes[i] = INF;
    sort(ALL(states));
    bool flag, flag2;
    bool fast = false;
    bool up = true;
    int p_ma = 0,p_mi=-1;
    while (1) {
        if (up) {
            FOR(p, p_ma, CINF) {
                if (p % 100000 == 0) {
                    get_time();
                    if (t_diff > TIME_LIMIT2)fast = true;
                }
                flag = false;
                REP(i, N) {
                    int x = states[i].id;
                    if (nodes[x] != INF)break;
                    flag2 = true;
                    FORD(k, SZ(new_graph[x]) - 1, 0) {
                        int j = new_graph[x][k];
                        if (check_edge[abs(p - nodes[j])]) {
                            flag2 = false;
                            break;
                        }
                    }
                    if (!flag2) {
                        if (!fast)continue;
                        else break;
                    }
                    for (int j : graph[x]) {
                        if (nodes[j] == INF) {
                            edge_num[j]++;
                            new_graph[j].emplace_back(x);
                        }
                        else check_edge[abs(nodes[j] - p)] = true;
                    }

                    edge_num[x] = N;
                    nodes[x] = p;
                    //check_node[p] = true;
                    flag = true;
                    break;
                }
                if (!flag)continue;
                else {
                    p_ma = p + 1;
                    break;
                }
            }
        }
        else {
            FORD(p, p_mi, -(CINF)) {
                if (p % 100000 == 0) {
                    get_time();
                    if (t_diff > TIME_LIMIT2)fast = true;
                }
                flag = false;
                REP(i, N) {
                    int x = states[i].id;
                    if (nodes[x] != INF)break;
                    flag2 = true;
                    FORD(k, SZ(new_graph[x]) - 1, 0) {
                        int j = new_graph[x][k];
                        if (check_edge[abs(p - nodes[j])]) {
                            flag2 = false;
                            break;
                        }
                    }
                    if (!flag2) {
                        if (!fast)continue;
                        else break;
                    }
                    for (int j : graph[x]) {
                        if (nodes[j] == INF) {
                            edge_num[j]++;
                            new_graph[j].emplace_back(x);
                        }
                        else check_edge[abs(nodes[j] - p)] = true;
                    }

                    edge_num[x] = N;
                    nodes[x] = p;
                    //check_node[p] = true;
                    flag = true;
                    break;
                }
                if (!flag)continue;
                else {
                    p_mi = p - 1;
                    break;
                }
            }
        }
        REP(i, N) {
            if (states[i].now >= N)break;
            states[i].now = edge_num[states[i].id];
        }
        sort(ALL(states));
        up = !up;
        if (states[0].now >= N)break;
    }
    //REP(i, N)cerr << nodes[i] << endl;
    int mi = *min_element(ALL(nodes));
    REP(i, N)nodes[i] -= mi;
}

void update() {
    if (chmin(best_ans, *max_element(ALL(nodes)))) {
        REP(i,N)if(best_nodes[i] != nodes[i])best_nodes[i]=nodes[i];
    }
    //REP(i, N)if (nodes[i] != -1)check_node[nodes[i]] = false;
    REP(i, E)if (nodes[edges[i].first] != -1 || nodes[edges[i].second] != -1) {
        check_edge[abs(nodes[edges[i].first] - nodes[edges[i].second])] = false;
    }
    REP(i, N)if (nodes[i] != -1)nodes[i] = -1;
    get_time();
}

void two_swap() {
    uniform_int_distribution<> dist_node(0, N - 1);
    int a = dist_node(rng);
    int b = dist_node(rng);
    while (a == b) {
        b = dist_node(rng);
    }
    if (a > b)swap(a, b);
    swap(order[a], order[b]);
    REP(i,N) {
        if (i < a && nodes[order[i]] != best_nodes[order[i]])nodes[order[i]] = best_nodes[order[i]];
        else if (i >= a && nodes[order[i]] != -1)nodes[order[i]] = -1;
    }

    greedy2();

    if (chmin(best_ans, *max_element(ALL(nodes)))) {
        //cerr << best_ans << endl;
        REP(i, N)if (best_nodes[i] != nodes[i])best_nodes[i] = nodes[i];
    }
    else swap(order[a], order[b]);

    //REP(i, N)if (nodes[i] != -1)check_node[nodes[i]] = false;
    REP(i, E)if (nodes[edges[i].first] != -1 || nodes[edges[i].second] != -1) {
        check_edge[abs(nodes[edges[i].first] - nodes[edges[i].second])] = false;
    }
    //REP(i, N)if (nodes[i] != -1)nodes[i] = -1;
}

void range_swap(int width=1) {
    
    uniform_int_distribution<> dist_node(0, N - width);
    int a = dist_node(rng);
    int b = dist_node(rng);
    while (abs(a-b)<width) {
        a = dist_node(rng);
        b = dist_node(rng);
    }
    if (a > b)swap(a, b);
    REP(i, width) {
        swap(order[a+i], order[b+i]);
    }
    REP(i, N) {
        if (i < a && nodes[order[i]] != best_nodes[order[i]])nodes[order[i]] = best_nodes[order[i]];
        else if (i >= a && nodes[order[i]] != -1)nodes[order[i]] = -1;
    }

    greedy2();

    if (chmin(best_ans, *max_element(ALL(nodes)))) {
        cerr << best_ans << endl;
        REP(i, N)if (best_nodes[i] != nodes[i])best_nodes[i] = nodes[i];
    }
    else {
        REP(i, width) {
            swap(order[a + i], order[b + i]);
        }
    }

    //REP(i, N)if (nodes[i] != -1)check_node[nodes[i]] = false;
    REP(i, E)if (nodes[edges[i].first] != -1 || nodes[edges[i].second] != -1) {
        check_edge[abs(nodes[edges[i].first] - nodes[edges[i].second])] = false;
    }
    //REP(i, N)if (nodes[i] != -1)nodes[i] = -1;
}

signed main() {
    cin.tie(0);
    ios::sync_with_stdio(false);
    //cout << fixed << setprecision(15);
    t_start = chrono::system_clock::now();
    get_time();


    init();
    REP(i, 4) {
        mode = i;
        greedy3();
        update();
        get_time();
        if (t_diff > TIME_LIMIT)break;
    }

    V<P> align(N);
    REP(i, N)align[i] = MP(best_nodes[i], i);
    sort(ALL(align));
    REP(i, N)order[i] = align[i].second;

    //cerr << best_ans << endl;
    //greedy2();
    //cerr << *max_element(ALL(nodes)) << endl;
    
    //REP(i, N)order[i] = i;
    
    while (t_diff < TIME_LIMIT) {
        //cerr << best_ans << endl;
        two_swap();
        get_time();
        cnt++;
        if (best_ans == E)break;
    }
    
    
    /*
    while (t_diff < TIME_LIMIT) {
        greedy2();
        update();
        shuffle(ALL(order), rng);
        cnt++;
        if (best_ans == E)break;
    }
    */
    REP(i, N)cout << best_nodes[i] << " ";
    cout << endl;

    //REP(i, N)cerr << i << " " << nodes[i] << endl;
    //sort(ALL(nodes));
    //REP(i, N-1)cerr << nodes[i+1]-nodes[i] << endl;

    get_time();

    cerr << "Node: " << N << endl;
    cerr << "Edge: " << E << endl;
    //cerr << "time:  " << t_diff << endl;
    //cerr << "score: " << score - price[SZ(machine)] << endl;
    //cerr << "last:  " << last_update << endl;
    cerr << "cnt:   " << cnt << endl;
    //cerr << "update:" << update_cnt << endl;
    return 0;
}
=======
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
const double TIME_LIMIT = 9950,TIME_LIMIT2=9500;
double last_update = 0, t_diff;
double start_temp, end_temp;
int cnt = 0;

mt19937 rng;
uniform_real_distribution<> dist(0.0, 1.0);
uniform_int_distribution<> dist_int(0, 3);

//V<int> x_vec = { -1, 0,1, 0 };
//V<int> y_vec = { 0, 1,0, -1 };

/*
double prob = exp((next_score-current_score) / get_temp());
if (prob > dist(rng)) {
*/

int mode = 0;
VV<int> graph, new_graph;

class state {
public:
    int now = 0;
    int id = 0;
    state() {}
    bool operator<(const state& l) const{
        if (mode == 0) {
            if (double(this->now) / double(SZ(graph[this->id])) != double(l.now) / double(SZ(graph[l.id]))) {
                return double(this->now) / double(SZ(graph[this->id])) < double(l.now) / double(SZ(graph[l.id]));
            }
            else {
                if (SZ(graph[this->id]) != SZ(graph[l.id]))return SZ(graph[this->id]) > SZ(graph[l.id]);
                else {
                    if (this->now != l.now)return this->now < l.now;
                    else {
                        return this->id < l.id;
                    }
                }
            }
        }
        else if (mode == 1) {
            if (double(this->now) / double(SZ(graph[this->id])) != double(l.now) / double(SZ(graph[l.id]))) {
                return double(this->now) / double(SZ(graph[this->id])) < double(l.now) / double(SZ(graph[l.id]));
            }
            else {
                if (SZ(graph[this->id]) != SZ(graph[l.id]))return SZ(graph[this->id]) < SZ(graph[l.id]);
                else {
                    if (this->now != l.now)return this->now < l.now;
                    else {
                        return this->id < l.id;
                    }
                }
            }
        }
        else if (mode == 2) {
            if (this->now != l.now)return this->now < l.now;
            else {
                if (SZ(graph[this->id]) != SZ(graph[l.id]))return SZ(graph[this->id]) > SZ(graph[l.id]);
                else {
                    return this->id < l.id;
                }
            }
        }
        else if (mode == 3){
            if (this->now != l.now)return this->now < l.now;
            else {
                if (SZ(graph[this->id]) != SZ(graph[l.id]))return SZ(graph[this->id]) < SZ(graph[l.id]);
                else {
                    return this->id < l.id;
                }
            }
        }
    }
};

int N, E;
int ans = INF, best_ans = INF;
V<int> nodes,best_nodes,order, best_order;
V<P> edges;
V<int> pool;
V<bool> check_node;
V<bool> check_edge;
V<bool> edge_pool;
V<state> states;
V<int> edge_num;

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
    best_nodes = V<int>(N, -1);
    order = V<int>(N);
    edges = V<P>(E);
    graph = VV<int>(N);
    check_node = V<bool>(2100000);
    check_edge = V<bool>(2100000);
    edge_pool = V<bool>(2100000);
    REP(i, E) {
        int a, b; cin >> a >> b;
        if (a > b)swap(a, b);
        edges[i] = MP(a, b);
        graph[a].emplace_back(b);
        graph[b].emplace_back(a);
    }
    REP(i, N)order[i] = i;
    states = V<state>(N);
    edge_num = V<int>(N);
    REP(i, N) {
        states[i].id = i;
    }
}

bool validate() {
    edge_pool = V<bool>(2100000);
    REP(i, E) {
        if (edge_pool[abs(nodes[edges[i].first] - nodes[edges[i].second])])return false;
        else edge_pool[abs(nodes[edges[i].first] - nodes[edges[i].second])] = true;
    }
    return true;
}

void greedy(int start) {
    queue<int> q;
    q.push(start);
    int min_node = 0;
    while (!q.empty()) {
        int x = q.front();
        q.pop();
        if (nodes[x] != -1)continue;
        FOR(p, min_node, CINF) {
            bool flag = true;
            for (int i : graph[x]) {
                if (nodes[i] == -1)continue;
                if (check_edge[abs(p - nodes[i])]) {
                    while (check_edge[abs(p - nodes[i])])p++;
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

bool greedy2() {
    new_graph = VV<int>(N);
    int min_node = 0;
    for(int x:order){  
        if (nodes[x] != -1) {
            for (int i : graph[x]) {
                if (nodes[i] == -1)new_graph[i].emplace_back(x);
                else check_edge[abs(nodes[i] - nodes[x])] = true;
            }
            min_node = nodes[x] + 1;
            continue;
        }
        FOR(p, min_node, CINF) {
            bool flag = true;
            if (p > best_ans) {
                nodes[x] = 2099999;
                return false;
            }
            FORD(k, SZ(new_graph[x]) - 1, 0) {
                int i = new_graph[x][k];
                if (check_edge[abs(p - nodes[i])]) {
                    while (check_edge[abs(p - nodes[i])])p++;
                    p--;
                    flag = false;
                    break;
                }
            }
            if (!flag)continue;
            for (int i : graph[x]) {
                if (nodes[i] == -1)new_graph[i].emplace_back(x);
                else check_edge[abs(nodes[i] - p)] = true;
            }
            nodes[x] = p;
            min_node = p + 1;
            break;
        }
    }
    return true;
}

void greedy3() {
    new_graph = VV<int>(N);
    REP(i, N)if (edge_num[i] != 0)edge_num[i] = 0;
    REP(i, N)if (states[i].now != 0)states[i].now = 0;
    
    sort(ALL(states));
    bool fast = false;
    REP(p, CINF) {
        if (p % 100000 == 0) {
            get_time();
            if (t_diff > TIME_LIMIT2)fast = true;
        }
        bool flag = false;
        REP(i, N) {
            int x = states[i].id;
            if (nodes[x] != -1)break;
            bool flag2 = true;
            FORD(k, SZ(new_graph[x]) - 1, 0) {
                int j = new_graph[x][k];
                if (check_edge[abs(p - nodes[j])]) {
                    flag2 = false;
                    break;
                }
            }
            if (!flag2) {
                if (!fast)continue;
                else break;
            }
            for (int j : graph[x]) {
                if (nodes[j] == -1) {
                    edge_num[j]++;
                    new_graph[j].emplace_back(x);
                }
                else check_edge[abs(nodes[j] - p)] = true;
            }

            edge_num[x] = N;
            nodes[x] = p;
            //check_node[p] = true;
            flag = true;
            break;
        }
        if (!flag)continue;
        REP(i, N) {
            if (states[i].now >= N)break;
            states[i].now = edge_num[states[i].id];
        }
        sort(ALL(states));

        if (states[0].now >= N)break;
    }
}

void greedy4() {
    mode = 2;
    new_graph = VV<int>(N);
    REP(i, N)if (edge_num[i] != 0)edge_num[i] = 0;
    REP(i, N)if (states[i].now != 0)states[i].now = 0;
    REP(i, N)nodes[i] = INF;
    sort(ALL(states));
    bool flag, flag2;
    bool fast = false;
    bool up = true;
    int p_ma = 0,p_mi=-1;
    while (1) {
        if (up) {
            FOR(p, p_ma, CINF) {
                if (p % 100000 == 0) {
                    get_time();
                    if (t_diff > TIME_LIMIT2)fast = true;
                }
                flag = false;
                REP(i, N) {
                    int x = states[i].id;
                    if (nodes[x] != INF)break;
                    flag2 = true;
                    FORD(k, SZ(new_graph[x]) - 1, 0) {
                        int j = new_graph[x][k];
                        if (check_edge[abs(p - nodes[j])]) {
                            flag2 = false;
                            break;
                        }
                    }
                    if (!flag2) {
                        if (!fast)continue;
                        else break;
                    }
                    for (int j : graph[x]) {
                        if (nodes[j] == INF) {
                            edge_num[j]++;
                            new_graph[j].emplace_back(x);
                        }
                        else check_edge[abs(nodes[j] - p)] = true;
                    }

                    edge_num[x] = N;
                    nodes[x] = p;
                    //check_node[p] = true;
                    flag = true;
                    break;
                }
                if (!flag)continue;
                else {
                    p_ma = p + 1;
                    break;
                }
            }
        }
        else {
            FORD(p, p_mi, -(CINF)) {
                if (p % 100000 == 0) {
                    get_time();
                    if (t_diff > TIME_LIMIT2)fast = true;
                }
                flag = false;
                REP(i, N) {
                    int x = states[i].id;
                    if (nodes[x] != INF)break;
                    flag2 = true;
                    FORD(k, SZ(new_graph[x]) - 1, 0) {
                        int j = new_graph[x][k];
                        if (check_edge[abs(p - nodes[j])]) {
                            flag2 = false;
                            break;
                        }
                    }
                    if (!flag2) {
                        if (!fast)continue;
                        else break;
                    }
                    for (int j : graph[x]) {
                        if (nodes[j] == INF) {
                            edge_num[j]++;
                            new_graph[j].emplace_back(x);
                        }
                        else check_edge[abs(nodes[j] - p)] = true;
                    }

                    edge_num[x] = N;
                    nodes[x] = p;
                    //check_node[p] = true;
                    flag = true;
                    break;
                }
                if (!flag)continue;
                else {
                    p_mi = p - 1;
                    break;
                }
            }
        }
        REP(i, N) {
            if (states[i].now >= N)break;
            states[i].now = edge_num[states[i].id];
        }
        sort(ALL(states));
        up = !up;
        if (states[0].now >= N)break;
    }
    //REP(i, N)cerr << nodes[i] << endl;
    int mi = *min_element(ALL(nodes));
    REP(i, N)nodes[i] -= mi;
}

void update() {
    if (chmin(best_ans, *max_element(ALL(nodes)))) {
        REP(i,N)if(best_nodes[i] != nodes[i])best_nodes[i]=nodes[i];
    }
    //REP(i, N)if (nodes[i] != -1)check_node[nodes[i]] = false;
    REP(i, E)if (nodes[edges[i].first] != -1 || nodes[edges[i].second] != -1) {
        check_edge[abs(nodes[edges[i].first] - nodes[edges[i].second])] = false;
    }
    REP(i, N)if (nodes[i] != -1)nodes[i] = -1;
    get_time();
}

void two_swap() {
    uniform_int_distribution<> dist_node(0, N - 1);
    int a = dist_node(rng);
    int b = dist_node(rng);
    while (a == b) {
        b = dist_node(rng);
    }
    if (a > b)swap(a, b);
    swap(order[a], order[b]);
    REP(i,N) {
        if (i < a && nodes[order[i]] != best_nodes[order[i]])nodes[order[i]] = best_nodes[order[i]];
        else if (i >= a && nodes[order[i]] != -1)nodes[order[i]] = -1;
    }

    greedy2();

    if (chmin(best_ans, *max_element(ALL(nodes)))) {
        //cerr << best_ans << endl;
        REP(i, N)if (best_nodes[i] != nodes[i])best_nodes[i] = nodes[i];
    }
    else swap(order[a], order[b]);

    //REP(i, N)if (nodes[i] != -1)check_node[nodes[i]] = false;
    REP(i, E)if (nodes[edges[i].first] != -1 || nodes[edges[i].second] != -1) {
        check_edge[abs(nodes[edges[i].first] - nodes[edges[i].second])] = false;
    }
    //REP(i, N)if (nodes[i] != -1)nodes[i] = -1;
}

void range_swap(int width=1) {
    
    uniform_int_distribution<> dist_node(0, N - width);
    int a = dist_node(rng);
    int b = dist_node(rng);
    while (abs(a-b)<width) {
        a = dist_node(rng);
        b = dist_node(rng);
    }
    if (a > b)swap(a, b);
    REP(i, width) {
        swap(order[a+i], order[b+i]);
    }
    REP(i, N) {
        if (i < a && nodes[order[i]] != best_nodes[order[i]])nodes[order[i]] = best_nodes[order[i]];
        else if (i >= a && nodes[order[i]] != -1)nodes[order[i]] = -1;
    }

    greedy2();

    if (chmin(best_ans, *max_element(ALL(nodes)))) {
        cerr << best_ans << endl;
        REP(i, N)if (best_nodes[i] != nodes[i])best_nodes[i] = nodes[i];
    }
    else {
        REP(i, width) {
            swap(order[a + i], order[b + i]);
        }
    }

    //REP(i, N)if (nodes[i] != -1)check_node[nodes[i]] = false;
    REP(i, E)if (nodes[edges[i].first] != -1 || nodes[edges[i].second] != -1) {
        check_edge[abs(nodes[edges[i].first] - nodes[edges[i].second])] = false;
    }
    //REP(i, N)if (nodes[i] != -1)nodes[i] = -1;
}

signed main() {
    cin.tie(0);
    ios::sync_with_stdio(false);
    //cout << fixed << setprecision(15);
    t_start = chrono::system_clock::now();
    get_time();


    init();
    REP(i, 4) {
        mode = i;
        greedy3();
        update();
        get_time();
        if (t_diff > TIME_LIMIT)break;
    }

    V<P> align(N);
    REP(i, N)align[i] = MP(best_nodes[i], i);
    sort(ALL(align));
    REP(i, N)order[i] = align[i].second;

    //cerr << best_ans << endl;
    //greedy2();
    //cerr << *max_element(ALL(nodes)) << endl;
    
    //REP(i, N)order[i] = i;
    
    while (t_diff < TIME_LIMIT) {
        //cerr << best_ans << endl;
        two_swap();
        get_time();
        cnt++;
        if (best_ans == E)break;
    }
    
    
    /*
    while (t_diff < TIME_LIMIT) {
        greedy2();
        update();
        shuffle(ALL(order), rng);
        cnt++;
        if (best_ans == E)break;
    }
    */
    REP(i, N)cout << best_nodes[i] << " ";
    cout << endl;

    //REP(i, N)cerr << i << " " << nodes[i] << endl;
    //sort(ALL(nodes));
    //REP(i, N-1)cerr << nodes[i+1]-nodes[i] << endl;

    get_time();

    cerr << "Node: " << N << endl;
    cerr << "Edge: " << E << endl;
    //cerr << "time:  " << t_diff << endl;
    //cerr << "score: " << score - price[SZ(machine)] << endl;
    //cerr << "last:  " << last_update << endl;
    cerr << "cnt:   " << cnt << endl;
    //cerr << "update:" << update_cnt << endl;
    return 0;
}
>>>>>>> origin/main
