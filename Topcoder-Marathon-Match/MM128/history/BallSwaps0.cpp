// C++11
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>
#include <set>
#include <string>

using namespace std;

int main() 
{
  int N;
  int C;

  cin >> N;
  cin >> C;
  
  vector< vector<int> > grid(N, vector<int>(N, -1));  
  for (int r=0; r<N; r++)
    for (int c=0; c<N; c++)
      cin >> grid[r][c];
    
  //return one move
  cout << "1" << endl;
  cout << "0 0 0 1" << endl;
  cout.flush();
}