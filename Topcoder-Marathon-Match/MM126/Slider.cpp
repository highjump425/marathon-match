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
using P = pair<ll, ll>;
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
#define INF 1000100010001000
#define endl "\n"


mt19937 rng;
uniform_real_distribution<> dist(0.0, 1.0);
chrono::system_clock::time_point t_start;
const double TIME_LIMIT = 9950;
double last_update = 0, t_diff;
const ll T0 = 2000, T1 = 200;
V<int> x_vec = { -1, 0, +1, 0 };
V<int> y_vec = { 0, -1, 0, +1 };
V<char> muki = { 'U','L','D','R' };
int N, C, H, block_num, best_score, update_num = 0;
V<int> four(4), hfour(4);
V<pair<int, int>> best_order, new_order, hole,timeline,new_timeline;
VV<int> board,grid,net,ami;
V<VV<int>> timelapse, new_timelapse;
V<tuple<int, int, int>> block;

void get_time() {
    auto t_now = chrono::system_clock::now();
    t_diff = chrono::duration_cast<chrono::milliseconds>(t_now - t_start).count();
}

double get_temp() {
    return T0 + (T1 - T0) * (1 - t_diff / TIME_LIMIT);
}

V<pair<int, int>> greed() {
    V<tuple<int, int, int, int>> root(block_num);
    REP(i, block_num) {
        int x, y, point;
        tie(x, y, point) = block[i];
        pair<int, int> p = { 1000,1000 };
        REP(j, H) {
            int hx, hy;
            tie(hx, hy) = hole[j];
            chmin(p, MP(min(abs(hx - x) + 1, abs(hy - y) + 1), abs(hx - x) + abs(hy - y)));
        }
        root[i] = make_tuple(p.first, p.second, -point, i);
    }
    sort(ALL(root));
    V<pair<int, int>> order(block_num);
    REP(i, block_num) {
        order[i] = { 0,get<3>(root[i]) };
    }
    return order;
}
/*
bool x_clear(const VV<int>& grid, int sx, int gx, int y) {
    FOR(i, sx, gx) {
        if (grid[i][y] != 0)return false;
    }
    return true;
}

bool y_clear(const VV<int>& grid, int sy, int gy, int x) {
    FOR(i, sy, gy) {
        if (grid[x][i] != 0)return false;
    }
    return true;
}

pair<int, pair<int, int>> path(const VV<int>& board, int x, int y) {
    pair<int, pair<int, int>> goal = { 1000,{0,0} };
    bool x_ok, y_ok;
    REP(p, H) {
        int hx, hy;
        tie(hx, hy) = hole[p];
        int turn = 1000;
        x_ok = true;
        turn = abs(hx - x);
        if (hx > x) {
            FOR(j, hx + 1, hx + abs(hx - x)) {
                if (j > N - 1 || board[j][y] != 0) {
                    turn = j - hx;
                    break;
                }
                else {
                    if (j - hx == goal.first - 1)break;
                }
            }
        }
        else if (hx < x) {
            FORD(j, hx - 1, hx - abs(hx - x)) {
                if (j < 0 || board[j][y] != 0) {
                    turn = hx - j;
                    break;
                }
                else {
                    if (hx - j == goal.first - 1)break;
                }
            }
        }
        if (turn < goal.first - 1) {
            if (hx > x) {
                x_ok = x_clear(board, x + 1, hx, y);
            }
            else if (hx < x) {
                x_ok = x_clear(board, hx, x - 1, y);
            }
            if (x_ok) {
                y_ok = true;
                if (hy > y) {
                    y_ok = y_clear(board, y + 1, hy - 1, hx);
                }
                else if (hy < y) {
                    y_ok = y_clear(board, hy + 1, y - 1, hx);
                }
                if (y_ok) {
                    if (chmin(goal.first, turn + 1)) {
                        goal.second = { p,0 };
                    }
                }
                if (goal.first == 1)break;
            }
        }
        y_ok = true;
        turn = abs(hy - y);
        if (hy > y) {
            FOR(j, hy + 1, hy + abs(hy - y)) {
                if (j > N - 1 || board[x][j] != 0) {
                    turn = j - hy;
                    break;
                }
                else {
                    if (j - hy == goal.first - 1)break;
                }
            }
        }
        else if (hy < y) {
            FORD(j, hy - 1, hy - abs(hy - y)) {
                if (j < 0 || board[x][j] != 0) {
                    turn = hy - j;
                    break;
                }
                else {
                    if (hx - j == goal.first - 1)break;
                }
            }
        }
        if (turn < goal.first - 1) {
            if (hy > y) {
                y_ok = y_clear(board, y + 1, hy, x);
            }
            else if (hy < y) {
                y_ok = y_clear(board, hy, y - 1, x);
            }
            if (y_ok) {
                x_ok = true;

                if (hx > x) {
                    x_ok = x_clear(board, x + 1, hx - 1, hy);
                }
                else if (hx < x) {
                    x_ok = x_clear(board, hx + 1, x - 1, hy);
                }
                if (x_ok) {
                    if (chmin(goal.first, turn + 1)) {
                        goal.second = { p,1 };
                    }
                }
            }
        }
    }
    return goal;
}

int get_score(const V<pair<int, int>>& query) {
    VV<int> board = grid;
    int score = 0, cnt = 0;
    int time = N * N;
    for (pair<int, int> p : query) {
        int i = p.second;
        int x, y, point;
        tie(x, y, point) = block[i];
        pair<int, pair<int, int>> goal = path(board, x, y);
        if (goal.first == 1000)continue;
        score += (point - 1) * max(0, (time - (goal.first - 1)));
        time -= goal.first;
        board[x][y] = 0;
        //cout << time << " " << score << endl;
        if (time < 0)break;
        cnt++;
    }
    return score;
}
*/

int fast_score(const V<pair<int, int>>& query,int start=0) {
    REP(i, N) {
        REP(j, N) {
            if (ami[i][j] != net[i][j])net[i][j] = ami[i][j];
        }
    }
    int score = 0;
    int time = N * N;
    int cnt = 0;
    bool yet = true;
    REP(j,SZ(query)) {
        int i = query[j].second;
        int x, y, point;
        tie(x, y, point) = block[i];
        if (query[j].first == 0) {
            if (j < start) {
                cnt++;
                if (timeline[j].second != 1000) {
                    score += (point - 1) * max(0, (time - (timeline[j].second - 1)));
                    time -= timeline[j].second;
                    int bj = net[x][y] % 100, aj = (net[x][y] % 10000) / 100, bi = (net[x][y] % 1000000) / 10000, ai = net[x][y] / 1000000;
                    if (bj != N)net[x][bj] += -((net[x][bj] % 10000) / 100 * 100) + aj * 100;
                    if (aj != N)net[x][aj] += -(net[x][aj] % 100) + bj;
                    if (bi != N)net[bi][y] += -(net[bi][y] / 1000000 * 1000000) + ai * 1000000;
                    if (ai != N)net[ai][y] += -((net[ai][y] % 1000000) / 10000 * 10000) + bi * 10000;
                }
                continue;
            }
            int min_pass = 1000;
            REP(q, H) {
                int hx, hy;
                tie(hx, hy) = hole[q];
                if (x == hx) {
                    if ((net[hx][hy] % 100) == y || ((net[hx][hy] % 10000) / 100) == y) {
                        min_pass = 1;
                        break;
                    }
                }
                if (y == hy) {
                    if (((net[hx][hy] % 1000000) / 10000) == x || (net[hx][hy] / 1000000) == x) {
                        min_pass = 1;
                        break;
                    }
                }
                if ((((net[hx][hy] % 100) < y || (net[hx][hy] % 100) == N)) && (y < ((net[hx][hy] % 10000) / 100))
                    && ((((net[x][y] % 1000000) / 10000) < hx) || (((net[x][y] % 1000000) / 10000) == N)) && (hx < (net[x][y] / 1000000))) {
                    if (hx > x) {
                        chmin(min_pass, min(abs(hx - x) + 1, (abs((net[x][y] / 1000000) - hx) + 1)));
                    }
                    else if (hx < x) {
                        if (((net[x][y] % 1000000) / 10000) == N) {
                            chmin(min_pass, min(abs(hx - x) + 1, (abs(-1 - hx) + 1)));
                        }
                        else {
                            chmin(min_pass, min(abs(hx - x) + 1, (abs(((net[x][y] % 1000000) / 10000) - hx) + 1)));
                        }
                    }
                }
                if (((((net[hx][hy] % 1000000) / 10000) < x) || (((net[hx][hy] % 1000000) / 10000) == N)) && (x < (net[hx][hy] / 1000000))
                    && ((((net[x][y] % 100) < hy) || ((net[x][y] % 100) == N))) && (hy < ((net[x][y] % 10000) / 100))) {
                    if (hy > y) {
                        chmin(min_pass, min(abs(hy - y) + 1, (abs(((net[x][y] % 10000) / 100) - hy) + 1)));
                    }
                    else if (hy < y) {
                        if ((net[x][y] % 100) == N) {
                            chmin(min_pass, min(abs(hy - y) + 1, (abs(-1 - hy) + 1)));
                        }
                        else {
                            chmin(min_pass, min(abs(hy - y) + 1, (abs((net[x][y] % 100) - hy) + 1)));
                        }
                    }
                }
            }

            new_timeline[cnt] = { i,min_pass };
            cnt++;
            if (min_pass == 1000)continue;
            score += (point - 1) * max(0, (time - (min_pass - 1)));
            time -= min_pass;
            int bj = net[x][y] % 100, aj = (net[x][y] % 10000) / 100, bi = (net[x][y] % 1000000) / 10000, ai = net[x][y] / 1000000;
            if (bj != N)net[x][bj] += -((net[x][bj] % 10000) / 100 * 100) + aj * 100;
            if (aj != N)net[x][aj] += -(net[x][aj] % 100) + bj;
            if (bi != N)net[bi][y] += -(net[bi][y] / 1000000 * 1000000) + ai * 1000000;
            if (ai != N)net[ai][y] += -((net[ai][y] % 1000000) / 10000 * 10000) + bi * 10000;
            net[x][y] = 0;
            if (time < 0)break;
        }
        else {
            //int a = uniform_int_distribution<int>(0, 3)(rng);
            int a = 0;
            int bj = net[x][y] % 100, aj = (net[x][y] % 10000) / 100, bi = (net[x][y] % 1000000) / 10000, ai = net[x][y] / 1000000;
            if (a == 0) {
                if (x == 0 || bi == x - 1)continue;
                if (bj != N)net[x][bj] += -((net[x][bj] % 10000) / 100 * 100) + aj * 100;
                if (aj != N)net[x][aj] += -(net[x][aj] % 100) + bj;
                if (bi != N)net[bi][y] += -(net[bi][y] / 1000000 * 1000000) + (bi+1) * 1000000;
                if (ai != N)net[ai][y] += -((net[ai][y] % 1000000) / 10000 * 10000) + (bi+1) * 10000;
                net[bi + 1][y] = net[x][y];
                net[x][y] = 0;
                int pos = 0;
                while (pos < y) {
                    if (net[bi + 1][pos] == 0)pos++;
                    else {
                        pos = (net[bi + 1][pos] % 10000) / 100;
                    }
                }
                if (pos == y) {
                    bool ok = false;
                    net[bi + 1][y] += -(net[bi + 1][y] % 100) + N;
                    FOR(i, pos + 1, N - 1) {
                        if (net[bi + 1][pos] == 0)continue;
                        else {
                            ok = true;
                            net[bi + 1][y] += -((net[bi + 1][y] / 10000) % 100) + pos * 100;
                            net[bi+1][pos] += -(net[bi + 1][pos] % 100) + y;
                            break;
                        }
                    }
                    if (!ok) {
                        net[bi + 1][y] += -((net[bi + 1][y] /10000)% 100) + N*100;
                    }
                }
                else {
                    net[bi + 1][y] += -((net[bi + 1][y] / 10000) % 100) + pos * 100;
                    net[bi+1][net[bi + 1][pos] % 100] += -((net[bi + 1][pos] / 10000) % 100) + y * 100;
                    net[bi + 1][y] += -(net[bi + 1][y] % 100) + (net[bi + 1][pos] % 100);
                    net[bi + 1][pos] += -(net[bi + 1][pos] % 100) + y;
                }
            }
            else if (a == 1) {

            }
            else if (a == 2) {

            }
            else if (a == 3) {

            }
        }
    }
    return score;
}

/*
int fast_score(const V<pair<int, int>>& query) {
    REP(i, N) {
        REP(j, N) {
            if (ami[i][j] != net[i][j])net[i][j] = ami[i][j];
        }
    }
    int score = 0;
    int time = N * N;
    for (auto p : query) {
        int i = p.second;
        int x, y, point;
        tie(x, y, point) = block[i];
        four = { (net[x][y] % 100),((net[x][y] % 10000) / 100), ((net[x][y] % 1000000) / 10000),(net[x][y] / 1000000) };
        int min_pass = 1000;
        REP(q, H) {
            int hx, hy;
            tie(hx, hy) = hole[q];
            hfour = { (net[hx][hy] % 100),((net[hx][hy] % 10000) / 100), ((net[hx][hy] % 1000000) / 10000),(net[hx][hy] / 1000000) };
            if (x == hx) {
                if (hfour[0] == y || hfour[1] == y) {
                    min_pass = 1;
                    break;
                }
            }
            if (y == hy) {
                if (hfour[2] == x || hfour[3] == x) {
                    min_pass = 1;
                    break;
                }
            }
            if ((hfour[0] < y || hfour[0] == N) && (y < hfour[1]) && ((four[2] < hx) || (four[2] == N)) && (hx < four[3])) {
                if (hx > x) {
                    chmin(min_pass, min(abs(hx - x) + 1, (abs(four[3] - hx) + 1)));
                }
                else if (hx < x) {
                    if (four[2] == N) {
                        chmin(min_pass, min(abs(hx - x) + 1, (abs(-1 - hx) + 1)));
                    }
                    else {
                        chmin(min_pass, min(abs(hx - x) + 1, (abs(four[2] - hx) + 1)));
                    }
                }
            }
            if (((hfour[2] < x) || (hfour[2] == N)) && (x < hfour[3]) && (((four[0] < hy) || (four[0] == N))) && (hy < four[1])) {
                if (hy > y) {
                    chmin(min_pass, min(abs(hy - y) + 1, (abs(four[1] - hy) + 1)));
                }
                else if (hy < y) {
                    if (four[0] == N) {
                        chmin(min_pass, min(abs(hy - y) + 1, (abs(-1 - hy) + 1)));
                    }
                    else {
                        chmin(min_pass, min(abs(hy - y) + 1, (abs(four[0] - hy) + 1)));
                    }
                }
            }
        }
        if (min_pass == 1000)continue;
        score += (point - 1) * max(0, (time - (min_pass - 1)));
        time -= min_pass;
        if (four[0] != N)net[x][four[0]] += -((net[x][four[0]] % 10000) / 100 * 100) + four[1] * 100;
        if (four[1] != N)net[x][four[1]] += -(net[x][four[1]] % 100) + four[0];
        if (four[2] != N)net[four[2]][y] += -(net[four[2]][y] / 1000000 * 1000000) + four[3] * 1000000;
        if (four[3] != N)net[four[3]][y] += -((net[four[3]][y] % 1000000) / 10000 * 10000) + four[2] * 10000;
        if (time < 0)break;
    }
    return score;
}
*/
/*
void output(const V<pair<int, int>>& query) {
    ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    V<string> ans;
    ans.reserve(N * N + 5);
    VV<int> board = grid;
    int score = 0;
    int time = N * N;
    for (pair<int, int> p : query) {
        int i = p.second;
        if (p.first == 0) {
            int x, y, point;
            tie(x, y, point) = block[i];
            pair<int, pair<int, int>> goal = path(board, x, y);
            if (goal.first == 1000)continue;
            score += (point - 1) * max(0, (time - (goal.first - 1)));
            time -= goal.first;
            int hx, hy;
            tie(hx, hy) = hole[goal.second.first];
            if (goal.second.second == 0) {
                if (goal.first - 1 != abs(hx - x)) {
                    if (hx > x) {
                        ans.emplace_back(to_string(x) + " " + to_string(y) + " S D");
                        x = hx + (goal.first - 2);
                    }
                    else if (hx < x) {
                        ans.emplace_back(to_string(x) + " " + to_string(y) + " S U");
                        x = hx - (goal.first - 2);
                    }
                    goal.first--;
                }
                REP(j, goal.first - 1) {
                    if (hx > x) {
                        ans.emplace_back(to_string(x) + " " + to_string(y) + " M D");
                        x++;
                    }
                    else if (hx < x) {
                        ans.emplace_back(to_string(x) + " " + to_string(y) + " M U");
                        x--;
                    }
                }

                if (hy > y)ans.emplace_back(to_string(x) + " " + to_string(y) + " S R");
                else ans.emplace_back(to_string(x) + " " + to_string(y) + " S L");

            }
            else {
                if (goal.first - 1 != abs(hy - y)) {
                    if (hy > y) {
                        ans.emplace_back(to_string(x) + " " + to_string(y) + " S R");
                        y = hy + (goal.first - 2);
                    }
                    else if (hy < y) {
                        ans.emplace_back(to_string(x) + " " + to_string(y) + " S L");
                        y = hy - (goal.first - 2);
                    }
                    goal.first--;
                }
                REP(j, goal.first - 1) {
                    if (hy > y) {
                        ans.emplace_back(to_string(x) + " " + to_string(y) + " M R");
                        y++;
                    }
                    else {
                        ans.emplace_back(to_string(x) + " " + to_string(y) + " M L");
                        y--;
                    }
                }
                if (hx > x)ans.emplace_back(to_string(x) + " " + to_string(y) + " S D");
                else ans.emplace_back(to_string(x) + " " + to_string(y) + " S U");
            }
            tie(x, y, point) = block[i];
            board[x][y] = 0;
            if (SZ(ans) > N* N)break;
        }
        else if (p.first == 5) {
            int x, y, point;
            tie(x, y, point) = block[i];
            if (x == -1)continue;
            pair<int, pair<int, int>> goal = path(board, x, y);
            if (goal.first == 1000)continue;
            int hx, hy;
            tie(hx, hy) = hole[goal.second.first];
            if (goal.second.second == 0) {
                if (goal.first <= 2 || goal.first - 1 == abs(hx - x))continue;
                if (hx > x) {
                    ans.emplace_back(to_string(x) + " " + to_string(y) + " S D");
                    board[x][y] = 0;
                    board[hx + goal.first - 2][y] = point;
                    block[i] = { hx + goal.first - 2 ,y,point };
                }
                else if (hx < x) {
                    ans.emplace_back(to_string(x) + " " + to_string(y) + " S U");
                    board[x][y] = 0;
                    board[hx - (goal.first - 2)][y] = point;
                    block[i] = { hx - (goal.first - 2) ,y,point };
                }
            }
            else {
                if (goal.first <= 2 || goal.first - 1 == abs(hy - y))continue;
                if (hy > y) {
                    ans.emplace_back(to_string(x) + " " + to_string(y) + " S R");
                    board[x][y] = 0;
                    board[x][hy + goal.first - 2] = point;
                    block[i] = { x,hy + goal.first - 2,point };
                }
                else if (hx < x) {
                    ans.emplace_back(to_string(x) + " " + to_string(y) + " S L");
                    board[x][y] = 0;
                    board[x][hy - (goal.first - 2)] = point;
                    block[i] = { x,hy - (goal.first - 2),point };
                }
            }
            time--;
            if (SZ(ans) > N* N)break;
        }
        else {
            int dir = p.first - 1;
            int x, y, point;
            tie(x, y, point) = block[i];
            board[x][y] = 0;
            ans.emplace_back(to_string(x) + " " + to_string(y) + " S " + muki[dir]);
            while (1) {
                if (x < 0 || N - 1 < x || y < 0 || N - 1 < y || board[x][y] >= 1) {
                    x -= x_vec[dir];
                    y -= y_vec[dir];
                    board[x][y] = point;
                    break;
                }
                else if (board[x][y] == -1)break;
                x += x_vec[dir];
                y += y_vec[dir];
            }
            time--;
        }
    }
    cout << min(SZ(ans), N * N) << endl;
    REP(i, min(SZ(ans), N * N)) {
        cout << ans[i] << endl;
    }
    best_score = score;
    return;
}
*/
void new_output(const V<pair<int, int>>& query) {
    ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    REP(i, N) {
        REP(j, N) {
            if (ami[i][j] != net[i][j])net[i][j] = ami[i][j];
        }
    }
    V<string> ans;
    ans.reserve(N * N + 5);
    VV<int> board = grid;
    int score = 0;
    int time = N * N;
    for (auto p : query) {
        int i = p.second;
        int x, y, point;
        tie(x, y, point) = block[i];
        int min_pass = 1000;
        pair<int, int> goal = { -1,-1 };
        REP(q, H) {
            int hx, hy;
            tie(hx, hy) = hole[q];
            if (x == hx) {
                if ((net[hx][hy] % 100) == y || ((net[hx][hy] % 10000) / 100) == y) {
                    min_pass = 1;
                    goal = { q,0 };
                    break;
                }
            }
            if (y == hy) {
                if (((net[hx][hy] % 1000000) / 10000) == x || (net[hx][hy] / 1000000) == x) {
                    min_pass = 1;
                    goal = { q,1 };
                    break;
                }
            }
            if ((((net[hx][hy] % 100) < y || (net[hx][hy] % 100) == N)) && (y < ((net[hx][hy] % 10000) / 100))
                && ((((net[x][y] % 1000000) / 10000) < hx) || (((net[x][y] % 1000000) / 10000) == N)) && (hx < (net[x][y] / 1000000))) {
                if (hx > x) {
                    if (chmin(min_pass, min(abs(hx - x) + 1, (abs((net[x][y] / 1000000) - hx) + 1)))) {
                        goal = { q,0 };
                    }
                }
                else if (hx < x) {
                    if (((net[x][y] % 1000000) / 10000) == N) {
                        if (chmin(min_pass, min(abs(hx - x) + 1, (abs(-1 - hx) + 1)))) {
                            goal = { q,0 };
                        }
                    }
                    else {
                        if (chmin(min_pass, min(abs(hx - x) + 1, (abs(((net[x][y] % 1000000) / 10000) - hx) + 1)))) {
                            goal = { q,0 };
                        }
                    }
                }
            }
            if (((((net[hx][hy] % 1000000) / 10000) < x) || (((net[hx][hy] % 1000000) / 10000) == N)) && (x < (net[hx][hy] / 1000000))
                && ((((net[x][y] % 100) < hy) || ((net[x][y] % 100) == N))) && (hy < ((net[x][y] % 10000) / 100))) {
                if (hy > y) {
                    if (chmin(min_pass, min(abs(hy - y) + 1, (abs(((net[x][y] % 10000) / 100) - hy) + 1)))) {
                        goal={q,1};
                    }
                }
                else if (hy < y) {
                    if ((net[x][y] % 100) == N) {
                        if (chmin(min_pass, min(abs(hy - y) + 1, (abs(-1 - hy) + 1)))) {
                            goal = { q,1 };
                        }
                    }
                    else {
                        if (chmin(min_pass, min(abs(hy - y) + 1, (abs((net[x][y] % 100) - hy) + 1)))) {
                            goal = { q,1 };
                        }
                    }
                }
            }
        }
        if (min_pass == 1000)continue;
        score += (point - 1) * max(0, (time - (min_pass - 1)));
        time -= min_pass;
        int hx, hy;
        tie(hx, hy) = hole[goal.first];
        if (goal.second == 0) {
            if (min_pass - 1 != abs(hx - x)) {
                if (hx > x) {
                    ans.emplace_back(to_string(x) + " " + to_string(y) + " S D");
                    x = hx + (min_pass - 2);
                }
                else if (hx < x) {
                    ans.emplace_back(to_string(x) + " " + to_string(y) + " S U");
                    x = hx - (min_pass - 2);
                }
                min_pass--;
            }
            REP(j, min_pass - 1) {
                if (hx > x) {
                    ans.emplace_back(to_string(x) + " " + to_string(y) + " M D");
                    x++;
                }
                else if (hx < x) {
                    ans.emplace_back(to_string(x) + " " + to_string(y) + " M U");
                    x--;
                }
            }

            if (hy > y)ans.emplace_back(to_string(x) + " " + to_string(y) + " S R");
            else ans.emplace_back(to_string(x) + " " + to_string(y) + " S L");

        }
        else {
            if (min_pass - 1 != abs(hy - y)) {
                if (hy > y) {
                    ans.emplace_back(to_string(x) + " " + to_string(y) + " S R");
                    y = hy + (min_pass - 2);
                }
                else if (hy < y) {
                    ans.emplace_back(to_string(x) + " " + to_string(y) + " S L");
                    y = hy - (min_pass - 2);
                }
                min_pass--;
            }
            REP(j, min_pass - 1) {
                if (hy > y) {
                    ans.emplace_back(to_string(x) + " " + to_string(y) + " M R");
                    y++;
                }
                else {
                    ans.emplace_back(to_string(x) + " " + to_string(y) + " M L");
                    y--;
                }
            }
            if (hx > x)ans.emplace_back(to_string(x) + " " + to_string(y) + " S D");
            else ans.emplace_back(to_string(x) + " " + to_string(y) + " S U");
        }
        tie(x, y, point) = block[i];
        int bj = net[x][y] % 100, aj = (net[x][y] % 10000) / 100, bi = (net[x][y] % 1000000) / 10000, ai = net[x][y] / 1000000;
        if (bj != N)net[x][bj] = net[x][bj] - ((net[x][bj] % 10000) / 100 * 100) + aj * 100;
        if (aj != N)net[x][aj] = net[x][aj] - (net[x][aj] % 100) + bj;
        if (bi != N)net[bi][y] = net[bi][y] - (net[bi][y] / 1000000 * 1000000) + ai * 1000000;
        if (ai != N)net[ai][y] = net[ai][y] - ((net[ai][y] % 1000000) / 10000 * 10000) + bi * 10000;
        //cout << x << " " << y << " " << min_pass << " " << goal.first << " " << goal.second << endl;
        if (SZ(ans) > N* N)break;
    }
    cout << min(SZ(ans), N * N) << endl;
    REP(i, min(SZ(ans), N * N)) {
        cout << ans[i] << endl;
    }
    best_score = score;
    return;
}

void passing(V<pair<int, int>>& order, int& score) {
    int a = uniform_int_distribution<int>(0, SZ(order) - 1)(rng);
    int b = uniform_int_distribution<int>(0, SZ(order) - 1)(rng);
    if (b == a)return;
    if (order[a].first != 0)return;
    if (order[b].first != 0)return;
    if (a > b)swap(a, b);
    if(SZ(new_order)!=SZ(order))new_order = order;
    else {
        REP(i, SZ(order)) {
            if (new_order[i] != order[i])new_order[i] = order[i];
        }
    }
    if (SZ(new_timeline) != SZ(timeline))new_timeline = timeline;
    else {
        REP(i, SZ(timeline)) {
            if (new_timeline[i] != timeline[i])new_timeline[i] = timeline[i];
        }
    }
    new_timeline = timeline;
    new_order.erase(new_order.begin() + b);
    new_order.insert(new_order.begin() + a, order[b]);
    int new_score;
    new_score = fast_score(new_order,a);
    if (new_score > score) {
        swap(score, new_score);
        swap(order, new_order);
        swap(timeline, new_timeline);
        if (chmax(best_score, score)) {
            best_order = order;
            last_update = t_diff;
        }
        update_num++;
    }
}

void retreat(V<pair<int, int>>& order, int& score) {
    int a = uniform_int_distribution<int>(0, block_num - 1)(rng);
    int b = uniform_int_distribution<int>(0, block_num - 1)(rng);
    if (b == a)return;
    if (a > b)swap(a, b);
    if (order[a].first != 0)return;
    if (order[b].first != 0)return;
    if (SZ(new_order) != SZ(order))new_order = order;
    else {
        REP(i, SZ(order)) {
            if (new_order[i] != order[i])new_order[i] = order[i];
        }
    }
    if (SZ(new_timeline) != SZ(timeline))new_timeline = timeline;
    else {
        REP(i, SZ(timeline)) {
            if (new_timeline[i] != timeline[i])new_timeline[i] = timeline[i];
        }
    }
    new_order.erase(new_order.begin() + a);
    new_order.insert(new_order.begin() + b, order[a]);
    int new_score;
    new_score = fast_score(new_order,a);
    if (new_score > score) {
        swap(score, new_score);
        swap(order, new_order);
        swap(timeline, new_timeline);
        if (chmax(best_score, score)) {
            best_order = order;
            last_update = t_diff;
        }
        update_num++;
    }
}

/*
void stepback(V<pair<int, int>>& order, int& score) {
    int a = uniform_int_distribution<int>(0, SZ(order) - 1)(rng);
    int b = uniform_int_distribution<int>(0, SZ(order) - 1)(rng);
    if (b == a)return;
    if (order[a].first != 0)return;
    if (order[b].first != 0)return;
    if (a > b)swap(a, b);
    new_order = order;
    new_order.erase(new_order.begin() + a);
    new_order.insert(new_order.begin() + b, order[a]);
    new_order.insert(new_order.begin() + a, MP(5, order[a].second));
    int new_score;
    new_score = get_score(new_order);
    if (new_score > score) {
        swap(score, new_score);
        swap(order, new_order);
        if (chmax(best_score, score)) {
            best_order = order;
            last_update = t_diff;
        }
        update_num++;
    }
}

void skip(V<pair<int, int>>& order, int& score) {
    int a = uniform_int_distribution<int>(0, block_num - 1)(rng);
    int b = uniform_int_distribution<int>(0, 3)(rng);
    int x, y, point;
    tie(x, y, point) = block[order[a].second];
    if (point <= C / 2)return;
    REP(i, 4) {
        int dir = b + i;
        dir %= 4;
        if (x + x_vec[dir] < 0 || N - 1 < x + x_vec[dir] || y + y_vec[dir] < 0 || N - 1 < y + y_vec[dir])continue;
        int new_score;
        new_order = order;
        new_order[a].first = dir + 1;
        new_score = get_score(new_order);
        if (new_score > score) {
            swap(score, new_score);
            swap(order, new_order);
            if (chmax(best_score, score)) {
                best_order = order;
                last_update = t_diff;
            }
            update_num++;
        }
        return;
    }
    return;
}
*/

int main() {
    cin.tie(0);
    ios::sync_with_stdio(false);
    cout << fixed << setprecision(15);
    t_start = chrono::system_clock::now();

    //input
    cin >> N >> C >> H;
    int itr = 0, score, init_score;
    grid.assign(N, V<int>(N, 0));
    board.assign(N, V<int>(N, 0));
    net.assign(N, V<int>(N, 0));
    ami.assign(N, V<int>(N, 0));
    block_num = 0;
    hole.reserve(H + 2);
    block.reserve(N * N+2);
    REP(i, N) {
        int aj = N;
        REP(j, N) {
            cin >> grid[i][j];
            if (grid[i][j] != 0) {
                net[i][j]+= aj;
                if (aj != N)net[i][aj]+= j*100;
                aj = j;
                if (grid[i][j] == -1)hole.emplace_back(i, j);
                else if (grid[i][j] > 0) {
                    block.emplace_back(i, j, grid[i][j]);
                    block_num++;
                }
            }
        }
        if (aj != N)net[i][aj] += N*100;
    }
    REP(j, N) {
        int ai = N;
        REP(i, N) {
            if (grid[i][j] != 0) {
                net[i][j] += ai*10000;
                if (ai != N)net[ai][j] += i*1000000;
                ai = i;
            }
        }
        if (ai != N)net[ai][j] += N*1000000;
    }
    ami = net;
    block_num = SZ(block);
    timeline.assign(block_num+2, { 0,0 });
    new_timeline.assign(block_num+2, { 0,0 });
    V<pair<int, int>> order = greed();
    best_order = order;
    score = fast_score(best_order,0);
    timeline = new_timeline;
    best_score = score;
    init_score = score;
    
    while (t_diff < TIME_LIMIT) {
        passing(order, score);
        retreat(order, score);
        if (itr % 50 == 0)get_time();
        itr++;
    }
    new_output(best_order);
    cout.flush();
    get_time();

    
    cerr << "ini_score: " << init_score << endl;
    cerr << "score: " << best_score << endl;
    cerr << "time: " << t_diff << endl;
    cerr << "last_update: " << last_update << endl;
    cerr << "update count: " << update_num << endl;
    cerr << "itrate: " << itr << endl;
    
}

/*
3 1 2 
1 1 -1 
1 1 1 
-1 1 1 

10 5 3 
5 5 3 0 2 -1 2 5 5 0 
2 0 2 5 3 1 5 4 0 5 
3 3 0 0 0 2 0 2 1 5 
3 1 0 2 2 0 3 0 4 0 
0 1 4 3 0 2 2 3 1 5 
4 0 0 3 3 5 0 2 1 3 
0 5 0 -1 2 5 0 3 5 0 
0 3 5 0 0 5 5 0 5 1 
-1 0 2 0 5 0 2 3 1 5 
3 0 0 3 3 1 4 2 0 1 

28 8 2 
0 0 5 0 0 0 1 0 2 3 0 4 6 0 1 2 0 0 5 0 7 0 8 0 3 0 0 0 
0 3 1 0 0 0 3 5 -1 7 5 2 0 0 0 0 1 0 0 0 2 0 0 0 5 0 0 2 
7 8 8 0 0 0 4 0 0 8 0 6 1 6 0 0 7 0 3 0 3 3 7 0 0 0 8 0 
3 8 0 4 0 0 0 0 0 6 6 4 7 0 8 0 0 0 0 2 6 0 0 0 5 7 3 2 
1 0 2 2 2 2 1 7 0 0 0 0 8 0 8 0 3 1 1 0 0 0 0 4 0 5 0 0 
0 5 0 0 0 3 0 0 0 8 0 0 0 0 6 2 7 1 0 0 0 8 4 0 6 0 1 1 
0 0 0 0 0 0 0 0 1 3 0 0 0 0 0 8 2 3 0 0 0 7 1 1 0 0 2 7 
3 0 0 0 6 7 0 3 0 8 0 3 5 0 2 0 0 0 0 0 5 0 0 0 0 3 0 0 
3 0 8 5 5 0 0 8 0 0 0 1 0 2 0 0 0 0 0 0 4 2 0 0 -1 6 8 6 
0 2 5 0 0 6 0 0 7 3 0 3 0 2 0 0 2 4 5 8 8 0 2 0 1 1 0 8 
0 3 5 0 0 0 1 8 5 0 0 0 6 6 0 1 0 5 2 0 1 5 6 8 0 7 0 6 
5 0 0 0 0 6 0 0 3 5 6 0 0 8 1 0 1 0 7 7 7 4 0 2 0 8 0 8 
2 0 1 6 6 0 4 3 6 1 0 2 0 3 2 0 0 0 8 0 2 0 6 0 7 0 0 0 
0 0 0 0 3 0 5 8 1 0 8 6 3 0 7 0 0 5 0 1 0 0 0 0 5 0 1 7 
0 8 0 2 3 0 2 0 0 0 6 4 3 4 0 7 4 0 0 0 0 0 0 4 0 0 6 0 
0 0 5 0 4 3 0 0 0 0 0 0 1 5 0 2 8 8 0 0 0 7 0 8 0 4 8 0 
3 8 3 0 0 7 0 2 0 0 8 2 0 0 1 3 4 1 5 0 1 7 7 0 0 0 7 0 
0 5 1 6 4 5 0 0 3 0 7 0 0 0 2 3 0 2 0 4 0 0 0 0 0 8 1 1 
2 0 0 2 5 7 0 7 0 0 7 5 0 2 0 0 1 0 0 0 0 0 6 8 2 0 6 6 
0 1 0 6 8 0 5 0 0 0 0 0 2 5 0 0 0 0 0 5 0 2 0 0 0 3 0 4 
0 0 0 5 3 0 4 0 0 0 0 7 0 0 0 0 3 0 0 0 1 0 7 7 1 6 0 0 
0 6 5 4 3 0 0 1 8 5 0 3 0 5 0 0 0 7 0 0 6 2 7 7 8 6 5 2 
0 3 0 4 5 0 0 1 0 7 7 0 0 0 0 2 0 0 1 3 0 4 7 7 0 0 3 0 
3 0 0 0 2 0 5 8 8 0 0 0 0 4 0 0 0 0 8 7 2 1 0 5 1 5 0 2 
4 0 0 0 6 0 0 2 0 3 1 0 2 0 4 5 0 3 0 5 0 4 0 6 3 8 0 0 
0 0 1 0 0 6 0 0 2 1 8 0 0 4 2 0 0 0 0 5 0 0 0 0 0 6 2 3 
0 2 0 0 4 5 4 0 4 4 0 0 0 0 0 0 0 0 0 3 5 0 0 5 0 0 0 2 
3 4 0 6 2 0 0 0 1 4 0 0 0 0 3 0 1 3 8 0 6 2 0 2 5 3 2 0 

20 3 2 
3 3 2 0 1 1 2 1 0 1 1 3 2 1 1 2 0 0 3 2 
2 1 1 1 0 0 0 3 3 2 0 2 0 0 1 1 2 2 3 2 
2 2 1 0 0 3 2 1 0 3 0 3 0 3 0 0 2 0 1 3 
3 2 0 2 2 0 3 1 0 0 2 0 0 1 0 2 2 1 0 0 
3 2 0 1 1 2 3 3 3 1 0 0 1 2 3 3 0 0 3 3 
0 0 2 3 0 3 2 1 0 1 2 2 0 1 2 3 3 3 0 0 
2 1 0 2 2 0 2 1 3 1 0 3 1 2 0 1 0 1 1 1 
0 3 2 2 0 0 0 0 3 0 0 0 1 0 3 0 1 2 1 1 
0 1 1 3 2 0 3 0 1 0 0 0 2 2 3 0 0 2 0 -1 
0 0 0 1 0 3 1 2 1 3 1 2 3 2 0 1 3 0 0 1 
0 1 1 3 0 0 0 0 1 0 0 3 2 2 1 0 0 0 3 1 
2 0 0 3 0 3 1 2 1 2 1 1 0 0 2 1 2 0 1 1 
1 0 2 0 3 2 1 3 2 1 0 1 0 0 3 3 0 3 2 1 
1 3 1 2 0 0 0 2 1 3 1 1 1 3 2 3 1 0 1 0 
3 1 3 1 0 1 0 2 1 1 0 1 3 2 1 1 0 0 2 0 
1 0 3 1 0 2 0 0 0 2 2 2 0 3 1 2 1 0 2 1 
0 3 3 1 2 0 -1 3 0 2 0 0 0 2 1 2 1 1 2 0 
1 3 3 3 1 0 0 2 2 2 0 0 0 3 2 2 2 1 0 2 
0 0 0 3 2 0 3 1 3 0 1 0 3 1 1 0 0 2 1 2 
0 0 0 3 2 3 3 0 2 1 2 2 0 0 3 3 3 0 1 2 
*/ 
