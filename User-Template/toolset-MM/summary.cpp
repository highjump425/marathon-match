#pragma GCC optimize("Ofast")
#include <iostream>
#include <fstream>
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
#define INF 100010001000
#define endl "\n"
#define EPS 1e-9


int main(int argc, char* argv[]) {
    string fname = "./result/";
    fname += argv[1];
    ifstream ifs(fname, ios::in);
    if (!ifs) {
        cerr << "Error: file not opened." << endl;
        return 1;
    }
    string str;
    ll cnt = 0;
    ll sum = 0;
    ll mi = INF;
    ll mid = -1;
    ll ma = 0;
    ll mad = -1;
    while (getline(ifs, str)) {
        if (str.find("Score") != string::npos) {
            V<string> v;
            string t;
            stringstream ss{ str };
            while (getline(ss, t, ' ')) {     // スペース（' '）で区切って，格納
                v.push_back(t);
            }
            ll score = stoll(v[2]);
            if (chmin(mi, score))mid=cnt;
            if (chmax(ma, score))mad = cnt;
            sum += score;
            cnt++;
        }
    }
    cout << "num: " << cnt << endl;
    cout << "ave: " << double(sum) / cnt << endl;
    cout << "min; " << mi << " (seed : " << mid << " )" << endl;
    cout << "max: " << ma << " (seed : " << mad << " )" << endl;
}