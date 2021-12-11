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

#define REP(i,n) for(int i=0;i<ll(n);i++)
#define FOR(i,a,b) for(int i=ll(a);i<=ll(b);i++)
#define FORD(i,a,b) for(int i=ll(a);i>=ll(b);i--)
#define MP make_pair
#define SZ(x) int(x.size())
#define ALL(x) x.begin(),x.end()
#define INF 10001000
#define endl "\n"

mt19937 rng;
ll k;
VV<pair<double, int>> retsu(30, V<pair<double, int>>(30, MP(4008, 1))), gyou(30, V<pair<double, int>>(30, MP(4008, 1)));
VV<pair<double, string>> grid;
VV<bool> check;
priority_queue < pair<double, P>, V<pair<double, P>>, greater<pair<double, P>>> q;
V<P> start, goal;
V<string> answer;
V<int> point;
int sx, sy, tx, ty;
bool go = true;

void get_distance() {
    cerr << std::right << std::setw(2) << -1 << "  ";
    REP(j, 29)cerr << std::right << std::setw(4) << j << " ";
    cerr << endl;
    cerr << endl;
    REP(i, 30) {
        cerr << std::right << std::setw(2) << i << "  ";
        REP(j, 29)cerr << std::right << std::setw(4) << int(gyou[i][j].first) << " ";
        cerr << endl;
    }
    cerr << endl;
    cerr << std::right << std::setw(2) << -1 << "  ";
    REP(j, 30)cerr << std::right << std::setw(4) << j << " ";
    cerr << endl;
    cerr << endl;
    REP(i, 29) {
        cerr << std::right << std::setw(2) << i << "  ";
        REP(j, 30)cerr << std::right << std::setw(4) << int(retsu[i][j].first) << " ";
        cerr << endl;
    }
}

void get_frequency() {
    cerr << std::right << std::setw(2) << -1 << "  ";
    REP(j, 29)cerr << std::right << std::setw(3) << j << " ";
    cerr << endl;
    cerr << endl;
    REP(i, 30) {
        cerr << std::right << std::setw(2) << i << "  ";
        REP(j, 29)cerr << std::right << std::setw(3) << int(gyou[i][j].second) << " ";
        cerr << endl;
    }
    cerr << endl;
    cerr << std::right << std::setw(2) << -1 << "  ";
    REP(j, 30)cerr << std::right << std::setw(3) << j << " ";
    cerr << endl;
    cerr << endl;
    REP(i, 29) {
        cerr << std::right << std::setw(2) << i << "  ";
        REP(j, 30)cerr << std::right << std::setw(3) << int(retsu[i][j].second) << " ";
        cerr << endl;
    }
}

void daiku() {
    grid = VV<pair<double, string>>(30, V<pair<double, string>>(30, MP(INF, "")));
    check = VV<bool>(30, V<bool>(30, false));
    q = priority_queue < pair<double, P>, V<pair<double, P>>, greater<pair<double, P>>>();
    grid[sx][sy] = MP(0, "");
    q.push(MP(0, MP(sx, sy)));
    while (!q.empty()) {
        auto p = q.top();
        q.pop();
        double dis = p.first;
        int x = p.second.first, y = p.second.second;
        if (check[x][y] || grid[x][y].first < dis)continue;
        if (0 < x) {
            if (!check[x - 1][y]) {
                if (chmin(grid[x - 1][y].first, grid[x][y].first + retsu[x - 1][y].first)) {
                    grid[x - 1][y].second = grid[x][y].second + 'U';
                    q.push(MP(grid[x - 1][y].first, MP(x - 1, y)));
                }
            }
        }
        if (x < 29) {
            if (!check[x + 1][y]) {
                if (chmin(grid[x + 1][y].first, grid[x][y].first + retsu[x][y].first)) {
                    grid[x + 1][y].second = grid[x][y].second + 'D';
                    q.push(MP(grid[x + 1][y].first, MP(x + 1, y)));
                }
            }
        }
        if (0 < y) {
            if (!check[x][y - 1]) {
                if (chmin(grid[x][y - 1].first, grid[x][y].first + gyou[x][y - 1].first)) {
                    grid[x][y - 1].second = grid[x][y].second + 'L';
                    q.push(MP(grid[x][y - 1].first, MP(x, y - 1)));
                }
            }
        }
        if (y < 29) {
            if (!check[x][y + 1]) {
                if (chmin(grid[x][y + 1].first, grid[x][y].first + gyou[x][y].first)) {
                    grid[x][y + 1].second = grid[x][y].second + 'R';
                    q.push(MP(grid[x][y + 1].first, MP(x, y + 1)));
                }
            }
        }
        check[x][y] = true;
    }
}

void repeat(int i, double rate) {
    int x, y;
    tie(x, y) = start[i];
    string ans = answer[i];
    double score = point[i];
    double sum = 0;
    for (char c : ans) {
        if (c == 'U' || c == 'D') {
            if (c == 'U')x -= 1;
            sum += retsu[x][y].first;
            if (c == 'D')x += 1;
        }
        else if (c == 'L' || c == 'R') {
            if (c == 'L')y -= 1;
            sum += gyou[x][y].first;
            if (c == 'R')y += 1;
        }
    }
    tie(x, y) = start[i];
    REP(j, SZ(ans)) {
        char c = ans[j];
        if (c == 'U' || c == 'D') {
            if (c == 'U')x -= 1;
            retsu[x][y].first = (1.0 * retsu[x][y].first + rate * score * retsu[x][y].first / sum) / (1.0 + rate);
            if (c == 'D')x += 1;
        }
        else if (c == 'L' || c == 'R') {
            if (c == 'L')y -= 1;
            gyou[x][y].first = (1.0 * gyou[x][y].first + rate * score * gyou[x][y].first / sum) / (1.0 + rate);
            if (c == 'R')y += 1;
        }
    }
}

void line(int i, double rate, double rate2) {
    int x, y;
    tie(x, y) = start[i];
    string ans = answer[i];
    double score = point[i];
    double sum = 0;
    for (char c : ans) {
        if (c == 'U' || c == 'D') {
            if (c == 'U')x -= 1;
            sum += retsu[x][y].first;
            if (c == 'D')x += 1;
        }
        else if (c == 'L' || c == 'R') {
            if (c == 'L')y -= 1;
            sum += gyou[x][y].first;
            if (c == 'R')y += 1;
        }
    }
    tie(x, y) = start[i];

    REP(j, SZ(ans)) {
        char c = ans[j];
        if (c == 'U' || c == 'D') {
            if (c == 'U')x -= 1;
            retsu[x][y].first = (1.0 * retsu[x][y].first + rate * score * retsu[x][y].first / sum) / (1.0 + rate);
            if (c == 'D')x += 1;
        }
        else if (c == 'L' || c == 'R') {
            if (c == 'L')y -= 1;
            gyou[x][y].first = (1.0 * gyou[x][y].first + rate * score * gyou[x][y].first / sum) / (1.0 + rate);
            if (c == 'R')y += 1;
        }
    }

    tie(x, y) = start[i];
    for (char c : ans) {
        if (c == 'U' || c == 'D') {
            if (c == 'U')x -= 1;
            if (go) {
                REP(j, 29)retsu[j][y].first = (1.0 * retsu[j][y].first + 2 * rate2 * retsu[x][y].first) / (1.0 + 2 * rate2);
            }
            else {
                if (x <= 14)REP(j, 15)retsu[j][y].first = (1.0 * retsu[j][y].first + rate2 * retsu[x][y].first) / (1.0 + rate2);
                else FOR(j, 14, 28)retsu[j][y].first = (1.0 * retsu[j][y].first + rate2 * retsu[x][y].first) / (1.0 + rate2);
                rate2 *= 1;
                if (x <= 9)REP(j, 10)retsu[j][y].first = (1.0 * retsu[j][y].first + rate2 * retsu[x][y].first) / (1.0 + rate2);
                else if (x <= 18) FOR(j, 10, 18)retsu[j][y].first = (1.0 * retsu[j][y].first + rate2 * retsu[x][y].first) / (1.0 + rate2);
                else FOR(j, 19, 28)retsu[j][y].first = (1.0 * retsu[j][y].first + rate2 * retsu[x][y].first) / (1.0 + rate2);
            }
            REP(j, 29)retsu[j][y].first = max(min(9000.0, retsu[j][y].first), 1000.0);
            if (c == 'D')x += 1;
        }
        else if (c == 'L' || c == 'R') {
            if (c == 'L')y -= 1;
            if (go) {
                REP(j, 29)gyou[x][j].first = (1.0 * gyou[x][j].first + 2 * rate2 * gyou[x][y].first) / (1.0 + 2 * rate2);
            }
            else {
                if (y <= 14)REP(j, 15)gyou[x][j].first = (1.0 * gyou[x][j].first + rate2 * gyou[x][y].first) / (1.0 + rate2);
                else FOR(j, 14, 28)gyou[x][j].first = (1.0 * gyou[x][j].first + rate2 * gyou[x][y].first) / (1.0 + rate2);
                rate2 *= 1;
                if (y <= 9)REP(j, 10)gyou[x][j].first = (1.0 * gyou[x][j].first + rate2 * gyou[x][y].first) / (1.0 + rate2);
                else if (y <= 18) FOR(j, 10, 18)gyou[x][j].first = (1.0 * gyou[x][j].first + rate2 * gyou[x][y].first) / (1.0 + rate2);
                else FOR(j, 19, 28)gyou[x][j].first = (1.0 * gyou[x][j].first + rate2 * gyou[x][y].first) / (1.0 + rate2);
            }
            REP(j, 29)gyou[x][j].first = max(min(9000.0, gyou[x][j].first), 1000.0);
            if (c == 'R')y += 1;
        }
    }

}

void gyouretsu() {
    cin >> sx >> sy >> tx >> ty;
    start.emplace_back(sx, sy);
    goal.emplace_back(tx, ty);
    string ans = "";
    daiku();
    double sum = grid[tx][ty].first;
    ans = grid[tx][ty].second;
    answer.push_back(ans);
    cout << ans << endl;
    cout.flush();
    double score; cin >> score;
    point.push_back(score);
    line(SZ(answer) - 1, 2.0, 0.2);
    if (k < 801)REP(i, 40)line(uniform_int_distribution<int>(0, SZ(answer) - 1)(rng), 1.0, 0.03);
    if (k < 1001)REP(i, max((1001 - k) / 3, 200ll))repeat(uniform_int_distribution<int>(0, SZ(answer) - 1)(rng), 1.0);
    
    REP(l, 30)REP(j, 30) {
        retsu[l][j].first = max(min(9000.0, retsu[l][j].first), 1000.0);
        gyou[l][j].first = max(min(9000.0, gyou[l][j].first), 1000.0);
    }

    int m_cnt = 0;
    REP(l, 30) {
        double r_mi = INF, r_ma = -1,g_mi = INF, g_ma = -1;
        REP(j, 29) {
            chmin(r_mi, retsu[j][l].first);
            chmax(r_ma, retsu[j][l].first);
            chmin(g_mi, gyou[l][j].first);
            chmax(g_ma, gyou[l][j].first);
        }
        if (r_ma - r_mi > 3000)m_cnt++;
        if (g_ma - g_mi > 3000)m_cnt++;
    }

    if (m_cnt > 15)go = false;
    else go = true;

    return;
}

signed main() {
    cin.tie(0);
    ios::sync_with_stdio(false);
    cout << fixed << setprecision(15);

    k = 1000;

    while (k) {
        gyouretsu();
        k--;
    }
    //get_distance();
    //get_frequency();

    return 0;
}
