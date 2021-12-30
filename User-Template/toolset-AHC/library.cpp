v.erase(remove(v.begin(), v.end(), value_to_remove), v.end());
int num = atoi(numStr.c_str());
sort(data.begin(), data.end());
reverse(data.begin(), data.end());
d.erase(unique(d.begin(), d.end()), d.end()); //重複なし
stoi(str)//文字列をintに変換
stoll(str)//文字列をlonglongに変換
str.substr(n)//n番以降スライス
str.substr(n,m)//n番以降m文字スライス
str.erase(n)//n番以降削除
str.erase(n,m)//n番以降m文字削除
str.find(str_to_find)//位置 or string::npos
cout << fixed << setprecision(15) << n;
sort(all(vp), [](P a, P b) {return a.first > b.first || (a.first == b.first && a.second < b.second);});//ペアfirst降順 second昇順
using PQ = priority_queue<ll>;
using PQG = priority_queue<ll, vector<ll>, greater<ll>>;
using PQP = priority_queue<P>;
using PQPG = priority_queue<P, vector<P>, greater<P>>;
V<ll> y_vec = { 0, 1,0, -1 };
V<ll> x_vec = { -1, 0,1, 0 };
V<ll> y_vec = {0,1,1,1,0,-1,-1,-1};
V<ll> x_vec = {-1,-1,0,1,1,1,0,-1};


//繰り返し二乗法による累乗計算
ll pow(ll a, ll n) {
    ll res = 1;
    while (n > 0) {
        if (n & 1) res *= a;
        a *= a;
        n >>= 1;
    }
    return res;
}

//ユニオンファインドツリー
class UnionFind {
public:
    vector<ll> parent;
    vector<ll> siz;
    map<ll, vector<ll> > group;
    ll n;

    UnionFind(ll n_) :n(n_), parent(n_), siz(n_, 1) {
        for (ll i = 0; i < n; i++) { parent[i] = i; }
    }

    ll root(ll x) {
        if (parent[x] == x) return x;
        return parent[x] = root(parent[x]);
    }

    bool unite(ll x, ll y) {
        ll rx = root(x);
        ll ry = root(y);
        if (rx == ry) return false;
        if (siz[rx] < siz[ry]) swap(rx, ry);
        siz[rx] += siz[ry];
        parent[ry] = rx;
        return true;
    }

    bool same(ll x, ll y) {
        return root(x) == root(y);
    }

    ll size(ll x) {
        return siz[root(x)];
    }
};

//迷路の移動
REP(i, 4) {
    ll x2 = x + x_vec[i];
    ll y2 = y + y_vec[i];
    if (0 <= x2&& x2 <= h - 1 && 0 <= y2 && y2 <= w - 1 && m[x2][y2] != INF) {
        if (d[x2][y2] > d[x][y] + 1) {
            d[x2][y2] = d[x][y] + 1;
            q.push(MP(x2,y2));
        }
    }
}

//エラストテネスの篩
void eratosthenes(V<bool> &prime,ll n){
    prime[0] = prime[1] = false;
    REP(i,n){
        if(prime[i]){
            for(int j = i + i; j < n; j+=i){
                prime[j] = false;
            }
        }
    }
}

//素因数分解
V<P> prime_factorize(ll N) {
    V<P> res;
    for (ll a = 2; a * a <= N; ++a) {
        if (N % a != 0) continue;
        long long ex = 0;
        while (N % a == 0) {
            ++ex;
            N /= a;
        }
        res.push_back({a, ex});
    }
    if (N != 1) res.push_back({N, 1});
    return res;
}

//素因数分解の高速版(先にエラストテネスの篩(最小の素数)を構築)
const int N_MAX=1001001;
V<ll> set_prime(N_MAX, 0);
void eratosthenes(ll n) {
        FOR(i, 2, n - 1) {
        if (set_prime[i] == 0) {
            for (int j = i; j < n; j += i) {
                if(set_prime[j]==0)set_prime[j] = i;
            }
        }
    }
}

V<P> prime_factorize(ll N) {
    V<P> res;
    while (N != 1) {
        ll a = set_prime[N];
        ll ex = 0;
        while (N % a == 0) {
            ++ex;
            N /= a;
        }
        res.push_back({ a, ex });
    }
    return res;
}

int main{
    eratosthenes(N);
}

//各桁の値リスト（k進数のとき）
V<ll> digit_val(ll n, ll k) {
    V<ll> ans;
    while (n != 0) {
        ans.push_back(n % k);
        n /= k;
    }
    return ans;
}

//約数列挙
V<ll> divisor(ll n) {
    V<ll> ret;
    for (ll i = 1; i * i <= n; i++) {
        if (n % i == 0) {
            ret.push_back(i);
            if (i * i != n) ret.push_back(n / i);
        }
    }
    sort(ALL(ret));
    return (ret);
}

//最大公約数（ユークリッドの互除法）
ll gcd(ll p, ll q){
    while(q != 0){
        ll r = p % q;
        p = q;
        q = r;
    }
    return p;
}

//拡張ユークリッドの互除法(gcd(a,b)=ax+byとなるgcd(a,b),x,yを出力。extgcd(a,M)でmod Mでのaの逆元を求めるのに使える。）
tuple<ll, ll, ll> extgcd(ll a, ll b) {
    if (b == 0) return { a,1,0 };
    ll g, x, y;
    tie(g, x, y) = extgcd(b, a % b);
    return { g, y, x - a / b * y };
}


//セグメントツリー 0-index [l,r)
template<typename T>
struct SegTree {
    int n;
    V<T> node;
    SegTree(int x) {
        n = 1;
        while (n < x) n *= 2;
        node.resize(2 * n - 1, INF);//min
        node.resize(2*n-1,-INF);//max
        node.resize(2*n-1,0);//sum,gcd;
    }

    SegTree(V<T> v) {
        int x = SZ(v);
        n = 1;
        while (n < x) n *= 2;
        node.resize(2 * n - 1, INF);//min
        node.resize(2*n-1,-INF);//max
        node.resize(2*n-1,0);//sum;
        node.resize(2 * n - 1, 0);//gcd

        REP(i, x) node[i + n - 1] = v[i];
        FORD(i, n - 2, 0) node[i] = min(node[2 * i + 1], node[2 * i + 2]);
        FORD(i,n-2,0) node[i] = max(node[2*i+1], node[2*i+2]);
        FORD(i,n-2,0) node[i] = node[2*i+1]+node[2*i+2];
        FORD(i, n - 2, 0) node[i] = gcd(node[2 * i + 1], node[2 * i + 2]);
    }

    void update(int x, T val) {
        x += n - 1;
        node[x] = val;
        while (x > 0) {
            x = (x - 1) / 2;
            node[x] = min(node[2 * x + 1], node[2 * x + 2]);
            node[x] = max(node[2*x+1], node[2*x+2]);
            node[x] = node[2*x+1]+node[2*x+2];
            node[x] = gcd(node[2 * x + 1], node[2 * x + 2]);
        }
    }

    void add(int x, T val) {
        x += n - 1;
        node[x] += val;
        while (x > 0) {
            x = (x - 1) / 2;
            node[x] = min(node[2 * x + 1], node[2 * x + 2]);
            node[x] = max(node[2*x+1], node[2*x+2]);
            node[x] = node[2*x+1]+node[2*x+2];
            node[x] = gcd(node[2 * x + 1], node[2 * x + 2]);

        }
    }

    T query(int a, int b, int k = 0, int l = 0, int r = -1) {
        if (r < 0)r = n;
        if (r <= a || b <= l) return INF;//min
        if (r <= a || b <= l) return -INF;//max
        if (r <= a || b <= l) return 0;//sum,gcd
        else if (a <= l && r <= b) return node[k];
        else {
            T vl = query(a, b, k * 2 + 1, l, (l + r) / 2);
            T vr = query(a, b, k * 2 + 2, (l + r) / 2, r);
            return min(vl, vr);//min
            return max(vl, vr);//max
            return vl+vr;//sum
            return gcd(vl, vr);//gcd

        }
    }

    T get(int x) {
        x += n - 1;
        return node[x];
    }
};


//遅延評価セグメントツリー(update:op=1,add:op=2)0-index [l,r)
template<typename T>
struct LazySegTree {
    int n;
    V<T> node,lazy;
    LazySegTree(int x) {
        n = 1;
        while (n < x) n *= 2;
        node.resize(2 * n - 1, INF);//min
        node.resize(2*n-1,-INF);//max
        node.resize(2*n-1,0);//sum
        lazy.resize(2 * n - 1, 0);
    }

    LazySegTree(V<T> v) {
        int x = SZ(v);
        n = 1;
        while (n < x) n *= 2;
        node.resize(2 * n - 1, INF);//min
        node.resize(2*n-1,-INF);//max
        node.resize(2*n-1,0);//sum
        lazy.resize(2 * n - 1, 0);

        REP(i, x) node[i + n - 1] = v[i];
        FORD(i, n - 2, 0) node[i] = min(node[2 * i + 1], node[2 * i + 2]);
        FORD(i,n-2,0) node[i] = max(node[2*i+1], node[2*i+2]);
        FORD(i, n - 2, 0)node[i] = node[2 * i + 1] + node[2 * i + 2];
    }

    void eval(int k,int op) {
        if (lazy[k] == 0)return;
        if (k < n - 1) {
            if (op == 1) {//update
                lazy[2 * k + 1] = lazy[k];
                lazy[2 * k + 2] = lazy[k];
            }
            else if (op == 2) {//add
                lazy[2 * k + 1] = lazy[k]/2;
                lazy[2 * k + 2] = lazy[k]/2;
            }
        }
        if (op == 1)node[k] = lazy[k];
        if (op == 2)node[k] += lazy[k];
        lazy[k] = 0;
    }

    void update(int a, int b, T val, int k = 0, int l = 0, int r = -1) {
        if (r < 0)r = n;

        eval(k, 1);
        if (b <= l || r <= a) return;
        if (a <= l && r <= b) {
            lazy[k] = val;
            eval(k, 1);
        }
        else {
            update(a, b, val, 2 * k + 1, l, (l + r) / 2);
            update(a, b, val, 2 * k + 2, (l + r) / 2, r);
            node[k] = min(node[2 * k + 1], node[2 * k + 2]);
            node[k] = max(node[2*k+1], node[2*k+2]);
            node[k] = node[2 * k + 1] + node[2 * k + 2];
        }
    }

    void add(int a, int b, T val,int k=0,int l=0,int r=-1) {
        if (r < 0)r = n;

        eval(k, 2);
        if (b <= l || r <= a) return;
        if (a <= l && r <= b) {
            lazy[k] = (r - l) * val;
            eval(k, 2);
        }
        else {
            add(a, b, val, 2 * k + 1, l, (l + r) / 2);
            add(a, b, val, 2 * k + 2, (l + r) / 2, r);
            node[k] = min(node[2 * k + 1], node[2 * k + 2]);
            node[k] = max(node[2*k+1], node[2*k+2]);
            node[k] = node[2 * k + 1] + node[2 * k + 2];
        }
    }

    T query(int a, int b,int op, int k = 0, int l = 0, int r = -1) {
        eval(k, op);
        if (r < 0)r = n;
        if (r <= a || b <= l) return INF;
        if (r <= a || b <= l) return -INF;
        if (r <= a || b <= l) return 0;
        else if (a <= l && r <= b) return node[k];
        else {
            T vl = query(a, b,op, k * 2 + 1, l, (l + r) / 2);
            T vr = query(a, b,op, k * 2 + 2, (l + r) / 2, r);
            return min(vl, vr);
            return max(vl, vr);
            return vl+vr;
        }
    }

};


/* BIT: 区間和の更新や計算を行う構造体(1点更新)
    初期値は a_1 = a_2 = ... = a_n = 0
    ・add(i,x): a_i += x とする
    ・sum(i): a_1 + a_2 + ... + a_i を計算する
    計算量は全て O(logn)
    1-indexに注意！！
*/
template <typename T>
struct BIT {
    int n;          // 配列の要素数(数列の要素数+1)
    vector<T> bit;  // データの格納先
    BIT(int n_) : n(n_ + 1), bit(n, 0) {}
    void add(int i, T x) {
        for (int idx = i; idx < n; idx += (idx & -idx)) {
            bit[idx] += x;
        }
    }
    T sum(int i) {
        T s(0);
        for (int idx = i; idx > 0; idx -= (idx & -idx)) {
            s += bit[idx];
        }
        return s;
    }
};

//行列の掛け算
using M = VV<ll>;
M operator*(M a, M b) {
    int n = a.size();
    int m = b.size();
    int l = b[0].size();
    M ans(n, vector<ll>(l, 0));
    REP(i, n)REP(j, m)REP(k, l)ans[i][k] += a[i][j] * b[j][k];
    return ans;
}


//Z_algorithm 文字列Sについて、Sと部分列S'(S[i]からS[SZ(S)-1])の共通文字列の文字数の配列Z[i]を構築
V<ll> Z_algorithm(const string &S) {
    ll N = (int)S.size();
    V<ll> res(N);
    res[0] = N;
    ll i = 1, j = 0;
    while (i < N) {
        while (i+j < N && S[j] == S[i+j]) ++j;
        res[i] = j;
        if (j == 0) {
            ++i; 
            continue;
        }
        int k = 1;
        while (i+k < N && k+res[k] < j) {
            res[i+k] = res[k];
            ++k;
        }
        i += k, j -= k;
    }
    return res;
}

//座標圧縮（１次元）
template <typename T>
V<T> compress(V<T>& X,int n) {
    vector<T> vals;
    REP(i, n)vals.push_back(X[i]);
    sort(ALL(vals));
    vals.erase(unique(ALL(vals)), vals.end());
    REP(i, SZ(X)) {
        X[i] = lower_bound(ALL(vals), X[i]) - vals.begin();
    }
    return vals;
}

//ダイクストラ法（グラフver) gはpair<next_node,dis>,dの初期値はINF
void Dijkstra(VV<P> &g,V<ll> &d) {
    d[0] = 0;
    priority_queue<P, V<P>, greater<P>> q;
    q.push(MP(0, 0));
    while (!q.empty()) {
        ll dis, x; tie(dis, x) = q.top();
        q.pop();
        if (d[x] < dis)continue;
        for (P r : g[x]) {
            if (chmin(d[r.first], dis + r.second)) {
                q.push(MP(d[r.first], r.first));
            }
        }
    }
}

//ダイクストラ法（迷路ver）dは初期値INFの2次元配列
V<ll> y_vec = { 0, 1,0, -1 };
V<ll> x_vec = { -1, 0,1, 0 };
void Dijkstra(VV<ll> &d){
    priority_queue<pair<ll, P>, V<pair<ll, P>>, greater<pair<ll, P>>> q;
    q.push(MP(0, MP(0, 0)));
    d[0][0] = 0;
    while (!q.empty()) {
        ll dis = q.top().first, x, y;
        tie(x, y) = q.top().second;
        q.pop();
        if (dis > d[x][y])continue;
        REP(i, 4) {
            ll nx = x + x_vec[i];
            ll ny = y + y_vec[i];
            if (nx<0 || nx>h - 1 || ny<0 || ny>w - 1||m[nx][ny]==1)continue;
            if (chmin(d[nx][ny], d[x][y])) {
                q.push(MP(d[nx][ny], MP(nx, ny)));
            }
        }
    }
}

//分数
struct fraction{
    p/q
    ll p,q;
    fraction(ll P = 0, ll Q = 1): p(P), q(Q){}
    bool operator<(const fraction &other)const{
        return p*other.q < other.p*q;
    }
    bool operator<=(const fraction &other)const{
        return p*other.q <= other.p*q;
    }
};

//Mapクラス

class Cell {
public:
    int x = -1;
    int y = -1;
    int value = 0;
    int state = 0;

    V<P> around4;
    Cell() {};
};

class Map {
public:
    int height;
    int width;
    VV<Cell> grid;
    VV<bool> visited;

    Map() {};

    Map(int h,int w) {
        height = h;
        width = w;
        grid = VV<Cell>(height, V<Cell>(width));
        REP(i, height)REP(j, width) {
            grid[i][j].x = i;
            grid[i][j].y = j;
            //各セルの情報をここで取得

        }
        REP(x, height)REP(y, width) {
            REP(i, 4) {
                int nx = x + x_vec[i];
                int ny = y + y_vec[i];
                if (nx < 0 || nx >= h || ny < 0 || ny >= w || grid[nx][ny].state == 1)continue;
                grid[x][y].around4.emprace_back(nx, ny);
            }
            shuffle(ALL(grid[x][y].around4, rng));
        }
    };


    //get
    Cell* getCell(int x,int y) {
        return &grid[x][y];
    }

    int get_state(int x,int y)const {
        return grid[x][y].state;
    }

    int get_value(int x,int y)const {
        return grid[x][y].value;
    }

    //set

    void set_status(int x,int y, int n) {
        grid[x][y].status = n;
    }

    void set_state(int x,int y, int n) {
        grid[x][y].state = n;
    }

    //Search
    void clear_visited() {
        visited = VV<bool>(height, V<bool>(width, false));
    }

    bool isvisited(int x,int y) const {
        return visited[x][y];
    }
};
