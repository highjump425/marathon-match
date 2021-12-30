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
const double TIME_LIMIT = 9500;
double last_update = 0, t_diff;
double start_temp, end_temp;

mt19937 rng;
using uni_int = uniform_int_distribution<>;
using uni_real = uniform_real_distribution<>;

V<ll> y_vec = { 0, 1,0, -1 };
V<ll> x_vec = { -1, 0,1, 0 };

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
    return start_temp + (end_temp - start_temp) * (t_diff / TIME_LIMIT);
}

//unordered_map<char, int> place = { {'.',0},{'T',1}, {'P',2}, {'b',3}, {'e',4}, {'E',5}, {'B',6}, };

int N, C, money, elapsedTime;
int numLeft,numElf,numPresent,numCareer,numTree;
int maxBox, numBox,maxPresent,turnLeft,boxLeft;
int prevP, prevE,prevNP;
double elfP;
V<bool> border;
V<int> elves,careers,presents,install,lim;

int pos(int x, int y) {
    return x * N + y;
}

int dis(int a, int b) {
    return abs(a / N - b / N) + abs(a % N - b % N);
}

int dis_border(int n) {
    return min(min(min(n / N, n % N), N - n / N - 1), N - n % N - 1);
}

class Cell {
public:
    //int x = -1;
    //int y = -1;
    //int value = INF;

    // 0:empty, 1:tree, 2: present, 3:box, 4:elf, 5:elf with present, 6: elf with box
    char state;

    bool reach=false;
    int value=0;

    V<int> around4;
    Cell() {};
};

//class Map {
//public:
//    int height;
//    int width;
//    V<Cell> grid;
//    V<bool> visited;
//
//    Map() {};
//
//    Map(int h, int w) {
//        height = h;
//        width = w;
//        grid = V<Cell>(height*width);
//        REP(i, height)REP(j, width) {
//            //grid[pos(i, j)].x = i;
//            //grid[pos(i, j)].y = j;
//            //ŠeƒZƒ‹‚Ìî•ñ‚ð‚±‚±‚ÅŽæ“¾
//            cin >> grid[pos(i, j)].state;
//        }
//        REP(x, height)REP(y, width) {
//            REP(i, 4) {
//                int nx = x + x_vec[i];
//                int ny = y + y_vec[i];
//                if (nx < 0 || nx >= h || ny < 0 || ny >= w || grid[pos(nx,ny)].state == 'T')continue;
//                grid[pos(x,y)].around4.emplace_back(pos(nx, ny));
//            }
//            shuffle(ALL(grid[pos(x,y)].around4), rng);
//        }
//    };
//
//    /*int pos(int x, int y)const {
//        return x * height + y;
//    }*/
//
//
//    //get
//    Cell* getCell(int x, int y) {
//        return &grid[pos(x,y)];
//    }
//
//    int get_state(int x, int y)const {
//        return grid[pos(x, y)].state;
//    }
//
//    /*int get_value(int x, int y)const {
//        return grid[pos(x, y)].value;
//    }*/
//
//    //set
//
//    /*void set_value(int x, int y, int n) {
//        grid[pos(x, y)].value = n;
//    }*/
//
//    void set_state(int x, int y, int n) {
//        grid[pos(x, y)].state = n;
//    }
//
//    //Search
//    void clear_visited() {
//        visited = V<bool>(height*width, false);
//    }
//
//    bool isvisited(int x, int y) const {
//        return visited[pos(x,y)];
//    }
//};

V<Cell> grid;
V<int> route;
V<int> escape;
V<int> around;
V<bool> reachable;
V<bool> visited;
V<int> order;
V<int> touch;
V<char> draft;
queue<int> q;

bool compP(int l, int r) {
    if (around[l] != around[r])return around[l] < around[r];
    if (escape[l] != escape[r])return escape[l] > escape[r];
    return l < r;
}

void update_grid() {
    REP(x, N)REP(y, N) {
        grid[pos(x, y)].around4 = V<int>();
        REP(i, 4) {
            int nx = x + x_vec[i];
            int ny = y + y_vec[i];
            if (nx < 0 || nx >= N || ny < 0 || ny >= N
                || grid[pos(nx, ny)].state == 'T')continue;
            grid[pos(x, y)].around4.emplace_back(pos(nx, ny));
        }
        shuffle(ALL(grid[pos(x, y)].around4), rng);
    }
}

void init() {
    cin >> N >> C >> elfP >> money;
    cerr << "N : " << N << endl;
    cerr << "C : " << C << endl;
    cerr << "money : " << money << endl;
    cerr << "elfP : " << elfP << endl;

    grid = V<Cell>(N * N);
    border = V<bool>(N * N);
    reachable = V<bool>(N * N);
    elves = V<int>(N * N);
    careers = V<int>(N * N);
    presents = V<int>(N * N);
    route = V<int>(N * N);
    escape = V<int>(N * N, INF);
    around = V<int>(N * N);
    touch = V<int>();
    draft = V<char>(N * N);
    order = V<int>(N * N);
    turnLeft = N * N;
    numLeft = 0;
    numElf = 0;
    numCareer = 0;
    numPresent = 0;
    numTree = 0;
    numBox = 0;
    maxBox = (money + N * N) / C;
    REP(i, N)REP(j, N) {
        char c; cin >> c;
        grid[pos(i, j)].state = c;
        if (c == 'P') {
            numLeft++;
            presents[numPresent] = pos(i, j);
            numPresent++;
        }
        else if (c == 'e') {
            elves[numElf] = pos(i, j);
            numElf++;
        }
        else if (c == 'E') {
            careers[numCareer] = pos(i, j);
            numCareer++;
        }
        else if (c == 'T')numTree++;
        if (i == 0 || i == N - 1 || j == 0 || j == N - 1) {
            border[pos(i, j)] = true;
            lim.emplace_back(pos(i, j));
        }
    }

    int x = 0, y = 0;
    int l = N - 1;
    int cnt = 0;
    int dir = 1;
    order[0] = pos(x, y);
    REP(i, l) {
        x += x_vec[dir];
        y += y_vec[dir];
        cnt++;
        order[cnt] = pos(x, y);
    }
    dir = (dir + 1) % 4;
    while (l > 0) {
        REP(i, l) {
            x += x_vec[dir];
            y += y_vec[dir];
            cnt++;
            order[cnt] = pos(x, y);
        }
        dir = (dir + 1) % 4;
        REP(i, l) {
            x += x_vec[dir];
            y += y_vec[dir];
            cnt++;
            order[cnt] = pos(x, y);
        }
        dir = (dir + 1) % 4;
        l--;
    }
    reverse(ALL(order));
    maxPresent = numPresent;
    boxLeft = maxBox;
    prevP = numPresent;
    prevE = 0;
    prevNP = INF;
    cerr << "treeP : " << double(numTree)/(N*N) << endl;
    cerr << "presentP : " << double(numPresent)/((N-6)*(N-6))<< endl;

    update_grid();
}

void output() {
    if (SZ(install) == 0)cout << -1 << endl;
    else {
        REP(i, SZ(install)) {
            cout << install[i]/N << " " << install[i]%N;
            cout << (i == SZ(install) - 1 ? endl : " ");
        }
    }
    cout.flush();
}

void get_input() {
    cin >> elapsedTime;
    cin >> money;
    numLeft = 0;
    numElf = 0;
    numCareer = 0;
    numPresent = 0;
    REP(i, N)REP(j, N) {
        char c; cin >> c;
        grid[pos(i, j)].state = c;
        if (c == 'P') {
            numLeft++;
            presents[numPresent] = pos(i, j);
            numPresent++;
        }
        else if (c == 'e') {
            elves[numElf] = pos(i, j);
            numElf++;
        }
        else if (c == 'E') {
            careers[numCareer] = pos(i, j);
            numCareer++;
        }
    }
    update_grid();
}

bool can_build(int n, int b) {
    return (numElf + turnLeft * elfP) < (numPresent - n) + ((money+turnLeft)/C - b);
}

void set_escape() {
    for (int l : lim) {
        if (grid[l].state == 'T')continue;
        q = queue<int>();
        visited = V<bool>(N * N);
        escape[l] = 1;
        q.push(l);
        q.push(1);
        while (!q.empty()) {
            int now = q.front(); q.pop();
            int dis = q.front(); q.pop();
            if (visited[now])continue;
            visited[now] = true;
            for (int nex : grid[now].around4) {
                if (chmin(escape[nex],dis+1)) {
                    q.push(nex);
                    q.push(dis + 1);
                }
            }
        }
    }
    //REP(i, N) {
    //    REP(j, N)cerr << setw(3) << min(escape[pos(i, j)], 999) << " ";
    //    cerr << endl;
    //}
}

void set_reach() {
    reachable = V<bool>(N * N);
    for (int l : lim) {
        if (reachable[l] || grid[l].state == 'T')continue;
        q = queue<int>();
        visited = V<bool>(N * N);
        reachable[l] = true;
        q.push(l);
        while (!q.empty()) {
            int now = q.front(); q.pop();
            if (visited[now])continue;
            visited[now] = true;
            for (int nex : grid[now].around4) {
                if (visited[nex])continue;
                reachable[nex] = true;
                if (grid[nex].state != 'b' && grid[nex].state != 'P') {
                    q.push(nex);
                }
            }
        }
    }
}

void set_route() {
    route = V<int>(N * N, INF);
    REP(e, numElf) {
        int min_dis = INF;
        int target = -1;
        grid[elves[e]].value = INF;
        grid[elves[e]].reach = false;
        q = queue<int>();
        visited = V<bool>(N * N);
        route[elves[e]] = 0;
        q.push(elves[e]);
        q.push(0);
        while (!q.empty()) {
            int now = q.front(); q.pop();
            int dis = q.front(); q.pop();
            if (dis >= min_dis)break;
            if (visited[now])continue;
            visited[now] = true;
            for (int nex : grid[now].around4) {
                if (visited[nex])continue;
                if (grid[nex].state == 'P') {
                    if (chmin(min_dis, dis + 1)) {
                        chmin(route[nex], dis + 1);
                        visited[nex] = true;
                        //if (min_dis == 1)grid[nex].state = 'E';
                        target = nex;
                        grid[elves[e]].reach = true;
                        grid[elves[e]].value = min_dis;
                    }
                }
                else if (grid[nex].state == 'b') {
                    if (grid[elves[e]].reach==false) {
                        if (chmin(grid[elves[e]].value, dis + 1)) {
                            target = nex;
                        }
                    }
                }
                else {
                    chmin(route[nex], dis + 1);
                    q.push(nex);
                    q.push(dis + 1);
                }
            }
        }
        if(target!=-1)grid[elves[e]].value += escape[target];
    }
    
}

void set_around() {
    REP(e, numCareer) {
        int need = 0;
        for (int p : grid[careers[e]].around4) {
            if (grid[p].state == '.') {
                if (border[p]) {
                    need = INF;
                    break;
                }
                need++;
            }
        }
        around[careers[e]] = need;
    }
    REP(e, numPresent) {
        int need = 0;
        for (int p : grid[presents[e]].around4) {
            if (route[p] == 1 && grid[p].state == '.')need++;
        }
        around[presents[e]] = need;
    }
}

void set_before(int l) {
    visited = V<bool>(N * N);
    q = queue<int>();
    reachable[l] = true;
    q.push(l);
    while (!q.empty()) {
        int now = q.front(); q.pop();
        if (visited[now])continue;
        visited[now] = true;
        for (int nex : grid[now].around4) {
            if (reachable[nex])continue;
            reachable[nex] = true;
            if (draft[nex] != 'b' && draft[nex] != 'P' && draft[nex] != 'a') {
                q.push(nex);
            }
        }
    }
}

void simulate() {
    reachable = V<bool>(N * N);
    visited = V<bool>(N * N);
    for (int l : lim) {
        if (reachable[l] || draft[l] == 'T')continue;
        q = queue<int>();
        reachable[l] = true;
        q.push(l);
        while (!q.empty()) {
            int now = q.front(); q.pop();
            if (visited[now])continue;
            visited[now] = true;
            for (int nex : grid[now].around4) {
                if (reachable[nex])continue;
                reachable[nex] = true;
                if (draft[nex] != 'b' && draft[nex] != 'P'&&draft[nex]!='a') {
                    q.push(nex);
                }
            }
        }
    }
}

bool type1(int i,int j,int &nb) {
    if (i == 0 || i == N - 1)return false;
    if (j >= N - 2)return false;
    if (draft[pos(i, j)] != '.' && draft[pos(i, j + 1)] == '.'
        && reachable[pos(i, j + 1)] && draft[pos(i, j + 2)] != '.') {
        int b = nb;
        b++;
        draft[pos(i, j + 1)] = 'a';
        simulate();
        REP(k, N * N) {
            if (draft[k] == 'a' && !reachable[k]) {
                b--;
            }
        }
        if (chmin(nb, b)) {
            REP(k, N * N) {
                if (draft[k] == 'a' && !reachable[k]) {
                    draft[k] = '.';
                }
            }
        }
        else {
            draft[pos(i, j + 1)] = '.';
            set_before(pos(i, j + 1));
        }
    }
    return true;
}

bool type2(int i, int j, int& nb) {
    if (j == 0 || j == N - 1)return false;
    if (i >= N - 2)return false;
    if (draft[pos(i, j)] != '.' && draft[pos(i + 1, j)] == '.'
        && reachable[pos(i + 1, j)] && draft[pos(i + 2, j)] != '.') {
        int b = nb;
        b++;
        draft[pos(i+1, j)] = 'a';
        simulate();
        REP(k, N * N) {
            if (draft[k] == 'a' && !reachable[k]) {
                b--;
            }
        }
        if (chmin(nb, b)) {
            //cerr << "Yes" << endl;
            REP(k, N * N) {
                if (draft[k] == 'a' && !reachable[k]) {
                    draft[k] = '.';
                }
            }
        }
        else {
            //cerr << "No" << endl;
            draft[pos(i+1, j)] = '.';
            set_before(pos(i+1, j));
        }
    }
    return true;
}

void update_wall(int &nb) {
    int b = nb;
    for (int x : order) {
        int i = x / N, j = x % N;
        if (type1(i, j, nb));

        if (type2(i, j, nb));
        /*if (i == 0 || i == N - 1)continue;
        if (j >= N - 2)continue;
        if (draft[pos(i, j)] != '.' && draft[pos(i, j + 1)] == '.'
            && reachable[pos(i, j + 1)] && draft[pos(i, j + 2)] != '.') {
            b++;
            draft[pos(i, j + 1)] = 'a';
            simulate();
            REP(k, N * N) {
                if (draft[k] == 'a' && !reachable[k]) {
                    b--;
                }
            }
            if (chmin(nb, b)) {
                REP(k, N * N) {
                    if (draft[k] == 'a' && !reachable[k]) {
                        draft[k] = '.';
                    }
                }
            }
            else {
                draft[pos(i, j + 1)] = '.';
                set_before(pos(i, j + 1));
            }
        }*/
    }
}

int set_wall() {
    touch=V<int>();
    draft=V<char>(N*N);
    int np = numPresent;
    int nb = 0;
    int cnt = 0;
    REP(i, N * N)draft[i] = grid[i].state;
    REP(i, numPresent) {
        touch.push_back(presents[i]);
    }
    REP(i, numCareer) {
        if (dis_border(careers[i])>=2) {
            touch.push_back(careers[i]);
        }
    }
    if (SZ(touch) == 0)return -1;

    

    while (true) {
        nb = 0;
        REP(i, N* N)if (draft[i] == 'a')draft[i] = '.';
        for (int r : touch) {
            for (int p : grid[r].around4) {
                if (draft[p] == '.') {
                    draft[p] = 'a';
                    nb++;
                }
            }
        }

        simulate();
        REP(i, N * N) {
            if (draft[i] == 'a' && !reachable[i]) {
                draft[i] = '.';
                nb--;
            }
        }

        //if (cnt == 0) {
        //    cerr << SZ(touch) << " " << np << " " << nb << " " << cnt << endl;
        //    REP(i, N) {
        //        REP(j, N) {
        //            cerr << draft[pos(i, j)];
        //        }
        //        cerr << endl;
        //    }
        //    cerr << endl;
        //}

        if(SZ(touch)<prevNP+3)update_wall(nb);

        //if (cnt == 0) {
        //    cerr << SZ(touch) << " " << np << " " << nb << " " << cnt << endl;
        //    REP(i, N) {
        //        REP(j, N) {
        //            cerr << draft[pos(i, j)];
        //        }
        //        cerr << endl;
        //    }
        //    cerr << endl;
        //}

        if (can_build(np, nb))break;

        for (int t : touch) {
            int need = 0;
            for (int p : grid[t].around4) {
                if (draft[p] == 'a')need++;
            }
            around[t] = need;
        }
        sort(ALL(touch), compP);

        if (draft[touch[SZ(touch) - 1]] == 'P')np--;
        draft[touch[SZ(touch) - 1]] = '.';

        for (int p : grid[touch[SZ(touch) - 1]].around4)if (draft[p] == 'a') {
            draft[p] = '.';
            nb--;
        }
        touch.pop_back();
        if (SZ(touch) == 0)return -1;
        cnt++;
    }
    //cerr << SZ(touch) << " " << np << " " << nb << " " << cnt << endl;
    /*cerr << SZ(touch) << " " << np << " " << nb <<" "<<cnt<<endl;
    REP(i, N) {
        REP(j, N) {
            cerr << draft[pos(i, j)];
        }
        cerr << endl;
    }
    cerr << endl;*/
    
    prevNP = np;
    return np;

}

//void solve1() {
//    set_reach();
//    set_route();
//    sort(careers.begin(), careers.begin() + numCareer, compP);
//    sort(presents.begin(), presents.begin() + numPresent, compP);
//
//    if (int(1.0 / elfP) / C >= 3) {
//        REP(p, numPresent) {
//            if (money / C == 0)break;
//            if (grid[presents[p]].state != 'P')continue;
//            int need = 0;
//            for (int r : grid[presents[p]].around4) {
//                if (route[r] == 1 && grid[r].state == '.') need++;
//            }
//            if (money / C < need)continue;
//            for (int r : grid[presents[p]].around4) {
//                if (route[r] == 1 && grid[r].state == '.') {
//                    install.push_back(r);
//                    grid[r].state = 'b';
//                    numBox++;
//                    money -= C;
//                }
//            }
//        }
//    }
//    else {
//        REP(e, numCareer) {
//            if (money / C == 0)break;
//            if (grid[careers[e]].state == 'E' && !border[careers[e]]) {
//                int need = 0;
//                for (int p : grid[careers[e]].around4) {
//                    if (grid[p].state == '.') {
//                        if (border[p]) {
//                            need = INF;
//                            break;
//                        }
//                        need++;
//                    }
//                }
//                if (need > money / C)continue;
//                if (need > 3)continue;
//                money -= need * C;
//                for (int p : grid[careers[e]].around4) {
//                    if (grid[p].state == '.') {
//                        install.emplace_back(p);
//                        grid[p].state = 'b';
//                        numBox++;
//                    }
//                }
//            }
//        }
//    }
//}
//
//void solve2(int turn,bool &flag) {
//    //if (numElf + turnLeft*elfP > (money + turnLeft) / C + numPresent)return;
//    //if (true)return;
//    if (numElf + numCareer == 0)return;
//    set_reach();
//    set_route();
//    set_around();
//    sort(careers.begin(), careers.begin() + numCareer, compP);
//    sort(presents.begin(), presents.begin() + numPresent, compP);
//
//    if ((numElf + int((N * N - turn) * elfP)) * 1.0 < maxBox)flag = true;
//    if (int(1.0 / elfP) / C >= 2) {
//        REP(p, numPresent) {
//            if (turn + escape[presents[p]] + 2 > N * N)continue;
//            if (money / C == 0)break;
//            if (grid[presents[p]].state != 'P')continue;
//            int need = 0;
//            for (int r : grid[presents[p]].around4) {
//                if (route[r] == 1 && grid[r].state == '.') need++;
//            }
//            if (money / C < need)continue;
//            for (int r : grid[presents[p]].around4) {
//                if (route[r] == 1 && grid[r].state == '.') {
//                    install.push_back(r);
//                    grid[r].state = 'b';
//                    numBox++;
//                    boxLeft--;
//                    money -= C;
//                }
//            }
//        }
//    }
//    if (flag) {
//        REP(e, numCareer) {
//            if (turn + escape[careers[e]] > N * N)continue;
//            if (money / C == 0)break;
//            if (dis_border(careers[e])>=2) {
//                int need = 0;
//                for (int p : grid[careers[e]].around4) {
//                    if (border[p] || grid[p].state == 'e'
//                        || grid[p].state == 'E' || grid[p].state == 'B') {
//                        need = INF;
//                        break;
//                    }
//                    else if (grid[p].state == '.')need++;
//                    
//                }
//                if (need > money / C||need>3)continue;
//                if (need > 2 && money / C < 5)continue;
//
//                money -= need * C;
//                for (int p : grid[careers[e]].around4) {
//                    if (grid[p].state == '.') {
//                        install.emplace_back(p);
//                        grid[p].state = 'b';
//                        boxLeft--;
//                        numBox++;
//                    }
//                }
//            }
//        }
//    }
//
//    set_route();
//    REP(e, numElf) {
//        if (money / C == 0)return;
//        if (grid[elves[e]].reach||turn+grid[elves[e]].value>=N*N)continue;
//        int nb = 0;
//        for (int a : grid[elves[e]].around4) {
//            if (grid[a].state == 'b')nb++;
//        }
//        if (nb > 0)continue;
//        for (int a : grid[elves[e]].around4) {
//            if (grid[a].state == '.' && !border[a]) {
//                install.push_back(a);
//                grid[a].state = 'b';
//                numBox++;
//                money -= C;
//                break;
//            }
//        }
//    }
//}

void solve(int turn) {
    if (money / C == 0)return;
    set_reach();
    set_route();
    set_around();
    int np=0;
    if ((prevP != numPresent || numCareer>0)&&elapsedTime<TIME_LIMIT) {
        //cerr << elapsedTime << endl;
        np=set_wall();
        prevP = numPresent;
        prevE = numCareer;
    }
    if (np == -1)return;

    int nb = 0;
    if (turn + 1 / (elfP) < N * N) {
        REP(i, N * N) {
            if (grid[i].state == '.' && draft[i] == 'b')draft[i] = 'a';
        }
        REP(i, N * N)if (draft[i] == 'a' && escape[i] + turn < N * N) {
            nb++;
        }
        if (nb + 3 <= money / C) {
            REP(p, N * N)if (draft[p] == 'a' && grid[p].state == '.' && escape[p] + turn < N * N) {
                install.emplace_back(p);
                grid[p].state = 'b';
                draft[p] = 'b';
                boxLeft--;
                numBox++;
                money -= C;
            }
        }
    }

    if (money / C == 0)return;

    REP(e, numCareer) {
        if (turn + escape[careers[e]] > N* N)continue;
        if (money / C == 0)break;
        if (draft[careers[e]]=='E') {
            int need = 0;
            for (int p : grid[careers[e]].around4) {
                if (border[p] || grid[p].state == 'e'
                    || grid[p].state == 'E' || grid[p].state == 'B') {
                    need = INF;
                    break;
                }
                else if (grid[p].state == '.'&&draft[p]=='a')need++;
            }
            if (need > money / C)continue;
            money -= need * C;
            for (int p : grid[careers[e]].around4) {
                if (grid[p].state == '.' && draft[p] == 'a') {
                    install.emplace_back(p);
                    grid[p].state = 'b';
                    if (draft[p] == 'a')draft[p] = 'b';
                    boxLeft--;
                    numBox++;
                }
            }
        }
    }

    set_route();
    if (turn + 1 / (elfP) >= N * N)return;
    REP(e, numElf) {
        if (money / C == 0)return;
        if (grid[elves[e]].reach || turn + grid[elves[e]].value >= N * N)continue;
        int nb = 0;
        for (int a : grid[elves[e]].around4) {
            if (grid[a].state == 'b')nb++;
        }
        if (nb > 0)continue;
        for (int a : grid[elves[e]].around4) {
            if (grid[a].state == '.' && !border[a]) {
                install.push_back(a);
                grid[a].state = 'b';
                numBox++;
                boxLeft--;
                money -= C;
                break;
            }
        }
    }
}

void solve2(int turn) {
    if (money / C == 0)return;
    set_reach();
    set_route();
    set_around();
    int np = 0;


    int nb = 0;
    if (turn + 1 / (elfP) < N * N) {
        REP(i, N * N) {
            if (grid[i].state == '.' && draft[i] == 'b')draft[i] = 'a';
        }
        REP(i, N * N)if (draft[i] == 'a' && escape[i] + turn < N * N) {
            nb++;
        }
        if (nb + 3 <= money / C) {
            REP(p, N * N)if (draft[p] == 'a' && grid[p].state == '.' && escape[p] + turn < N * N) {
                install.emplace_back(p);
                grid[p].state = 'b';
                draft[p] = 'b';
                boxLeft--;
                numBox++;
                money -= C;
            }
        }
    }

    if (money / C == 0)return;

    REP(e, numCareer) {
        if (turn + escape[careers[e]] > N* N)continue;
        if (money / C == 0)break;
        if (draft[careers[e]] == 'E') {
            int need = 0;
            for (int p : grid[careers[e]].around4) {
                if (border[p] || grid[p].state == 'e'
                    || grid[p].state == 'E' || grid[p].state == 'B') {
                    need = INF;
                    break;
                }
                else if (grid[p].state == '.')need++;
            }
            if (need > money / C)continue;
            money -= need * C;
            for (int p : grid[careers[e]].around4) {
                if (grid[p].state == '.') {
                    install.emplace_back(p);
                    grid[p].state = 'b';
                    if (draft[p] == 'a')draft[p] = 'b';
                    boxLeft--;
                    numBox++;
                }
            }
        }
    }

    set_route();
    if (turn + 1 / (elfP) >= N * N)return;
    REP(e, numElf) {
        if (money / C == 0)return;
        if (grid[elves[e]].reach || turn + grid[elves[e]].value >= N * N)continue;
        int nb = 0;
        for (int a : grid[elves[e]].around4) {
            if (grid[a].state == 'b')nb++;
        }
        if (nb > 0)continue;
        for (int a : grid[elves[e]].around4) {
            if (grid[a].state == '.' && !border[a]) {
                install.push_back(a);
                grid[a].state = 'b';
                numBox++;
                boxLeft--;
                money -= C;
                break;
            }
        }
    }
}

signed main(int argc, char* argv[]) {
    cin.tie(0);
    ios::sync_with_stdio(false);
    cout << fixed << setprecision(5);
    cerr << fixed << setprecision(5);
    t_start = chrono::system_clock::now();

    get_time();

    init();
    set_escape();
    bool flag = false;
    int low = int(numPresent / elfP) + N/2;
    bool quit = false;
    
    //cerr << low << endl;
    //cerr << N * N * elfP << " " << numPresent + maxBox - ((low + 99) / 100) * 5 << endl;
    if (N * N * elfP >= numPresent + maxBox)quit = true;
    set_reach();
    if (!quit) {
        int np = set_wall();
        if (np <= low / 100)quit = true;
    }
    REP(turn,N*N){
        install = V<int>();
        turnLeft = N * N - turn - 1;

        if (!quit) {
            solve(turn);
        }
        //if(!quit)solve2(turn,flag);

        
        output();

        get_input();
    }

    //get_time();
    //cerr << "time:  " << int(t_diff) << " ms" << endl;
    //cerr << "score: " << money+numLeft*100+1 << endl;
    //cerr << "last:  " << last_update << endl;
    //cerr << "cnt:   " << cnt << endl;
    //cerr << "update:" << update_cnt << endl;

    return 0;
}
