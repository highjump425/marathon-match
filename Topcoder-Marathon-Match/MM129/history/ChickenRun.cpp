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
#define INF 10001000
#define endl "\n"

chrono::system_clock::time_point t_start;
const double TIME_LIMIT = 9950;
double last_update = 0, t_diff;
double start_temp, end_temp;

mt19937 rng;
uniform_real_distribution<> dist(0.0, 1.0);
uniform_int_distribution<> dist_int(0, 3);
uniform_int_distribution<> jun(0, 23);

V<int> x_vec = { -1, 0,1, 0 };
V<int> y_vec = { 0, 1,0, -1 };

int N,elaspedTime,key;
VV<int> grid,check1,check2;
V<VV<int>> rooms;
VV<int> perm_dir(24);
V<P> chickens, farmers,destinations;
VV<P> teams;
V<string> moves;
V<bool> reached;
map<P, bool> chased;

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
    return start_temp + (end_temp - start_temp) * (t_diff/TIME_LIMIT);
}

bool reachable(P p) {
    int x, y; tie(x, y) = p;
    if (x < 0 || x >= N || y < 0 || y >= N || grid[x][y] >=2 )return false;
    else return true;
}

bool is_farmer(P p) {
    int x, y; tie(x, y) = p;
    if (x < 0 || x >= N || y < 0 || y >= N || grid[x][y] != 2)return false;
    else return true;
}

void get_rooms() {
    rooms = V<VV<int>>(N, VV<int>(N, V<int>(4)));
    REP(i, N)REP(j, N) {
        REP(k, 4) {
            int cnt = 0;
            int x = i, y = j;
            while (1) {
                x += x_vec[k];
                y += y_vec[k];
                if (x < 0 || x >= N || y < 0 || y >= N || grid[x][y] == INF)break;
                cnt++;
            }
            rooms[i][j][k] = cnt;
        }
    }
}

void input() {
    grid = VV<int>(N, V<int>(N));
    chickens = V<P>();
    farmers = V<P>();
    moves = V<string>();
    REP(i, N)REP(j, N) {
        char c; cin >> c;
        if (c == '.')continue;
        if (c == 'C') {
            grid[i][j] = -1;
            chickens.emplace_back(i, j);
        }
        else if (c == '#') {
            grid[i][j] = INF;
        }
        else {
            farmers.emplace_back(i, j);
            grid[i][j] = 2;
            /*
            REP(k, 4) {
                int x = i + x_vec[k];
                int y = j + y_vec[k];
                if (x < 0 || x >= N || y < 0 || y >= N || grid[x][y] != 0)continue;
                grid[x][y] = 1;
            }
            */
        }
    }
    cerr << N * N << " " << SZ(chickens) <<" "<< SZ(farmers) << endl;
    get_rooms();
}

void update() {
    grid = VV<int>(N, V<int>(N));
    chickens = V<P>();
    moves = V<string>();
    REP(i, N)REP(j, N) {
        char c; cin >> c;
        if (c == '.')continue;
        if (c == 'C') {
            grid[i][j] = -1;
            chickens.emplace_back(i, j);
        }
        else if (c == '#') {
            grid[i][j] = INF;
        }
        else {
            grid[i][j] = 2;
        }
    }
}

string order(P s, P t) {
    return to_string(s.first) + " " + to_string(s.second) +
        " " + to_string(t.first) + " " + to_string(t.second);
}

void set_destination(int r) {
    //if (r >= N)r = (N-1) - (r-N+1);
    ll cnt=0;
    V<bool> line(N);
    //V<P> target(SZ(farmers),MP(-1,-1));
    destinations = V<P>(SZ(farmers),MP(-1,-1));
    reached = V<bool>(SZ(farmers),false);

    if (r<(N-SZ(farmers)+1)) {
        REP(i, N) {
            if (grid[i][r] == INF) {
                line[i] = true;
            }
        }
        REP(i, N) {
            if (line[i] == true)continue;
            //if ((i==0||line[i-1]==true) && (i<N-1&&line[i + 1] == false))continue;
            destinations[cnt] = MP(i, r);
            cnt++;
            line[i] = true;
            //if (i < N - 1)line[i + 1] = true;
            if (cnt == SZ(farmers))break;
        }
    }
    else if(r<2* (N - SZ(farmers) + 1)){
        r = r - (N - SZ(farmers) + 1);
        FORD(i, N - 1, 0) {
            if (grid[r][i] == INF) {
                line[i] = true;
            }
        }
        FORD(i, N - 1, 0) {
            if (line[i] == true)continue;
            //if ((i==N-1||line[i+1]==true) && (i>0&&line[i - 1] == false))continue;
            destinations[SZ(farmers)-1-cnt] = MP(r, i);
            cnt++;
            line[i] = true;
            //if (i < N - 1)line[i + 1] = true;
            if (cnt == SZ(farmers))break;
        }
    }
    else if (r <3* (N - SZ(farmers) + 1)) {
        r = N - 1 - (r - 2 * (N - SZ(farmers) + 1));
        FORD(i, N - 1, 0) {
            if (grid[i][r] == INF) {
                line[i] = true;
            }
        }
        FORD(i, N - 1, 0) {
            if (line[i] == true)continue;
            //if ((i == N-1 || line[i + 1] == true) && (i > 0 && line[i - 1] == false))continue;
            destinations[cnt] = MP(i, r);
            cnt++;
            line[i] = true;
            //if (i < N - 1)line[i + 1] = true;
            if (cnt == SZ(farmers))break;
        }
    }
    else {
        r = N - 1 - (r - 3 * (N - SZ(farmers) + 1));
        REP(i, N) {
            if (grid[r][i] == INF) {
                line[i] = true;
            }
        }
        REP(i, N) {
            if (line[i] == true)continue;
            //if ((i == 0 || line[i - 1] == true) && (i < N - 1 && line[i + 1] == false))continue;
            destinations[SZ(farmers) - 1 - cnt] = MP(r, i);
            cnt++;
            line[i] = true;
            //if (i < N - 1)line[i + 1] = true;
            if (cnt == SZ(farmers))break;
        }
    }

    //REP(i, SZ(destinations))cerr << destinations[i].first << " " << destinations[i].second << endl;
}

P get_direction(P s,P t) {
    int sx, sy; tie(sx, sy) = s;
    int tx, ty; tie(tx, ty) = t;
    P res = { -1,-1 };
    if ((sx == tx && sy == ty) || (grid[tx][ty] ==INF))return res;
    VV<int> check(N, V<int>(N,-1));
    queue<P> q;
    REP(i, 4) {
        int k = perm_dir[key][i];
        int nx = sx + x_vec[k];
        int ny = sy + y_vec[k];
        if (nx < 0 || nx >= N || ny < 0 || ny >= N || grid[nx][ny]==INF)continue;
        q.emplace(nx, ny);
        check[nx][ny] = k;
    }
    while (!q.empty()) {
        int x, y; tie(x, y) = q.front();
        q.pop();
        REP(i, 4) {
            int k = perm_dir[key][i];
            int nx = x + x_vec[k];
            int ny = y + y_vec[k];
            if (nx < 0 || nx >= N || ny < 0 || ny >= N || grid[nx][ny] ==INF)continue;
            if (check[nx][ny] != -1)continue;
            check[nx][ny] = check[x][y];
            q.emplace(nx, ny);
        }
    }
    if (check[tx][ty] == -1)return res;
    else {
        int dir = check[tx][ty];
        return MP(sx + x_vec[dir], sy + y_vec[dir]);
    }
}

P get_direction2(P s, P t) {
    int sx, sy; tie(sx, sy) = s;
    int tx, ty; tie(tx, ty) = t;
    P res = { -1,-1 };
    if ((sx == tx && sy == ty) || (grid[tx][ty] >= 2))return res;
    VV<int> check(N, V<int>(N, -1));
    queue<P> q;
    REP(i, 4) {
        int k = perm_dir[key][i];
        int nx = sx + x_vec[k];
        int ny = sy + y_vec[k];
        if (nx < 0 || nx >= N || ny < 0 || ny >= N || grid[nx][ny] >= 2)continue;
        q.emplace(nx, ny);
        check[nx][ny] = k;
    }
    while (!q.empty()) {
        int x, y; tie(x, y) = q.front();
        q.pop();
        REP(i, 4) {
            int k = perm_dir[key][i];
            int nx = x + x_vec[k];
            int ny = y + y_vec[k];
            if (nx < 0 || nx >= N || ny < 0 || ny >= N || grid[nx][ny] >= 2)continue;
            if (check[nx][ny] != -1)continue;
            check[nx][ny] = check[x][y];
            q.emplace(nx, ny);
        }
    }
    if (check[tx][ty] == -1)return res;
    else {
        int dir = check[tx][ty];
        return MP(sx + x_vec[dir], sy + y_vec[dir]);
    }
}

P nearest_chicken(P s) {
    int sx, sy; tie(sx, sy) = s;
    VV<int> check(N, V<int>(N, -1));
    queue<P> q;
    //int id=jun(rng);
    REP(i, 4) {
        int k = perm_dir[key][i];
        int nx = sx + x_vec[k];
        int ny = sy + y_vec[k];
        if (nx < 0 || nx >= N || ny < 0 || ny >= N)continue;
        if (grid[nx][ny] == -1)return MP(nx, ny);
        else if (grid[nx][ny] >= 2)continue;
        q.emplace(nx, ny);
        check[nx][ny] = k;
    }
    int dir = -1;
    while (!q.empty()) {
        int x, y; tie(x, y) = q.front();
        q.pop();
        //int id = jun(rng);
        REP(i, 4) {
            int k = perm_dir[key][i];
            int nx = x + x_vec[k];
            int ny = y + y_vec[k];
            if (nx < 0 || nx >= N || ny < 0 || ny >= N)continue;
            if(grid[nx][ny] >= INF|| check[nx][ny] != -1)continue;
            if (grid[nx][ny] == -1) {
                dir = check[x][y];
                return MP(sx + x_vec[dir], sy + y_vec[dir]);
            }
            check[nx][ny] = check[x][y];
            q.emplace(nx, ny);
        }
    }
    return { -1,-1 };
}

P nearest_chicken2(P s) {
    int sx, sy; tie(sx, sy) = s;
    VV<int> check(N, V<int>(N, -1));
    queue<P> q;
    //int id=jun(rng);
    REP(i, 4) {
        int k = perm_dir[key][i];
        int nx = sx + x_vec[k];
        int ny = sy + y_vec[k];
        if (nx < 0 || nx >= N || ny < 0 || ny >= N)continue;
        if (grid[nx][ny] == -1)return MP(nx, ny);
        else if (grid[nx][ny] >= 2)continue;
        q.emplace(nx, ny);
        check[nx][ny] = k;
    }
    int dir = -1;
    while (!q.empty()) {
        int x, y; tie(x, y) = q.front();
        q.pop();
        //int id = jun(rng);
        REP(i, 4) {
            int k = perm_dir[key][i];
            int nx = x + x_vec[k];
            int ny = y + y_vec[k];
            if (nx < 0 || nx >= N || ny < 0 || ny >= N)continue;
            if (grid[nx][ny] >= INF || check[nx][ny] != -1)continue;
            if (grid[nx][ny] == -1) {
                dir = check[x][y];
                return MP(nx, ny);
            }
            check[nx][ny] = check[x][y];
            q.emplace(nx, ny);
        }
    }
    return { -1,-1 };
}

P nearest_chicken3(V<P> team) {
    int sx, sy; tie(sx, sy) = team[0];
    check1 = VV<int>(N, V<int>(N, INF));
    check2 = VV<int>(N, V<int>(N, INF));
    queue<P> q;
    check1[sx][sy] = 0;
    q.emplace(sx, sy);
    while (!q.empty()) {
        int x, y; tie(x, y) = q.front();
        q.pop();
        REP(i, 4) {
            int k = perm_dir[key][i];
            int nx = x + x_vec[k];
            int ny = y + y_vec[k];
            if (nx < 0 || nx >= N || ny < 0 || ny >= N|| grid[nx][ny] >= INF)continue;
            if (chmin(check1[nx][ny], check1[x][y] + 1)) {
                q.emplace(nx, ny);
            }
        }
    }
    tie(sx, sy) = team[1];
    check2[sx][sy] = 0;
    q.emplace(sx, sy);
    while (!q.empty()) {
        int x, y; tie(x, y) = q.front();
        q.pop();
        REP(i, 4) {
            int k = perm_dir[key][i];
            int nx = x + x_vec[k];
            int ny = y + y_vec[k];
            if (nx < 0 || nx >= N || ny < 0 || ny >= N || grid[nx][ny] >= INF)continue;
            if (chmin(check2[nx][ny], check2[x][y] + 1)) {
                q.emplace(nx, ny);
            }
        }
    }
    P res = MP(-1, -1), res2 = MP(-1, -1);
    int dis = 2*INF, dis2 = 2*INF;
    for (P p : chickens) {
        int x, y; tie(x, y) = p;
        int r1 = min(x - 0, N - 1 - x);
        int r2 = min(y - 0, N - 1 - y);
        //int r1 = min(rooms[x][y][0], rooms[x][y][2]);
        //int r2 = min(rooms[x][y][1], rooms[x][y][3]);
        if (grid[x][y] != -1)continue;
        if (check1[x][y] == INF && check2[x][y] == INF) continue;
        if (check1[x][y] == INF || check2[x][y] == INF) {
            if (2*chmin(dis2, int(pow(2*min(check1[x][y],check2[x][y]),2)) + 1 * (r1+r2)))res2 = p;
        }
        else {
            if (2 * chmin(dis2, int(pow(check1[x][y] + check2[x][y],2)) + 1 * (r1 + r2)))res2 = p;
        }
        if (chased[p])continue;
        if (check1[x][y] == INF || check2[x][y] == INF) {
            if (2 * chmin(dis, 2 * min(check1[x][y], check2[x][y]) + 1 * (r1 + r2)))res = p;
        }
        else {
            if (2 * chmin(dis, check1[x][y] + check2[x][y] + 1 * (r1 + r2)))res = p;
        }
    }
    if (res.first != -1)return res;
    else return res2;
    //return res2;
}

bool manual(int id) {
    bool res = false;
    if (teams[id][0] > teams[id][1])swap(teams[id][0], teams[id][1]);
    int x1, y1; tie(x1, y1) = teams[id][0];
    int x2, y2; tie(x2, y2) = teams[id][1];
    if (abs(x1 - x2) + abs(y1 - y2) > 2)return false;
    if (abs(x1 - x2) + abs(y1 - y2) == 1) {
        if (x1 == x2) {
            bool ok1 = false, ok2 = false;
            REP(i, 4) {
                int nx = x1 + x_vec[(3 + i) % 4];
                int ny = y1 + y_vec[(3 + i) % 4];
                if (reachable(MP(nx, ny)) && grid[nx][ny] == -1) {
                    ok1 = true;
                    moves.emplace_back(order(teams[id][0], MP(nx, ny)));
                    teams[id][0] = MP(nx, ny);
                    grid[x1][y1] = 0;
                    grid[nx][ny] = 2;
                    break;
                }
            }
            REP(i, 4) {
                int nx = x2 + x_vec[(1 + i) % 4];
                int ny = y2 + y_vec[(1 + i) % 4];
                if (reachable(MP(nx, ny)) && grid[nx][ny] == -1) {
                    ok2 = true;
                    moves.emplace_back(order(teams[id][1], MP(nx, ny)));
                    teams[id][1] = MP(nx, ny);
                    grid[x2][y2] = 0;
                    grid[nx][ny] = 2;
                    break;
                }
            }
            if (ok1 || ok2)return true;

            if ((reachable(MP(x1 - 1, y1 - 1)) && grid[x1 - 1][y1 - 1] == -1)
                || (reachable(MP(x1 + 1, y1 - 1)) && grid[x1 + 1][y1 - 1] == -1)) {
                if (reachable(MP(x1, y1 - 1))) {
                    moves.emplace_back(order(teams[id][0], MP(x1, y1 - 1)));
                    teams[id][0] = MP(x1, y1 - 1);
                    grid[x1][y1] = 0;
                    grid[x1][y1 - 1] = 2;
                    return true;
                }
            }
            if ((reachable(MP(x2 - 1, y2 + 1)) && grid[x2 - 1][y2 + 1] == -1)
                || (reachable(MP(x2 + 1, y2 + 1)) && grid[x2 + 1][y2 + 1] == -1)) {
                if (reachable(MP(x2, y2 + 1))) {
                    moves.emplace_back(order(teams[id][1], MP(x2, y2 + 1)));
                    teams[id][1] = MP(x2, y2 + 1);
                    grid[x2][y2] = 0;
                    grid[x2][y2 + 1] = 2;
                    return true;
                }
            }

            if (min(rooms[x1][y1][0], rooms[x2][y2][0]) < min(rooms[x1][y1][2], rooms[x2][y2][2])) {
                if ((reachable(MP(x1 - 2, y1)) && grid[x1 - 2][y1] == -1)
                    || (reachable(MP(x2 - 2, y2)) && grid[x2 - 2][y2] == -1)) {
                    if (reachable(MP(x1 - 1, y1)) || reachable(MP(x2 - 1, y2))) {
                        if (reachable(MP(x1 - 1, y1))) {
                            moves.emplace_back(order(teams[id][0], MP(x1 - 1, y1)));
                            teams[id][0] = MP(x1 - 1, y1);
                            grid[x1][y1] = 0;
                            grid[x1 - 1][y1] = 2;
                        }
                        if (reachable(MP(x2 - 1, y2))) {
                            moves.emplace_back(order(teams[id][1], MP(x2 - 1, y2)));
                            teams[id][1] = MP(x2 - 1, y2);
                            grid[x2][y2] = 0;
                            grid[x2 - 1][y2] = 2;
                        }
                        return true;
                    }
                }
                if ((reachable(MP(x1 + 2, y1)) && grid[x1 + 2][y1] == -1)
                    || (reachable(MP(x2 + 2, y2)) && grid[x2 + 2][y2] == -1)) {
                    if (reachable(MP(x1 + 1, y1)) || reachable(MP(x2 + 1, y2))) {
                        if (reachable(MP(x1 + 1, y1))) {
                            moves.emplace_back(order(teams[id][0], MP(x1 + 1, y1)));
                            teams[id][0] = MP(x1 + 1, y1);
                            grid[x1][y1] = 0;
                            grid[x1 + 1][y1] = 2;
                        }
                        if (reachable(MP(x2 + 1, y2))) {
                            moves.emplace_back(order(teams[id][1], MP(x2 + 1, y2)));
                            teams[id][1] = MP(x2 + 1, y2);
                            grid[x2][y2] = 0;
                            grid[x2 + 1][y2] = 2;
                        }
                        return true;
                    }
                }
            }
            else {
                if ((reachable(MP(x1 + 2, y1)) && grid[x1 + 2][y1] == -1)
                    || (reachable(MP(x2 + 2, y2)) && grid[x2 + 2][y2] == -1)) {
                    if (reachable(MP(x1 + 1, y1)) || reachable(MP(x2 + 1, y2))) {
                        if (reachable(MP(x1 + 1, y1))) {
                            moves.emplace_back(order(teams[id][0], MP(x1 + 1, y1)));
                            teams[id][0] = MP(x1 + 1, y1);
                            grid[x1][y1] = 0;
                            grid[x1 + 1][y1] = 2;
                        }
                        if (reachable(MP(x2 + 1, y2))) {
                            moves.emplace_back(order(teams[id][1], MP(x2 + 1, y2)));
                            teams[id][1] = MP(x2 + 1, y2);
                            grid[x2][y2] = 0;
                            grid[x2 + 1][y2] = 2;
                        }
                        return true;
                    }
                }
                if ((reachable(MP(x1 - 2, y1)) && grid[x1 - 2][y1] == -1)
                    || (reachable(MP(x2 - 2, y2)) && grid[x2 - 2][y2] == -1)) {
                    if (reachable(MP(x1 - 1, y1)) || reachable(MP(x2 - 1, y2))) {
                        if (reachable(MP(x1 - 1, y1))) {
                            moves.emplace_back(order(teams[id][0], MP(x1 - 1, y1)));
                            teams[id][0] = MP(x1 - 1, y1);
                            grid[x1][y1] = 0;
                            grid[x1 - 1][y1] = 2;
                        }
                        if (reachable(MP(x2 - 1, y2))) {
                            moves.emplace_back(order(teams[id][1], MP(x2 - 1, y2)));
                            teams[id][1] = MP(x2 - 1, y2);
                            grid[x2][y2] = 0;
                            grid[x2 - 1][y2] = 2;
                        }
                        return true;
                    }
                }
            }

            if ((reachable(MP(x1 - 2, y1 - 1)) && grid[x1 - 2][y1 - 1] == -1)
                || (reachable(MP(x1 + 2, y1 - 1)) && grid[x1 + 2][y1 - 1] == -1)) {
                if (reachable(MP(x1, y1 - 1))) {
                    moves.emplace_back(order(teams[id][0], MP(x1, y1 - 1)));
                    teams[id][0] = MP(x1, y1 - 1);
                    grid[x1][y1] = 0;
                    grid[x1][y1 - 1] = 2;
                    return true;
                }
            }
            if ((reachable(MP(x2 - 2, y2 + 1)) && grid[x2 - 2][y2 + 1] == -1)
                || (reachable(MP(x2 + 2, y2 + 1)) && grid[x2 + 2][y2 + 1] == -1)) {
                if (reachable(MP(x2, y2 + 1))) {
                    moves.emplace_back(order(teams[id][1], MP(x2, y2 + 1)));
                    teams[id][1] = MP(x2, y2 + 1);
                    grid[x2][y2] = 0;
                    grid[x2][y2 + 1] = 2;
                    return true;
                }
            }

            if (rooms[x1][y1][3] < rooms[x2][y2][1]) {
                if (reachable(MP(x1, y1 - 2)) && grid[x1][y1 - 2] == -1) {
                    if (reachable(MP(x1 - 1, y1)) || reachable(MP(x1 + 1, y1))) {
                        if (reachable(MP(x1 - 1, y1))) {
                            moves.emplace_back(order(teams[id][0], MP(x1 - 1, y1)));
                            teams[id][0] = MP(x1 - 1, y1);
                            grid[x1][y1] = 0;
                            grid[x1 - 1][y1] = 2;
                            moves.emplace_back(order(teams[id][1], MP(x1, y1)));
                            teams[id][1] = MP(x1, y1);
                            grid[x2][y2] = 0;
                            grid[x1][y1] = 2;
                            return true;
                        }
                        if (reachable(MP(x1 + 1, y1))) {
                            moves.emplace_back(order(teams[id][0], MP(x1 + 1, y1)));
                            teams[id][0] = MP(x1 + 1, y1);
                            grid[x1][y1] = 0;
                            grid[x1 + 1][y1] = 2;
                            moves.emplace_back(order(teams[id][1], MP(x1, y1)));
                            teams[id][1] = MP(x1, y1);
                            grid[x2][y2] = 0;
                            grid[x1][y1] = 2;
                            return true;
                        }
                    }
                }
                if (reachable(MP(x2, y2 + 2)) && grid[x2][y2 + 2] == -1) {
                    if (reachable(MP(x2 - 1, y2)) || reachable(MP(x2 + 1, y2))) {
                        if (reachable(MP(x2 - 1, y2))) {
                            moves.emplace_back(order(teams[id][1], MP(x2 - 1, y2)));
                            teams[id][1] = MP(x2 - 1, y2);
                            grid[x2][y2] = 0;
                            grid[x2 - 1][y2] = 2;
                            moves.emplace_back(order(teams[id][0], MP(x2, y2)));
                            teams[id][0] = MP(x2, y2);
                            grid[x1][y1] = 0;
                            grid[x2][y2] = 2;
                            return true;
                        }
                        if (reachable(MP(x2 + 1, y2))) {
                            moves.emplace_back(order(teams[id][1], MP(x2 + 1, y2)));
                            teams[id][1] = MP(x2 + 1, y2);
                            grid[x2][y2] = 0;
                            grid[x2 + 1][y2] = 2;
                            moves.emplace_back(order(teams[id][0], MP(x2, y2)));
                            teams[id][0] = MP(x2, y2);
                            grid[x1][y1] = 0;
                            grid[x2][y2] = 2;
                            return true;
                        }
                    }
                }
            }
            else {
                if (reachable(MP(x2, y2 + 2)) && grid[x2][y2 + 2] == -1) {
                    if (reachable(MP(x2 - 1, y2)) || reachable(MP(x2 + 1, y2))) {
                        if (reachable(MP(x2 - 1, y2))) {
                            moves.emplace_back(order(teams[id][1], MP(x2 - 1, y2)));
                            teams[id][1] = MP(x2 - 1, y2);
                            grid[x2][y2] = 0;
                            grid[x2 - 1][y2] = 2;
                            moves.emplace_back(order(teams[id][0], MP(x2, y2)));
                            teams[id][0] = MP(x2, y2);
                            grid[x1][y1] = 0;
                            grid[x2][y2] = 2;
                            return true;
                        }
                        if (reachable(MP(x2 + 1, y2))) {
                            moves.emplace_back(order(teams[id][1], MP(x2 + 1, y2)));
                            teams[id][1] = MP(x2 + 1, y2);
                            grid[x2][y2] = 0;
                            grid[x2 + 1][y2] = 2;
                            moves.emplace_back(order(teams[id][0], MP(x2, y2)));
                            teams[id][0] = MP(x2, y2);
                            grid[x1][y1] = 0;
                            grid[x2][y2] = 2;
                            return true;
                        }
                    }
                }
                if (reachable(MP(x1, y1 - 2)) && grid[x1][y1 - 2] == -1) {
                    if (reachable(MP(x1 - 1, y1)) || reachable(MP(x1 + 1, y1))) {
                        if (reachable(MP(x1 - 1, y1))) {
                            moves.emplace_back(order(teams[id][0], MP(x1 - 1, y1)));
                            teams[id][0] = MP(x1 - 1, y1);
                            grid[x1][y1] = 0;
                            grid[x1 - 1][y1] = 2;
                            moves.emplace_back(order(teams[id][1], MP(x1, y1)));
                            teams[id][1] = MP(x1, y1);
                            grid[x2][y2] = 0;
                            grid[x1][y1] = 2;
                            return true;
                        }
                        if (reachable(MP(x1 + 1, y1))) {
                            moves.emplace_back(order(teams[id][0], MP(x1 + 1, y1)));
                            teams[id][0] = MP(x1 + 1, y1);
                            grid[x1][y1] = 0;
                            grid[x1 + 1][y1] = 2;
                            moves.emplace_back(order(teams[id][1], MP(x1, y1)));
                            teams[id][1] = MP(x1, y1);
                            grid[x2][y2] = 0;
                            grid[x1][y1] = 2;
                            return true;
                        }
                    }
                }
            }

            if (reachable(MP(x1 - 1, y1 - 2)) && grid[x1 - 1][y1 - 2] == -1) {
                if (reachable(MP(x1 - 1, y1))) {
                    moves.emplace_back(order(teams[id][0], MP(x1 - 1, y1)));
                    teams[id][0] = MP(x1 - 1, y1);
                    grid[x1][y1] = 0;
                    grid[x1 - 1][y1] = 2;
                    moves.emplace_back(order(teams[id][1], MP(x1, y1)));
                    teams[id][1] = MP(x1, y1);
                    grid[x2][y2] = 0;
                    grid[x1][y1] = 2;
                    return true;
                }
            }
            if (reachable(MP(x1 + 1, y1 - 2)) && grid[x1 + 1][y1 - 2] == -1) {
                if (reachable(MP(x1 + 1, y1))) {
                    moves.emplace_back(order(teams[id][0], MP(x1 + 1, y1)));
                    teams[id][0] = MP(x1 + 1, y1);
                    grid[x1][y1] = 0;
                    grid[x1 + 1][y1] = 2;
                    moves.emplace_back(order(teams[id][1], MP(x1, y1)));
                    teams[id][1] = MP(x1, y1);
                    grid[x2][y2] = 0;
                    grid[x1][y1] = 2;
                    return true;
                }
            }
            if (reachable(MP(x2 - 1, y2 + 2)) && grid[x2 - 1][y2 + 2] == -1) {
                if (reachable(MP(x2 - 1, y2))) {
                    moves.emplace_back(order(teams[id][1], MP(x2 - 1, y2)));
                    teams[id][1] = MP(x2 - 1, y2);
                    grid[x2][y2] = 0;
                    grid[x2 - 1][y2] = 2;
                    moves.emplace_back(order(teams[id][0], MP(x2, y2)));
                    teams[id][0] = MP(x2, y2);
                    grid[x1][y1] = 0;
                    grid[x2][y2] = 2;
                    return true;
                }
            }
            if (reachable(MP(x2 + 1, y2 + 2)) && grid[x2 + 1][y2 + 2] == -1) {
                if (reachable(MP(x2 + 1, y2))) {
                    moves.emplace_back(order(teams[id][1], MP(x2 + 1, y2)));
                    teams[id][1] = MP(x2 + 1, y2);
                    grid[x2][y2] = 0;
                    grid[x2 + 1][y2] = 2;
                    moves.emplace_back(order(teams[id][0], MP(x2, y2)));
                    teams[id][0] = MP(x2, y2);
                    grid[x1][y1] = 0;
                    grid[x2][y2] = 2;
                    return true;
                }
            }
        }
        else {
            bool ok1 = false, ok2 = false;
            REP(i, 4) {
                int nx = x1 + x_vec[(0 + i) % 4];
                int ny = y1 + y_vec[(0 + i) % 4];
                if (reachable(MP(nx, ny)) && grid[nx][ny] == -1) {
                    ok1 = true;
                    moves.emplace_back(order(teams[id][0], MP(nx, ny)));
                    teams[id][0] = MP(nx, ny);
                    grid[x1][y1] = 0;
                    grid[nx][ny] = 2;
                    break;
                }
            }
            REP(i, 4) {
                int nx = x2 + x_vec[(2 + i) % 4];
                int ny = y2 + y_vec[(2 + i) % 4];
                if (reachable(MP(nx, ny)) && grid[nx][ny] == -1) {
                    ok2 = true;
                    moves.emplace_back(order(teams[id][1], MP(nx, ny)));
                    teams[id][1] = MP(nx, ny);
                    grid[x2][y2] = 0;
                    grid[nx][ny] = 2;
                    break;
                }
            }
            if (ok1 || ok2)return true;
            if ((reachable(MP(x1 - 1, y1 - 1)) && grid[x1 - 1][y1 - 1] == -1)
                || (reachable(MP(x1 - 1, y1 + 1)) && grid[x1 - 1][y1 + 1] == -1)) {
                if (reachable(MP(x1 - 1, y1))) {
                    moves.emplace_back(order(teams[id][0], MP(x1 - 1, y1)));
                    teams[id][0] = MP(x1 - 1, y1);
                    grid[x1][y1] = 0;
                    grid[x1 - 1][y1] = 2;
                    return true;
                }
            }
            if ((reachable(MP(x2 + 1, y2 - 1)) && grid[x2 + 1][y2 - 1] == -1)
                || (reachable(MP(x2 + 1, y2 + 1)) && grid[x2 + 1][y2 + 1] == -1)) {
                if (reachable(MP(x2 + 1, y2))) {
                    moves.emplace_back(order(teams[id][1], MP(x2 + 1, y2)));
                    teams[id][1] = MP(x2 + 1, y2);
                    grid[x2][y2] = 0;
                    grid[x2 + 1][y2] = 2;
                    return true;
                }
            }

            if (min(rooms[x1][y1][3], rooms[x2][y2][3]) < min(rooms[x1][y1][1], rooms[x2][y2][1])) {
                if ((reachable(MP(x1, y1 - 2)) && grid[x1][y1 - 2] == -1)
                    || (reachable(MP(x2, y2 - 2)) && grid[x2][y2 - 2] == -1)) {
                    if (reachable(MP(x1, y1 - 1)) || reachable(MP(x2, y2 - 1))) {
                        if (reachable(MP(x1, y1 - 1))) {
                            moves.emplace_back(order(teams[id][0], MP(x1, y1 - 1)));
                            teams[id][0] = MP(x1, y1 - 1);
                            grid[x1][y1] = 0;
                            grid[x1][y1 - 1] = 2;
                        }
                        if (reachable(MP(x2, y2 - 1))) {
                            moves.emplace_back(order(teams[id][1], MP(x2, y2 - 1)));
                            teams[id][1] = MP(x2, y2 - 1);
                            grid[x2][y2] = 0;
                            grid[x2][y2 - 1] = 2;
                        }
                        return true;
                    }
                }
                if ((reachable(MP(x1, y1 + 2)) && grid[x1][y1 + 2] == -1)
                    || (reachable(MP(x2, y2 + 2)) && grid[x2][y2 + 2] == -1)) {
                    if (reachable(MP(x1, y1 + 1)) || reachable(MP(x2, y2 + 1))) {
                        if (reachable(MP(x1, y1 + 1))) {
                            moves.emplace_back(order(teams[id][0], MP(x1, y1 + 1)));
                            teams[id][0] = MP(x1, y1 + 1);
                            grid[x1][y1] = 0;
                            grid[x1][y1 + 1] = 2;
                        }
                        if (reachable(MP(x2, y2 + 1))) {
                            moves.emplace_back(order(teams[id][1], MP(x2, y2 + 1)));
                            teams[id][1] = MP(x2, y2 + 1);
                            grid[x2][y2] = 0;
                            grid[x2][y2 + 1] = 2;
                        }
                        return true;
                    }
                }
            }
            else {
                if ((reachable(MP(x1, y1 + 2)) && grid[x1][y1 + 2] == -1)
                    || (reachable(MP(x2, y2 + 2)) && grid[x2][y2 + 2] == -1)) {
                    if (reachable(MP(x1, y1 + 1)) || reachable(MP(x2, y2 + 1))) {
                        if (reachable(MP(x1, y1 + 1))) {
                            moves.emplace_back(order(teams[id][0], MP(x1, y1 + 1)));
                            teams[id][0] = MP(x1, y1 + 1);
                            grid[x1][y1] = 0;
                            grid[x1][y1 + 1] = 2;
                        }
                        if (reachable(MP(x2, y2 + 1))) {
                            moves.emplace_back(order(teams[id][1], MP(x2, y2 + 1)));
                            teams[id][1] = MP(x2, y2 + 1);
                            grid[x2][y2] = 0;
                            grid[x2][y2 + 1] = 2;
                        }
                        return true;
                    }
                }
                if ((reachable(MP(x1, y1 - 2)) && grid[x1][y1 - 2] == -1)
                    || (reachable(MP(x2, y2 - 2)) && grid[x2][y2 - 2] == -1)) {
                    if (reachable(MP(x1, y1 - 1)) || reachable(MP(x2, y2 - 1))) {
                        if (reachable(MP(x1, y1 - 1))) {
                            moves.emplace_back(order(teams[id][0], MP(x1, y1 - 1)));
                            teams[id][0] = MP(x1, y1 - 1);
                            grid[x1][y1] = 0;
                            grid[x1][y1 - 1] = 2;
                        }
                        if (reachable(MP(x2, y2 - 1))) {
                            moves.emplace_back(order(teams[id][1], MP(x2, y2 - 1)));
                            teams[id][1] = MP(x2, y2 - 1);
                            grid[x2][y2] = 0;
                            grid[x2][y2 - 1] = 2;
                        }
                        return true;
                    }
                }
            }

            if ((reachable(MP(x1 - 1, y1 - 2)) && grid[x1 - 1][y1 - 2] == -1)
                || (reachable(MP(x1 - 1, y1 + 2)) && grid[x1 - 1][y1 + 2] == -1)) {
                if (reachable(MP(x1 - 1, y1))) {
                    moves.emplace_back(order(teams[id][0], MP(x1 - 1, y1)));
                    teams[id][0] = MP(x1 - 1, y1);
                    grid[x1][y1] = 0;
                    grid[x1 - 1][y1] = 2;
                    return true;
                }
            }
            if ((reachable(MP(x2 + 1, y2 - 2)) && grid[x2 + 1][y2 - 2] == -1)
                || (reachable(MP(x2 + 1, y2 + 2)) && grid[x2 + 1][y2 + 2] == -1)) {
                if (reachable(MP(x2 + 1, y2))) {
                    moves.emplace_back(order(teams[id][1], MP(x2 + 1, y2)));
                    teams[id][1] = MP(x2 + 1, y2);
                    grid[x2][y2] = 0;
                    grid[x2 + 1][y2] = 2;
                    return true;
                }
            }

            if (rooms[x1][y1][0] < rooms[x2][y2][2]) {
                if (reachable(MP(x1 - 2, y1)) && grid[x1 - 2][y1] == -1) {
                    if (reachable(MP(x1, y1 - 1)) || reachable(MP(x1, y1 + 1))) {
                        if (reachable(MP(x1, y1 - 1))) {
                            moves.emplace_back(order(teams[id][0], MP(x1, y1 - 1)));
                            teams[id][0] = MP(x1, y1 - 1);
                            grid[x1][y1] = 0;
                            grid[x1][y1 - 1] = 2;
                            moves.emplace_back(order(teams[id][1], MP(x1, y1)));
                            teams[id][1] = MP(x1, y1);
                            grid[x2][y2] = 0;
                            grid[x1][y1] = 2;
                            return true;
                        }
                        if (reachable(MP(x1, y1 + 1))) {
                            moves.emplace_back(order(teams[id][0], MP(x1, y1 + 1)));
                            teams[id][0] = MP(x1, y1 + 1);
                            grid[x1][y1] = 0;
                            grid[x1][y1 + 1] = 2;
                            moves.emplace_back(order(teams[id][1], MP(x1, y1)));
                            teams[id][1] = MP(x1, y1);
                            grid[x2][y2] = 0;
                            grid[x1][y1] = 2;
                            return true;
                        }
                    }
                }
                if (reachable(MP(x2 + 2, y2)) && grid[x2 + 2][y2] == -1) {
                    if (reachable(MP(x2, y2 - 1)) || reachable(MP(x2, y2 + 1))) {
                        if (reachable(MP(x2, y2 - 1))) {
                            moves.emplace_back(order(teams[id][1], MP(x2, y2 - 1)));
                            teams[id][1] = MP(x2, y2 - 1);
                            grid[x2][y2] = 0;
                            grid[x2][y2 - 1] = 2;
                            moves.emplace_back(order(teams[id][0], MP(x2, y2)));
                            teams[id][0] = MP(x2, y2);
                            grid[x1][y1] = 0;
                            grid[x2][y2] = 2;
                            return true;
                        }
                        if (reachable(MP(x2, y2 + 1))) {
                            moves.emplace_back(order(teams[id][1], MP(x2, y2 + 1)));
                            teams[id][1] = MP(x2, y2 + 1);
                            grid[x2][y2] = 0;
                            grid[x2][y2 + 1] = 2;
                            moves.emplace_back(order(teams[id][0], MP(x2, y2)));
                            teams[id][0] = MP(x2, y2);
                            grid[x1][y1] = 0;
                            grid[x2][y2] = 2;
                            return true;
                        }
                    }

                }
            }
            else {
                if (reachable(MP(x2 + 2, y2)) && grid[x2 + 2][y2] == -1) {
                    if (reachable(MP(x2, y2 - 1)) || reachable(MP(x2, y2 + 1))) {
                        if (reachable(MP(x2, y2 - 1))) {
                            moves.emplace_back(order(teams[id][1], MP(x2, y2 - 1)));
                            teams[id][1] = MP(x2, y2 - 1);
                            grid[x2][y2] = 0;
                            grid[x2][y2 - 1] = 2;
                            moves.emplace_back(order(teams[id][0], MP(x2, y2)));
                            teams[id][0] = MP(x2, y2);
                            grid[x1][y1] = 0;
                            grid[x2][y2] = 2;
                            return true;
                        }
                        if (reachable(MP(x2, y2 + 1))) {
                            moves.emplace_back(order(teams[id][1], MP(x2, y2 + 1)));
                            teams[id][1] = MP(x2, y2 + 1);
                            grid[x2][y2] = 0;
                            grid[x2][y2 + 1] = 2;
                            moves.emplace_back(order(teams[id][0], MP(x2, y2)));
                            teams[id][0] = MP(x2, y2);
                            grid[x1][y1] = 0;
                            grid[x2][y2] = 2;
                            return true;
                        }
                    }

                }
                if (reachable(MP(x1 - 2, y1)) && grid[x1 - 2][y1] == -1) {
                    if (reachable(MP(x1, y1 - 1)) || reachable(MP(x1, y1 + 1))) {
                        if (reachable(MP(x1, y1 - 1))) {
                            moves.emplace_back(order(teams[id][0], MP(x1, y1 - 1)));
                            teams[id][0] = MP(x1, y1 - 1);
                            grid[x1][y1] = 0;
                            grid[x1][y1 - 1] = 2;
                            moves.emplace_back(order(teams[id][1], MP(x1, y1)));
                            teams[id][1] = MP(x1, y1);
                            grid[x2][y2] = 0;
                            grid[x1][y1] = 2;
                            return true;
                        }
                        if (reachable(MP(x1, y1 + 1))) {
                            moves.emplace_back(order(teams[id][0], MP(x1, y1 + 1)));
                            teams[id][0] = MP(x1, y1 + 1);
                            grid[x1][y1] = 0;
                            grid[x1][y1 + 1] = 2;
                            moves.emplace_back(order(teams[id][1], MP(x1, y1)));
                            teams[id][1] = MP(x1, y1);
                            grid[x2][y2] = 0;
                            grid[x1][y1] = 2;
                            return true;
                        }
                    }
                }
            }

            if (reachable(MP(x1 - 2, y1 - 1)) && grid[x1 - 2][y1 - 1] == -1) {
                if (reachable(MP(x1, y1 - 1))) {
                    moves.emplace_back(order(teams[id][0], MP(x1, y1 - 1)));
                    teams[id][0] = MP(x1, y1 - 1);
                    grid[x1][y1] = 0;
                    grid[x1][y1 - 1] = 2;
                    moves.emplace_back(order(teams[id][1], MP(x1, y1)));
                    teams[id][1] = MP(x1, y1);
                    grid[x2][y2] = 0;
                    grid[x1][y1] = 2;
                    return true;
                }
            }
            if (reachable(MP(x1 - 2, y1 + 1)) && grid[x1 - 2][y1 + 1] == -1) {
                if (reachable(MP(x1, y1 + 1))) {
                    moves.emplace_back(order(teams[id][0], MP(x1, y1 + 1)));
                    teams[id][0] = MP(x1, y1 + 1);
                    grid[x1][y1] = 0;
                    grid[x1][y1 + 1] = 2;
                    moves.emplace_back(order(teams[id][1], MP(x1, y1)));
                    teams[id][1] = MP(x1, y1);
                    grid[x2][y2] = 0;
                    grid[x1][y1] = 2;
                    return true;
                }
            }
            if (reachable(MP(x2 + 2, y2 - 1)) && grid[x2 + 2][y2 - 1] == -1) {
                if (reachable(MP(x2, y2 - 1))) {
                    moves.emplace_back(order(teams[id][1], MP(x2, y2 - 1)));
                    teams[id][1] = MP(x2, y2 - 1);
                    grid[x2][y2] = 0;
                    grid[x2][y2 - 1] = 2;
                    moves.emplace_back(order(teams[id][0], MP(x2, y2)));
                    teams[id][0] = MP(x2, y2);
                    grid[x1][y1] = 0;
                    grid[x2][y2] = 2;
                    return true;
                }
            }
            if (reachable(MP(x2 + 2, y2 + 1)) && grid[x2 + 2][y2 + 1] == -1) {
                if (reachable(MP(x2, y2 + 1))) {
                    moves.emplace_back(order(teams[id][1], MP(x2, y2 + 1)));
                    teams[id][1] = MP(x2, y2 + 1);
                    grid[x2][y2] = 0;
                    grid[x2][y2 + 1] = 2;
                    moves.emplace_back(order(teams[id][0], MP(x2, y2)));
                    teams[id][0] = MP(x2, y2);
                    grid[x1][y1] = 0;
                    grid[x2][y2] = 2;
                    return true;
                }
            }
        }
    }
    else if (abs(x1 - x2) + abs(y1 - y2) == 2 && abs(x1 - x2) == 1) {
        if (y1 < y2) {
            int ok1 = -1, ok2 = -1;
            REP(i, 4) {
                int nx = x1 + x_vec[(3 + i) % 4];
                int ny = y1 + y_vec[(3 + i) % 4];
                if (reachable(MP(nx, ny)) && grid[nx][ny] == -1) {
                    ok1 = (3 + i) % 4;
                    moves.emplace_back(order(teams[id][0], MP(nx, ny)));
                    teams[id][0] = MP(nx, ny);
                    grid[x1][y1] = 0;
                    grid[nx][ny] = 2;
                    break;
                }
            }
            REP(i, 4) {
                int nx = x2 + x_vec[(1 + i) % 4];
                int ny = y2 + y_vec[(1 + i) % 4];
                if (reachable(MP(nx, ny)) && grid[nx][ny] == -1) {
                    ok2 = (1 + i) % 4;
                    moves.emplace_back(order(teams[id][1], MP(nx, ny)));
                    teams[id][1] = MP(nx, ny);
                    grid[x2][y2] = 0;
                    grid[nx][ny] = 2;
                    break;
                }
            }
            if (ok1!=-1 && ok2!=-1)return true;
            if (ok1!=-1 && ok2==-1) {
                if (ok1 == 3) {
                    if (reachable(MP(x2 - 1, y2))) {
                        moves.emplace_back(order(teams[id][1], MP(x2-1, y2)));
                        teams[id][1] = MP(x2-1, y2);
                        grid[x2][y2] = 0;
                        grid[x2-1][y2] = 2;
                        return true;
                    }
                    if (reachable(MP(x2, y2-1))) {
                        moves.emplace_back(order(teams[id][1], MP(x2, y2-1)));
                        teams[id][1] = MP(x2, y2-1);
                        grid[x2][y2] = 0;
                        grid[x2][y2-1] = 2;
                        return true;
                    }
                }
                else if (ok1 == 0) {
                    if (reachable(MP(x2, y2 - 1))) {
                        moves.emplace_back(order(teams[id][1], MP(x2, y2 - 1)));
                        teams[id][1] = MP(x2, y2 - 1);
                        grid[x2][y2] = 0;
                        grid[x2][y2 - 1] = 2;
                        return true;
                    }
                    if (reachable(MP(x2 - 1, y2))) {
                        moves.emplace_back(order(teams[id][1], MP(x2 - 1, y2)));
                        teams[id][1] = MP(x2 - 1, y2);
                        grid[x2][y2] = 0;
                        grid[x2 - 1][y2] = 2;
                        return true;
                    }
                }
                else if (ok1 == 1) {
                    if (reachable(MP(x2+1, y2))) {
                        moves.emplace_back(order(teams[id][1], MP(x2+1, y2)));
                        teams[id][1] = MP(x2+1, y2);
                        grid[x2][y2] = 0;
                        grid[x2+1][y2] = 2;
                        return true;
                    }
                }
                else if (ok1 == 2) {
                    if (reachable(MP(x2, y2+1))) {
                        moves.emplace_back(order(teams[id][1], MP(x2, y2+1)));
                        teams[id][1] = MP(x2, y2+1);
                        grid[x2][y2] = 0;
                        grid[x2][y2+1] = 2;
                        return true;
                    }
                }
                return true;
            }
            else if (ok1==-1 && ok2!=-1) {
                if (ok2 == 1) {
                    if (reachable(MP(x1 + 1, y1))) {
                        moves.emplace_back(order(teams[id][0], MP(x1 + 1, y1)));
                        teams[id][0] = MP(x1 + 1, y1);
                        grid[x1][y1] = 0;
                        grid[x1 + 1][y1] = 2;
                        return true;
                    }
                    if (reachable(MP(x1, y1+1))) {
                        moves.emplace_back(order(teams[id][0], MP(x1, y1+1)));
                        teams[id][0] = MP(x1, y1+1);
                        grid[x1][y1] = 0;
                        grid[x1][y1+1] = 2;
                        return true;
                    }
                }
                else if (ok2 == 2) {
                    if (reachable(MP(x1, y1 + 1))) {
                        moves.emplace_back(order(teams[id][0], MP(x1, y1 + 1)));
                        teams[id][0] = MP(x1, y1 + 1);
                        grid[x1][y1] = 0;
                        grid[x1][y1 + 1] = 2;
                        return true;
                    }
                    if (reachable(MP(x1 + 1, y1))) {
                        moves.emplace_back(order(teams[id][0], MP(x1 + 1, y1)));
                        teams[id][0] = MP(x1 + 1, y1);
                        grid[x1][y1] = 0;
                        grid[x1 + 1][y1] = 2;
                        return true;
                    }
                }
                else if (ok2 == 3) {
                    if (reachable(MP(x1 - 1, y1))) {
                        moves.emplace_back(order(teams[id][0], MP(x1 - 1, y1)));
                        teams[id][0] = MP(x1 - 1, y1);
                        grid[x1][y1] = 0;
                        grid[x1 - 1][y1] = 2;
                        return true;
                    }
                }
                else if (ok2 == 0) {
                    if (reachable(MP(x1, y1 - 1))) {
                        moves.emplace_back(order(teams[id][0], MP(x1, y1 - 1)));
                        teams[id][0] = MP(x1, y1 - 1);
                        grid[x1][y1] = 0;
                        grid[x1][y1 - 1] = 2;
                        return true;
                    }
                }
                return true;
            }

            if ((reachable(MP(x1 - 1, y1 + 1)) && grid[x1 - 1][y1 + 1] == -1)
                || (reachable(MP(x1 + 1, y1 - 1)) && grid[x1 + 1][y1 - 1] == -1)
                || (reachable(MP(x1 - 1, y1 - 1)) && grid[x1 - 1][y1 - 1] == -1)) {
                if (min(rooms[x1][y1][1], rooms[x1][y1][3])
                    < min(rooms[x1][y1][0], rooms[x1][y1][2])) {
                    if (reachable(MP(x1 - 1, y1)) && reachable(MP(x2, y2 - 1))) {
                        moves.emplace_back(order(teams[id][0], MP(x1 - 1, y1)));
                        teams[id][0] = MP(x1 - 1, y1);
                        grid[x1][y1] = 0;
                        grid[x1 - 1][y1] = 2;
                        moves.emplace_back(order(teams[id][1], MP(x2, y2 - 1)));
                        teams[id][1] = MP(x2, y2 - 1);
                        grid[x2][y2] = 0;
                        grid[x2][y2 - 1] = 2;
                        return true;
                    }
                    if (reachable(MP(x1, y1-1)) && reachable(MP(x2-1, y2))) {
                        moves.emplace_back(order(teams[id][0], MP(x1, y1-1)));
                        teams[id][0] = MP(x1, y1-1);
                        grid[x1][y1] = 0;
                        grid[x1][y1-1] = 2;
                        moves.emplace_back(order(teams[id][1], MP(x2-1, y2)));
                        teams[id][1] = MP(x2-1, y2);
                        grid[x2][y2] = 0;
                        grid[x2-1][y2] = 2;
                        return true;
                    }
                }
                else {
                    if (reachable(MP(x1, y1 - 1)) && reachable(MP(x2 - 1, y2))) {
                        moves.emplace_back(order(teams[id][0], MP(x1, y1 - 1)));
                        teams[id][0] = MP(x1, y1 - 1);
                        grid[x1][y1] = 0;
                        grid[x1][y1 - 1] = 2;
                        moves.emplace_back(order(teams[id][1], MP(x2 - 1, y2)));
                        teams[id][1] = MP(x2 - 1, y2);
                        grid[x2][y2] = 0;
                        grid[x2 - 1][y2] = 2;
                        return true;
                    }
                    if (reachable(MP(x1 - 1, y1)) && reachable(MP(x2, y2 - 1))) {
                        moves.emplace_back(order(teams[id][0], MP(x1 - 1, y1)));
                        teams[id][0] = MP(x1 - 1, y1);
                        grid[x1][y1] = 0;
                        grid[x1 - 1][y1] = 2;
                        moves.emplace_back(order(teams[id][1], MP(x2, y2 - 1)));
                        teams[id][1] = MP(x2, y2 - 1);
                        grid[x2][y2] = 0;
                        grid[x2][y2 - 1] = 2;
                        return true;
                    }
                }
            }
            if ((reachable(MP(x2 - 1, y2 + 1)) && grid[x2 - 1][y2 + 1] == -1)
                || (reachable(MP(x2 + 1, y2 - 1)) && grid[x2 + 1][y2 - 1] == -1)
                || (reachable(MP(x2 + 1, y2 + 1)) && grid[x2 + 1][y2 + 1] == -1)) {
                if (min(rooms[x2][y2][1], rooms[x2][y2][3])
                    < min(rooms[x2][y2][0], rooms[x1][y2][2])) {
                    if (reachable(MP(x1, y1+1)) && reachable(MP(x2+1, y2))) {
                        moves.emplace_back(order(teams[id][0], MP(x1, y1+1)));
                        teams[id][0] = MP(x1, y1+1);
                        grid[x1][y1] = 0;
                        grid[x1][y1+1] = 2;
                        moves.emplace_back(order(teams[id][1], MP(x2+1, y2)));
                        teams[id][1] = MP(x2+1, y2);
                        grid[x2][y2] = 0;
                        grid[x2+1][y2] = 2;
                        return true;
                    }
                    if (reachable(MP(x1 + 1, y1)) && reachable(MP(x2, y2 + 1))) {
                        moves.emplace_back(order(teams[id][0], MP(x1 + 1, y1)));
                        teams[id][0] = MP(x1 + 1, y1);
                        grid[x1][y1] = 0;
                        grid[x1 + 1][y1] = 2;
                        moves.emplace_back(order(teams[id][1], MP(x2, y2 + 1)));
                        teams[id][1] = MP(x2, y2 + 1);
                        grid[x2][y2] = 0;
                        grid[x2][y2 + 1] = 2;
                        return true;
                    }
                }
                else {
                    if (reachable(MP(x1 + 1, y1)) && reachable(MP(x2, y2 + 1))) {
                        moves.emplace_back(order(teams[id][0], MP(x1 + 1, y1)));
                        teams[id][0] = MP(x1 + 1, y1);
                        grid[x1][y1] = 0;
                        grid[x1 + 1][y1] = 2;
                        moves.emplace_back(order(teams[id][1], MP(x2, y2 + 1)));
                        teams[id][1] = MP(x2, y2 + 1);
                        grid[x2][y2] = 0;
                        grid[x2][y2 + 1] = 2;
                        return true;
                    }
                    if (reachable(MP(x1, y1 + 1)) && reachable(MP(x2 + 1, y2))) {
                        moves.emplace_back(order(teams[id][0], MP(x1, y1 + 1)));
                        teams[id][0] = MP(x1, y1 + 1);
                        grid[x1][y1] = 0;
                        grid[x1][y1 + 1] = 2;
                        moves.emplace_back(order(teams[id][1], MP(x2 + 1, y2)));
                        teams[id][1] = MP(x2 + 1, y2);
                        grid[x2][y2] = 0;
                        grid[x2 + 1][y2] = 2;
                        return true;
                    }
                }
            }

            /*
            if (min(rooms[x1][y1][1], rooms[x1][y1][3]) < min(rooms[x2][y2][0], rooms[x2][y2][2])) {
                if ((reachable(MP(x1 - 1, y1 + 1)) && grid[x1 - 1][y1 + 1] == -1)
                    || (reachable(MP(x1 + 1, y1 - 1)) && grid[x1 + 1][y1 - 1] == -1)
                    || (reachable(MP(x1 - 1, y1 - 1)) && grid[x1 - 1][y1 - 1] == -1)) {
                    if (reachable(MP(x1 - 1, y1)) && reachable(MP(x2, y2 - 1))) {
                        moves.emplace_back(order(teams[id][0], MP(x1-1, y1)));
                        teams[id][0] = MP(x1-1, y1);
                        grid[x1][y1] = 0;
                        grid[x1-1][y1] = 2;
                        moves.emplace_back(order(teams[id][1], MP(x2, y2-1)));
                        teams[id][1] = MP(x2, y2-1);
                        grid[x2][y2] = 0;
                        grid[x2][y2-1] = 2;
                        return true;
                    }
                }
                if ((reachable(MP(x2 - 1, y2 + 1)) && grid[x2 - 1][y2 + 1] == -1)
                    || (reachable(MP(x2 + 1, y2 - 1)) && grid[x2 + 1][y2 - 1] == -1)
                    || (reachable(MP(x2 + 1, y2 + 1)) && grid[x2 + 1][y2 + 1] == -1)) {
                    if (reachable(MP(x1 + 1, y1)) && reachable(MP(x2, y2 + 1))) {
                        moves.emplace_back(order(teams[id][0], MP(x1 + 1, y1)));
                        teams[id][0] = MP(x1 + 1, y1);
                        grid[x1][y1] = 0;
                        grid[x1 + 1][y1] = 2;
                        moves.emplace_back(order(teams[id][1], MP(x2, y2 + 1)));
                        teams[id][1] = MP(x2, y2 + 1);
                        grid[x2][y2] = 0;
                        grid[x2][y2 + 1] = 2;
                        return true;
                    }
                }
            }
            else {
                if ((reachable(MP(x2 - 1, y2 + 1)) && grid[x2 - 1][y2 + 1] == -1)
                    || (reachable(MP(x2 + 1, y2 - 1)) && grid[x2 + 1][y2 - 1] == -1)
                    || (reachable(MP(x2 + 1, y2 + 1)) && grid[x2 + 1][y2 + 1] == -1)) {
                    if (reachable(MP(x1 + 1, y1)) && reachable(MP(x2, y2 + 1))) {
                        moves.emplace_back(order(teams[id][0], MP(x1 + 1, y1)));
                        teams[id][0] = MP(x1 + 1, y1);
                        grid[x1][y1] = 0;
                        grid[x1 + 1][y1] = 2;
                        moves.emplace_back(order(teams[id][1], MP(x2, y2 + 1)));
                        teams[id][1] = MP(x2, y2 + 1);
                        grid[x2][y2] = 0;
                        grid[x2][y2 + 1] = 2;
                        return true;
                    }
                }
                if ((reachable(MP(x1 - 1, y1 + 1)) && grid[x1 - 1][y1 + 1] == -1)
                    || (reachable(MP(x1 + 1, y1 - 1)) && grid[x1 + 1][y1 - 1] == -1)
                    || (reachable(MP(x1 - 1, y1 - 1)) && grid[x1 - 1][y1 - 1] == -1)) {
                    if (reachable(MP(x1 - 1, y1)) && reachable(MP(x2, y2 - 1))) {
                        moves.emplace_back(order(teams[id][0], MP(x1 - 1, y1)));
                        teams[id][0] = MP(x1 - 1, y1);
                        grid[x1][y1] = 0;
                        grid[x1 - 1][y1] = 2;
                        moves.emplace_back(order(teams[id][1], MP(x2, y2 - 1)));
                        teams[id][1] = MP(x2, y2 - 1);
                        grid[x2][y2] = 0;
                        grid[x2][y2 - 1] = 2;
                        return true;
                    }
                }
            }
            */

            if (reachable(MP(x1 - 2, y1)) && grid[x1 - 2][y1] == -1) {
                if (reachable(MP(x1 - 1, y1)) && reachable(MP(x2, y2 - 1))) {
                    moves.emplace_back(order(teams[id][0], MP(x1 - 1, y1)));
                    teams[id][0] = MP(x1 - 1, y1);
                    grid[x1][y1] = 0;
                    grid[x1 - 1][y1] = 2;
                    moves.emplace_back(order(teams[id][1], MP(x2, y2 - 1)));
                    teams[id][1] = MP(x2, y2 - 1);
                    grid[x2][y2] = 0;
                    grid[x2][y2 - 1] = 2;
                    return true;
                }
            }
            if (reachable(MP(x1, y1-2)) && grid[x1][y1-2] == -1) {
                if (reachable(MP(x1, y1-1)) && reachable(MP(x2-1, y2))) {
                    moves.emplace_back(order(teams[id][0], MP(x1, y1 - 1)));
                    teams[id][0] = MP(x1, y1 - 1);
                    grid[x1][y1] = 0;
                    grid[x1][y1 - 1] = 2;
                    moves.emplace_back(order(teams[id][1], MP(x2 - 1, y2)));
                    teams[id][1] = MP(x2 - 1, y2);
                    grid[x2][y2] = 0;
                    grid[x2 - 1][y2] = 2;
                    return true;
                }
            }
            if (reachable(MP(x2 + 2, y2)) && grid[x2 + 2][y2] == -1) {
                if (reachable(MP(x1, y1+1)) && reachable(MP(x2+1, y2))) {
                    moves.emplace_back(order(teams[id][0], MP(x1, y1 + 1)));
                    teams[id][0] = MP(x1, y1 + 1);
                    grid[x1][y1] = 0;
                    grid[x1][y1 + 1] = 2;
                    moves.emplace_back(order(teams[id][1], MP(x2 + 1, y2)));
                    teams[id][1] = MP(x2 + 1, y2);
                    grid[x2][y2] = 0;
                    grid[x2 + 1][y2] = 2;
                    return true;
                }
            }
            if (reachable(MP(x2, y2+2)) && grid[x2][y2+2] == -1) {
                if (reachable(MP(x1 + 1, y1)) && reachable(MP(x2, y2 + 1))) {
                    moves.emplace_back(order(teams[id][0], MP(x1 + 1, y1)));
                    teams[id][0] = MP(x1 + 1, y1);
                    grid[x1][y1] = 0;
                    grid[x1 + 1][y1] = 2;
                    moves.emplace_back(order(teams[id][1], MP(x2, y2 + 1)));
                    teams[id][1] = MP(x2, y2 + 1);
                    grid[x2][y2] = 0;
                    grid[x2][y2 + 1] = 2;
                    return true;
                }
            }

            if ((reachable(MP(x1 - 1, y1 + 2)) && grid[x1 - 1][y1 + 2] == -1)
                || (reachable(MP(x1 +2, y1 -1)) && grid[x1 +2][y1 -1] == -1)) {
                return true;
            }

        }
        else {
            int ok1 = -1, ok2 = -1;
            REP(i, 4) {
                int nx = x1 + x_vec[(0 + i) % 4];
                int ny = y1 + y_vec[(0 + i) % 4];
                if (reachable(MP(nx, ny)) && grid[nx][ny] == -1) {
                    ok1 = (0 + i) % 4;
                    moves.emplace_back(order(teams[id][0], MP(nx, ny)));
                    teams[id][0] = MP(nx, ny);
                    grid[x1][y1] = 0;
                    grid[nx][ny] = 2;
                    break;
                }
            }
            REP(i, 4) {
                int nx = x2 + x_vec[(2 + i) % 4];
                int ny = y2 + y_vec[(2 + i) % 4];
                if (reachable(MP(nx, ny)) && grid[nx][ny] == -1) {
                    ok2 = (2 + i) % 4;
                    moves.emplace_back(order(teams[id][1], MP(nx, ny)));
                    teams[id][1] = MP(nx, ny);
                    grid[x2][y2] = 0;
                    grid[nx][ny] = 2;
                    break;
                }
            }
            if (ok1 != -1 && ok2 != -1)return true;
            if (ok1 != -1 && ok2 == -1) {
                if (ok1 == 0) {
                    if (reachable(MP(x2, y2+1))) {
                        moves.emplace_back(order(teams[id][1], MP(x2, y2+1)));
                        teams[id][1] = MP(x2, y2+1);
                        grid[x2][y2] = 0;
                        grid[x2][y2+1] = 2;
                        return true;
                    }
                    if (reachable(MP(x2-1, y2))) {
                        moves.emplace_back(order(teams[id][1], MP(x2-1, y2)));
                        teams[id][1] = MP(x2-1, y2);
                        grid[x2][y2] = 0;
                        grid[x2-1][y2] = 2;
                        return true;
                    }
                }
                else if (ok1 == 1) {
                    if (reachable(MP(x2 - 1, y2))) {
                        moves.emplace_back(order(teams[id][1], MP(x2 - 1, y2)));
                        teams[id][1] = MP(x2 - 1, y2);
                        grid[x2][y2] = 0;
                        grid[x2 - 1][y2] = 2;
                        return true;
                    }
                    if (reachable(MP(x2, y2 + 1))) {
                        moves.emplace_back(order(teams[id][1], MP(x2, y2 + 1)));
                        teams[id][1] = MP(x2, y2 + 1);
                        grid[x2][y2] = 0;
                        grid[x2][y2 + 1] = 2;
                        return true;
                    }
                }
                else if (ok1 == 2) {
                    if (reachable(MP(x2, y2-1))) {
                        moves.emplace_back(order(teams[id][1], MP(x2, y2-1)));
                        teams[id][1] = MP(x2, y2-1);
                        grid[x2][y2] = 0;
                        grid[x2][y2-1] = 2;
                        return true;
                    }
                }
                else if (ok1 == 3) {
                    if (reachable(MP(x2 + 1, y2))) {
                        moves.emplace_back(order(teams[id][1], MP(x2 + 1, y2)));
                        teams[id][1] = MP(x2 + 1, y2);
                        grid[x2][y2] = 0;
                        grid[x2 + 1][y2] = 2;
                        return true;
                    }
                }
                return true;
            }
            else if (ok1 == -1 && ok2 != -1) {
                if (ok2 == 2) {
                    if (reachable(MP(x1, y1-1))) {
                        moves.emplace_back(order(teams[id][0], MP(x1, y1-1)));
                        teams[id][0] = MP(x1, y1-1);
                        grid[x1][y1] = 0;
                        grid[x1][y1-1] = 2;
                        return true;
                    }
                    if (reachable(MP(x1+1, y1))) {
                        moves.emplace_back(order(teams[id][0], MP(x1+1, y1)));
                        teams[id][0] = MP(x1+1, y1);
                        grid[x1][y1] = 0;
                        grid[x1+1][y1] = 2;
                        return true;
                    }
                }
                else if (ok2 == 3) {
                    if (reachable(MP(x1 + 1, y1))) {
                        moves.emplace_back(order(teams[id][0], MP(x1 + 1, y1)));
                        teams[id][0] = MP(x1 + 1, y1);
                        grid[x1][y1] = 0;
                        grid[x1 + 1][y1] = 2;
                        return true;
                    }
                    if (reachable(MP(x1, y1 - 1))) {
                        moves.emplace_back(order(teams[id][0], MP(x1, y1 - 1)));
                        teams[id][0] = MP(x1, y1 - 1);
                        grid[x1][y1] = 0;
                        grid[x1][y1 - 1] = 2;
                        return true;
                    }
                }
                else if (ok2 == 0) {
                    if (reachable(MP(x1, y1+1))) {
                        moves.emplace_back(order(teams[id][0], MP(x1, y1+1)));
                        teams[id][0] = MP(x1, y1+1);
                        grid[x1][y1] = 0;
                        grid[x1][y1+1] = 2;
                        return true;
                    }
                }
                else if (ok2 == 1) {
                    if (reachable(MP(x1-1, y1))) {
                        moves.emplace_back(order(teams[id][0], MP(x1-1, y1)));
                        teams[id][0] = MP(x1-1, y1);
                        grid[x1][y1] = 0;
                        grid[x1-1][y1] = 2;
                        return true;
                    }
                }
                return true;
            }

            if ((reachable(MP(x1 - 1, y1 - 1)) && grid[x1 - 1][y1 - 1] == -1)
                || (reachable(MP(x1 + 1, y1 + 1)) && grid[x1 + 1][y1 + 1] == -1)
                || (reachable(MP(x1 - 1, y1 + 1)) && grid[x1 - 1][y1 + 1] == -1)) {
                if (min(rooms[x1][y1][1], rooms[x1][y1][3])
                    < min(rooms[x1][y1][0], rooms[x1][y1][2])) {
                    if (reachable(MP(x1 - 1, y1)) && reachable(MP(x2, y2 + 1))) {
                        moves.emplace_back(order(teams[id][0], MP(x1 - 1, y1)));
                        teams[id][0] = MP(x1 - 1, y1);
                        grid[x1][y1] = 0;
                        grid[x1 - 1][y1] = 2;
                        moves.emplace_back(order(teams[id][1], MP(x2, y2 + 1)));
                        teams[id][1] = MP(x2, y2 + 1);
                        grid[x2][y2] = 0;
                        grid[x2][y2 + 1] = 2;
                        return true;
                    }
                    if (reachable(MP(x1, y1+1)) && reachable(MP(x2-1, y2))) {
                        moves.emplace_back(order(teams[id][0], MP(x1, y1+1)));
                        teams[id][0] = MP(x1, y1+1);
                        grid[x1][y1] = 0;
                        grid[x1][y1+1] = 2;
                        moves.emplace_back(order(teams[id][1], MP(x2-1, y2)));
                        teams[id][1] = MP(x2-1, y2);
                        grid[x2][y2] = 0;
                        grid[x2-1][y2] = 2;
                        return true;
                    }
                }
                else {
                    if (reachable(MP(x1, y1 + 1)) && reachable(MP(x2 - 1, y2))) {
                        moves.emplace_back(order(teams[id][0], MP(x1, y1 + 1)));
                        teams[id][0] = MP(x1, y1 + 1);
                        grid[x1][y1] = 0;
                        grid[x1][y1 + 1] = 2;
                        moves.emplace_back(order(teams[id][1], MP(x2 - 1, y2)));
                        teams[id][1] = MP(x2 - 1, y2);
                        grid[x2][y2] = 0;
                        grid[x2 - 1][y2] = 2;
                        return true;
                    }
                    if (reachable(MP(x1 - 1, y1)) && reachable(MP(x2, y2 + 1))) {
                        moves.emplace_back(order(teams[id][0], MP(x1 - 1, y1)));
                        teams[id][0] = MP(x1 - 1, y1);
                        grid[x1][y1] = 0;
                        grid[x1 - 1][y1] = 2;
                        moves.emplace_back(order(teams[id][1], MP(x2, y2 + 1)));
                        teams[id][1] = MP(x2, y2 + 1);
                        grid[x2][y2] = 0;
                        grid[x2][y2 + 1] = 2;
                        return true;
                    }
                }
            }
            if ((reachable(MP(x2 - 1, y2 - 1)) && grid[x2 - 1][y2 - 1] == -1)
                || (reachable(MP(x2 + 1, y2 + 1)) && grid[x2 + 1][y2 + 1] == -1)
                || (reachable(MP(x2 + 1, y2 - 1)) && grid[x2 + 1][y2 - 1] == -1)) {
                if (min(rooms[x2][y2][1], rooms[x2][y2][3])
                    < min(rooms[x2][y2][0], rooms[x1][y2][2])) {
                    if (reachable(MP(x1, y1 - 1)) && reachable(MP(x2 + 1, y2))) {
                        moves.emplace_back(order(teams[id][0], MP(x1, y1 - 1)));
                        teams[id][0] = MP(x1, y1 - 1);
                        grid[x1][y1] = 0;
                        grid[x1][y1 - 1] = 2;
                        moves.emplace_back(order(teams[id][1], MP(x2 + 1, y2)));
                        teams[id][1] = MP(x2 + 1, y2);
                        grid[x2][y2] = 0;
                        grid[x2 + 1][y2] = 2;
                        return true;
                    }
                    if (reachable(MP(x1 + 1, y1)) && reachable(MP(x2, y2 - 1))) {
                        moves.emplace_back(order(teams[id][0], MP(x1 + 1, y1)));
                        teams[id][0] = MP(x1 + 1, y1);
                        grid[x1][y1] = 0;
                        grid[x1 + 1][y1] = 2;
                        moves.emplace_back(order(teams[id][1], MP(x2, y2 - 1)));
                        teams[id][1] = MP(x2, y2 - 1);
                        grid[x2][y2] = 0;
                        grid[x2][y2 - 1] = 2;
                        return true;
                    }
                }
                else {
                    if (reachable(MP(x1 + 1, y1)) && reachable(MP(x2, y2 - 1))) {
                        moves.emplace_back(order(teams[id][0], MP(x1 + 1, y1)));
                        teams[id][0] = MP(x1 + 1, y1);
                        grid[x1][y1] = 0;
                        grid[x1 + 1][y1] = 2;
                        moves.emplace_back(order(teams[id][1], MP(x2, y2 - 1)));
                        teams[id][1] = MP(x2, y2 - 1);
                        grid[x2][y2] = 0;
                        grid[x2][y2 - 1] = 2;
                        return true;
                    }
                    if (reachable(MP(x1, y1 - 1)) && reachable(MP(x2 + 1, y2))) {
                        moves.emplace_back(order(teams[id][0], MP(x1, y1 - 1)));
                        teams[id][0] = MP(x1, y1 - 1);
                        grid[x1][y1] = 0;
                        grid[x1][y1 - 1] = 2;
                        moves.emplace_back(order(teams[id][1], MP(x2 + 1, y2)));
                        teams[id][1] = MP(x2 + 1, y2);
                        grid[x2][y2] = 0;
                        grid[x2 + 1][y2] = 2;
                        return true;
                    }
                }
            }

            /*
            if (min(rooms[x1][y1][0], rooms[x1][y1][2]) < min(rooms[x2][y2][0], rooms[x2][y2][2])) {
                if ((reachable(MP(x1 - 1, y1 - 1)) && grid[x1 - 1][y1 - 1] == -1)
                    || (reachable(MP(x1 + 1, y1 + 1)) && grid[x1 + 1][y1 + 1] == -1)
                    || (reachable(MP(x1 - 1, y1 + 1)) && grid[x1 - 1][y1 + 1] == -1)) {
                    if (reachable(MP(x1, y1+1)) && reachable(MP(x2-1, y2))) {
                        moves.emplace_back(order(teams[id][0], MP(x1, y1+1)));
                        teams[id][0] = MP(x1, y1+1);
                        grid[x1][y1] = 0;
                        grid[x1][y1+1] = 2;
                        moves.emplace_back(order(teams[id][1], MP(x2-1, y2)));
                        teams[id][1] = MP(x2-1, y2);
                        grid[x2][y2] = 0;
                        grid[x2-1][y2] = 2;
                        return true;
                    }
                }
                if ((reachable(MP(x2 - 1, y2 - 1)) && grid[x2 - 1][y2 - 1] == -1)
                    || (reachable(MP(x2 + 1, y2 + 1)) && grid[x2 + 1][y2 + 1] == -1)
                    || (reachable(MP(x2 + 1, y2 - 1)) && grid[x2 + 1][y2 - 1] == -1)) {
                    if (reachable(MP(x1, y1-1)) && reachable(MP(x2+1, y2))) {
                        moves.emplace_back(order(teams[id][0], MP(x1, y1 - 1)));
                        teams[id][0] = MP(x1, y1 - 1);
                        grid[x1][y1] = 0;
                        grid[x1][y1 - 1] = 2;
                        moves.emplace_back(order(teams[id][1], MP(x2 + 1, y2)));
                        teams[id][1] = MP(x2 + 1, y2);
                        grid[x2][y2] = 0;
                        grid[x2 + 1][y2] = 2;
                        return true;
                    }
                }
            }
            else {
                if ((reachable(MP(x2 - 1, y2 - 1)) && grid[x2 - 1][y2 - 1] == -1)
                    || (reachable(MP(x2 + 1, y2 + 1)) && grid[x2 + 1][y2 + 1] == -1)
                    || (reachable(MP(x2 + 1, y2 - 1)) && grid[x2 + 1][y2 - 1] == -1)) {
                    if (reachable(MP(x1, y1 - 1)) && reachable(MP(x2 + 1, y2))) {
                        moves.emplace_back(order(teams[id][0], MP(x1, y1 - 1)));
                        teams[id][0] = MP(x1, y1 - 1);
                        grid[x1][y1] = 0;
                        grid[x1][y1 - 1] = 2;
                        moves.emplace_back(order(teams[id][1], MP(x2 + 1, y2)));
                        teams[id][1] = MP(x2 + 1, y2);
                        grid[x2][y2] = 0;
                        grid[x2 + 1][y2] = 2;
                        return true;
                    }
                }
                if ((reachable(MP(x1 - 1, y1 - 1)) && grid[x1 - 1][y1 - 1] == -1)
                    || (reachable(MP(x1 + 1, y1 + 1)) && grid[x1 + 1][y1 + 1] == -1)
                    || (reachable(MP(x1 - 1, y1 + 1)) && grid[x1 - 1][y1 + 1] == -1)) {
                    if (reachable(MP(x1, y1 + 1)) && reachable(MP(x2 - 1, y2))) {
                        moves.emplace_back(order(teams[id][0], MP(x1, y1 + 1)));
                        teams[id][0] = MP(x1, y1 + 1);
                        grid[x1][y1] = 0;
                        grid[x1][y1 + 1] = 2;
                        moves.emplace_back(order(teams[id][1], MP(x2 - 1, y2)));
                        teams[id][1] = MP(x2 - 1, y2);
                        grid[x2][y2] = 0;
                        grid[x2 - 1][y2] = 2;
                        return true;
                    }
                }
            }
            */

if (reachable(MP(x1 - 2, y1)) && grid[x1 - 2][y1] == -1) {
    if (reachable(MP(x1 - 1, y1)) && reachable(MP(x2, y2 + 1))) {
        moves.emplace_back(order(teams[id][0], MP(x1 - 1, y1)));
        teams[id][0] = MP(x1 - 1, y1);
        grid[x1][y1] = 0;
        grid[x1 - 1][y1] = 2;
        moves.emplace_back(order(teams[id][1], MP(x2, y2 + 1)));
        teams[id][1] = MP(x2, y2 + 1);
        grid[x2][y2] = 0;
        grid[x2][y2 + 1] = 2;
        return true;
    }
}
if (reachable(MP(x1, y1 + 2)) && grid[x1][y1 + 2] == -1) {
    if (reachable(MP(x1, y1 + 1)) && reachable(MP(x2 - 1, y2))) {
        moves.emplace_back(order(teams[id][0], MP(x1, y1 + 1)));
        teams[id][0] = MP(x1, y1 + 1);
        grid[x1][y1] = 0;
        grid[x1][y1 + 1] = 2;
        moves.emplace_back(order(teams[id][1], MP(x2 - 1, y2)));
        teams[id][1] = MP(x2 - 1, y2);
        grid[x2][y2] = 0;
        grid[x2 - 1][y2] = 2;
        return true;
    }
}
if (reachable(MP(x2 + 2, y2)) && grid[x2 + 2][y2] == -1) {
    if (reachable(MP(x1, y1 - 1)) && reachable(MP(x2 + 1, y2))) {
        moves.emplace_back(order(teams[id][0], MP(x1, y1 - 1)));
        teams[id][0] = MP(x1, y1 - 1);
        grid[x1][y1] = 0;
        grid[x1][y1 - 1] = 2;
        moves.emplace_back(order(teams[id][1], MP(x2 + 1, y2)));
        teams[id][1] = MP(x2 + 1, y2);
        grid[x2][y2] = 0;
        grid[x2 + 1][y2] = 2;
        return true;
    }
}
if (reachable(MP(x2, y2 - 2)) && grid[x2][y2 - 2] == -1) {
    if (reachable(MP(x1 + 1, y1)) && reachable(MP(x2, y2 - 1))) {
        moves.emplace_back(order(teams[id][0], MP(x1 + 1, y1)));
        teams[id][0] = MP(x1 + 1, y1);
        grid[x1][y1] = 0;
        grid[x1 + 1][y1] = 2;
        moves.emplace_back(order(teams[id][1], MP(x2, y2 - 1)));
        teams[id][1] = MP(x2, y2 - 1);
        grid[x2][y2] = 0;
        grid[x2][y2 - 1] = 2;
        return true;
    }
}

if ((reachable(MP(x1 - 1, y1 - 2)) && grid[x1 - 1][y1 - 2] == -1)
    || (reachable(MP(x1 + 2, y1 + 1)) && grid[x1 + 2][y1 + 1] == -1)) {
    return true;
}

        }
    }
    else if (abs(x1 - x2) + abs(y1 - y2) == 2) {
        if (x1 == x2) {
            int ok1 = -1, ok2 = -1;
            REP(i, 4) {
                int nx = x1 + x_vec[(1 + i) % 4];
                int ny = y1 + y_vec[(1 + i) % 4];
                if (reachable(MP(nx, ny)) && grid[nx][ny] == -1) {
                    ok1 = (1 + i) % 4;
                    moves.emplace_back(order(teams[id][0], MP(nx, ny)));
                    teams[id][0] = MP(nx, ny);
                    grid[x1][y1] = 0;
                    grid[nx][ny] = 2;
                    break;
                }
            }
            REP(i, 4) {
                int nx = x2 + x_vec[(3 + i) % 4];
                int ny = y2 + y_vec[(3 + i) % 4];
                if (reachable(MP(nx, ny)) && grid[nx][ny] == -1) {
                    ok2 = (3 + i) % 4;
                    moves.emplace_back(order(teams[id][1], MP(nx, ny)));
                    teams[id][1] = MP(nx, ny);
                    grid[x2][y2] = 0;
                    grid[nx][ny] = 2;
                    break;
                }
            }
            if (ok1 != -1 && ok2 != -1)return true;
            if (ok1 != -1 && ok2 == -1) {
                if (ok1 == 0) {
                    if (reachable(MP(x2 - 1, y2))) {
                        moves.emplace_back(order(teams[id][1], MP(x2 - 1, y2)));
                        teams[id][1] = MP(x2 - 1, y2);
                        grid[x2][y2] = 0;
                        grid[x2 - 1][y2] = 2;
                        return true;
                    }
                }
                else if (ok1 == 1) {
                    if (reachable(MP(x2, y2 + 1))) {
                        moves.emplace_back(order(teams[id][1], MP(x2, y2 + 1)));
                        teams[id][1] = MP(x2, y2 + 1);
                        grid[x2][y2] = 0;
                        grid[x2][y2 + 1] = 2;
                        return true;
                    }
                }
                else if (ok1 == 2) {
                    if (reachable(MP(x2 + 1, y2))) {
                        moves.emplace_back(order(teams[id][1], MP(x2 + 1, y2)));
                        teams[id][1] = MP(x2 + 1, y2);
                        grid[x2][y2] = 0;
                        grid[x2 + 1][y2] = 2;
                        return true;
                    }
                }
                else if (ok1 == 3) {
                    if (reachable(MP(x2, y2 - 1))) {
                        moves.emplace_back(order(teams[id][1], MP(x2, y2 - 1)));
                        teams[id][1] = MP(x2, y2 - 1);
                        grid[x2][y2] = 0;
                        grid[x2][y2 - 1] = 2;
                        return true;
                    }
                }
                return true;
            }
            else if (ok1 == -1 && ok2 != -1) {
                if (ok2 == 0) {
                    if (reachable(MP(x1 - 1, y1))) {
                        moves.emplace_back(order(teams[id][0], MP(x1 - 1, y1)));
                        teams[id][0] = MP(x1 - 1, y1);
                        grid[x1][y1] = 0;
                        grid[x1 - 1][y1] = 2;
                        return true;
                    }
                }
                else if (ok2 == 1) {
                    if (reachable(MP(x1, y1+1))) {
                        moves.emplace_back(order(teams[id][0], MP(x1, y1+1)));
                        teams[id][0] = MP(x1, y1+1);
                        grid[x1][y1] = 0;
                        grid[x1][y1+1] = 2;
                        return true;
                    }
                }
                else if (ok2 == 2) {
                    if (reachable(MP(x1 + 1, y1))) {
                        moves.emplace_back(order(teams[id][0], MP(x1 + 1, y1)));
                        teams[id][0] = MP(x1 + 1, y1);
                        grid[x1][y1] = 0;
                        grid[x1 + 1][y1] = 2;
                        return true;
                    }
                }
                else if (ok2 == 3) {
                    if (reachable(MP(x1, y1 - 1))) {
                        moves.emplace_back(order(teams[id][0], MP(x1, y1 - 1)));
                        teams[id][0] = MP(x1, y1 - 1);
                        grid[x1][y1] = 0;
                        grid[x1][y1 - 1] = 2;
                        return true;
                    }
                }
                return true;
            }

            if (rooms[x1][y1 + 1][0] < rooms[x1][y1 + 1][2]) {
                if (reachable(MP(x1 - 1, y1 + 1)) && grid[x1 - 1][y1 + 1] == -1) {
                    if (!reachable(MP(x1 - 2, y1 + 1)) || grid[x1 - 2][y1 + 1] == -1
                        ||is_farmer(MP(x1-2,y1))||is_farmer(MP(x1-3,y1+1))
                        ||is_farmer(MP(x2-2,y2))) {
                        if (reachable(MP(x1 - 1, y1)) || reachable(MP(x2 - 1, y2))) {
                            if (reachable(MP(x1 - 1, y1))) {
                                moves.emplace_back(order(teams[id][0], MP(x1 - 1, y1)));
                                teams[id][0] = MP(x1 - 1, y1);
                                grid[x1][y1] = 0;
                                grid[x1 - 1][y1] = 2;
                                return true;
                            }
                            if (reachable(MP(x2 - 1, y2))) {
                                moves.emplace_back(order(teams[id][1], MP(x2 - 1, y2)));
                                teams[id][1] = MP(x2 - 1, y2);
                                grid[x2][y2] = 0;
                                grid[x2 - 1][y2] = 2;
                                return true;
                            }
                        }
                        else {
                            if (reachable(MP(x2, y2 - 1))) {
                                moves.emplace_back(order(teams[id][1], MP(x2, y2 - 1)));
                                teams[id][1] = MP(x2, y2 - 1);
                                grid[x2][y2] = 0;
                                grid[x2][y2 - 1] = 2;
                                return true;
                            }
                        }
                    }
                    else {
                        if (reachable(MP(x1 - 1, y1)) && reachable(MP(x2 - 1, y2)))return true;
                    }
                }
                if (reachable(MP(x1 + 1, y1 + 1)) && grid[x1 + 1][y1 + 1] == -1) {
                    if (!reachable(MP(x1 + 2, y1 + 1)) || grid[x1 + 2][y1 + 1] == -1
                        || is_farmer(MP(x1 + 2, y1)) || is_farmer(MP(x1 + 3, y1 + 1))
                        || is_farmer(MP(x2 + 2, y2))) {
                        if (reachable(MP(x1 + 1, y1)) || reachable(MP(x2 + 1, y2))) {
                            if (reachable(MP(x1 + 1, y1))) {
                                moves.emplace_back(order(teams[id][0], MP(x1 + 1, y1)));
                                teams[id][0] = MP(x1 + 1, y1);
                                grid[x1][y1] = 0;
                                grid[x1 + 1][y1] = 2;
                                return true;
                            }
                            if (reachable(MP(x2 + 1, y2))) {
                                moves.emplace_back(order(teams[id][1], MP(x2 + 1, y2)));
                                teams[id][1] = MP(x2 + 1, y2);
                                grid[x2][y2] = 0;
                                grid[x2 + 1][y2] = 2;
                                return true;
                            }
                        }
                        else {
                            if (reachable(MP(x2, y2 - 1))) {
                                moves.emplace_back(order(teams[id][1], MP(x2, y2 - 1)));
                                teams[id][1] = MP(x2, y2 - 1);
                                grid[x2][y2] = 0;
                                grid[x2][y2 - 1] = 2;
                                return true;
                            }
                        }
                    }
                    else {
                        if (reachable(MP(x1 + 1, y1)) && reachable(MP(x2 + 1, y2))) {
                            moves.emplace_back(order(teams[id][0], MP(x1 + 1, y1)));
                            teams[id][0] = MP(x1 + 1, y1);
                            grid[x1][y1] = 0;
                            grid[x1 + 1][y1] = 2;
                            moves.emplace_back(order(teams[id][1], MP(x2 + 1, y2)));
                            teams[id][1] = MP(x2 + 1, y2);
                            grid[x2][y2] = 0;
                            grid[x2 + 1][y2] = 2;
                            return true;
                        }
                    }
                }
            }
            else {
                if (reachable(MP(x1 + 1, y1 + 1)) && grid[x1 + 1][y1 + 1] == -1) {
                    if (!reachable(MP(x1 + 2, y1 + 1)) || grid[x1 + 2][y1 + 1] == -1
                        || is_farmer(MP(x1 + 2, y1)) || is_farmer(MP(x1 + 3, y1 + 1))
                        || is_farmer(MP(x2 + 2, y2))) {
                        if (reachable(MP(x1 + 1, y1)) || reachable(MP(x2 + 1, y2))) {
                            if (reachable(MP(x1 + 1, y1))) {
                                moves.emplace_back(order(teams[id][0], MP(x1 + 1, y1)));
                                teams[id][0] = MP(x1 + 1, y1);
                                grid[x1][y1] = 0;
                                grid[x1 + 1][y1] = 2;
                                return true;
                            }
                            if (reachable(MP(x2 + 1, y2))) {
                                moves.emplace_back(order(teams[id][1], MP(x2 + 1, y2)));
                                teams[id][1] = MP(x2 + 1, y2);
                                grid[x2][y2] = 0;
                                grid[x2 + 1][y2] = 2;
                                return true;
                            }
                        }
                        else {
                            if (reachable(MP(x2, y2 - 1))) {
                                moves.emplace_back(order(teams[id][1], MP(x2, y2 - 1)));
                                teams[id][1] = MP(x2, y2 - 1);
                                grid[x2][y2] = 0;
                                grid[x2][y2 - 1] = 2;
                                return true;
                            }
                        }
                    }
                    else {
                        if (reachable(MP(x1 + 1, y1)) && reachable(MP(x2 + 1, y2)))return true;
                    }
                }
                if (reachable(MP(x1 - 1, y1 + 1)) && grid[x1 - 1][y1 + 1] == -1) {
                    if (!reachable(MP(x1 - 2, y1 + 1)) || grid[x1 - 2][y1 + 1] == -1
                        || is_farmer(MP(x1 - 2, y1)) || is_farmer(MP(x1 - 3, y1 + 1))
                        || is_farmer(MP(x2 - 2, y2))) {
                        if (reachable(MP(x1 - 1, y1)) || reachable(MP(x2 - 1, y2))) {
                            if (reachable(MP(x1 - 1, y1))) {
                                moves.emplace_back(order(teams[id][0], MP(x1 - 1, y1)));
                                teams[id][0] = MP(x1 - 1, y1);
                                grid[x1][y1] = 0;
                                grid[x1 - 1][y1] = 2;
                                return true;
                            }
                            if (reachable(MP(x2 - 1, y2))) {
                                moves.emplace_back(order(teams[id][1], MP(x2 - 1, y2)));
                                teams[id][1] = MP(x2 - 1, y2);
                                grid[x2][y2] = 0;
                                grid[x2 - 1][y2] = 2;
                                return true;
                            }
                        }
                        else {
                            if (reachable(MP(x2, y2 - 1))) {
                                moves.emplace_back(order(teams[id][1], MP(x2, y2 - 1)));
                                teams[id][1] = MP(x2, y2 - 1);
                                grid[x2][y2] = 0;
                                grid[x2][y2 - 1] = 2;
                                return true;
                            }
                        }
                    }
                    else {
                        if (reachable(MP(x1 - 1, y1)) && reachable(MP(x2 - 1, y2))) {
                            moves.emplace_back(order(teams[id][0], MP(x1 - 1, y1)));
                            teams[id][0] = MP(x1 - 1, y1);
                            grid[x1][y1] = 0;
                            grid[x1 - 1][y1] = 2;
                            moves.emplace_back(order(teams[id][1], MP(x2 - 1, y2)));
                            teams[id][1] = MP(x2 - 1, y2);
                            grid[x2][y2] = 0;
                            grid[x2 - 1][y2] = 2;
                            return true;
                        }
                    }
                }
            }

            if (rooms[x1][y1 + 1][0] < rooms[x1][y1 + 1][2]) {
                if (reachable(MP(x1 - 2, y1 + 1)) && grid[x1 - 2][y1 + 1] == -1) {
                    if (reachable(MP(x1 - 1, y1)) && reachable(MP(x2 - 1, y2))) {
                        if (rooms[x1][y1][0] >= (rooms[x1][y1 + 1][0] - 1)
                            && rooms[x2][y2][0] >= (rooms[x1][y1 + 1][0] - 1)) {
                            moves.emplace_back(order(teams[id][0], MP(x1 - 1, y1)));
                            teams[id][0] = MP(x1 - 1, y1);
                            grid[x1][y1] = 0;
                            grid[x1 - 1][y1] = 2;
                            moves.emplace_back(order(teams[id][1], MP(x2 - 1, y2)));
                            teams[id][1] = MP(x2 - 1, y2);
                            grid[x2][y2] = 0;
                            grid[x2 - 1][y2] = 2;
                            return true;
                        }
                    }
                }
                if (reachable(MP(x1 + 2, y1 + 1)) && grid[x1 + 2][y1 + 1] == -1) {
                    if (reachable(MP(x1 + 1, y1)) && reachable(MP(x2 + 1, y2))) {
                        if (rooms[x1][y1][2] >= (rooms[x1][y1 + 1][2] - 1)
                            && rooms[x2][y2][2] >= (rooms[x1][y1 + 1][2] - 1)) {
                            moves.emplace_back(order(teams[id][0], MP(x1 + 1, y1)));
                            teams[id][0] = MP(x1 + 1, y1);
                            grid[x1][y1] = 0;
                            grid[x1 + 1][y1] = 2;
                            moves.emplace_back(order(teams[id][1], MP(x2 + 1, y2)));
                            teams[id][1] = MP(x2 + 1, y2);
                            grid[x2][y2] = 0;
                            grid[x2 + 1][y2] = 2;
                            return true;
                        }
                    }
                }
            }
            else {
                if (reachable(MP(x1 + 2, y1 + 1)) && grid[x1 + 2][y1 + 1] == -1) {
                    if (reachable(MP(x1 + 1, y1)) && reachable(MP(x2 + 1, y2))) {
                        if (rooms[x1][y1][2] >= (rooms[x1][y1 + 1][2] - 1)
                            && rooms[x2][y2][2] >= (rooms[x1][y1 + 1][2] - 1)) {
                            moves.emplace_back(order(teams[id][0], MP(x1 + 1, y1)));
                            teams[id][0] = MP(x1 + 1, y1);
                            grid[x1][y1] = 0;
                            grid[x1 + 1][y1] = 2;
                            moves.emplace_back(order(teams[id][1], MP(x2 + 1, y2)));
                            teams[id][1] = MP(x2 + 1, y2);
                            grid[x2][y2] = 0;
                            grid[x2 + 1][y2] = 2;
                            return true;
                        }
                    }
                }
                if (reachable(MP(x1 - 2, y1 + 1)) && grid[x1 - 2][y1 + 1] == -1) {
                    if (reachable(MP(x1 - 1, y1)) && reachable(MP(x2 - 1, y2))) {
                        if (rooms[x1][y1][0] >= (rooms[x1][y1 + 1][0] - 1)
                            && rooms[x2][y2][0] >= (rooms[x1][y1 + 1][0] - 1)) {
                            moves.emplace_back(order(teams[id][0], MP(x1 - 1, y1)));
                            teams[id][0] = MP(x1 - 1, y1);
                            grid[x1][y1] = 0;
                            grid[x1 - 1][y1] = 2;
                            moves.emplace_back(order(teams[id][1], MP(x2 - 1, y2)));
                            teams[id][1] = MP(x2 - 1, y2);
                            grid[x2][y2] = 0;
                            grid[x2 - 1][y2] = 2;
                            return true;
                        }
                    }
                }
            }

            if ((reachable(MP(x1 - 1, y1 - 1)) && grid[x1 - 1][y1 - 1] == -1)
                || (reachable(MP(x1 + 1, y1 - 1)) && grid[x1 + 1][y1 - 1] == -1)) {
                if (reachable(MP(x1, y1 - 1)) && reachable(MP(x2, y2 - 1))) {
                    moves.emplace_back(order(teams[id][0], MP(x1, y1 - 1)));
                    teams[id][0] = MP(x1, y1 - 1);
                    grid[x1][y1] = 0;
                    grid[x1][y1 - 1] = 2;
                    moves.emplace_back(order(teams[id][1], MP(x2, y2 - 1)));
                    teams[id][1] = MP(x2, y2 - 1);
                    grid[x2][y2] = 0;
                    grid[x2][y2 - 1] = 2;
                    return true;
                }
            }
            if ((reachable(MP(x2 - 1, y2 + 1)) && grid[x2 - 1][y2 + 1] == -1)
                || (reachable(MP(x2 + 1, y2 + 1)) && grid[x2 + 1][y2 + 1] == -1)) {
                if (reachable(MP(x1, y1 + 1)) && reachable(MP(x2, y2 + 1))) {
                    moves.emplace_back(order(teams[id][0], MP(x1, y1 + 1)));
                    teams[id][0] = MP(x1, y1 + 1);
                    grid[x1][y1] = 0;
                    grid[x1][y1 + 1] = 2;
                    moves.emplace_back(order(teams[id][1], MP(x2, y2 + 1)));
                    teams[id][1] = MP(x2, y2 + 1);
                    grid[x2][y2] = 0;
                    grid[x2][y2 + 1] = 2;
                    return true;
                }
            }

            if ((reachable(MP(x1 - 2, y1)) && grid[x1 - 2][y1] == -1)
                || (reachable(MP(x2 - 2, y2)) && grid[x2-2][y2] == -1)) {
                if (reachable(MP(x1-1, y1)) && reachable(MP(x2-1, y2))) {
                    moves.emplace_back(order(teams[id][0], MP(x1 - 1, y1)));
                    teams[id][0] = MP(x1 - 1, y1);
                    grid[x1][y1] = 0;
                    grid[x1 - 1][y1] = 2;
                    moves.emplace_back(order(teams[id][1], MP(x2 - 1, y2)));
                    teams[id][1] = MP(x2 - 1, y2);
                    grid[x2][y2] = 0;
                    grid[x2 - 1][y2] = 2;
                    return true;
                }
            }
            if ((reachable(MP(x1 + 2, y1)) && grid[x1 + 2][y1] == -1)
                || (reachable(MP(x2 + 2, y2)) && grid[x2 + 2][y2] == -1)) {
                if (reachable(MP(x1 + 1, y1)) && reachable(MP(x2 + 1, y2))) {
                    moves.emplace_back(order(teams[id][0], MP(x1 + 1, y1)));
                    teams[id][0] = MP(x1 + 1, y1);
                    grid[x1][y1] = 0;
                    grid[x1 + 1][y1] = 2;
                    moves.emplace_back(order(teams[id][1], MP(x2 + 1, y2)));
                    teams[id][1] = MP(x2 + 1, y2);
                    grid[x2][y2] = 0;
                    grid[x2 + 1][y2] = 2;
                    return true;
                }
            }

            if (reachable(MP(x1, y1-2)) && grid[x1][y1-2] == -1) {
                if (reachable(MP(x1, y1 - 1)) && reachable(MP(x2, y2 - 1))) {
                    moves.emplace_back(order(teams[id][0], MP(x1, y1 - 1)));
                    teams[id][0] = MP(x1, y1 - 1);
                    grid[x1][y1] = 0;
                    grid[x1][y1 - 1] = 2;
                    moves.emplace_back(order(teams[id][1], MP(x2, y2 - 1)));
                    teams[id][1] = MP(x2, y2 - 1);
                    grid[x2][y2] = 0;
                    grid[x2][y2 - 1] = 2;
                    return true;
                }
            }
            if (reachable(MP(x2, y2 + 2)) && grid[x2][y2 + 2] == -1) {
                if (reachable(MP(x1, y1 + 1)) && reachable(MP(x2, y2 + 1))) {
                    moves.emplace_back(order(teams[id][0], MP(x1, y1 + 1)));
                    teams[id][0] = MP(x1, y1 + 1);
                    grid[x1][y1] = 0;
                    grid[x1][y1 + 1] = 2;
                    moves.emplace_back(order(teams[id][1], MP(x2, y2 + 1)));
                    teams[id][1] = MP(x2, y2 + 1);
                    grid[x2][y2] = 0;
                    grid[x2][y2 + 1] = 2;
                    return true;
                }
            }
        }
        else {
            int ok1 = -1, ok2 = -1;
            REP(i, 4) {
                int nx = x1 + x_vec[(2 + i) % 4];
                int ny = y1 + y_vec[(2 + i) % 4];
                if (reachable(MP(nx, ny)) && grid[nx][ny] == -1) {
                    ok1 = (2 + i) % 4;
                    moves.emplace_back(order(teams[id][0], MP(nx, ny)));
                    teams[id][0] = MP(nx, ny);
                    grid[x1][y1] = 0;
                    grid[nx][ny] = 2;
                    break;
                }
            }
            REP(i, 4) {
                int nx = x2 + x_vec[(0 + i) % 4];
                int ny = y2 + y_vec[(0 + i) % 4];
                if (reachable(MP(nx, ny)) && grid[nx][ny] == -1) {
                    ok2 = (0 + i) % 4;
                    moves.emplace_back(order(teams[id][1], MP(nx, ny)));
                    teams[id][1] = MP(nx, ny);
                    grid[x2][y2] = 0;
                    grid[nx][ny] = 2;
                    break;
                }
            }
            if (ok1 != -1 && ok2 != -1)return true;
            if (ok1 != -1 && ok2 == -1) {
                if (ok1 == 0) {
                    if (reachable(MP(x2 - 1, y2))) {
                        moves.emplace_back(order(teams[id][1], MP(x2 - 1, y2)));
                        teams[id][1] = MP(x2 - 1, y2);
                        grid[x2][y2] = 0;
                        grid[x2 - 1][y2] = 2;
                        return true;
                    }
                }
                else if (ok1 == 1) {
                    if (reachable(MP(x2, y2 + 1))) {
                        moves.emplace_back(order(teams[id][1], MP(x2, y2 + 1)));
                        teams[id][1] = MP(x2, y2 + 1);
                        grid[x2][y2] = 0;
                        grid[x2][y2 + 1] = 2;
                        return true;
                    }
                }
                else if (ok1 == 2) {
                    if (reachable(MP(x2 + 1, y2))) {
                        moves.emplace_back(order(teams[id][1], MP(x2 + 1, y2)));
                        teams[id][1] = MP(x2 + 1, y2);
                        grid[x2][y2] = 0;
                        grid[x2 + 1][y2] = 2;
                        return true;
                    }
                }
                else if (ok1 == 3) {
                    if (reachable(MP(x2, y2 - 1))) {
                        moves.emplace_back(order(teams[id][1], MP(x2, y2 - 1)));
                        teams[id][1] = MP(x2, y2 - 1);
                        grid[x2][y2] = 0;
                        grid[x2][y2 - 1] = 2;
                        return true;
                    }
                }
                return true;
            }
            else if (ok1 == -1 && ok2 != -1) {
                if (ok2 == 0) {
                    if (reachable(MP(x1 - 1, y1))) {
                        moves.emplace_back(order(teams[id][0], MP(x1 - 1, y1)));
                        teams[id][0] = MP(x1 - 1, y1);
                        grid[x1][y1] = 0;
                        grid[x1 - 1][y1] = 2;
                        return true;
                    }
                }
                else if (ok2 == 1) {
                    if (reachable(MP(x1, y1 + 1))) {
                        moves.emplace_back(order(teams[id][0], MP(x1, y1 + 1)));
                        teams[id][0] = MP(x1, y1 + 1);
                        grid[x1][y1] = 0;
                        grid[x1][y1 + 1] = 2;
                        return true;
                    }
                }
                else if (ok2 == 2) {
                    if (reachable(MP(x1 + 1, y1))) {
                        moves.emplace_back(order(teams[id][0], MP(x1 + 1, y1)));
                        teams[id][0] = MP(x1 + 1, y1);
                        grid[x1][y1] = 0;
                        grid[x1 + 1][y1] = 2;
                        return true;
                    }
                }
                else if (ok2 == 3) {
                    if (reachable(MP(x1, y1 - 1))) {
                        moves.emplace_back(order(teams[id][0], MP(x1, y1 - 1)));
                        teams[id][0] = MP(x1, y1 - 1);
                        grid[x1][y1] = 0;
                        grid[x1][y1 - 1] = 2;
                        return true;
                    }
                }
                return true;
            }
            if (rooms[x1 + 1][y1][1] < rooms[x1 + 1][y1][3]) {
                if (reachable(MP(x1 + 1, y1 + 1)) && grid[x1 + 1][y1 + 1] == -1) {
                    if (!reachable(MP(x1 + 1, y1 + 2)) || grid[x1 + 1][y1 + 2] == -1
                        || is_farmer(MP(x1, y1 + 2)) || is_farmer(MP(x1 + 1, y1 + 3))
                        || is_farmer(MP(x2, y2 + 2))) {
                        if (reachable(MP(x1, y1+1)) || reachable(MP(x2, y2+1))) {
                            if (reachable(MP(x1, y1 + 1))) {
                                moves.emplace_back(order(teams[id][0], MP(x1, y1 + 1)));
                                teams[id][0] = MP(x1, y1 + 1);
                                grid[x1][y1] = 0;
                                grid[x1][y1 + 1] = 2;
                                return true;
                            }
                            if (reachable(MP(x2, y2 + 1))) {
                                moves.emplace_back(order(teams[id][1], MP(x2, y2 + 1)));
                                teams[id][1] = MP(x2, y2 + 1);
                                grid[x2][y2] = 0;
                                grid[x2][y2 + 1] = 2;
                                return true;
                            }
                        }
                        else {
                            if (reachable(MP(x2 - 1, y2))) {
                                moves.emplace_back(order(teams[id][1], MP(x2 - 1, y2)));
                                teams[id][1] = MP(x2 - 1, y2);
                                grid[x2][y2] = 0;
                                grid[x2 - 1][y2] = 2;
                                return true;
                            }
                        }
                    }
                    else {
                        if (reachable(MP(x1, y1+1)) && reachable(MP(x2, y2+1)))return true;
                    }
                }
                if (reachable(MP(x1 + 1, y1 - 1)) && grid[x1 + 1][y1 - 1] == -1) {
                    if (!reachable(MP(x1 + 1, y1 - 2)) || grid[x1 + 1][y1 - 2] == -1
                        || is_farmer(MP(x1, y1 - 2)) || is_farmer(MP(x1 + 1, y1 - 3))
                        || is_farmer(MP(x2, y2 - 2))) {
                        if (reachable(MP(x1, y1 - 1)) || reachable(MP(x2, y2 - 1))) {
                            if (reachable(MP(x1, y1 - 1))) {
                                moves.emplace_back(order(teams[id][0], MP(x1, y1 - 1)));
                                teams[id][0] = MP(x1, y1 - 1);
                                grid[x1][y1] = 0;
                                grid[x1][y1 - 1] = 2;
                                return true;
                            }
                            if (reachable(MP(x2, y2 - 1))) {
                                moves.emplace_back(order(teams[id][1], MP(x2, y2 - 1)));
                                teams[id][1] = MP(x2, y2 - 1);
                                grid[x2][y2] = 0;
                                grid[x2][y2 - 1] = 2;
                                return true;
                            }
                        }
                        else {
                            if (reachable(MP(x2 - 1, y2))) {
                                moves.emplace_back(order(teams[id][1], MP(x2 - 1, y2)));
                                teams[id][1] = MP(x2 - 1, y2);
                                grid[x2][y2] = 0;
                                grid[x2 - 1][y2] = 2;
                                return true;
                            }
                        }
                    }
                    else {
                        if (reachable(MP(x1, y1 - 1)) && reachable(MP(x2, y2 - 1))) {
                            moves.emplace_back(order(teams[id][0], MP(x1, y1 - 1)));
                            teams[id][0] = MP(x1, y1 - 1);
                            grid[x1][y1] = 0;
                            grid[x1][y1 - 1] = 2;
                            moves.emplace_back(order(teams[id][1], MP(x2, y2 - 1)));
                            teams[id][1] = MP(x2, y2 - 1);
                            grid[x2][y2] = 0;
                            grid[x2][y2 - 1] = 2;
                            return true;
                        }
                    }
                }
            }
            else {
                if (reachable(MP(x1 + 1, y1 - 1)) && grid[x1 + 1][y1 - 1] == -1) {
                    if (!reachable(MP(x1 + 1, y1 - 2)) || grid[x1 + 1][y1 - 2] == -1
                        || is_farmer(MP(x1, y1 - 2)) || is_farmer(MP(x1 + 1, y1 - 3))
                        || is_farmer(MP(x2, y2 - 2))) {
                        if (reachable(MP(x1, y1 - 1)) || reachable(MP(x2, y2 - 1))) {
                            if (reachable(MP(x1, y1 - 1))) {
                                moves.emplace_back(order(teams[id][0], MP(x1, y1 - 1)));
                                teams[id][0] = MP(x1, y1 - 1);
                                grid[x1][y1] = 0;
                                grid[x1][y1 - 1] = 2;
                                return true;
                            }
                            if (reachable(MP(x2, y2 - 1))) {
                                moves.emplace_back(order(teams[id][1], MP(x2, y2 - 1)));
                                teams[id][1] = MP(x2, y2 - 1);
                                grid[x2][y2] = 0;
                                grid[x2][y2 - 1] = 2;
                                return true;
                            }
                        }
                        else {
                            if (reachable(MP(x2 - 1, y2))) {
                                moves.emplace_back(order(teams[id][1], MP(x2 - 1, y2)));
                                teams[id][1] = MP(x2 - 1, y2);
                                grid[x2][y2] = 0;
                                grid[x2 - 1][y2] = 2;
                                return true;
                            }
                        }
                    }
                    else {
                        if (reachable(MP(x1, y1 - 1)) && reachable(MP(x2, y2 - 1)))return true;
                    }
                }
                if (reachable(MP(x1 + 1, y1 + 1)) && grid[x1 + 1][y1 + 1] == -1) {
                    if (!reachable(MP(x1 + 1, y1 + 2)) || grid[x1 + 1][y1 + 2] == -1
                        || is_farmer(MP(x1, y1 + 2)) || is_farmer(MP(x1 + 1, y1 + 3))
                        || is_farmer(MP(x2, y2 + 2))) {
                        if (reachable(MP(x1, y1 + 1)) || reachable(MP(x2, y2 + 1))) {
                            if (reachable(MP(x1, y1 + 1))) {
                                moves.emplace_back(order(teams[id][0], MP(x1, y1 + 1)));
                                teams[id][0] = MP(x1, y1 + 1);
                                grid[x1][y1] = 0;
                                grid[x1][y1 + 1] = 2;
                                return true;
                            }
                            if (reachable(MP(x2, y2 + 1))) {
                                moves.emplace_back(order(teams[id][1], MP(x2, y2 + 1)));
                                teams[id][1] = MP(x2, y2 + 1);
                                grid[x2][y2] = 0;
                                grid[x2][y2 + 1] = 2;
                                return true;
                            }
                        }
                        else {
                            if (reachable(MP(x2 - 1, y2))) {
                                moves.emplace_back(order(teams[id][1], MP(x2 - 1, y2)));
                                teams[id][1] = MP(x2 - 1, y2);
                                grid[x2][y2] = 0;
                                grid[x2 - 1][y2] = 2;
                                return true;
                            }
                        }
                    }
                    else {
                        if (reachable(MP(x1, y1 + 1)) && reachable(MP(x2, y2 + 1))) {
                            moves.emplace_back(order(teams[id][0], MP(x1, y1 + 1)));
                            teams[id][0] = MP(x1, y1 + 1);
                            grid[x1][y1] = 0;
                            grid[x1][y1 + 1] = 2;
                            moves.emplace_back(order(teams[id][1], MP(x2, y2 + 1)));
                            teams[id][1] = MP(x2, y2 + 1);
                            grid[x2][y2] = 0;
                            grid[x2][y2 + 1] = 2;
                            return true;
                        }
                    }
                }
            }
            if (rooms[x1 + 1][y1][1] < rooms[x1 + 1][y1][3]) {
                if (reachable(MP(x1+1, y1 + 2)) && grid[x1 +1][y1 + 2] == -1) {
                    if (reachable(MP(x1, y1+1)) && reachable(MP(x2, y2+1))) {
                        if (rooms[x1][y1][1] >= (rooms[x1+1][y1][1] - 1)
                            && rooms[x2][y2][1] >= (rooms[x1+1][y1][1] - 1)) {
                            moves.emplace_back(order(teams[id][0], MP(x1, y1 + 1)));
                            teams[id][0] = MP(x1, y1 + 1);
                            grid[x1][y1] = 0;
                            grid[x1][y1 + 1] = 2;
                            moves.emplace_back(order(teams[id][1], MP(x2, y2 + 1)));
                            teams[id][1] = MP(x2, y2 + 1);
                            grid[x2][y2] = 0;
                            grid[x2][y2 + 1] = 2;
                            return true;
                        }
                    }
                }
                if (reachable(MP(x1 + 1, y1 - 2)) && grid[x1 + 1][y1 - 2] == -1) {
                    if (reachable(MP(x1, y1 - 1)) && reachable(MP(x2, y2 - 1))) {
                        if (rooms[x1][y1][3] >= (rooms[x1 + 1][y1][3] - 1)
                            && rooms[x2][y2][3] >= (rooms[x1 + 1][y1][3] - 1)) {
                            moves.emplace_back(order(teams[id][0], MP(x1, y1 - 1)));
                            teams[id][0] = MP(x1, y1 - 1);
                            grid[x1][y1] = 0;
                            grid[x1][y1 - 1] = 2;
                            moves.emplace_back(order(teams[id][1], MP(x2, y2 - 1)));
                            teams[id][1] = MP(x2, y2 - 1);
                            grid[x2][y2] = 0;
                            grid[x2][y2 - 1] = 2;
                            return true;
                        }
                    }
                }
            }
            else {
                if (reachable(MP(x1 + 1, y1 - 2)) && grid[x1 + 1][y1 - 2] == -1) {
                    if (reachable(MP(x1, y1 - 1)) && reachable(MP(x2, y2 - 1))) {
                        if (rooms[x1][y1][3] >= (rooms[x1 + 1][y1][3] - 1)
                            && rooms[x2][y2][3] >= (rooms[x1 + 1][y1][3] - 1)) {
                            moves.emplace_back(order(teams[id][0], MP(x1, y1 - 1)));
                            teams[id][0] = MP(x1, y1 - 1);
                            grid[x1][y1] = 0;
                            grid[x1][y1 - 1] = 2;
                            moves.emplace_back(order(teams[id][1], MP(x2, y2 - 1)));
                            teams[id][1] = MP(x2, y2 - 1);
                            grid[x2][y2] = 0;
                            grid[x2][y2 - 1] = 2;
                            return true;
                        }
                    }
                }
                if (reachable(MP(x1 + 1, y1 + 2)) && grid[x1 + 1][y1 + 2] == -1) {
                    if (reachable(MP(x1, y1 + 1)) && reachable(MP(x2, y2 + 1))) {
                        if (rooms[x1][y1][1] >= (rooms[x1 + 1][y1][1] - 1)
                            && rooms[x2][y2][1] >= (rooms[x1 + 1][y1][1] - 1)) {
                            moves.emplace_back(order(teams[id][0], MP(x1, y1 + 1)));
                            teams[id][0] = MP(x1, y1 + 1);
                            grid[x1][y1] = 0;
                            grid[x1][y1 + 1] = 2;
                            moves.emplace_back(order(teams[id][1], MP(x2, y2 + 1)));
                            teams[id][1] = MP(x2, y2 + 1);
                            grid[x2][y2] = 0;
                            grid[x2][y2 + 1] = 2;
                            return true;
                        }
                    }
                }
            }
            if ((reachable(MP(x1 - 1, y1 - 1)) && grid[x1 - 1][y1 - 1] == -1)
                || (reachable(MP(x1 - 1, y1 + 1)) && grid[x1 - 1][y1 + 1] == -1)) {
                if (reachable(MP(x1-1, y1)) && reachable(MP(x2-1, y2))) {
                    moves.emplace_back(order(teams[id][0], MP(x1 - 1, y1)));
                    teams[id][0] = MP(x1 - 1, y1);
                    grid[x1][y1] = 0;
                    grid[x1 - 1][y1] = 2;
                    moves.emplace_back(order(teams[id][1], MP(x2 - 1, y2)));
                    teams[id][1] = MP(x2 - 1, y2);
                    grid[x2][y2] = 0;
                    grid[x2 - 1][y2] = 2;
                    return true;
                }
            }
            if ((reachable(MP(x2 + 1, y2 - 1)) && grid[x2 + 1][y2 - 1] == -1)
                || (reachable(MP(x2 + 1, y2 + 1)) && grid[x2 + 1][y2 + 1] == -1)) {
                if (reachable(MP(x1+1, y1)) && reachable(MP(x2+1, y2))) {
                    moves.emplace_back(order(teams[id][0], MP(x1 + 1, y1)));
                    teams[id][0] = MP(x1 + 1, y1);
                    grid[x1][y1] = 0;
                    grid[x1 + 1][y1] = 2;
                    moves.emplace_back(order(teams[id][1], MP(x2 + 1, y2)));
                    teams[id][1] = MP(x2 + 1, y2);
                    grid[x2][y2] = 0;
                    grid[x2 + 1][y2] = 2;
                    return true;
                }
            }
            if ((reachable(MP(x1, y1+2)) && grid[x1][y1+2] == -1)
                || (reachable(MP(x2, y2+2)) && grid[x2][y2+2] == -1)) {
                if (reachable(MP(x1, y1 + 1)) && reachable(MP(x2, y2 + 1))) {
                    moves.emplace_back(order(teams[id][0], MP(x1, y1 + 1)));
                    teams[id][0] = MP(x1, y1 + 1);
                    grid[x1][y1] = 0;
                    grid[x1][y1 + 1] = 2;
                    moves.emplace_back(order(teams[id][1], MP(x2, y2 + 1)));
                    teams[id][1] = MP(x2, y2 + 1);
                    grid[x2][y2] = 0;
                    grid[x2][y2 + 1] = 2;
                    return true;
                }
            }
            if ((reachable(MP(x1, y1 - 2)) && grid[x1][y1 - 2] == -1)
                || (reachable(MP(x2, y2 - 2)) && grid[x2][y2 - 2] == -1)) {
                if (reachable(MP(x1, y1 - 1)) && reachable(MP(x2, y2 - 1))) {
                    moves.emplace_back(order(teams[id][0], MP(x1, y1 - 1)));
                    teams[id][0] = MP(x1, y1 - 1);
                    grid[x1][y1] = 0;
                    grid[x1][y1 - 1] = 2;
                    moves.emplace_back(order(teams[id][1], MP(x2, y2 - 1)));
                    teams[id][1] = MP(x2, y2 - 1);
                    grid[x2][y2] = 0;
                    grid[x2][y2 - 1] = 2;
                    return true;
                }
            }
            if (reachable(MP(x1-2, y1)) && grid[x1-2][y1] == -1) {
                if (reachable(MP(x1 - 1, y1)) && reachable(MP(x2 - 1, y2))) {
                    moves.emplace_back(order(teams[id][0], MP(x1 - 1, y1)));
                    teams[id][0] = MP(x1 - 1, y1);
                    grid[x1][y1] = 0;
                    grid[x1 - 1][y1] = 2;
                    moves.emplace_back(order(teams[id][1], MP(x2 - 1, y2)));
                    teams[id][1] = MP(x2 - 1, y2);
                    grid[x2][y2] = 0;
                    grid[x2 - 1][y2] = 2;
                    return true;
                }
            }
            if (reachable(MP(x2 + 2, y2)) && grid[x2 + 2][y2] == -1) {
                if (reachable(MP(x1 + 1, y1)) && reachable(MP(x2 + 1, y2))) {
                    moves.emplace_back(order(teams[id][0], MP(x1 + 1, y1)));
                    teams[id][0] = MP(x1 + 1, y1);
                    grid[x1][y1] = 0;
                    grid[x1 + 1][y1] = 2;
                    moves.emplace_back(order(teams[id][1], MP(x2 + 1, y2)));
                    teams[id][1] = MP(x2 + 1, y2);
                    grid[x2][y2] = 0;
                    grid[x2 + 1][y2] = 2;
                    return true;
                }
            }
        }
    }

    return false;
}

int manhattan(int x) {
    if (SZ(teams[x]) == 2) {
        return abs(teams[x][0].first - teams[x][1].first) +
            abs(teams[x][0].second - teams[x][1].second);
    }
    else return 0;
}

void make_teams() {
    teams = VV<P>((SZ(farmers)+1) / 2);
    REP(i, SZ(farmers)) {
        teams[i / 2].emplace_back(farmers[i]);
    }
    int sum = 0;
    REP(i, SZ(teams)) {
        if (SZ(teams[i]) == 1)continue;
        sum += abs(teams[i][0].first - teams[i][1].first) +
            abs(teams[i][0].second - teams[i][1].second);
    }
    uniform_int_distribution<> mem(0, SZ(farmers)-1);
    REP(_, 100000) {
        int a = mem(rng);
        int b = mem(rng);
        while (a / 2 == b / 2) {
            a = mem(rng);
            b = mem(rng);
        }
        int bef = manhattan(a / 2) + manhattan(b / 2);
        swap(teams[a / 2][a % 2], teams[b / 2][b % 2]);
        int aft = manhattan(a / 2) + manhattan(b / 2);
        if(bef<aft)swap(teams[a / 2][a % 2], teams[b / 2][b % 2]);
    }
    get_time();
}

void align() {
    REP(f, SZ(farmers)) {
        P p = farmers[f];
        bool ok = false;
        REP(k, 4) {
            int x = p.first + x_vec[k];
            int y = p.second + y_vec[k];
            if (x < 0 || x >= N || y < 0 || y >= N || grid[x][y] >= 0)continue;
            if (grid[x][y] == -1) {
                moves.emplace_back(order(p, { x, y }));
                farmers[f] = MP(x, y);
                grid[p.first][p.second] = 0;
                grid[x][y] = 2;
                ok = true;
                /*
                REP(i, 4) {
                    int r = p.first + x_vec[i];
                    int c = p.second + y_vec[i];
                    if (r < 0 || r >= N || r < 0 || r >= N || grid[r][c] !=1 )continue;
                    grid[r][c] = 0;
                }
                REP(i, 4) {
                    int r = x + x_vec[i];
                    int c = y + y_vec[i];
                    if (r < 0 || r >= N || r < 0 || r >= N || grid[r][c] !=0 )continue;
                    grid[r][c] = 1;
                }
                */
            }
            if (ok)break;
        }
        if (ok)continue;

        if (destinations[f].first == -1) {
            continue;
            P t = nearest_chicken(p);
            if (t.first == -1)continue;
            moves.emplace_back(order(p, t));
            farmers[f] = t;
            grid[p.first][p.second] = 0;
            grid[t.first][t.second] = 2;
        }
        else {
            P dir = get_direction2(p, destinations[f]);
            if (dir.first == -1 || grid[dir.first][dir.second] >= 2) {
                continue;
                dir = get_direction(p, destinations[f]);
                if (dir.first == -1 || grid[dir.first][dir.second] >= 2)continue;
            }
            grid[p.first][p.second] = 0;
            grid[dir.first][dir.second] = 2;
            farmers[f] = dir;
            moves.emplace_back(order(p, dir));
        }
        /*
        int dir = muki(rng);
        int x = p.first + x_vec[dir];
        int y = p.second + y_vec[dir];
        if (x < 0 || x >= N || y < 0 || y >= N || grid[x][y] >= 2)continue;
        moves.emplace_back(order(p.first, p.second, x, y));
        grid[p.first][p.second] = 1;
        grid[x][y] = 2;
        REP(i, 4) {
            int r = p.first + x_vec[i];
            int c = p.second + y_vec[i];
            if (r < 0 || r >= N || r < 0 || r >= N || grid[r][c] != 1)continue;
            grid[r][c] = 0;
        }
        REP(i, 4) {
            int r = x + x_vec[i];
            int c = y + y_vec[i];
            if (r < 0 || r >= N || r < 0 || r >= N || grid[r][c] != 0)continue;
            grid[r][c] = 1;
        }
        */
    }
}

void nearest() {
    P t;
    REP(i, SZ(farmers)) {
        P p = farmers[i];
        bool ok = false;
        //if (i % 2 == 0 && i / 2 < SZ(farmers) / 2) {
        if (i %2 == 0) {
            key = jun(rng);
            t = nearest_chicken2(p);
        }
        else {
            REP(j, 4) {
                int k = perm_dir[key][j];
                int x = p.first + x_vec[k];
                int y = p.second + y_vec[k];
                if (x < 0 || x >= N || y < 0 || y >= N || grid[x][y] >= 0)continue;
                if (grid[x][y] == -1) {
                    moves.emplace_back(order(p, { x, y }));
                    farmers[i] = MP(x, y);
                    grid[p.first][p.second] = 0;
                    grid[x][y] = 2;
                    ok = true;
                    break;
                }
            }
        }
        //cerr <<i<<" "<< t.first << " " << t.second << endl;
        
        if (ok==true||t.first == -1)continue;
        P m = get_direction2(p, t);
        if (m.first == -1)continue;
        moves.emplace_back(order(p, m));
        farmers[i] = m;
        grid[p.first][p.second] = 0;
        grid[m.first][m.second] = 2;
    }
}

void nearest2() {
    P t;
    REP(i, SZ(teams)) {
        REP(j, SZ(teams[i])){
            P p = teams[i][j];
            bool ok = false;
            if (j == 0) {
                key = jun(rng);
                t = nearest_chicken2(p);
            }
            else {
                REP(l, 4) {
                    int k = perm_dir[key][l];
                    int x = p.first + x_vec[k];
                    int y = p.second + y_vec[k];
                    if (x < 0 || x >= N || y < 0 || y >= N || grid[x][y] >= 0)continue;
                    if (grid[x][y] == -1) {
                        moves.emplace_back(order(p, { x, y }));
                        teams[i][j] = MP(x, y);
                        grid[p.first][p.second] = 0;
                        grid[x][y] = 2;
                        ok = true;
                        break;
                    }
                }
            }
            if (ok == true || t.first == -1)continue;
            P m = get_direction2(p, t);
            if (m.first == -1)continue;
            moves.emplace_back(order(p, m));
            teams[i][j] = m;
            grid[p.first][p.second] = 0;
            grid[m.first][m.second] = 2;
        }
    }
}

void nearest3() {
    P t;
    chased = map<P, bool>();
    REP(i, SZ(teams)) {
        key = jun(rng);
        if (SZ(teams[i]) == 2) {
            t = nearest_chicken3(teams[i]);
            if (t.first!=-1&&check1[t.first][t.second] < check2[t.first][t.second]) {
                swap(teams[i][0], teams[i][1]);
            }
        }
        //cerr << t.first << " " << t.second << endl;
        REP(j, SZ(teams[i])) {
            P p = teams[i][j];
            bool ok = false;
            REP(l, 4) {
                int k = perm_dir[key][l];
                int x = p.first + x_vec[k];
                int y = p.second + y_vec[k];
                if (x < 0 || x >= N || y < 0 || y >= N || grid[x][y] >= 0)continue;
                if (grid[x][y] == -1) {
                    moves.emplace_back(order(p,MP(x,y)));
                    teams[i][j] = MP(x, y);
                    grid[p.first][p.second] = 0;
                    grid[x][y] = 2;
                    ok = true;
                    break;
                }
            }
            if (ok == true || t.first == -1)continue;
            P m = get_direction2(p, t);
            if (m.first == -1) continue;
            moves.emplace_back(order(p, m));
            teams[i][j] = m;
            grid[p.first][p.second] = 0;
            grid[m.first][m.second] = 2;
            if(SZ(teams[i])>0)chased[t] = true;
        }
    }
}

void nearest4() {
    P t=MP(-1,-1);
    chased = map<P, bool>();
    REP(i, SZ(teams)) {
        //REP(j, SZ(teams[i]))cerr << teams[i][j].first << " " << teams[i][j].second << " ";
        //cerr << endl;
        key = jun(rng);
        if (SZ(teams[i]) == 2) {
            if (manual(i))continue;
            t = nearest_chicken3(teams[i]);
            if (t.first != -1 && check1[t.first][t.second] < check2[t.first][t.second]) {
                swap(teams[i][0], teams[i][1]);
            }
        }
        else {
            if (t.first == -1) {
                t = nearest_chicken2(teams[i][0]);
            }
        }
        REP(j, SZ(teams[i])) {
            P p = teams[i][j];
            bool ok = false;
            REP(l, 4) {
                int k = perm_dir[key][l];
                int x = p.first + x_vec[k];
                int y = p.second + y_vec[k];
                if (x < 0 || x >= N || y < 0 || y >= N || grid[x][y] >= 0)continue;
                if (grid[x][y] == -1) {
                    moves.emplace_back(order(p, MP(x, y)));
                    teams[i][j] = MP(x, y);
                    grid[p.first][p.second] = 0;
                    grid[x][y] = 2;
                    ok = true;
                    break;
                }
            }
            if (ok == true || t.first == -1)continue;
            P m = get_direction2(p, t);
            if (m.first == -1) continue;
            moves.emplace_back(order(p, m));
            teams[i][j] = m;
            grid[p.first][p.second] = 0;
            grid[m.first][m.second] = 2;
            if (SZ(teams[i]) > 0)chased[t] = true;
        }
    }
}

signed main() {
    cin.tie(0);
    ios::sync_with_stdio(false);
    cout << fixed << setprecision(15);
    t_start = chrono::system_clock::now();

    uniform_int_distribution<> muki(0, 3);
    
    V<int> a(4);
    int cnt_dir = 0;
    REP(i, 4)a[i] = i;
    do {
        perm_dir[cnt_dir] = a;
        cnt_dir++;
    } while (next_permutation(ALL(a)));


    cin >> N;
    input();
    make_teams();
    //set_destination(0);
    //bool flag = true;
    //int step = 0;
    REP(turn, N * N) {
        //cerr << turn << ":" << endl;
        nearest4();
        //cerr << "-----------" << endl;
        /*
        key = jun(rng);
        align();
        if (SZ(moves) == 0) {
            if(flag)step = (step + (N - SZ(farmers) + 1)/2) 
                % (4 * (N - SZ(farmers) + 1));
            else step = (step + (N - SZ(farmers) + 2) / 2)
                % (4 * (N - SZ(farmers) + 1));
            //cerr << step << endl;
            set_destination(step);
            align();
        }
        */
        
        cout << SZ(moves) << endl;
        //cerr << SZ(moves) << endl;
        for (string m : moves) {
            cout << m << endl;
            //cerr << m << endl;
        }
        //cerr << endl;
        cout.flush();

        cin >> elaspedTime;
        update();
        if (SZ(chickens) == 0)break;
    }

    cout << -1 << endl;
    cout.flush();
    
    get_time();

    //cerr << "time:  " << t_diff << endl;
    //cerr << "score: " << score - price[SZ(machine)] << endl;
    //cerr << "last:  " << last_update << endl;
    //cerr << "cnt:   " << cnt << endl;
    //cerr << "update:" << update_cnt << endl;
    return 0;
}
