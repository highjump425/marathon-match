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
#include <atcoder/segtree>
#include <atcoder/fenwicktree>


using namespace std;
using namespace atcoder;

using ll = long long;
using P = pair<ll, ll>;
template <class T>using V = vector<T>;
template <class T>using VV = V<V<T>>;
template <class T, class U>bool chmin(T& t, const U& u) { if (t > u) { t = u; return 1; } else return 0; }
template <class T, class U>bool chmax(T& t, const U& u) { if (t < u) { t = u; return 1; } else return 0; }

#define REP(i,n) for(ll i=0;i<ll(n);i++)
#define FOR(i,a,b) for(ll i=ll(a);i<=ll(b);i++)
#define FORD(i,a,b) for(ll i=ll(a);i>=ll(b);i--)
#define MP make_pair
#define SZ(x) ll(x.size())
#define ALL(x) x.begin(),x.end()
#define NIL -1
#define INF 1000100010001000
#define CINF 1ll<<63-1
#define PI 3.14159265358979323846
#define endl "\n"
#define EPS 1e-9
#define YES cout << "YES" << endl
#define NO cout << "NO" << endl
#define Yes cout << "Yes" << endl
#define No cout << "No" << endl

mt19937 rng;
uniform_real_distribution<> dist(0.0, 1.0);
chrono::system_clock::time_point t_start;
const double TIME_LIMIT = 1950;
double last_update = 0,t_diff;
ll cnt = 0,best_score,new_best_score;
const int T0 = 2000, T1 = 20;
int si, sj;
VV<int> tile(50, V<int>(50)),dead(50,V<int>(50));
VV<int> point(50, V<int>(50));
V<int> x_vec = { -1, 0, +1, 0 };
V<int> y_vec = { 0, -1, 0, +1 };
V<char> muki = { 'U','L','D','R' };
map<char, int> m = { {'U',0},{'L',1},{'D',2},{'R',3} };
V<bool> check(3000, true);
V<char> best_ans,new_best_ans;
V<P> best_root,root;

/*
double prob = exp(dif / get_temp());
if (prob > dist(rng)) {
*/


void get_time() {
    auto t_now = chrono::system_clock::now();
    t_diff = chrono::duration_cast<chrono::milliseconds>(t_now - t_start).count();
}

double get_temp() {
    return T0 + (T1 - T0) * (1 - t_diff / TIME_LIMIT);
}

P get_pos(V<char> dir,int num) {
    P res = { si,sj };
    FOR(i,0,num){
        res.first += x_vec[m[dir[i]]];
        res.second += y_vec[m[dir[i]]];
    }
    return res;
}

void roll_back(V<char> &ans,int &pi,int &pj,ll &score){
    char c = ans.back();
    ans.pop_back();
    score -= point[pi][pj];
    int k = uniform_int_distribution<int>(0, 1)(rng);
    check[tile[pi][pj]] = true;
    REP(i, 4) {
        if (muki[i] == c) {
            pi -= x_vec[i];
            pj -= y_vec[i];
        }
    }
}

void random_walk(int &pi,int &pj, int &direction,V<char> &ans,ll &score,ll &num) {
    direction %= 4;
    pi += x_vec[direction];
    pj += y_vec[direction];
    if (pi < 0 || 49 < pi || pj < 0 || 49 < pj || check[tile[pi][pj]] == false) {
        pi -= x_vec[direction];
        pj -= y_vec[direction];
        dead[pi][pj]++;
        int k = uniform_int_distribution<int>(0, 1)(rng);
        if (k)direction++;
        else direction += 3;

        int e = uniform_int_distribution<int>(1, 3)(rng);
        if (SZ(ans) >= 20 && dead[pi][pj] > 50) {
            REP(i, e) {
                roll_back(ans, pi, pj, score);
            }
        }
        else if (SZ(ans) >= 3 && dead[pi][pj] > 50)roll_back(ans, pi, pj, score);
    }
    else {
        num++;
        check[tile[pi][pj]] = false;
        score += point[pi][pj];
        ans.push_back(muki[direction]);
        if (chmax(best_score, score))best_ans = ans;
        last_update = t_diff;
        cnt++;
        int tika = INF;
        int nera = 0;
        if (chmin(tika, pi - 0))nera = 0;
        if (chmin(tika, 49 - pi))nera = 2;
        if (chmin(tika, pj - 0))nera = 1;
        if (chmin(tika, 49 - pj))nera = 3;

        direction = nera;
    }
}

void dfs(int& pi, int& pj, V<char>& ans, ll& score, int& num) {
    if (t_diff > double(500))return;
    REP(i, 4) {
        pi += x_vec[i];
        pj += y_vec[i];
        if (pi < 0 || 49 < pi || pj < 0 || 49 < pj || check[tile[pi][pj]] == false) {
            pi -= x_vec[i];
            pj -= y_vec[i];
            continue;
        }
        else {
            num++;
            check[tile[pi][pj]] = false;
            score += point[pi][pj];
            ans.push_back(muki[i]);
            root.push_back({ pi,pj });
            if (chmax(best_score, score)) {
                best_ans = ans;
                best_root = root;
                last_update = t_diff;
            }
            dfs(pi, pj, ans, score, num);
            ans.pop_back();
            root.pop_back();
            score -= point[pi][pj];
            check[tile[pi][pj]] = true;
            num--;
            pi -= x_vec[i];
            pj -= y_vec[i];
            
        }
        cnt++;
        if (cnt % 1000 == 0)get_time();
    }
}

void connect_dfs(int pi, int pj,int ei,int ej, V<char>& new_ans, int& new_score, int depth) {
    if (depth > 20)return;
    REP(i, 4) {
        pi += x_vec[i];
        pj += y_vec[i];
        if (pi < 0 || 49 < pi || pj < 0 || 49 < pj || check[tile[pi][pj]] == false) {
            pi -= x_vec[i];
            pj -= y_vec[i];
        }
        else {
            check[tile[pi][pj]] = false;
            new_score += point[pi][pj];
            new_ans.push_back(muki[i]);
            if (pi == ei && pj == ej) {
                if (chmax(new_best_score, new_score))new_best_ans = new_ans;
                last_update = t_diff;
            }
            else connect_dfs(pi, pj, ei, ej, new_ans, new_score,depth+1);
            new_ans.pop_back();
            new_score -= point[pi][pj];
            check[tile[pi][pj]] = true;
            pi -= x_vec[i];
            pj -= y_vec[i];
        }
    }
    return;
}

void reconnect(V<char>& ans,ll &score) {
    int num = uniform_int_distribution<int>(0, SZ(ans))(rng);
    int diff = uniform_int_distribution<int>(1, 15)(rng);
    //int diff = 8;
    if (num + diff > SZ(ans) - 1)return;
    int bi, bj, ei, ej;
    tie(bi, bj) = root[num];
    tie(ei, ej) = root[num + diff];
    int init_score = 0;
    FOR(i, num + 1, num + diff) {
        check[tile[root[i].first][root[i].second]] = true;
        init_score += point[root[i].first][root[i].second];
    }
    new_best_ans = {};
    new_best_score = 0;
    V<char> new_ans;
    V<P> new_root;
    int new_score = 0;
    connect_dfs(bi, bj, ei, ej, new_ans, new_score,0);

    double prob = exp((new_best_score-init_score) / get_temp());
    if (new_best_score!=0&&prob > dist(rng)) {
        score += new_best_score - init_score;
        ans.erase(ans.begin() + num, ans.begin() + num + diff);
        root.erase(root.begin() + num + 1, root.begin() + num + diff+1);
        for (char c : new_best_ans) {
            bi += x_vec[m[c]];
            bj += y_vec[m[c]];
            check[tile[bi][bj]] = false;
            new_root.push_back({ bi,bj });
        }
        ans.insert(ans.begin() + num, ALL(new_best_ans));
        root.insert(root.begin() + num + 1, ALL(new_root));

        if (chmax(best_score, score)) {
            best_ans = ans;
            last_update = t_diff;
        }
        cnt++;
    }
    else {
            FOR(i, num + 1, num + diff) {
                check[tile[root[i].first][root[i].second]] = false;
            }
    }
    return;
}

signed main() {
    cin.tie(0);
    ios::sync_with_stdio(false);
    cout << fixed << setprecision(15);
    t_start = chrono::system_clock::now();

    //input
    int pi,pj; cin >> si >> sj;
    REP(i, 50)REP(j, 50)cin >> tile[i][j];
    REP(i, 50)REP(j, 50)cin >> point[i][j];

    //init_ans
    int itr = 0,num = 0;
    pi = si; pj = sj;
    V<char> ans;
    root.push_back({ si,sj });
    check[tile[si][sj]] = false;
    ll score = point[si][sj];
    get_time();
    int direction = 0;
    dfs(pi, pj, ans, score, num);

    ans = best_ans;
    score = best_score;
    root = best_root;
    REP(i, SZ(root)) {
        check[tile[root[i].first][root[i].second]] = false;
    }
    cnt = 0;

    while (t_diff < TIME_LIMIT) {
        
        reconnect(ans, score);

        if(itr%100==0)get_time();

        itr++;
    }

    REP(i, SZ(best_ans)) {
        cout << best_ans[i];
    }
    cout << endl;

    get_time();

    // prepare_diff

    cerr << "score: " << best_score << endl;
    cerr << "time: " << t_diff << endl;
    cerr << "last_update: " << last_update << endl;
    cerr << "update count: " << cnt << endl;
    cerr << "itrate: " << itr << endl;

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
#include <atcoder/segtree>
#include <atcoder/fenwicktree>


using namespace std;
using namespace atcoder;

using ll = long long;
using P = pair<ll, ll>;
template <class T>using V = vector<T>;
template <class T>using VV = V<V<T>>;
template <class T, class U>bool chmin(T& t, const U& u) { if (t > u) { t = u; return 1; } else return 0; }
template <class T, class U>bool chmax(T& t, const U& u) { if (t < u) { t = u; return 1; } else return 0; }

#define REP(i,n) for(ll i=0;i<ll(n);i++)
#define FOR(i,a,b) for(ll i=ll(a);i<=ll(b);i++)
#define FORD(i,a,b) for(ll i=ll(a);i>=ll(b);i--)
#define MP make_pair
#define SZ(x) ll(x.size())
#define ALL(x) x.begin(),x.end()
#define NIL -1
#define INF 1000100010001000
#define CINF 1ll<<63-1
#define PI 3.14159265358979323846
#define endl "\n"
#define EPS 1e-9
#define YES cout << "YES" << endl
#define NO cout << "NO" << endl
#define Yes cout << "Yes" << endl
#define No cout << "No" << endl

mt19937 rng;
uniform_real_distribution<> dist(0.0, 1.0);
chrono::system_clock::time_point t_start;
const double TIME_LIMIT = 1950;
double last_update = 0,t_diff;
ll cnt = 0,best_score,new_best_score;
const int T0 = 2000, T1 = 20;
int si, sj;
VV<int> tile(50, V<int>(50)),dead(50,V<int>(50));
VV<int> point(50, V<int>(50));
V<int> x_vec = { -1, 0, +1, 0 };
V<int> y_vec = { 0, -1, 0, +1 };
V<char> muki = { 'U','L','D','R' };
map<char, int> m = { {'U',0},{'L',1},{'D',2},{'R',3} };
V<bool> check(3000, true);
V<char> best_ans,new_best_ans;
V<P> best_root,root;

/*
double prob = exp(dif / get_temp());
if (prob > dist(rng)) {
*/


void get_time() {
    auto t_now = chrono::system_clock::now();
    t_diff = chrono::duration_cast<chrono::milliseconds>(t_now - t_start).count();
}

double get_temp() {
    return T0 + (T1 - T0) * (1 - t_diff / TIME_LIMIT);
}

P get_pos(V<char> dir,int num) {
    P res = { si,sj };
    FOR(i,0,num){
        res.first += x_vec[m[dir[i]]];
        res.second += y_vec[m[dir[i]]];
    }
    return res;
}

void roll_back(V<char> &ans,int &pi,int &pj,ll &score){
    char c = ans.back();
    ans.pop_back();
    score -= point[pi][pj];
    int k = uniform_int_distribution<int>(0, 1)(rng);
    check[tile[pi][pj]] = true;
    REP(i, 4) {
        if (muki[i] == c) {
            pi -= x_vec[i];
            pj -= y_vec[i];
        }
    }
}

void random_walk(int &pi,int &pj, int &direction,V<char> &ans,ll &score,ll &num) {
    direction %= 4;
    pi += x_vec[direction];
    pj += y_vec[direction];
    if (pi < 0 || 49 < pi || pj < 0 || 49 < pj || check[tile[pi][pj]] == false) {
        pi -= x_vec[direction];
        pj -= y_vec[direction];
        dead[pi][pj]++;
        int k = uniform_int_distribution<int>(0, 1)(rng);
        if (k)direction++;
        else direction += 3;

        int e = uniform_int_distribution<int>(1, 3)(rng);
        if (SZ(ans) >= 20 && dead[pi][pj] > 50) {
            REP(i, e) {
                roll_back(ans, pi, pj, score);
            }
        }
        else if (SZ(ans) >= 3 && dead[pi][pj] > 50)roll_back(ans, pi, pj, score);
    }
    else {
        num++;
        check[tile[pi][pj]] = false;
        score += point[pi][pj];
        ans.push_back(muki[direction]);
        if (chmax(best_score, score))best_ans = ans;
        last_update = t_diff;
        cnt++;
        int tika = INF;
        int nera = 0;
        if (chmin(tika, pi - 0))nera = 0;
        if (chmin(tika, 49 - pi))nera = 2;
        if (chmin(tika, pj - 0))nera = 1;
        if (chmin(tika, 49 - pj))nera = 3;

        direction = nera;
    }
}

void dfs(int& pi, int& pj, V<char>& ans, ll& score, int& num) {
    if (t_diff > double(500))return;
    REP(i, 4) {
        pi += x_vec[i];
        pj += y_vec[i];
        if (pi < 0 || 49 < pi || pj < 0 || 49 < pj || check[tile[pi][pj]] == false) {
            pi -= x_vec[i];
            pj -= y_vec[i];
            continue;
        }
        else {
            num++;
            check[tile[pi][pj]] = false;
            score += point[pi][pj];
            ans.push_back(muki[i]);
            root.push_back({ pi,pj });
            if (chmax(best_score, score)) {
                best_ans = ans;
                best_root = root;
                last_update = t_diff;
            }
            dfs(pi, pj, ans, score, num);
            ans.pop_back();
            root.pop_back();
            score -= point[pi][pj];
            check[tile[pi][pj]] = true;
            num--;
            pi -= x_vec[i];
            pj -= y_vec[i];
            
        }
        cnt++;
        if (cnt % 1000 == 0)get_time();
    }
}

void connect_dfs(int pi, int pj,int ei,int ej, V<char>& new_ans, int& new_score, int depth) {
    if (depth > 20)return;
    REP(i, 4) {
        pi += x_vec[i];
        pj += y_vec[i];
        if (pi < 0 || 49 < pi || pj < 0 || 49 < pj || check[tile[pi][pj]] == false) {
            pi -= x_vec[i];
            pj -= y_vec[i];
        }
        else {
            check[tile[pi][pj]] = false;
            new_score += point[pi][pj];
            new_ans.push_back(muki[i]);
            if (pi == ei && pj == ej) {
                if (chmax(new_best_score, new_score))new_best_ans = new_ans;
                last_update = t_diff;
            }
            else connect_dfs(pi, pj, ei, ej, new_ans, new_score,depth+1);
            new_ans.pop_back();
            new_score -= point[pi][pj];
            check[tile[pi][pj]] = true;
            pi -= x_vec[i];
            pj -= y_vec[i];
        }
    }
    return;
}

void reconnect(V<char>& ans,ll &score) {
    int num = uniform_int_distribution<int>(0, SZ(ans))(rng);
    int diff = uniform_int_distribution<int>(1, 15)(rng);
    //int diff = 8;
    if (num + diff > SZ(ans) - 1)return;
    int bi, bj, ei, ej;
    tie(bi, bj) = root[num];
    tie(ei, ej) = root[num + diff];
    int init_score = 0;
    FOR(i, num + 1, num + diff) {
        check[tile[root[i].first][root[i].second]] = true;
        init_score += point[root[i].first][root[i].second];
    }
    new_best_ans = {};
    new_best_score = 0;
    V<char> new_ans;
    V<P> new_root;
    int new_score = 0;
    connect_dfs(bi, bj, ei, ej, new_ans, new_score,0);

    double prob = exp((new_best_score-init_score) / get_temp());
    if (new_best_score!=0&&prob > dist(rng)) {
        score += new_best_score - init_score;
        ans.erase(ans.begin() + num, ans.begin() + num + diff);
        root.erase(root.begin() + num + 1, root.begin() + num + diff+1);
        for (char c : new_best_ans) {
            bi += x_vec[m[c]];
            bj += y_vec[m[c]];
            check[tile[bi][bj]] = false;
            new_root.push_back({ bi,bj });
        }
        ans.insert(ans.begin() + num, ALL(new_best_ans));
        root.insert(root.begin() + num + 1, ALL(new_root));

        if (chmax(best_score, score)) {
            best_ans = ans;
            last_update = t_diff;
        }
        cnt++;
    }
    else {
            FOR(i, num + 1, num + diff) {
                check[tile[root[i].first][root[i].second]] = false;
            }
    }
    return;
}

signed main() {
    cin.tie(0);
    ios::sync_with_stdio(false);
    cout << fixed << setprecision(15);
    t_start = chrono::system_clock::now();

    //input
    int pi,pj; cin >> si >> sj;
    REP(i, 50)REP(j, 50)cin >> tile[i][j];
    REP(i, 50)REP(j, 50)cin >> point[i][j];

    //init_ans
    int itr = 0,num = 0;
    pi = si; pj = sj;
    V<char> ans;
    root.push_back({ si,sj });
    check[tile[si][sj]] = false;
    ll score = point[si][sj];
    get_time();
    int direction = 0;
    dfs(pi, pj, ans, score, num);

    ans = best_ans;
    score = best_score;
    root = best_root;
    REP(i, SZ(root)) {
        check[tile[root[i].first][root[i].second]] = false;
    }
    cnt = 0;

    while (t_diff < TIME_LIMIT) {
        
        reconnect(ans, score);

        if(itr%100==0)get_time();

        itr++;
    }

    REP(i, SZ(best_ans)) {
        cout << best_ans[i];
    }
    cout << endl;

    get_time();

    // prepare_diff

    cerr << "score: " << best_score << endl;
    cerr << "time: " << t_diff << endl;
    cerr << "last_update: " << last_update << endl;
    cerr << "update count: " << cnt << endl;
    cerr << "itrate: " << itr << endl;

    return 0;
}



>>>>>>> origin/main
