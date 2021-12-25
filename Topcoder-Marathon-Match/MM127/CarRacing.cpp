<<<<<<< HEAD
//#pragma GCC optimize("Ofast")
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

mt19937 rng;
V<int> ans,ranking;
V<P> juni,confirm;
VV<int> check;
string result;
int N, K, cnt = 0;

void selection() {
    //selection sort
    REP(i, N) {
        int pos = i;
        int zan = i;
        int next = i + 1;
        while (next < N) {
            int num = min(N - 1, next + K - 2) - next + 2;
            string race = to_string(ans[zan]) + " ";
            FOR(j, next, min(N - 1, next + K - 2)) {
                race += to_string(ans[j]) + " ";
            }
            cout << race << endl;
            cout.flush();
            cnt++;
            getline(cin, result);
            stringstream res{ result };
            string rank;
            int j = 0;
            while (getline(res, rank, ' ')) {
                if (rank == "0") {
                    if (j == 0)break;
                    zan = next + j - 1;
                    break;
                }
                j++;
            }
            //cerr << zan << " " << next << " " << cnt << endl;
            next = min(N - 1, next + K - 2) + 1;
        }

        if (pos != zan)swap(ans[pos], ans[zan]);
    }
    REP(i, N)cout << ans[i] << " ";
    cout << endl;
    cout.flush();

    return;
}

void merge(V<int> &array,int l,int m,int r) {
    int nl = m - l + 1;
    int nr = r - m;
    //cerr << l << " " << m << " " << r <<endl;

    V<int> left(nl), right(nr);
    REP(i, nl)left[i] = array[l + i];
    REP(i, nr)right[i] = array[m + 1 + i];

    m = l;
    l = 0;
    r = 0;
    while (l != nl && r!= nr) {
        int lt, rt;
        if (nl - l >= (K + 1) / 2 && nr - r >= K / 2) {
            //cerr << 0 << endl;
            lt = (K + 1) / 2;
            rt = K / 2;
        }
        else if (nl - l >= (K + 1) / 2) {
            //cerr << 1 << endl;
            rt = nr - r;
            lt = min(nl - l, K - rt);
        }
        else if (nr - r >= K / 2) {
            //cerr << 2 << endl;
            lt = nl - l;
            rt = min(nr - r, K - lt);
        }
        else {
            //cerr << 3 << endl;
            lt = nl - l;
            rt = nr - r;
        }
        string race = "";
        FOR(i, l, l + lt - 1)race += to_string(left[i]) + " ";
        FOR(i, r, r + rt - 1)race += to_string(right[i]) + " ";
        cout << race << endl;
        //cerr << "race " << race << endl;
        cout.flush();
        cnt++;
        getline(cin, result);
        //cerr << "result " << result << endl;
        stringstream res{ result };
        string rank;
        ranking = V<int>(lt+rt);
        int j = 0;
        while (getline(res, rank, ' ')) {
            ranking[j] = stoi(rank);
            j++;
        }
        if (ranking[lt-1]< ranking[lt+rt-1]) {
            //r += ranking[lt - 1] - (lt - 1);
            //l += lt;
            int lc = 0, rc = 0;
            while (lc < lt) {
                if (ranking[lc] < ranking[lt+rc]) {
                    array[m] = left[l];
                    m++; lc++; l++;
                }
                else {
                    array[m] = right[r];
                    m++; rc++; r++;
                }
            }
        }
        else {
            //l += ranking[lt+rt - 1] - (rt - 1);
            //r += rt;
            int lc = 0, rc = 0;
            while (rc < rt) {
                if (ranking[lc] < ranking[lt + rc]) {
                    array[m] = left[l];
                    m++; lc++; l++;
                }
                else {
                    array[m] = right[r];
                    m++; rc++; r++;
                }
            }
        }
        
    }
    if (r == nr) {
        FOR(i, l, nl - 1) {
            array[m] = left[i];
            m++;
        }
    }
    else {
        FOR(i, r, nr - 1) {
            array[m] = right[i];
            m++;
        }
    }
    //cerr << "zan ";
    //REP(i, N)cerr << ans[i] << " ";
    //cerr << endl;
}

void mergesort(V<int> &array,int l,int r ) {
    //cerr << l << " " << r << endl;
    if (r - l+1 > K) {
        int m = l + (r - l) / 2;

        mergesort(array, l, m);
        mergesort(array, m + 1, r);
        merge(array, l, m, r);
    }
    else if(r-l+1>=2){
        V<P> middle(r - l + 1);
        string race = "";
        REP(i, r - l + 1) {
            middle[i].second = array[l + i];
            race += to_string(array[l + i]) + " ";
        }
        cout << race << endl;
        cout.flush();
        cnt++;
        getline(cin, result);
        stringstream res{ result };
        string rank;
        int j = 0;
        while (getline(res, rank, ' ')) {
            middle[j].first = stoi(rank);
            j++;
        }
        sort(ALL(middle));
        REP(i, r - l + 1)array[l + i] = middle[i].second;

    }
}

void mergesort2(V<int>& array, int l, int r) {
    //cerr << l << " " << r << endl;
    int num = (r + 1 + K - 1) / K;
    V<P> stop(num);
    int j = 0;
    REP(i, num) {
        j = min(r, K*(i+1)-1);
        stop[i] = MP(i*K,j);
    }
    REP(i, num) {
        if (stop[i].second - stop[i].first + 1 == 1)continue;
        int l = stop[i].first, r = stop[i].second;
        V<P> middle(r - l + 1);
        string race = "";
        REP(i, r - l + 1) {
            middle[i].second = array[l + i];
            race += to_string(array[l + i]) + " ";
        }
        cout << race << endl;
        cout.flush();
        cnt++;
        getline(cin, result);
        stringstream res{ result };
        string rank;
        int j = 0;
        while (getline(res, rank, ' ')) {
            middle[j].first = stoi(rank);
            j++;
        }
        sort(ALL(middle));
        REP(i, r - l + 1)array[l + i] = middle[i].second;
    }

    int roop = 0;
    while (SZ(stop) > 1) {
        V<P> ds;
        if (roop % 2 == 0) {
            for (int i = 1; i < SZ(stop); i += 2) {
                merge(array, stop[i - 1].first, stop[i - 1].second, stop[i].second);
                ds.emplace_back(stop[i - 1].first, stop[i].second);
            }
            if (SZ(stop) % 2 == 1)ds.emplace_back(stop[SZ(stop) - 1]);
        }
        else {
            for (int i = SZ(stop) - 1; i >= 1; i -= 2) {
                merge(array, stop[i - 1].first, stop[i - 1].second, stop[i].second);
                ds.emplace_back(stop[i - 1].first, stop[i].second);
            }
            if (SZ(stop) % 2 == 1)ds.emplace_back(stop[0]);
            reverse(ALL(ds));
        }

        roop++;
        swap(stop, ds);
    }

}

void update() {
    REP(i, SZ(juni)) {
        REP(j, i) {
            int x = juni[j].second;
            int y = juni[i].second;
            if (check[x][y] != 0)continue;
            check[x][y]++;
            check[y][x]--;
            confirm[x].second++;
            confirm[y].first++;
        }
    }
    REP(i, SZ(juni) - 1) {
        int x = juni[i].second;
        int y = juni[i + 1].second;
        REP(j, N) {
            if (j == x || j == y || (check[y][j] != 0 && check[x][j] != 0))continue;
            if (check[x][j] < 0 && check[y][j] == 0) {
                check[y][j]--;
                check[j][y]++;
                confirm[y].first++;
                confirm[j].second++;
            }
            else if (check[x][j] == 0 && check[y][j] > 0) {
                check[x][j]++;
                check[j][x]--;
                confirm[x].second++;
                confirm[j].first++;
            }
        }
    }
}

void update2(V<int> &high,V<int> &low,int pivot) {
    REP(i, SZ(high)) {
        int x = high[i];
        int y = pivot;
        REP(j, N) {
            if (j == x || j == y || (check[y][j] != 0 && check[x][j] != 0))continue;
            if (check[x][j] < 0 && check[y][j] == 0) {
                check[y][j]--;
                check[j][y]++;
                confirm[y].first++;
                confirm[j].second++;
            }
            else if (check[x][j] == 0 && check[y][j] > 0) {
                check[x][j]++;
                check[j][x]--;
                confirm[x].second++;
                confirm[j].first++;
            }
        }
    }
    REP(i, SZ(low)) {
        int x = pivot;
        int y = low[i];
        REP(j, N) {
            if (j == x || j == y || (check[y][j] != 0 && check[x][j] != 0))continue;
            if (check[x][j] < 0 && check[y][j] == 0) {
                check[y][j]--;
                check[j][y]++;
                confirm[y].first++;
                confirm[j].second++;
            }
            else if (check[x][j] == 0 && check[y][j] > 0) {
                check[x][j]++;
                check[j][x]--;
                confirm[x].second++;
                confirm[j].first++;
            }
        }
    }
}

int partition(V<int>& array, int l, int r) {
    int i = l - 1;
    int m = l;
    int mt = K - 1;
    string race = to_string(array[r]) + " ";
    REP(i, mt) {
        race += to_string(array[m + i]) + " ";
    }
    //cerr << "race " << race << endl;
    cout << race << endl;
    cout.flush();
    cnt++;
    getline(cin, result);
    //cerr << "result " << result << endl;
    stringstream res{ result };
    string rank;
    ranking = V<int>(mt + 1);
    int j = 0;
    while (getline(res, rank, ' ')) {
        ranking[j] = stoi(rank);
        j++;
    }
    REP(j, mt+1){
        if (j == 0)continue;
        if (ranking[j] == K / 2) {
            swap(array[l + j-1], array[r]);
            swap(ranking[j], ranking[0]);
            break;
        }
    }
    FOR(j, 1, mt) {
        if (ranking[j] < ranking[0]) {
            i++;
            swap(array[i], array[m + j - 1]);
        }
    }

    m += mt;

    while (m < r) {
        int mt = min(K - 1, r - m);
        string race = to_string(array[r])+" ";
        REP(i, mt) {
            race += to_string(array[m + i]) + " ";
        }
        cout << race << endl;
        cout.flush();
        //cerr << "race " << race << endl;
        cnt++;
        getline(cin, result);
        //cerr << "result " << result << endl;
        stringstream res{ result };
        string rank;
        ranking = V<int>(mt + 1);
        int j = 0;
        while (getline(res, rank, ' ')) {
            ranking[j] = stoi(rank);
            j++;
        }
        FOR(j, 1, mt) {
            if (ranking[j] < ranking[0]) {
                i++;
                swap(array[i], array[m + j - 1]);
            }
        }
        m += mt;
    }
    swap(array[i + 1], array[r]);
    return (i + 1);
}

int partition2(V<int>& array, int l, int r) {
    int m = l,mt;
    int pivot;
    juni = V<P>();
    V<int> high, low;
    bool ok = false;
    int ma = 0, diff = INF;
    FOR(i, l, r) {
        int hi = 0, lo = 0;
        FOR(j, l, r) {
            if (i == j || check[array[i]][array[j]] == 0)continue;
            if (check[array[i]][array[j]] > 0)lo++;
            else hi++;
        }
        if(abs(hi-lo)<=K/4&&ma<lo+hi){
            ma = lo + hi;
            diff = abs(hi - lo);
            pivot = array[i];
        }
    }
    //cerr << ma <<" "<<diff<< endl;
    //if(check[array[l]][array[l+1]]==0){
    if (ma==0) {
        string race = "";
        FOR(i, l,min(r,l+K-1)) {
            race += to_string(array[i]) + " ";
            juni.push_back(MP(0,array[i]));
        }
        mt = SZ(juni);
        int k = r;
        while (SZ(juni) < K && (k < l - 1 || r <= k)) {
            k++;
            if (k == N) {
                k = -1;
                continue;
            }
            if (confirm[array[k]].first + confirm[array[k]].second == N - 1) {
                continue;
            }
            juni.emplace_back(0, array[k]);
            race += to_string(array[k]) + " ";
        }
        cout << race << endl;
        cerr << "race0 " << race << endl;
        cout.flush();
        cnt++;
        getline(cin, result);
        //cerr << "result " << result << endl;
        stringstream res{ result };
        string rank;
        int j = 0;
        while (getline(res, rank, ' ')) {
            juni[j].first = stoi(rank);
            j++;
        }
        sort(juni.begin(),juni.begin()+mt-1);
        REP(j, mt) {
            if (j < mt / 2)high.push_back(juni[j].second);
            else if (j == K / 2)pivot = juni[j].second;
            else low.push_back(juni[j].second);
        }
        m += mt;
        sort(ALL(juni));
        update();
    }
    /*
    else {
        if (check[array[l]][array[l + 1]] > 0) {
            high.push_back(array[l]);
            pivot = array[l + 1];
        }
        else {
            high.push_back(array[l+1]);
            pivot = array[l];
        }
        m += 2;
    }
    */
    
    while (m <= r) {
        //int mt = min(K - 1, r - m+1);
        juni = V<P>();
        juni.emplace_back(0,pivot);
        string race = to_string(pivot) + " ";
        int p = m;
        while (SZ(juni) < K && p <= r) {
            if (pivot == array[p]) {
                p++;
                continue;
            }
            if (check[pivot][array[p]] == 0) {
                race += to_string(array[p]) + " ";
                juni.emplace_back(0,array[p]);
            }
            else {
                if (check[pivot][array[p]] > 0)low.push_back(array[p]);
                else high.push_back(array[p]);
            }
            p++;
        }
        int mt = SZ(juni)-1;
        int k = r;
        while (SZ(juni) < K && (k < m - 1 || r <= k)) {
            k++;
            if (k == N) {
                k = -1;
                continue;
            }
            if (array[k]==pivot||confirm[array[k]].first + confirm[array[k]].second == N - 1) {
                continue;
            }
            juni.emplace_back(0, array[k]);
            race += to_string(array[k]) + " ";
        }
        if (SZ(juni) == 1) {
            m = p;
            continue;
        }
        cout << race << endl;
        cerr << "race1 " << race << endl;
        cout.flush();
        cnt++;
        getline(cin, result);
        //cerr << "result " << result << endl;
        stringstream res{ result };
        string rank;
        int j = 0;
        while (getline(res, rank, ' ')) {
            juni[j].first = stoi(rank);
            j++;
        }
        FOR(j, 1, mt) {
            if (juni[j].first < juni[0].first)high.push_back(juni[j].second);
            else low.push_back(juni[j].second);
        }
        sort(ALL(juni));
        update();
        m =p;
    }
    update2(high, low, pivot);
    REP(i,SZ(high)){
        array[l + i] = high[i];
    }
    array[l + SZ(high)] = pivot;
    REP(i, SZ(low)) {
        array[l + SZ(high) + 1 + i] = low[i];
    }
    //cerr << SZ(high) << " " << SZ(low) << endl;
    //REP(i, N)cerr << array[i] << " ";
    //cerr << endl;
    return (l + SZ(high));
}

void quicksort(V<int> &array, int l, int r) {
    if (r - l + 1 <= 1)return;
    //cerr << l << " " << r << endl;
    //REP(i, N)cerr << ans[i] << " ";
    //cerr << endl;
    bool ok = false;
    int pivot;
    FOR(i, l, r) {
        if (confirm[array[i]].first + confirm[array[i]].second == N - 1) {
            pivot = array[i];
            ok = true;
            break;
        }
    }
    if (ok) {
        V<int> high, low;
        //cerr << "pivot " << pivot << endl;
        FOR(i, l, r) {
            if (check[pivot][array[i]] == 0)continue;
            if (check[pivot][array[i]] > 0)low.push_back(array[i]);
            else high.push_back(array[i]);
        }
        
        REP(i, SZ(high))array[l + i] = high[i];
        array[l + SZ(high)] = pivot;
        REP(i, SZ(low))array[l + SZ(high) + 1 + i] = low[i];

        update2(high,low,pivot);

        pivot = l + SZ(high);

        quicksort(array, l, pivot - 1);
        quicksort(array, pivot+ 1, r);
    }
    else if (r - l + 1 > K) {
        pivot =partition2(array,l,r);
        //cerr << pivot - l << " " << r - pivot << endl;
        quicksort(array, l, pivot - 1);
        quicksort(array, pivot + 1, r);
    }
    else if (r - l + 1 >= 2) {
        juni = V<P>(r - l + 1);
        string race = "";
        REP(i, r - l + 1) {
            juni[i].second = array[l + i];
            race += to_string(array[l + i]) + " ";
        }
        int k = r;
        while (SZ(juni) < K && (k<l-1 || r<=k)){
            k++;
            if (k == N) {
                k = -1;
                continue;
            }
            if (confirm[array[k]].first + confirm[array[k]].second == N - 1) {
                continue;
            }
            juni.emplace_back(0, array[k]);
            race += to_string(array[k])+" ";
        }
        cout << race << endl;
        //cerr << "race2 " << race << endl;
        cout.flush();
        cnt++;
        getline(cin, result);
        //cerr << "result " << result << endl;
        stringstream res{ result };
        string rank;
        int j = 0;
        while (getline(res, rank, ' ')) {
            juni[j].first = stoi(rank);
            j++;
        }
        sort(juni.begin(), juni.begin() + r -l+1);
        REP(i, r - l + 1)array[l + i] = juni[i].second;
        sort(ALL(juni));
        update();
    }
}

void quicksort2(V<int>& array) {
    queue<P> q;
    q.push(MP(0, N - 1));

    //priority_queue<pair<int,P>> q;
    //q.push(MP(N,MP(0, N - 1)));
    while (!q.empty()) {
        int l, r;
        tie(l, r) = q.front();
        //tie(l, r) = q.top().second;
        //cerr << "turn : " <<q.top().first<<" "<< l << " " << r << endl;
        q.pop();
        if (r - l + 1 <= 1)continue;
        //REP(i, N)cerr << ans[i] << " ";
        //cerr << endl;
        bool ok = false;
        int pivot;
        /*
        FOR(i, l, r) {
            if (confirm[array[i]].first + confirm[array[i]].second == N - 1) {
                pivot = array[i];
                ok = true;
                break;
            }
        }
        */
        FOR(i, l, r) {
            int ma = 0;
            FOR(j, l, r) {
                if (i == j)continue;
                if (check[array[i]][array[j]] != 0)ma++;
            }
            if (ma == r - l) {
                ok = true;
                pivot = array[i];
            }
        }

        if (ok) {
            V<int> high, low;
            //cerr << "pivot " << pivot << endl;
            FOR(i, l, r) {
                if (check[pivot][array[i]] == 0)continue;
                if (check[pivot][array[i]] > 0)low.push_back(array[i]);
                else high.push_back(array[i]);
            }

            REP(i, SZ(high))array[l + i] = high[i];
            array[l + SZ(high)] = pivot;
            REP(i, SZ(low))array[l + SZ(high) + 1 + i] = low[i];

            update2(high, low, pivot);

            pivot = l + SZ(high);
            if(l<pivot-1)shuffle(array.begin() + l, array.begin() + pivot-1, rng);
            if(pivot+1<r)shuffle(array.begin() + pivot + 1, array.begin() + r, rng);
            q.push(MP(l, pivot - 1));
            q.push(MP(pivot + 1, r));
            //q.push(MP(pivot-1-l+1,MP(l, pivot - 1)));
            //q.push(MP(r-(pivot+1)+1,MP(pivot + 1, r)));
        }
        else if (r - l + 1 >= 2) {
            pivot = partition2(array, l, r);
            //cerr << pivot - l << " " << r - pivot << endl;
            if (l < pivot - 1)shuffle(array.begin() + l, array.begin() + pivot - 1, rng);
            if (pivot + 1 < r)shuffle(array.begin() + pivot + 1, array.begin() + r, rng);
            q.push(MP(l, pivot - 1));
            q.push(MP(pivot + 1, r));
            //q.push(MP(pivot - 1 - l + 1, MP(l, pivot - 1)));
            //q.push(MP(r - (pivot + 1) + 1, MP(pivot + 1, r)));
        }
        /*
        else if (r - l + 1 >= 2) {
            juni = V<P>(r - l + 1);
            string race = "";
            REP(i, r - l + 1) {
                juni[i].second = array[l + i];
                race += to_string(array[l + i]) + " ";
            }
            int k = r;
            while (SZ(juni) < K && (k < l - 1 || r <= k)) {
                k++;
                if (k == N) {
                    k = -1;
                    continue;
                }
                if (confirm[array[k]].first + confirm[array[k]].second == N - 1) {
                    continue;
                }
                juni.emplace_back(0, array[k]);
                race += to_string(array[k]) + " ";
            }
            cout << race << endl;
            //cerr << "race2 " << race << endl;
            cout.flush();
            cnt++;
            getline(cin, result);
            //cerr << "result " << result << endl;
            stringstream res{ result };
            string rank;
            int j = 0;
            while (getline(res, rank, ' ')) {
                juni[j].first = stoi(rank);
                j++;
            }
            sort(juni.begin(), juni.begin() + r - l + 1);
            REP(i, r - l + 1)array[l + i] = juni[i].second;
            sort(ALL(juni));
            update();
        }
        */
        //cerr << "turn : " << l << " " << r << endl;

        //cerr << "cnt  : " << cnt << endl;
        //REP(i, N)cerr <<i<<": "<<array[i]<<" "<< confirm[array[i]].first << " " << confirm[array[i]].second << endl;
        //cerr << endl;
    }
}

signed main() {
    cin.tie(0);
    ios::sync_with_stdio(false);
    cout << fixed << setprecision(15);

    cin >> N >> K;
    cerr << N << " " << K << endl;

    getline(cin, result); // read the endline

    check = VV<int>(N, V<int>(N));
    ans = V<int>(N);
    confirm = V<P>(N, MP(0, 0));
    REP(i, N)ans[i] = i;
    quicksort2(ans);
    /*
    REP(i, N) {
        REP(j, N) {
            //cerr << check[i][j] << " ";
            if (check[i][j] > 0)cerr << "+ ";
            else if (check[i][j] == 0)cerr << "0 ";
            else cerr << "- ";
        }
        cerr << endl;
    }
    cerr << endl;
    */
    //REP(i, N)if(confirm[i].first + confirm[i].second!=N-1)cerr << ans[i] << endl;
    //REP(i, N)cerr << ans[i] << " ";
    //cerr << endl;
    REP(i, N)cout << ans[i] << " ";
    cout << endl;
    cout.flush();

}
=======
//#pragma GCC optimize("Ofast")
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

mt19937 rng;
V<int> ans,ranking;
V<P> juni,confirm;
VV<int> check;
string result;
int N, K, cnt = 0;

void selection() {
    //selection sort
    REP(i, N) {
        int pos = i;
        int zan = i;
        int next = i + 1;
        while (next < N) {
            int num = min(N - 1, next + K - 2) - next + 2;
            string race = to_string(ans[zan]) + " ";
            FOR(j, next, min(N - 1, next + K - 2)) {
                race += to_string(ans[j]) + " ";
            }
            cout << race << endl;
            cout.flush();
            cnt++;
            getline(cin, result);
            stringstream res{ result };
            string rank;
            int j = 0;
            while (getline(res, rank, ' ')) {
                if (rank == "0") {
                    if (j == 0)break;
                    zan = next + j - 1;
                    break;
                }
                j++;
            }
            //cerr << zan << " " << next << " " << cnt << endl;
            next = min(N - 1, next + K - 2) + 1;
        }

        if (pos != zan)swap(ans[pos], ans[zan]);
    }
    REP(i, N)cout << ans[i] << " ";
    cout << endl;
    cout.flush();

    return;
}

void merge(V<int> &array,int l,int m,int r) {
    int nl = m - l + 1;
    int nr = r - m;
    //cerr << l << " " << m << " " << r <<endl;

    V<int> left(nl), right(nr);
    REP(i, nl)left[i] = array[l + i];
    REP(i, nr)right[i] = array[m + 1 + i];

    m = l;
    l = 0;
    r = 0;
    while (l != nl && r!= nr) {
        int lt, rt;
        if (nl - l >= (K + 1) / 2 && nr - r >= K / 2) {
            //cerr << 0 << endl;
            lt = (K + 1) / 2;
            rt = K / 2;
        }
        else if (nl - l >= (K + 1) / 2) {
            //cerr << 1 << endl;
            rt = nr - r;
            lt = min(nl - l, K - rt);
        }
        else if (nr - r >= K / 2) {
            //cerr << 2 << endl;
            lt = nl - l;
            rt = min(nr - r, K - lt);
        }
        else {
            //cerr << 3 << endl;
            lt = nl - l;
            rt = nr - r;
        }
        string race = "";
        FOR(i, l, l + lt - 1)race += to_string(left[i]) + " ";
        FOR(i, r, r + rt - 1)race += to_string(right[i]) + " ";
        cout << race << endl;
        //cerr << "race " << race << endl;
        cout.flush();
        cnt++;
        getline(cin, result);
        //cerr << "result " << result << endl;
        stringstream res{ result };
        string rank;
        ranking = V<int>(lt+rt);
        int j = 0;
        while (getline(res, rank, ' ')) {
            ranking[j] = stoi(rank);
            j++;
        }
        if (ranking[lt-1]< ranking[lt+rt-1]) {
            //r += ranking[lt - 1] - (lt - 1);
            //l += lt;
            int lc = 0, rc = 0;
            while (lc < lt) {
                if (ranking[lc] < ranking[lt+rc]) {
                    array[m] = left[l];
                    m++; lc++; l++;
                }
                else {
                    array[m] = right[r];
                    m++; rc++; r++;
                }
            }
        }
        else {
            //l += ranking[lt+rt - 1] - (rt - 1);
            //r += rt;
            int lc = 0, rc = 0;
            while (rc < rt) {
                if (ranking[lc] < ranking[lt + rc]) {
                    array[m] = left[l];
                    m++; lc++; l++;
                }
                else {
                    array[m] = right[r];
                    m++; rc++; r++;
                }
            }
        }
        
    }
    if (r == nr) {
        FOR(i, l, nl - 1) {
            array[m] = left[i];
            m++;
        }
    }
    else {
        FOR(i, r, nr - 1) {
            array[m] = right[i];
            m++;
        }
    }
    //cerr << "zan ";
    //REP(i, N)cerr << ans[i] << " ";
    //cerr << endl;
}

void mergesort(V<int> &array,int l,int r ) {
    //cerr << l << " " << r << endl;
    if (r - l+1 > K) {
        int m = l + (r - l) / 2;

        mergesort(array, l, m);
        mergesort(array, m + 1, r);
        merge(array, l, m, r);
    }
    else if(r-l+1>=2){
        V<P> middle(r - l + 1);
        string race = "";
        REP(i, r - l + 1) {
            middle[i].second = array[l + i];
            race += to_string(array[l + i]) + " ";
        }
        cout << race << endl;
        cout.flush();
        cnt++;
        getline(cin, result);
        stringstream res{ result };
        string rank;
        int j = 0;
        while (getline(res, rank, ' ')) {
            middle[j].first = stoi(rank);
            j++;
        }
        sort(ALL(middle));
        REP(i, r - l + 1)array[l + i] = middle[i].second;

    }
}

void mergesort2(V<int>& array, int l, int r) {
    //cerr << l << " " << r << endl;
    int num = (r + 1 + K - 1) / K;
    V<P> stop(num);
    int j = 0;
    REP(i, num) {
        j = min(r, K*(i+1)-1);
        stop[i] = MP(i*K,j);
    }
    REP(i, num) {
        if (stop[i].second - stop[i].first + 1 == 1)continue;
        int l = stop[i].first, r = stop[i].second;
        V<P> middle(r - l + 1);
        string race = "";
        REP(i, r - l + 1) {
            middle[i].second = array[l + i];
            race += to_string(array[l + i]) + " ";
        }
        cout << race << endl;
        cout.flush();
        cnt++;
        getline(cin, result);
        stringstream res{ result };
        string rank;
        int j = 0;
        while (getline(res, rank, ' ')) {
            middle[j].first = stoi(rank);
            j++;
        }
        sort(ALL(middle));
        REP(i, r - l + 1)array[l + i] = middle[i].second;
    }

    int roop = 0;
    while (SZ(stop) > 1) {
        V<P> ds;
        if (roop % 2 == 0) {
            for (int i = 1; i < SZ(stop); i += 2) {
                merge(array, stop[i - 1].first, stop[i - 1].second, stop[i].second);
                ds.emplace_back(stop[i - 1].first, stop[i].second);
            }
            if (SZ(stop) % 2 == 1)ds.emplace_back(stop[SZ(stop) - 1]);
        }
        else {
            for (int i = SZ(stop) - 1; i >= 1; i -= 2) {
                merge(array, stop[i - 1].first, stop[i - 1].second, stop[i].second);
                ds.emplace_back(stop[i - 1].first, stop[i].second);
            }
            if (SZ(stop) % 2 == 1)ds.emplace_back(stop[0]);
            reverse(ALL(ds));
        }

        roop++;
        swap(stop, ds);
    }

}

void update() {
    REP(i, SZ(juni)) {
        REP(j, i) {
            int x = juni[j].second;
            int y = juni[i].second;
            if (check[x][y] != 0)continue;
            check[x][y]++;
            check[y][x]--;
            confirm[x].second++;
            confirm[y].first++;
        }
    }
    REP(i, SZ(juni) - 1) {
        int x = juni[i].second;
        int y = juni[i + 1].second;
        REP(j, N) {
            if (j == x || j == y || (check[y][j] != 0 && check[x][j] != 0))continue;
            if (check[x][j] < 0 && check[y][j] == 0) {
                check[y][j]--;
                check[j][y]++;
                confirm[y].first++;
                confirm[j].second++;
            }
            else if (check[x][j] == 0 && check[y][j] > 0) {
                check[x][j]++;
                check[j][x]--;
                confirm[x].second++;
                confirm[j].first++;
            }
        }
    }
}

void update2(V<int> &high,V<int> &low,int pivot) {
    REP(i, SZ(high)) {
        int x = high[i];
        int y = pivot;
        REP(j, N) {
            if (j == x || j == y || (check[y][j] != 0 && check[x][j] != 0))continue;
            if (check[x][j] < 0 && check[y][j] == 0) {
                check[y][j]--;
                check[j][y]++;
                confirm[y].first++;
                confirm[j].second++;
            }
            else if (check[x][j] == 0 && check[y][j] > 0) {
                check[x][j]++;
                check[j][x]--;
                confirm[x].second++;
                confirm[j].first++;
            }
        }
    }
    REP(i, SZ(low)) {
        int x = pivot;
        int y = low[i];
        REP(j, N) {
            if (j == x || j == y || (check[y][j] != 0 && check[x][j] != 0))continue;
            if (check[x][j] < 0 && check[y][j] == 0) {
                check[y][j]--;
                check[j][y]++;
                confirm[y].first++;
                confirm[j].second++;
            }
            else if (check[x][j] == 0 && check[y][j] > 0) {
                check[x][j]++;
                check[j][x]--;
                confirm[x].second++;
                confirm[j].first++;
            }
        }
    }
}

int partition(V<int>& array, int l, int r) {
    int i = l - 1;
    int m = l;
    int mt = K - 1;
    string race = to_string(array[r]) + " ";
    REP(i, mt) {
        race += to_string(array[m + i]) + " ";
    }
    //cerr << "race " << race << endl;
    cout << race << endl;
    cout.flush();
    cnt++;
    getline(cin, result);
    //cerr << "result " << result << endl;
    stringstream res{ result };
    string rank;
    ranking = V<int>(mt + 1);
    int j = 0;
    while (getline(res, rank, ' ')) {
        ranking[j] = stoi(rank);
        j++;
    }
    REP(j, mt+1){
        if (j == 0)continue;
        if (ranking[j] == K / 2) {
            swap(array[l + j-1], array[r]);
            swap(ranking[j], ranking[0]);
            break;
        }
    }
    FOR(j, 1, mt) {
        if (ranking[j] < ranking[0]) {
            i++;
            swap(array[i], array[m + j - 1]);
        }
    }

    m += mt;

    while (m < r) {
        int mt = min(K - 1, r - m);
        string race = to_string(array[r])+" ";
        REP(i, mt) {
            race += to_string(array[m + i]) + " ";
        }
        cout << race << endl;
        cout.flush();
        //cerr << "race " << race << endl;
        cnt++;
        getline(cin, result);
        //cerr << "result " << result << endl;
        stringstream res{ result };
        string rank;
        ranking = V<int>(mt + 1);
        int j = 0;
        while (getline(res, rank, ' ')) {
            ranking[j] = stoi(rank);
            j++;
        }
        FOR(j, 1, mt) {
            if (ranking[j] < ranking[0]) {
                i++;
                swap(array[i], array[m + j - 1]);
            }
        }
        m += mt;
    }
    swap(array[i + 1], array[r]);
    return (i + 1);
}

int partition2(V<int>& array, int l, int r) {
    int m = l,mt;
    int pivot;
    juni = V<P>();
    V<int> high, low;
    bool ok = false;
    int ma = 0, diff = INF;
    FOR(i, l, r) {
        int hi = 0, lo = 0;
        FOR(j, l, r) {
            if (i == j || check[array[i]][array[j]] == 0)continue;
            if (check[array[i]][array[j]] > 0)lo++;
            else hi++;
        }
        if(abs(hi-lo)<=K/4&&ma<lo+hi){
            ma = lo + hi;
            diff = abs(hi - lo);
            pivot = array[i];
        }
    }
    //cerr << ma <<" "<<diff<< endl;
    //if(check[array[l]][array[l+1]]==0){
    if (ma==0) {
        string race = "";
        FOR(i, l,min(r,l+K-1)) {
            race += to_string(array[i]) + " ";
            juni.push_back(MP(0,array[i]));
        }
        mt = SZ(juni);
        int k = r;
        while (SZ(juni) < K && (k < l - 1 || r <= k)) {
            k++;
            if (k == N) {
                k = -1;
                continue;
            }
            if (confirm[array[k]].first + confirm[array[k]].second == N - 1) {
                continue;
            }
            juni.emplace_back(0, array[k]);
            race += to_string(array[k]) + " ";
        }
        cout << race << endl;
        cerr << "race0 " << race << endl;
        cout.flush();
        cnt++;
        getline(cin, result);
        //cerr << "result " << result << endl;
        stringstream res{ result };
        string rank;
        int j = 0;
        while (getline(res, rank, ' ')) {
            juni[j].first = stoi(rank);
            j++;
        }
        sort(juni.begin(),juni.begin()+mt-1);
        REP(j, mt) {
            if (j < mt / 2)high.push_back(juni[j].second);
            else if (j == K / 2)pivot = juni[j].second;
            else low.push_back(juni[j].second);
        }
        m += mt;
        sort(ALL(juni));
        update();
    }
    /*
    else {
        if (check[array[l]][array[l + 1]] > 0) {
            high.push_back(array[l]);
            pivot = array[l + 1];
        }
        else {
            high.push_back(array[l+1]);
            pivot = array[l];
        }
        m += 2;
    }
    */
    
    while (m <= r) {
        //int mt = min(K - 1, r - m+1);
        juni = V<P>();
        juni.emplace_back(0,pivot);
        string race = to_string(pivot) + " ";
        int p = m;
        while (SZ(juni) < K && p <= r) {
            if (pivot == array[p]) {
                p++;
                continue;
            }
            if (check[pivot][array[p]] == 0) {
                race += to_string(array[p]) + " ";
                juni.emplace_back(0,array[p]);
            }
            else {
                if (check[pivot][array[p]] > 0)low.push_back(array[p]);
                else high.push_back(array[p]);
            }
            p++;
        }
        int mt = SZ(juni)-1;
        int k = r;
        while (SZ(juni) < K && (k < m - 1 || r <= k)) {
            k++;
            if (k == N) {
                k = -1;
                continue;
            }
            if (array[k]==pivot||confirm[array[k]].first + confirm[array[k]].second == N - 1) {
                continue;
            }
            juni.emplace_back(0, array[k]);
            race += to_string(array[k]) + " ";
        }
        if (SZ(juni) == 1) {
            m = p;
            continue;
        }
        cout << race << endl;
        cerr << "race1 " << race << endl;
        cout.flush();
        cnt++;
        getline(cin, result);
        //cerr << "result " << result << endl;
        stringstream res{ result };
        string rank;
        int j = 0;
        while (getline(res, rank, ' ')) {
            juni[j].first = stoi(rank);
            j++;
        }
        FOR(j, 1, mt) {
            if (juni[j].first < juni[0].first)high.push_back(juni[j].second);
            else low.push_back(juni[j].second);
        }
        sort(ALL(juni));
        update();
        m =p;
    }
    update2(high, low, pivot);
    REP(i,SZ(high)){
        array[l + i] = high[i];
    }
    array[l + SZ(high)] = pivot;
    REP(i, SZ(low)) {
        array[l + SZ(high) + 1 + i] = low[i];
    }
    //cerr << SZ(high) << " " << SZ(low) << endl;
    //REP(i, N)cerr << array[i] << " ";
    //cerr << endl;
    return (l + SZ(high));
}

void quicksort(V<int> &array, int l, int r) {
    if (r - l + 1 <= 1)return;
    //cerr << l << " " << r << endl;
    //REP(i, N)cerr << ans[i] << " ";
    //cerr << endl;
    bool ok = false;
    int pivot;
    FOR(i, l, r) {
        if (confirm[array[i]].first + confirm[array[i]].second == N - 1) {
            pivot = array[i];
            ok = true;
            break;
        }
    }
    if (ok) {
        V<int> high, low;
        //cerr << "pivot " << pivot << endl;
        FOR(i, l, r) {
            if (check[pivot][array[i]] == 0)continue;
            if (check[pivot][array[i]] > 0)low.push_back(array[i]);
            else high.push_back(array[i]);
        }
        
        REP(i, SZ(high))array[l + i] = high[i];
        array[l + SZ(high)] = pivot;
        REP(i, SZ(low))array[l + SZ(high) + 1 + i] = low[i];

        update2(high,low,pivot);

        pivot = l + SZ(high);

        quicksort(array, l, pivot - 1);
        quicksort(array, pivot+ 1, r);
    }
    else if (r - l + 1 > K) {
        pivot =partition2(array,l,r);
        //cerr << pivot - l << " " << r - pivot << endl;
        quicksort(array, l, pivot - 1);
        quicksort(array, pivot + 1, r);
    }
    else if (r - l + 1 >= 2) {
        juni = V<P>(r - l + 1);
        string race = "";
        REP(i, r - l + 1) {
            juni[i].second = array[l + i];
            race += to_string(array[l + i]) + " ";
        }
        int k = r;
        while (SZ(juni) < K && (k<l-1 || r<=k)){
            k++;
            if (k == N) {
                k = -1;
                continue;
            }
            if (confirm[array[k]].first + confirm[array[k]].second == N - 1) {
                continue;
            }
            juni.emplace_back(0, array[k]);
            race += to_string(array[k])+" ";
        }
        cout << race << endl;
        //cerr << "race2 " << race << endl;
        cout.flush();
        cnt++;
        getline(cin, result);
        //cerr << "result " << result << endl;
        stringstream res{ result };
        string rank;
        int j = 0;
        while (getline(res, rank, ' ')) {
            juni[j].first = stoi(rank);
            j++;
        }
        sort(juni.begin(), juni.begin() + r -l+1);
        REP(i, r - l + 1)array[l + i] = juni[i].second;
        sort(ALL(juni));
        update();
    }
}

void quicksort2(V<int>& array) {
    queue<P> q;
    q.push(MP(0, N - 1));

    //priority_queue<pair<int,P>> q;
    //q.push(MP(N,MP(0, N - 1)));
    while (!q.empty()) {
        int l, r;
        tie(l, r) = q.front();
        //tie(l, r) = q.top().second;
        //cerr << "turn : " <<q.top().first<<" "<< l << " " << r << endl;
        q.pop();
        if (r - l + 1 <= 1)continue;
        //REP(i, N)cerr << ans[i] << " ";
        //cerr << endl;
        bool ok = false;
        int pivot;
        /*
        FOR(i, l, r) {
            if (confirm[array[i]].first + confirm[array[i]].second == N - 1) {
                pivot = array[i];
                ok = true;
                break;
            }
        }
        */
        FOR(i, l, r) {
            int ma = 0;
            FOR(j, l, r) {
                if (i == j)continue;
                if (check[array[i]][array[j]] != 0)ma++;
            }
            if (ma == r - l) {
                ok = true;
                pivot = array[i];
            }
        }

        if (ok) {
            V<int> high, low;
            //cerr << "pivot " << pivot << endl;
            FOR(i, l, r) {
                if (check[pivot][array[i]] == 0)continue;
                if (check[pivot][array[i]] > 0)low.push_back(array[i]);
                else high.push_back(array[i]);
            }

            REP(i, SZ(high))array[l + i] = high[i];
            array[l + SZ(high)] = pivot;
            REP(i, SZ(low))array[l + SZ(high) + 1 + i] = low[i];

            update2(high, low, pivot);

            pivot = l + SZ(high);
            if(l<pivot-1)shuffle(array.begin() + l, array.begin() + pivot-1, rng);
            if(pivot+1<r)shuffle(array.begin() + pivot + 1, array.begin() + r, rng);
            q.push(MP(l, pivot - 1));
            q.push(MP(pivot + 1, r));
            //q.push(MP(pivot-1-l+1,MP(l, pivot - 1)));
            //q.push(MP(r-(pivot+1)+1,MP(pivot + 1, r)));
        }
        else if (r - l + 1 >= 2) {
            pivot = partition2(array, l, r);
            //cerr << pivot - l << " " << r - pivot << endl;
            if (l < pivot - 1)shuffle(array.begin() + l, array.begin() + pivot - 1, rng);
            if (pivot + 1 < r)shuffle(array.begin() + pivot + 1, array.begin() + r, rng);
            q.push(MP(l, pivot - 1));
            q.push(MP(pivot + 1, r));
            //q.push(MP(pivot - 1 - l + 1, MP(l, pivot - 1)));
            //q.push(MP(r - (pivot + 1) + 1, MP(pivot + 1, r)));
        }
        /*
        else if (r - l + 1 >= 2) {
            juni = V<P>(r - l + 1);
            string race = "";
            REP(i, r - l + 1) {
                juni[i].second = array[l + i];
                race += to_string(array[l + i]) + " ";
            }
            int k = r;
            while (SZ(juni) < K && (k < l - 1 || r <= k)) {
                k++;
                if (k == N) {
                    k = -1;
                    continue;
                }
                if (confirm[array[k]].first + confirm[array[k]].second == N - 1) {
                    continue;
                }
                juni.emplace_back(0, array[k]);
                race += to_string(array[k]) + " ";
            }
            cout << race << endl;
            //cerr << "race2 " << race << endl;
            cout.flush();
            cnt++;
            getline(cin, result);
            //cerr << "result " << result << endl;
            stringstream res{ result };
            string rank;
            int j = 0;
            while (getline(res, rank, ' ')) {
                juni[j].first = stoi(rank);
                j++;
            }
            sort(juni.begin(), juni.begin() + r - l + 1);
            REP(i, r - l + 1)array[l + i] = juni[i].second;
            sort(ALL(juni));
            update();
        }
        */
        //cerr << "turn : " << l << " " << r << endl;

        //cerr << "cnt  : " << cnt << endl;
        //REP(i, N)cerr <<i<<": "<<array[i]<<" "<< confirm[array[i]].first << " " << confirm[array[i]].second << endl;
        //cerr << endl;
    }
}

signed main() {
    cin.tie(0);
    ios::sync_with_stdio(false);
    cout << fixed << setprecision(15);

    cin >> N >> K;
    cerr << N << " " << K << endl;

    getline(cin, result); // read the endline

    check = VV<int>(N, V<int>(N));
    ans = V<int>(N);
    confirm = V<P>(N, MP(0, 0));
    REP(i, N)ans[i] = i;
    quicksort2(ans);
    /*
    REP(i, N) {
        REP(j, N) {
            //cerr << check[i][j] << " ";
            if (check[i][j] > 0)cerr << "+ ";
            else if (check[i][j] == 0)cerr << "0 ";
            else cerr << "- ";
        }
        cerr << endl;
    }
    cerr << endl;
    */
    //REP(i, N)if(confirm[i].first + confirm[i].second!=N-1)cerr << ans[i] << endl;
    //REP(i, N)cerr << ans[i] << " ";
    //cerr << endl;
    REP(i, N)cout << ans[i] << " ";
    cout << endl;
    cout.flush();

}
>>>>>>> origin/main
