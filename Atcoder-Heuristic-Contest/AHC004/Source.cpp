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
const double TIME_LIMIT = 2950;
double last_update = 0, t_diff;
const int T0 = 2000, T1 = 200;
VV<int> grid,best_grid;
V<int> board;
V<pair<int,string>> hint;
V<int> y_vec = { 0, 1};
V<int> x_vec = { 1, 0};
V<int> backup(20);
VV<int> table;
VV<pair<P, int>> rireki;

int N, M;
ll score,best_score;
ll cnt = 0,update_cnt=0;

mt19937 rng;
uniform_real_distribution<> dist(0.0, 1.0);
uniform_int_distribution<> init(0, 7);
uniform_int_distribution<> zahyo(0, 19);
uniform_int_distribution<> nibun(0, 1);


/*
double prob = exp((new_best_score-init_score) / get_temp());
    if (new_best_score!=0&&prob > dist(rng)) {
*/


void get_time() {
    auto t_now = chrono::system_clock::now();
    t_diff = chrono::duration_cast<chrono::milliseconds>(t_now - t_start).count();
}

double get_temp() {
    return T0 + (T1 - T0) * (1 - t_diff / TIME_LIMIT);
}

void get_table() {
    REP(k, M) {
        int n = SZ(hint[k].second);
        table[k] = V<int>(n+1);
        table[k][0] = -1;
        int j = -1;
        REP (i,n) {
            while (j >= 0 && hint[k].second[i] != hint[k].second[j]) j = table[k][j];
            j++;
            table[k][i + 1] = j;
        }
    }
}

bool check(int x, int y,int n,int d) {
    int tx = x, ty = y;
    bool ok = true;
    REP(j, SZ(hint[n].second)) {
        if (char('A' + grid[tx][ty]) != hint[n].second[j]) {
            ok = false;
            break;
        }
        tx += x_vec[d];
        ty += y_vec[d];
        tx %= N;
        ty %= N;
    }
    return ok;
}

void change(int x, int y, int n, int d) {
    REP(i, SZ(hint[n].second)) {
        backup[i] = grid[x][y];
        grid[x][y] = hint[n].second[i] - 'A';
        x += x_vec[d];
        y += y_vec[d];
        x %= N;
        y %= N;
    }
}

void unchange(int x, int y, int n, int d) {
    REP(i, SZ(hint[n].second)) {
        grid[x][y] = backup[i];
        x += x_vec[d];
        y += y_vec[d];
        x %= N;
        y %= N;
    }
}

int get_score() {
    int res = 0;
    REP(i, M) {
        bool ok = false;
        //cerr << i<<" "<<SZ(rireki[i]) << endl;
        REP(j,SZ(rireki[i])){
            int x, y, d;
            tie(x, y) = rireki[i][j].first;
            d = rireki[i][j].second;
            bool flag = check(x, y, i, d);
            if (flag) {
                ok = true;
                break;
            }
        }
        if (ok) {
            res++;
            board[i] = 1;
            continue;
        }
        REP(j, N){
            REP(k, N) {
                REP(l, 2) {
                    ok = check(j, k, i,l);
                    if (ok) {
                        rireki[i].emplace_back(MP(j, k), l);
                        break;
                    }
                }
                if (ok)break;
            }
            if (ok)break;
        }
        if (ok) {
            board[i] = 1;
            res++;
        }
        else {
            board[i] = 0;
        }
    }
    return res;
}

int new_get_score() {
    int res = 0;
    REP(i, M) {
        bool ok = false;
        //cerr << i<<" "<<SZ(rireki[i]) << endl;
        REP(j, SZ(rireki[i])) {
            int x, y, d;
            tie(x, y) = rireki[i][j].first;
            d = rireki[i][j].second;
            bool flag = check(x, y, i, d);
            if (flag) {
                ok = true;
                break;
            }
        }
        if (ok) {
            res++;
            board[i] = 1;
            continue;
        }
        REP(j, N) {//x
            int m = 0, k = 0;
            while (m < N&&ok == false) {
                if (hint[i].second[k] == grid[j][(m + k)%N]) {
                    k++;
                    if (k == hint[i].first) {
                        rireki[i].emplace_back(MP(j, m), 1);
                        ok = true;
                        break;
                    }
                }
                else {
                    m = m + k - table[i][k];
                    if (k > 0) k = table[i][k];
                }
                if (ok)break;
            }
            if (ok)break;
        }
        if (ok)break;
        REP(j, N) {//y
            int m = 0, k = 0;
            while (m < N&&ok==false) {
                if (hint[i].second[k] == grid[(m + k) % N][j]) {
                    k++;
                    if (k == hint[i].first) {
                        rireki[i].emplace_back(MP(m, j), 0);
                        ok = true;
                    }
                }
                else {
                    m = m + k - table[i][k];
                    if (k > 0) k = table[i][k];
                }
            }
            if (ok)break;
        }
        if (ok) {
            board[i] = 1;
            res++;
        }
        else {
            board[i] = 0;
        }
    }
    return res;
}


int get_fullscore() {
    int res = 0;
    REP(i, M) {
        bool ok = false;
        rireki[i] = V<pair<P, int>>();
        REP(j, N) {
            REP(k, N) {
                REP(l, 2) {
                    bool flag = check(j, k, i, l);
                    if (flag) {
                        rireki[i].emplace_back(MP(j, k), l);
                        ok = true;
                    }
                }
            }
        }
        if (ok) {
            res++;
            board[i] = 1;
        }
        else {
            board[i] = 0;
        }
    }
    return res;
}

void out() {
    REP(i, N) {
        REP(j, N) {
            cout << char('A' + best_grid[i][j]);
        }
        cout << endl;
    }
}

signed main() {
    cin.tie(0);
    ios::sync_with_stdio(false);
    cout << fixed << setprecision(15);
    t_start = chrono::system_clock::now();

    //input
    cin >> N >> M;
    hint = V<pair<int,string>>(M);
    grid = VV<int>(N, V<int>(N));
    best_grid = VV<int>(N, V<int>(N));
    rireki = VV<pair<P, int>>(M);
    table = VV<int>(M);
    board = V<int>(M);

    //init
    REP(i, M) {
        cin >> hint[i].second;
        hint[i].first = SZ(hint[i].second);
    }
    get_table();

    //sort(ALL(hint));
    REP(i, N)REP(j, N)grid[i][j] = init(rng);
    ll p = get_fullscore();
    score = p * 100000000 / M;
    best_score = score;
    best_grid = grid;

    uniform_int_distribution<> clue(0, M - 1);
    int c = 0;

    while (t_diff < TIME_LIMIT) {
        int x = zahyo(rng);
        int y = zahyo(rng);
        int c = clue(rng);
        int d = nibun(rng);
        //cerr << x << " " << y << " " << c << " " << d << endl;
        if (board[c]==1) {
            c++;
            c %= M;
            continue;
        }
        if (grid[x][y] != (hint[c].second[0] - 'A'))continue;
        change(x, y, c, d);
        rireki[c].emplace_back(MP(x,y), d);

        ll p = get_score();
        //if (p!= q)cerr << "YES" << endl;
        //cerr << p << " " << q << endl;
        score = p * 100000000 / M;
        double prob = exp((score - best_score) / get_temp());
        if (prob > dist(rng)) {
            update_cnt++;
            last_update = t_diff;
            if (chmax(best_score, score)) {
                REP(i, N)REP(j, N) {
                    if (best_grid[i][j] == grid[i][j])continue;
                    best_grid[i][j] = grid[i][j];
                }
            }
        }
        else {
            unchange(x, y, c, d);
            rireki[c].pop_back();
        }
        cnt++;
        c++;
        c %= M;
        get_time();
    }

    out();
    get_time();
    cerr << "time:  " << t_diff << endl;
    cerr << "last:  " << last_update << endl;
    cerr << "cnt:   " << cnt << endl;
    cerr << "update:" << update_cnt << endl;
    cerr << "score: " << best_score<<endl;

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
const double TIME_LIMIT = 2950;
double last_update = 0, t_diff;
const int T0 = 2000, T1 = 200;
VV<int> grid,best_grid;
V<int> board;
V<pair<int,string>> hint;
V<int> y_vec = { 0, 1};
V<int> x_vec = { 1, 0};
V<int> backup(20);
VV<int> table;
VV<pair<P, int>> rireki;

int N, M;
ll score,best_score;
ll cnt = 0,update_cnt=0;

mt19937 rng;
uniform_real_distribution<> dist(0.0, 1.0);
uniform_int_distribution<> init(0, 7);
uniform_int_distribution<> zahyo(0, 19);
uniform_int_distribution<> nibun(0, 1);


/*
double prob = exp((new_best_score-init_score) / get_temp());
    if (new_best_score!=0&&prob > dist(rng)) {
*/


void get_time() {
    auto t_now = chrono::system_clock::now();
    t_diff = chrono::duration_cast<chrono::milliseconds>(t_now - t_start).count();
}

double get_temp() {
    return T0 + (T1 - T0) * (1 - t_diff / TIME_LIMIT);
}

void get_table() {
    REP(k, M) {
        int n = SZ(hint[k].second);
        table[k] = V<int>(n+1);
        table[k][0] = -1;
        int j = -1;
        REP (i,n) {
            while (j >= 0 && hint[k].second[i] != hint[k].second[j]) j = table[k][j];
            j++;
            table[k][i + 1] = j;
        }
    }
}

bool check(int x, int y,int n,int d) {
    int tx = x, ty = y;
    bool ok = true;
    REP(j, SZ(hint[n].second)) {
        if (char('A' + grid[tx][ty]) != hint[n].second[j]) {
            ok = false;
            break;
        }
        tx += x_vec[d];
        ty += y_vec[d];
        tx %= N;
        ty %= N;
    }
    return ok;
}

void change(int x, int y, int n, int d) {
    REP(i, SZ(hint[n].second)) {
        backup[i] = grid[x][y];
        grid[x][y] = hint[n].second[i] - 'A';
        x += x_vec[d];
        y += y_vec[d];
        x %= N;
        y %= N;
    }
}

void unchange(int x, int y, int n, int d) {
    REP(i, SZ(hint[n].second)) {
        grid[x][y] = backup[i];
        x += x_vec[d];
        y += y_vec[d];
        x %= N;
        y %= N;
    }
}

int get_score() {
    int res = 0;
    REP(i, M) {
        bool ok = false;
        //cerr << i<<" "<<SZ(rireki[i]) << endl;
        REP(j,SZ(rireki[i])){
            int x, y, d;
            tie(x, y) = rireki[i][j].first;
            d = rireki[i][j].second;
            bool flag = check(x, y, i, d);
            if (flag) {
                ok = true;
                break;
            }
        }
        if (ok) {
            res++;
            board[i] = 1;
            continue;
        }
        REP(j, N){
            REP(k, N) {
                REP(l, 2) {
                    ok = check(j, k, i,l);
                    if (ok) {
                        rireki[i].emplace_back(MP(j, k), l);
                        break;
                    }
                }
                if (ok)break;
            }
            if (ok)break;
        }
        if (ok) {
            board[i] = 1;
            res++;
        }
        else {
            board[i] = 0;
        }
    }
    return res;
}

int new_get_score() {
    int res = 0;
    REP(i, M) {
        bool ok = false;
        //cerr << i<<" "<<SZ(rireki[i]) << endl;
        REP(j, SZ(rireki[i])) {
            int x, y, d;
            tie(x, y) = rireki[i][j].first;
            d = rireki[i][j].second;
            bool flag = check(x, y, i, d);
            if (flag) {
                ok = true;
                break;
            }
        }
        if (ok) {
            res++;
            board[i] = 1;
            continue;
        }
        REP(j, N) {//x
            int m = 0, k = 0;
            while (m < N&&ok == false) {
                if (hint[i].second[k] == grid[j][(m + k)%N]) {
                    k++;
                    if (k == hint[i].first) {
                        rireki[i].emplace_back(MP(j, m), 1);
                        ok = true;
                        break;
                    }
                }
                else {
                    m = m + k - table[i][k];
                    if (k > 0) k = table[i][k];
                }
                if (ok)break;
            }
            if (ok)break;
        }
        if (ok)break;
        REP(j, N) {//y
            int m = 0, k = 0;
            while (m < N&&ok==false) {
                if (hint[i].second[k] == grid[(m + k) % N][j]) {
                    k++;
                    if (k == hint[i].first) {
                        rireki[i].emplace_back(MP(m, j), 0);
                        ok = true;
                    }
                }
                else {
                    m = m + k - table[i][k];
                    if (k > 0) k = table[i][k];
                }
            }
            if (ok)break;
        }
        if (ok) {
            board[i] = 1;
            res++;
        }
        else {
            board[i] = 0;
        }
    }
    return res;
}


int get_fullscore() {
    int res = 0;
    REP(i, M) {
        bool ok = false;
        rireki[i] = V<pair<P, int>>();
        REP(j, N) {
            REP(k, N) {
                REP(l, 2) {
                    bool flag = check(j, k, i, l);
                    if (flag) {
                        rireki[i].emplace_back(MP(j, k), l);
                        ok = true;
                    }
                }
            }
        }
        if (ok) {
            res++;
            board[i] = 1;
        }
        else {
            board[i] = 0;
        }
    }
    return res;
}

void out() {
    REP(i, N) {
        REP(j, N) {
            cout << char('A' + best_grid[i][j]);
        }
        cout << endl;
    }
}

signed main() {
    cin.tie(0);
    ios::sync_with_stdio(false);
    cout << fixed << setprecision(15);
    t_start = chrono::system_clock::now();

    //input
    cin >> N >> M;
    hint = V<pair<int,string>>(M);
    grid = VV<int>(N, V<int>(N));
    best_grid = VV<int>(N, V<int>(N));
    rireki = VV<pair<P, int>>(M);
    table = VV<int>(M);
    board = V<int>(M);

    //init
    REP(i, M) {
        cin >> hint[i].second;
        hint[i].first = SZ(hint[i].second);
    }
    get_table();

    //sort(ALL(hint));
    REP(i, N)REP(j, N)grid[i][j] = init(rng);
    ll p = get_fullscore();
    score = p * 100000000 / M;
    best_score = score;
    best_grid = grid;

    uniform_int_distribution<> clue(0, M - 1);
    int c = 0;

    while (t_diff < TIME_LIMIT) {
        int x = zahyo(rng);
        int y = zahyo(rng);
        int c = clue(rng);
        int d = nibun(rng);
        //cerr << x << " " << y << " " << c << " " << d << endl;
        if (board[c]==1) {
            c++;
            c %= M;
            continue;
        }
        if (grid[x][y] != (hint[c].second[0] - 'A'))continue;
        change(x, y, c, d);
        rireki[c].emplace_back(MP(x,y), d);

        ll p = get_score();
        //if (p!= q)cerr << "YES" << endl;
        //cerr << p << " " << q << endl;
        score = p * 100000000 / M;
        double prob = exp((score - best_score) / get_temp());
        if (prob > dist(rng)) {
            update_cnt++;
            last_update = t_diff;
            if (chmax(best_score, score)) {
                REP(i, N)REP(j, N) {
                    if (best_grid[i][j] == grid[i][j])continue;
                    best_grid[i][j] = grid[i][j];
                }
            }
        }
        else {
            unchange(x, y, c, d);
            rireki[c].pop_back();
        }
        cnt++;
        c++;
        c %= M;
        get_time();
    }

    out();
    get_time();
    cerr << "time:  " << t_diff << endl;
    cerr << "last:  " << last_update << endl;
    cerr << "cnt:   " << cnt << endl;
    cerr << "update:" << update_cnt << endl;
    cerr << "score: " << best_score<<endl;

    return 0;
}
>>>>>>> origin/main
