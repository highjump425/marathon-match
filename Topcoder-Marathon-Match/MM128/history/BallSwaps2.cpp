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
const double TIME_LIMIT = 9950;
double last_update = 0, t_diff;
const ll T0 = 2000, T1 = 200;
int N, C;
VV<int> grid,goal,temp;
V<int> color;
V<tuple<int,int,int,int>> best_ans,ans;

mt19937 rng;
uniform_real_distribution<> dist(0.0, 1.0);
uniform_int_distribution<> init(0, 7);

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

signed main() {
    cin.tie(0);
    ios::sync_with_stdio(false);
    cout << fixed << setprecision(15);
    t_start = chrono::system_clock::now();

    //input
    cin >> N >> C;
    grid = VV<int>(N, V<int>(N));
    color = V<int>(C);
    ans.reserve(N * N * N);
    REP(i, N)REP(j, N) {
        cin >> grid[i][j];
        color[grid[i][j]]++;
    }

    get_time();

    goal=VV<int>(N, V<int>(N,-1));
    V<int> color_order(C);
    REP(i, C)color_order[i] = i;
    int sum = 0;
    /*
    REP(c, C) {
        REP(_, color[c]) {
            ll x, y;
            x = sum / N;
            if (x % 2 == 0)y = sum % N;
            else y = N - 1 - (sum % N);
            goal[x][y] = c;
            sum++;
        }
    }
    */
    do {
        if (t_diff > TIME_LIMIT)break;
        ll x = 0, y = 0;
        ll mode = 0;
        ans = V<tuple<int, int, int, int>>();
        goal = VV<int>(N, V<int>(N, -1));
        temp = grid;
        for(int c:color_order) {
            REP(_, color[c]) {
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

        /*
        VV<int> check(N, V<int>(N));
        x = 0, y = 0;
        mode = 0;
        REP(_, N* N) {
            check[x][y] = 1;
            if (temp[x][y] != goal[x][y]) {
                int tx, ty;
                int min_dis = INF;
                REP(i, N)REP(j, N) {
                    if (check[i][j])continue;
                    if (temp[i][j] != goal[x][y])continue;
                    if (chmin(min_dis, abs(x - i) + abs(y - j))) {
                        tx = i; ty = j;
                    }
                }
                if (mode == 0 || mode == 2) {
                    while (ty != y) {
                        if (ty > y) {
                            ans.emplace_back(make_tuple(tx, ty, tx, ty - 1));
                            swap(temp[tx][ty], temp[tx][ty - 1]);
                            ty--;
                        }
                        else {
                            ans.emplace_back(make_tuple(tx, ty, tx, ty + 1));
                            swap(temp[tx][ty], temp[tx][ty + 1]);
                            ty++;
                        }
                    }
                }

                while (x != tx) {
                    if (tx > x) {
                        ans.emplace_back(make_tuple(tx, ty, tx - 1, ty));
                        swap(temp[tx][ty], temp[tx - 1][ty]);
                        tx--;
                    }
                    else {
                        ans.emplace_back(make_tuple(tx, ty, tx + 1, ty));
                        swap(temp[tx][ty], temp[tx + 1][ty]);
                        tx++;
                    }
                }
                if (mode == 1 || mode == 3) {
                    while (ty != y) {
                        if (ty > y) {
                            ans.emplace_back(make_tuple(tx, ty, tx, ty - 1));
                            swap(temp[tx][ty], temp[tx][ty - 1]);
                            ty--;
                        }
                        else {
                            ans.emplace_back(make_tuple(tx, ty, tx, ty + 1));
                            swap(temp[tx][ty], temp[tx][ty + 1]);
                            ty++;
                        }
                    }
                }

            }
            if (mode == 0) {
                if (y < N - 1 && check[x][y + 1] == 0)y++;
                else {
                    mode = 1;
                    x++;
                }
            }
            else if (mode == 1) {
                if (x < N - 1 && check[x + 1][y] == 0)x++;
                else {
                    mode = 2;
                    y--;
                }
            }
            else if (mode == 2) {
                if (y > 0 && check[x][y - 1] == 0)y--;
                else {
                    mode = 3;
                    x--;
                }
            }
            else if (mode == 3) {
                if (x > 0 && check[x - 1][y] == 0)x--;
                else {
                    mode = 0;
                    y++;
                }
            }
        }
        */
        REP(x, N)REP(y, N) {
            if (temp[x][y] == goal[x][y])continue;
            int tx, ty;
            int min_dis = INF;
            REP(i, N)REP(j, N) {
                if (i < x || (i == x && j <= y))continue;
                if (temp[i][j] != goal[x][y])continue;
                if (chmin(min_dis, abs(x - i) + abs(y - j))) {
                    tx = i; ty = j;
                }
            }
            while (ty != y) {
                if (ty > y) {
                    ans.emplace_back(make_tuple(tx, ty, tx, ty - 1));
                    swap(temp[tx][ty], temp[tx][ty - 1]);
                    ty--;
                }
                else {
                    ans.emplace_back(make_tuple(tx, ty, tx, ty + 1));
                    swap(temp[tx][ty], temp[tx][ty + 1]);
                    ty++;
                }
            }
            while (x != tx) {
                if (tx > x) {
                    ans.emplace_back(make_tuple(tx, ty, tx - 1, ty));
                    swap(temp[tx][ty], temp[tx - 1][ty]);
                    tx--;
                }
                else {
                    ans.emplace_back(make_tuple(tx, ty, tx + 1, ty));
                    swap(temp[tx][ty], temp[tx + 1][ty]);
                    tx++;
                }
            }
        }
        if (SZ(best_ans) == 0 || SZ(best_ans) > SZ(ans))best_ans.swap(ans);
        get_time();
    }
    while (next_permutation(ALL(color_order)));

    cout << SZ(best_ans) << endl;
    REP(i, SZ(best_ans)) {
        cout << get<0>(best_ans[i]) << " " << get<1>(best_ans[i]) << " " 
             << get<2>(best_ans[i]) << " " << get<3>(best_ans[i]) << endl;
    }

    /*
    cerr << "time:  " << t_diff << endl;
    cerr << "last:  " << last_update << endl;
    cerr << "cnt:   " << cnt << endl;
    cerr << "update:" << update_cnt << endl;
    cerr << "score: " << best_score << endl;
    */

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
const double TIME_LIMIT = 9950;
double last_update = 0, t_diff;
const ll T0 = 2000, T1 = 200;
int N, C;
VV<int> grid,goal,temp;
V<int> color;
V<tuple<int,int,int,int>> best_ans,ans;

mt19937 rng;
uniform_real_distribution<> dist(0.0, 1.0);
uniform_int_distribution<> init(0, 7);

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

signed main() {
    cin.tie(0);
    ios::sync_with_stdio(false);
    cout << fixed << setprecision(15);
    t_start = chrono::system_clock::now();

    //input
    cin >> N >> C;
    grid = VV<int>(N, V<int>(N));
    color = V<int>(C);
    ans.reserve(N * N * N);
    REP(i, N)REP(j, N) {
        cin >> grid[i][j];
        color[grid[i][j]]++;
    }

    get_time();

    goal=VV<int>(N, V<int>(N,-1));
    V<int> color_order(C);
    REP(i, C)color_order[i] = i;
    int sum = 0;
    /*
    REP(c, C) {
        REP(_, color[c]) {
            ll x, y;
            x = sum / N;
            if (x % 2 == 0)y = sum % N;
            else y = N - 1 - (sum % N);
            goal[x][y] = c;
            sum++;
        }
    }
    */
    do {
        if (t_diff > TIME_LIMIT)break;
        ll x = 0, y = 0;
        ll mode = 0;
        ans = V<tuple<int, int, int, int>>();
        goal = VV<int>(N, V<int>(N, -1));
        temp = grid;
        for(int c:color_order) {
            REP(_, color[c]) {
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

        /*
        VV<int> check(N, V<int>(N));
        x = 0, y = 0;
        mode = 0;
        REP(_, N* N) {
            check[x][y] = 1;
            if (temp[x][y] != goal[x][y]) {
                int tx, ty;
                int min_dis = INF;
                REP(i, N)REP(j, N) {
                    if (check[i][j])continue;
                    if (temp[i][j] != goal[x][y])continue;
                    if (chmin(min_dis, abs(x - i) + abs(y - j))) {
                        tx = i; ty = j;
                    }
                }
                if (mode == 0 || mode == 2) {
                    while (ty != y) {
                        if (ty > y) {
                            ans.emplace_back(make_tuple(tx, ty, tx, ty - 1));
                            swap(temp[tx][ty], temp[tx][ty - 1]);
                            ty--;
                        }
                        else {
                            ans.emplace_back(make_tuple(tx, ty, tx, ty + 1));
                            swap(temp[tx][ty], temp[tx][ty + 1]);
                            ty++;
                        }
                    }
                }

                while (x != tx) {
                    if (tx > x) {
                        ans.emplace_back(make_tuple(tx, ty, tx - 1, ty));
                        swap(temp[tx][ty], temp[tx - 1][ty]);
                        tx--;
                    }
                    else {
                        ans.emplace_back(make_tuple(tx, ty, tx + 1, ty));
                        swap(temp[tx][ty], temp[tx + 1][ty]);
                        tx++;
                    }
                }
                if (mode == 1 || mode == 3) {
                    while (ty != y) {
                        if (ty > y) {
                            ans.emplace_back(make_tuple(tx, ty, tx, ty - 1));
                            swap(temp[tx][ty], temp[tx][ty - 1]);
                            ty--;
                        }
                        else {
                            ans.emplace_back(make_tuple(tx, ty, tx, ty + 1));
                            swap(temp[tx][ty], temp[tx][ty + 1]);
                            ty++;
                        }
                    }
                }

            }
            if (mode == 0) {
                if (y < N - 1 && check[x][y + 1] == 0)y++;
                else {
                    mode = 1;
                    x++;
                }
            }
            else if (mode == 1) {
                if (x < N - 1 && check[x + 1][y] == 0)x++;
                else {
                    mode = 2;
                    y--;
                }
            }
            else if (mode == 2) {
                if (y > 0 && check[x][y - 1] == 0)y--;
                else {
                    mode = 3;
                    x--;
                }
            }
            else if (mode == 3) {
                if (x > 0 && check[x - 1][y] == 0)x--;
                else {
                    mode = 0;
                    y++;
                }
            }
        }
        */
        REP(x, N)REP(y, N) {
            if (temp[x][y] == goal[x][y])continue;
            int tx, ty;
            int min_dis = INF;
            REP(i, N)REP(j, N) {
                if (i < x || (i == x && j <= y))continue;
                if (temp[i][j] != goal[x][y])continue;
                if (chmin(min_dis, abs(x - i) + abs(y - j))) {
                    tx = i; ty = j;
                }
            }
            while (ty != y) {
                if (ty > y) {
                    ans.emplace_back(make_tuple(tx, ty, tx, ty - 1));
                    swap(temp[tx][ty], temp[tx][ty - 1]);
                    ty--;
                }
                else {
                    ans.emplace_back(make_tuple(tx, ty, tx, ty + 1));
                    swap(temp[tx][ty], temp[tx][ty + 1]);
                    ty++;
                }
            }
            while (x != tx) {
                if (tx > x) {
                    ans.emplace_back(make_tuple(tx, ty, tx - 1, ty));
                    swap(temp[tx][ty], temp[tx - 1][ty]);
                    tx--;
                }
                else {
                    ans.emplace_back(make_tuple(tx, ty, tx + 1, ty));
                    swap(temp[tx][ty], temp[tx + 1][ty]);
                    tx++;
                }
            }
        }
        if (SZ(best_ans) == 0 || SZ(best_ans) > SZ(ans))best_ans.swap(ans);
        get_time();
    }
    while (next_permutation(ALL(color_order)));

    cout << SZ(best_ans) << endl;
    REP(i, SZ(best_ans)) {
        cout << get<0>(best_ans[i]) << " " << get<1>(best_ans[i]) << " " 
             << get<2>(best_ans[i]) << " " << get<3>(best_ans[i]) << endl;
    }

    /*
    cerr << "time:  " << t_diff << endl;
    cerr << "last:  " << last_update << endl;
    cerr << "cnt:   " << cnt << endl;
    cerr << "update:" << update_cnt << endl;
    cerr << "score: " << best_score << endl;
    */

    return 0;
}
>>>>>>> origin/main
