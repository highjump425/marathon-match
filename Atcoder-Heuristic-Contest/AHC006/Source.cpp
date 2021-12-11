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
const double TIME_LIMIT = 1950;
double last_update = 0, t_diff;
double start_temp, end_temp;

mt19937 rng;
using uni_int = uniform_int_distribution<>;
using uni_real = uniform_real_distribution<>;
//uni_real dist(0.0, 1.0);
//uni_int dist_int(0, 7);

/*
double prob = exp((next_score-current_score) / get_temp());
if (prob > dist(rng)) {
*/

V<int> y_vec = { 0, 1,0, -1 };
V<int> x_vec = { -1, 0,1, 0 };

void get_time() {
    auto t_now = chrono::system_clock::now();
    t_diff = chrono::duration_cast<chrono::milliseconds>(t_now - t_start).count();
}

//double get_temp() {
//    start_temp = 10000; end_temp = 0.1;
//    return start_temp + (end_temp - start_temp) * ((t_diff - TIME_LIMIT1) / (TIME_LIMIT - TIME_LIMIT1));
//}

int N = 1000;
int K = 50;
int score = 0;
V<int> place;
V<P> route;

struct Pack {
    int id = 0;
    P start;
    P goal;

    Pack(int i,P s, P g) :id(i),start(s), goal(g) {};

};

//struct Cell {
//    int x = 0;
//    int y = 0;
//    V<int> Start;
//    V<int> Goal;
//    V<P> around4;
//    Cell(int X, int Y) :x(X), y(Y) {};
//};

//VV<Cell> map(800, V<Cell>(800));
V<Pack> order;

int dist(P& a, P& b) {
    return abs(a.first - b.first) + abs(a.second - b.second);
}

int get_score() {
    int length = 0;
    FOR(i, 1, SZ(route) - 1) {
        length += dist(route[i], route[i - 1]);
    }
    return length;
}

void out() {
    cout << SZ(place) << " ";
    REP(i,SZ(place))cout << place[i] + 1 <<(i==SZ(place)-1?endl:" ");
    cout << SZ(route) << " ";
    REP(i,SZ(route))cout <<route[i].first<<" "<<route[i].second<< (i == SZ(route) - 1 ? endl : " ");
}

int change(P p) {
    return p.first * 801 + p.second;
}

V<P> weight(N);
V<P> table(900 * 900, {-1,-1});
V<int> check(N);
V<bool> used(N);
int update_cnt = 0;
int cnt = 0;

bool validate() {
    check = V<int>(N);
    FOR(i, 1, SZ(route)-2) {
        P p = table[change(route[i])];
        if (p.first == 1) {
            if (check[p.second] != 0)return false;
            check[p.second] = 1;
        }
        else {
            if (check[p.second] != 1)return false;
            check[p.second] = 2;
        }
    }
    return true;
}

void two_swap() {
    uni_int dist_node(1, SZ(route)-2);
    int a = dist_node(rng);
    int b = dist_node(rng);
    while (a == b) {
        b = dist_node(rng);
    }
    if (a > b)swap(a, b);
    swap(route[a], route[b]);
    if (!validate()) {
        swap(route[a], route[b]);
        return;
    }
    int new_score = get_score();
    //cerr << score << " " << new_score << endl;
    if (chmin(score, new_score)) {
        //cerr << "Yes" << endl;
        update_cnt++;
        last_update = t_diff;
    }
    else {
        swap(route[a], route[b]);
    }
}

void reverse() {
    uni_int dist_node(1, SZ(route) - 2);
    int a = dist_node(rng);
    int b = dist_node(rng);
    while (a == b) {
        b = dist_node(rng);
    }
    if (a > b)swap(a, b);
    REP(i,(b-a + 1) / 2) {
        swap(route[a+i], route[b-i]);
    }
    if (!validate()) {
        REP(i, (b-a + 1) / 2) {
            swap(route[a + i], route[b - i]);
        }
        return;
    }
    int new_score = get_score();
    //cerr << score << " " << new_score << endl;
    if (chmin(score, new_score)) {
        //cerr << "Yes" << endl;
        update_cnt++;
        last_update = t_diff;
    }
    else {
        REP(i, (b - a + 1) / 2) {
            swap(route[a + i], route[b - i]);
        }
    }

}

void add() {
    int mi = INF;
    int num = -1;
    int a = -1;
    int b = -1;
    REP(i, N) {
        if (used[i])continue;
        if (table[change(order[i].start)].first != -1 || table[change(order[i].goal)].first != -1)continue;
        int temp = INF;
        int id = -1;
        FOR(j, 1, SZ(route)-2) {
            int d = dist(route[j - 1], order[i].start) + dist(route[j], order[i].start)
                - dist(route[j - 1], route[j]);
            if(chmin(temp, d))id = j;
        }
        int temp2 = INF;
        int id2 = -1;
        FOR(j, id+1, SZ(route)-1) {
            int d = dist(route[j - 1], order[i].goal) + dist(route[j], order[i].goal)
                - dist(route[j - 1], route[j]);
            if (chmin(temp2, d))id2 = j;
        }
        if (chmin(mi, temp + temp2)) {
            num = i;
            a = id;
            b = id2;
        }
    }
    place.push_back(num);
    used[num] = true;
    table[change(order[num].start)] = { 1,num };
    table[change(order[num].goal)] = { 2,num };
    route.insert(route.begin() + b, order[num].goal);
    route.insert(route.begin() + a, order[num].start);
}

void del() {
    V<int> cal(N);
    int temp = 0;
    int id = -1;
    FOR(i, 1, SZ(route)-2) {
        P p = table[change(route[i])];
        cal[p.second] += dist(route[i], route[i - 1]) + dist(route[i], route[i + 1])
            - dist(route[i - 1], route[i + 1]);
    }
    for (int i : place) {
        if (chmax(temp, cal[i])) {
            id = i;
        }
    }
    REP(i, SZ(place))if (place[i] == id)place.erase(place.begin() + i);
    FORD(i, 102, 1) {
        P p = table[change(route[i])];
        if (p.second == id)route.erase(route.begin() + i);
    }
    used[id] = false;
    table[change(order[id].start)] = { -1,-1 };
    table[change(order[id].goal)] = { -1,-1 };
}

signed main() {
    cin.tie(0);
    ios::sync_with_stdio(false);
    cout << fixed << setprecision(15);
    cerr << fixed << setprecision(15);
    t_start = chrono::system_clock::now();

    //REP(x, 800)REP(y, 800) {
    //    Cell cell(x, y);
    //    REP(i, 4) {
    //        int nx = x + x_vec[i];
    //        int ny = y + y_vec[i];
    //        if (nx < 0 || nx >= 800 || ny < 0 || ny >= 800)continue;
    //        cell.around4.emplace_back(nx,ny)
    //    }
    //}

    REP(i, N) {
        int a, b, c, d; cin >> a >> b >> c >> d;
        Pack p(i,MP(a, b), MP(c, d));
        order.emplace_back(p);
    }
    REP(i, N) {
        P s = MP(400, 400);
        int sd = dist(order[i].start, s);
        int gd = dist(order[i].goal, s);
        weight[i] = MP(sd * sd + gd * gd, i);
    }
    sort(ALL(weight));
    REP(i, N) {
        int j = weight[i].second;
        if (table[change(order[j].start)].first != -1 || table[change(order[j].goal)].first != -1)continue;
        table[change(order[j].start)] = MP(1, j);
        table[change(order[j].goal)] = MP(2, j);
        place.push_back(j);
        used[j] = true;
        if (SZ(place) == 50)break;
    }

    P now = MP(400, 400);
    route.emplace_back(now);
    cnt = 0;
    while (cnt < 50) {
        int mi = INF;
        P next;
        int id = -1;
        bool flag = true;
        for(int i:place){
            if (check[i] == 2)continue;
            if (check[i] == 0) {
                int dis = dist(now, order[i].start);
                if (chmin(mi, dis)) {
                    next = order[i].start;
                    id = i;
                    flag = true;
                }
            }
            else if (check[i] == 1) {
                int dis = dist(now, order[i].goal);
                if (chmin(mi, dis)) {
                    next = order[i].goal;
                    id = i;
                    flag = false;
                }
            }
        }
        route.emplace_back(next);
        now = next;

        if (flag) {
            check[id] = 1;
        }
        else {
            check[id] = 2;
            cnt++;
        }
    }
    route.emplace_back(400, 400);

    score = get_score();
    cnt = 0;

    while (t_diff < TIME_LIMIT) {
        two_swap();
        reverse();
        if (cnt % 10 == 0) {
            add();
            REP(i, 100) {
                two_swap();
                reverse();
            }
            del();
        }
        get_time();
        cnt++;
    }
    
    

    score = get_score();

    
    out();

    get_time();

    cerr << "time:  " << int(t_diff) << " ms" << endl;
    cerr << "score: " << int(round(100000000.0 / (1000.0 + score))) << endl;
    cerr << "last:  " << int(last_update) << " ms" << endl;
    cerr << "cnt:   " << cnt << endl;
    cerr << "update:" << update_cnt << endl;
    return 0;
}
