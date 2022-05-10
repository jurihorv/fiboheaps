#include <bits/stdc++.h>

#define TRACE(x) cerr << #x << " = " << x << endl

#define fi first
#define se second

#define pb push_back
#define mp make_pair

#define FOR(i, a, b) for(int i = (a); i < (b); i++)
#define REP(i, n) FOR(i, 0, n)

using namespace std;

typedef long long ll;
typedef pair<int, int> pii;

template<class Num>
Num mabs(Num x){
  if(x < 0) return -x;
  return x;
}

const int inf = INT_MAX;

void solve(){
  int n, m; cin >> n >> m;
  vector<vector<pii>> edge(n + 1);
  REP(i, m){
    int u, v, c; cin >> u >> v >> c;
    edge[u].pb(mp(v, c));
    edge[v].pb(mp(u, c));
  }
  
  int source; cin >> source;

  vector<int> dist(n + 1, inf);
  dist[source] = 0;
  set<pii> S;
  S.insert(mp(0, source));
  while(!S.empty()){
    pii t = *S.begin();
    S.erase(S.begin());
    
    for(pii &nei : edge[t.se]){
      int ndist = t.fi + nei.se;
      if(ndist >= dist[nei.fi]) continue;
      if(S.find(mp(dist[nei.fi], nei.fi)) != S.end())
        S.erase(S.find(mp(dist[nei.fi], nei.fi)));
      dist[nei.fi] = ndist; 
      S.insert(mp(dist[nei.fi], nei.fi));
    }
  }

  for(int i = 1; i <= n; i++){
    if(dist[i] == inf)
      dist[i] = -1;
    if(i != source)
      cout << dist[i] << " ";
  }

  cout << endl; 
}

int main(){
  int test = 1;
  //cin >> test;
  
  while(test--){
    solve();
  }
  return 0;
}
