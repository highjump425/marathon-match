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
const double TIME_LIMIT = 9950;
double last_update = 0, t_diff;
const ll T0 = 1, T1 = 2;
int N, C,score,best_score,cnt=0,update_cnt;
VV<int> grid,best_goal,goal,temp,check;
V<int> color_map;
V<P> position,temp_pos;
V<P> color;
V<tuple<int,int,int,int>> best_ans,ans;
map<P, P> destination;
V<ll> y_vec = { 0, 1,0, -1 };
V<ll> x_vec = { -1, 0,1, 0 };

mt19937 rng;
uniform_real_distribution<> dist(0.0, 1.0);
uniform_int_distribution<> rand_int(0, 899);

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

void init() {
    sort(ALL(color), greater<P>());
    ll x = 0, y = 0;
    ll mode = 0;
    score = 0;
    ans = V<tuple<int, int, int, int>>();
    goal = VV<int>(N, V<int>(N, -1));
    temp_pos = position;
    temp = grid;
    for (P p : color) {
        int c = p.second;
        REP(_, p.first) {
            goal[x][y] = c;
            if (mode == 0) {
                if (y < N - 1 && goal[x][y + 1] == -1)y++;
                else {
                    mode = 1;
                    x++;
                }
            }
            else if (mode == 1) {
                if (x < N - 1 && goal[x + 1][y] == -1)x++;
                else {
                    mode = 2;
                    y--;
                }
            }
            else if (mode == 2) {
                if (y > 0 && goal[x][y - 1] == -1)y--;
                else {
                    mode = 3;
                    x--;
                }
            }
            else if (mode == 3) {
                if (x > 0 && goal[x - 1][y] == -1)x--;
                else {
                    mode = 0;
                    y++;
                }
            }
        }
    }

    REP(x, N)REP(y, N) {
        if (color_map[temp[x][y]] == goal[x][y]) {
            goal[x][y] = temp[x][y];
            continue;
        }
        int tx, ty;
        int min_dis = INF;
        REP(i, N)REP(j, N) {
            if (i < x || (i == x && j <= y))continue;
            if (color_map[temp[i][j]] != goal[x][y])continue;
            if (chmin(min_dis, abs(x - i) + abs(y - j))) {
                tx = i; ty = j;
            }
        }
        goal[x][y] = temp[tx][ty];
        while (ty != y) {
            if (ty > y) {
                score++;
                swap(temp_pos[temp[tx][ty]], temp_pos[temp[tx][ty - 1]]);
                swap(temp[tx][ty], temp[tx][ty - 1]);
                ty--;
            }
            else {
                score++;
                swap(temp_pos[temp[tx][ty]], temp_pos[temp[tx][ty + 1]]);
                swap(temp[tx][ty], temp[tx][ty + 1]);
                ty++;
            }
        }
        while (x != tx) {
            if (tx > x) {
                score++;
                swap(temp_pos[temp[tx][ty]], temp_pos[temp[tx-1][ty]]);
                swap(temp[tx][ty], temp[tx - 1][ty]);
                tx--;
            }
            else {
                score++;
                swap(temp_pos[temp[tx][ty]], temp_pos[temp[tx-1][ty]]);
                swap(temp[tx][ty], temp[tx + 1][ty]);
                tx++;
            }
        }
    }
    if (best_score == 0 || best_score > score) {
        //cerr << score << endl;
        best_score = score;
        best_goal = goal;
    }

    get_time();
}

void new_init() {
    V<int> color_order(C);
    REP(i, C)color_order[i] = i;

    do {
        if (t_diff > TIME_LIMIT)break;
        ll x = 0, y = 0;
        ll mode = 0;
        score = 0;
        goal = VV<int>(N, V<int>(N, -1));
        temp_pos = position;
        temp = grid;
        for (int c : color_order) {
            REP(_, color[c].first) {
                goal[x][y] = c;
                if (mode == 0) {
                    if (y < N - 1 && goal[x][y + 1] == -1)y++;
                    else {
                        mode = 1;
                        x++;
                    }
                }
                else if (mode == 1) {
                    if (x < N - 1 && goal[x + 1][y] == -1)x++;
                    else {
                        mode = 2;
                        y--;
                    }
                }
                else if (mode == 2) {
                    if (y > 0 && goal[x][y - 1] == -1)y--;
                    else {
                        mode = 3;
                        x--;
                    }
                }
                else if (mode == 3) {
                    if (x > 0 && goal[x - 1][y] == -1)x--;
                    else {
                        mode = 0;
                        y++;
                    }
                }
            }
        }

        REP(x, N)REP(y, N) {
            if (color_map[temp[x][y]] == goal[x][y]) {
                goal[x][y] = temp[x][y];
                continue;
            }
            int tx, ty;
            int min_dis = INF;
            REP(i, N)REP(j, N) {
                if (i < x || (i == x && j <= y))continue;
                if (color_map[temp[i][j]] != goal[x][y])continue;
                if (chmin(min_dis, abs(x - i) + abs(y - j))) {
                    tx = i; ty = j;
                }
            }
            goal[x][y] = temp[tx][ty];
            while (ty != y) {
                if (ty > y) {
                    score++;
                    swap(temp_pos[temp[tx][ty]], temp_pos[temp[tx][ty - 1]]);
                    swap(temp[tx][ty], temp[tx][ty - 1]);
                    ty--;
                }
                else {
                    score++;
                    swap(temp_pos[temp[tx][ty]], temp_pos[temp[tx][ty + 1]]);
                    swap(temp[tx][ty], temp[tx][ty + 1]);
                    ty++;
                }
            }
            while (x != tx) {
                if (tx > x) {
                    score++;
                    swap(temp_pos[temp[tx][ty]], temp_pos[temp[tx - 1][ty]]);
                    swap(temp[tx][ty], temp[tx - 1][ty]);
                    tx--;
                }
                else {
                    score++;
                    swap(temp_pos[temp[tx][ty]], temp_pos[temp[tx - 1][ty]]);
                    swap(temp[tx][ty], temp[tx + 1][ty]);
                    tx++;
                }
            }
        }
        if (best_score == 0 || best_score > score) {
            //cerr << score << endl;
            best_score = score;
            best_goal = goal;
        }
        get_time();
    } while (next_permutation(ALL(color_order)));

    goal = best_goal;
}

int get_score() {
    temp = grid;
    temp_pos = position;
    int res = 0;

    REP(x, N)REP(y, N) {
        int tx, ty; tie(tx, ty) = temp_pos[goal[x][y]];
        while (ty != y) {
            if (ty > y) {
                res++;
                swap(temp_pos[temp[tx][ty]], temp_pos[temp[tx][ty - 1]]);
                swap(temp[tx][ty], temp[tx][ty - 1]);
                ty--;
            }
            else {
                res++;
                swap(temp_pos[temp[tx][ty]], temp_pos[temp[tx][ty + 1]]);
                swap(temp[tx][ty], temp[tx][ty + 1]);
                ty++;
            }
        }
        while (x != tx) {
            if (tx > x) {
                res++;
                swap(temp_pos[temp[tx][ty]], temp_pos[temp[tx - 1][ty]]);
                swap(temp[tx][ty], temp[tx - 1][ty]);
                tx--;
            }
            else {
                res++;
                swap(temp_pos[temp[tx][ty]], temp_pos[temp[tx + 1][ty]]);
                swap(temp[tx][ty], temp[tx + 1][ty]);
                tx++;
            }
        }
    }
    return res;
}

void check_dfs(int x, int y) {
    check[x][y] = 1;
    REP(i, 4) {
        int nx = x + x_vec[i];
        int ny = y + y_vec[i];
        if (nx < 0 || nx >= N || ny < 0 || ny >= N || check[nx][ny])continue;
        if (color_map[goal[x][y]] != color_map[goal[nx][ny]])continue;
        check_dfs(nx, ny);
    }
}

bool check_connect() {
    check = VV<int>(N, V<int>(N));
    int num = 0;
    REP(i, N)REP(j, N) {
        if (!check[i][j]) {
            check_dfs(i, j);
            num++;
        }
        if (num > C)return false;
    }
    return true;
}

void change_connect() {
    uniform_int_distribution<> rand_int(0, N - 1);
    int x1 = rand_int(rng), x2 = rand_int(rng), y1 = rand_int(rng), y2 = rand_int(rng);
    if (goal[x1][y1] == goal[x2][y2] || color_map[goal[x1][y1]] != color_map[goal[x2][y2]])return;

    swap(goal[x1][y1], goal[x2][y2]);
    score = get_score();
    if (chmin(best_score, score)) {
        best_goal = goal;
        update_cnt++;
        last_update = t_diff;
    }
    else {
        swap(goal[x1][y1], goal[x2][y2]);
    }
    cnt++;
}

void change_place() {
    uniform_int_distribution<> rand_int(0, N - 1);
    int x1 = rand_int(rng), x2 = rand_int(rng), y1 = rand_int(rng), y2 = rand_int(rng);
    if (goal[x1][y1] == goal[x2][y2] || color_map[goal[x1][y1]] == color_map[goal[x2][y2]])return;

    swap(goal[x1][y1], goal[x2][y2]);

    if (check_connect()) {
        score = get_score();
        double prob = exp((score - best_score) / get_temp());
        if (prob > dist(rng)) {
            if (chmin(best_score, score)) {
                best_goal = goal;
                update_cnt++;
                last_update = t_diff;
            }
        }
        /*
        else {
            swap(goal[x1][y1], goal[x2][y2]);
        }
        */
    }
    else {
        swap(goal[x1][y1], goal[x2][y2]);
        change_place();
    }
    cnt++;
}

void output() {
    ans = V<tuple<int, int, int, int>>();
    temp = grid;
    temp_pos = position;

    REP(x, N)REP(y, N) {
        int tx, ty; tie(tx, ty) = temp_pos[best_goal[x][y]];
        while (ty != y) {
            if (ty > y) {
                ans.emplace_back(make_tuple(tx, ty, tx, ty - 1));
                swap(temp_pos[temp[tx][ty]], temp_pos[temp[tx][ty - 1]]);
                swap(temp[tx][ty], temp[tx][ty - 1]);
                ty--;
            }
            else {
                ans.emplace_back(make_tuple(tx, ty, tx, ty + 1));
                swap(temp_pos[temp[tx][ty]], temp_pos[temp[tx][ty + 1]]);
                swap(temp[tx][ty], temp[tx][ty + 1]);
                ty++;
            }
        }
        while (x != tx) {
            if (tx > x) {
                ans.emplace_back(make_tuple(tx, ty, tx - 1, ty));
                swap(temp_pos[temp[tx][ty]], temp_pos[temp[tx-1][ty]]);
                swap(temp[tx][ty], temp[tx - 1][ty]);
                tx--;
            }
            else {
                ans.emplace_back(make_tuple(tx, ty, tx + 1, ty));
                swap(temp_pos[temp[tx][ty]], temp_pos[temp[tx + 1][ty]]);
                swap(temp[tx][ty], temp[tx + 1][ty]);
                tx++;
            }
        }
    }

    cout << SZ(ans) << endl;
    REP(i, SZ(ans)) {
        cout << get<0>(ans[i]) << " " << get<1>(ans[i]) << " "
            << get<2>(ans[i]) << " " << get<3>(ans[i]) << endl;
    }
}

signed main() {
    cin.tie(0);
    ios::sync_with_stdio(false);
    cout << fixed << setprecision(15);
    t_start = chrono::system_clock::now();

    //input
    cin >> N >> C;
    grid = VV<int>(N, V<int>(N));
    check = VV<int>(N, V<int>(N));
    goal = VV<int>(N, V<int>(N, -1));
    color_map = V<int>(N * N);
    position = V<P>(N * N);
    color = V<P>(C);
    ans.reserve(N * N * N);
    REP(i, N)REP(j, N) {
        int pos = i * N + j;
        int col; cin >> col;
        color_map[pos] = col;
        grid[i][j] = pos;
        position[pos] = MP(i, j);
        color[col].first++;
        color[col].second = col;
    }

    get_time();
    new_init();
    
    while (t_diff < TIME_LIMIT) {
        if(cnt%(N*50)==0)change_place();
        change_connect();
        get_time();
    }
    output();

    
    cerr << "time:  " << t_diff << endl;
    cerr << "last:  " << last_update << endl;
    cerr << "cnt:   " << cnt << endl;
    cerr << "update:" << update_cnt << endl;
    /*
    cerr << "score: " << best_score << endl;
    */

    return 0;
}
