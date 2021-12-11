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
int cnt = 0, update_cnt = 0;
unordered_map<char, int> dir_map;
V<char> direction = { 'U','R','D','L' };
V<ll> y_vec = { 0, 1,0, -1 };
V<ll> x_vec = { -1, 0,1, 0 };

int N, sx, sy, ox, oy, tx, ty, jikan, best_jikan;
string ans, best_ans;
VV<int> grid, kyori;
VV<string> michi;
VV<pair<int, P>> tate, yoko;
V<int> check_t, check_y;
VV<P> match;
V<P> goal, route, best_route;
priority_queue<pair<int, P>, V<pair<int, P>>, greater<pair<int, P>>> q;

mt19937 rng;
uniform_real_distribution<> dist(0.0, 1.0);
uniform_int_distribution<> dist_int(0, 7);


/*
double prob = exp((next_score-current_score) / get_temp());
if (prob > dist(rng)) {
*/


void get_time() {
    auto t_now = chrono::system_clock::now();
    t_diff = chrono::duration_cast<chrono::milliseconds>(t_now - t_start).count();
}

double get_temp() {
    return T0 + (T1 - T0) * (t_diff / TIME_LIMIT);
}

void Dijkstra() {
    kyori = VV<int>(N, V<int>(N, INF));
    michi = VV<string>(N, V<string>(N));
    q = priority_queue<pair<int, P>, V<pair<int, P>>, greater<pair<int, P>>>();
    kyori[ox][oy] = 0;
    q.push(MP(0, MP(ox, oy)));
    while (!q.empty()) {
        int dis = q.top().first, x, y; tie(x, y) = q.top().second;
        q.pop();
        if (dis > kyori[x][y])continue;
        REP(i, 4) {
            int nx, ny;
            nx = x + x_vec[i]; ny = y + y_vec[i];
            if (nx<0 || nx>N - 1 || ny<0 || ny>N - 1 || grid[nx][ny] == 0)continue;
            if (chmin(kyori[nx][ny], kyori[x][y] + grid[nx][ny])) {
                michi[nx][ny] = michi[x][y] + direction[i];
                q.push(MP(kyori[nx][ny], MP(nx, ny)));
            }
        }
    }
}

void mini_Dijkstra() {
    kyori = VV<int>(N, V<int>(N, INF));
    michi = VV<string>(N, V<string>(N));
    q = priority_queue<pair<int, P>, V<pair<int, P>>, greater<pair<int, P>>>();
    kyori[ox][oy] = 0;
    q.push(MP(0, MP(ox, oy)));
    while (!q.empty()) {
        int dis = q.top().first, x, y; tie(x, y) = q.top().second;
        q.pop();
        if (dis > kyori[tx][ty] || dis > kyori[x][y])continue;
        REP(i, 4) {
            int nx, ny;
            nx = x + x_vec[i]; ny = y + y_vec[i];
            if (nx<0 || nx>N - 1 || ny<0 || ny>N - 1 || grid[nx][ny] == 0)continue;
            if (chmin(kyori[nx][ny], kyori[x][y] + grid[nx][ny])) {
                michi[nx][ny] = michi[x][y] + direction[i];
                q.push(MP(kyori[nx][ny], MP(nx, ny)));
            }
        }
    }
}

void init() {
    cin >> N >> sx >> sy;
    ans = "";
    grid = VV<int>(N, V<int>(N, 0));
    match = VV<P>(N, V<P>(N, MP(-1, -1)));
    dir_map['U'] = 0; dir_map['R'] = 1; dir_map['D'] = 2; dir_map['L'] = 3;
    REP(i, N) {
        string s; cin >> s;
        REP(j, N) {
            if (s[j] == '#')continue;
            grid[i][j] = s[j] - '0';
        }
    }
    int hor_cnt = 0;
    REP(i, N) {
        bool ok = false;
        REP(j, N) {
            if (!ok && j < N - 1) {
                if (grid[i][j] != 0 && grid[i][j + 1] != 0) {
                    ok = true;
                    yoko.emplace_back(V <pair<int, P>>());
                }
            }
            if (ok) {
                if ((i > 0 && grid[i - 1][j] != 0) || (i < N - 1 && grid[i + 1][j] != 0)) {
                    yoko[hor_cnt].emplace_back(MP(grid[i][j], MP(i, j)));
                    match[i][j].second = hor_cnt;
                }
                if (j < N - 1 && grid[i][j + 1] == 0) {
                    ok = false;
                    hor_cnt++;
                }
            }
        }
        if (ok)hor_cnt++;
    }
    int ver_cnt = 0;
    REP(j, N) {
        bool ok = false;
        REP(i, N) {
            if (!ok && i < N - 1) {
                if (grid[i][j] != 0 && grid[i + 1][j] != 0) {
                    ok = true;
                    tate.emplace_back(V <pair<int, P>>());
                }
            }
            if (ok) {
                if ((j > 0 && grid[i][j - 1] != 0) || (j < N - 1 && grid[i][j + 1] != 0)) {
                    tate[ver_cnt].emplace_back(MP(grid[i][j], MP(i, j)));
                    match[i][j].first = ver_cnt;
                }
                if (i < N - 1 && grid[i + 1][j] == 0) {
                    ok = false;
                    ver_cnt++;
                }
            }
        }
        if (ok)ver_cnt++;
    }
    check_t = V<int>(SZ(tate));
    check_y = V<int>(SZ(yoko));
    REP(i, SZ(tate))sort(ALL(tate[i]));
    REP(i, SZ(yoko))sort(ALL(yoko[i]));

    REP(i, SZ(tate))goal.emplace_back(tate[i][0].second);
    REP(i, SZ(yoko))goal.emplace_back(yoko[i][0].second);

    sort(ALL(goal));
    goal.erase(unique(goal.begin(), goal.end()), goal.end());

    ox = sx; oy = sy;
    jikan = 0;
    ans = "";
    route.emplace_back(MP(sx, sy));
    while (SZ(goal) > 0) {
        Dijkstra();
        int mi = INF, id = INF;
        REP(i, SZ(goal)) {
            int x, y; tie(x, y) = goal[i];
            if (chmin(mi, kyori[x][y])) {
                id = i;
            }
        }
        ox = goal[id].first;
        oy = goal[id].second;
        ans += michi[ox][oy];
        jikan += kyori[ox][oy];
        route.emplace_back(goal[id]);
        //cerr << jikan << endl;
        goal.erase(goal.begin() + id);

    }
    Dijkstra();
    ans += michi[sx][sy];
    jikan += kyori[sx][sy];
    route.emplace_back(MP(sx, sy));
    best_jikan = jikan;
    best_route = route;
    best_ans = ans;
}

int get_score() {
    ox = sx; oy = sy;
    check_t = V<int>(SZ(tate), 0);
    check_y = V<int>(SZ(yoko), 0);
    jikan = 0;
    ans = "";
    REP(i, SZ(route)) {
        if (i == 0)continue;
        tie(tx, ty) = route[i];
        //cerr << tx << " " << ty << endl;
        int t_id, y_id;
        tie(t_id, y_id) = match[tx][ty];
        if (i != SZ(route) - 1 && t_id != -1 && check_t[t_id] && check_y[y_id])continue;
        //cerr << tx << " " << ty << endl;
        mini_Dijkstra();

        REP(i, SZ(michi[tx][ty])) {
            ox += x_vec[dir_map[michi[tx][ty][i]]];
            oy += y_vec[dir_map[michi[tx][ty][i]]];
            if (match[ox][oy].first == -1)continue;
            check_t[match[ox][oy].first] = 1;
            check_y[match[ox][oy].second] = 1;
        }

        ox = tx;
        oy = ty;
        ans += michi[ox][oy];
        jikan += kyori[ox][oy];
    }
    //cerr << jikan << endl;
    //cerr << best_jikan << endl;

    if (chmin(best_jikan, jikan)) {
        if (best_ans == ans)cerr << "YYYYY" << endl;
        best_route = route;
        best_ans = ans;
        update_cnt++;
        last_update = t_diff;
    }
    return jikan;
}

void change_jun() {
    uniform_int_distribution<> dist_int(1, SZ(route) - 2);
    //uniform_int_distribution<> dist_n(1, 10);

    int a = dist_int(rng), b = dist_int(rng);
    //int c = a + dist_n(rng);
    if (a == b)return;
    route[a].swap(route[b]);
    if (best_jikan != get_score())route[a].swap(route[b]);
    cnt++;
}

signed main() {
    cin.tie(0);
    ios::sync_with_stdio(false);
    cout << fixed << setprecision(15);
    t_start = chrono::system_clock::now();

    init();
    int a, b;
    a = get_score();
    reverse(ALL(route));
    b = get_score();
    if (a < b)reverse(ALL(route));

    while (t_diff < TIME_LIMIT) {
        change_jun();
        get_time();
    }

    cout << best_ans << endl;

    get_time();

    cerr << "time:  " << t_diff << endl;
    cerr << "score: " << round(10000 + double(10000000) * N / best_jikan) << endl;
    cerr << "last:  " << last_update << endl;
    cerr << "cnt:   " << cnt << endl;
    cerr << "update:" << update_cnt << endl;

    return 0;
}
