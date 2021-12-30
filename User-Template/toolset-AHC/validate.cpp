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
    string fname1 = "./result/";
    fname1 += argv[1];
    int cnt = 0;
    V<ll> score1;
    ifstream ifs1(fname1, ios::in);
    if (!ifs1) {
        cerr << "Error: file1 not opened." << endl;
        return 1;
    }

    string str;
    while (getline(ifs1, str)) {
        if (str.find("Score") != string::npos) {
            V<string> v;
            string t;
            stringstream ss{ str };
            while (getline(ss, t, ' ')) {     // スペース（' '）で区切って，格納
                v.push_back(t);
            }
            ll score = stoll(v[2]);
            if(score==-1)score1.emplace_back(cnt+1);
            cnt++;
        }
    }
    
    if (SZ(score1)==0) {
        cerr << "Scores are adequate." << endl;
        return 1;
    }
    else {
        cerr <<SZ(score1)<< " errors happend int the below seeds" << endl;
        for (ll p : score1)cerr << "[ " << p << " ]" << endl;
    }
    
}