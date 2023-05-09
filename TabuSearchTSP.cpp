#include <bits/stdc++.h>
#define ll long long
#define pi 3.141592653589793238462
#define big 1000000007
#define fi first
#define se second
#define FOR(i,a,b) for(int i=a; i<=b; ++i)

using namespace std;

int n, c[1005][1005], x[1005], xOPT[1005], fOPT, f, k, tabu[1005][1005], tenure;
struct Edge{
  int u, v, cost;
};
bool operator < (Edge a, Edge b) {
  if(a.cost==b.cost) return a.u<b.u;
  return a.cost > b.cost;
}
int countf(){
  int sum=0;
  FOR(i,1,n-1) sum+=c[x[i]][x[i+1]];
  sum+=c[x[n]][1];
  return sum;
}
void TabusearchTSP(){
  FOR(i,1,n) x[i]=i;
  FOR(i,1,n) xOPT[i]=i;
  f=countf();
  fOPT=f;
  while(k--){
    //cout<<f<<'\n';
    //FOR(i,1,n) cout<<x[i]<<"->"; cout<<1<<'\n';
    FOR(i,2,n) FOR(j,i+1,n) if(tabu[i][j]) tabu[i][j]--;
    priority_queue<Edge> q;
    for(int i=2; i<n; ++i){
      for(int j=i+1; j<=n; ++j){
        swap(x[i],x[j]);
        q.push({i,j,countf()});
        swap(x[i],x[j]);
      }
    }
    while(!q.empty()){
      Edge tmp = q.top();
      q.pop();
      int a1=x[tmp.u], a2=x[tmp.v];
      if(a1>a2) swap(a1,a2);
      if(tabu[a1][a2]){
        swap(x[tmp.u],x[tmp.v]);
        f=countf();
        if(f<fOPT){
          FOR(i,1,n) xOPT[i]=x[i];
          fOPT=f;
          tabu[a1][a2]=tenure;
          break;
        }
        else swap(x[tmp.u],x[tmp.v]);
      }
      else if(!tabu[a1][a2]){
        swap(x[tmp.u],x[tmp.v]);
        f=countf();
        if(f<fOPT){
          FOR(i,1,n) xOPT[i]=x[i];
          fOPT=f;
        }
          tabu[a1][a2]=tenure;
          break;
      }
    }
    while(!q.empty()) q.pop();
  }
}

int init(){
    double x[1005],y[1005],aa;
    FOR(i,1,n){
        cin >> aa;
        cin >> x[i] >> y[i];
    }
    FOR(i,1,n) FOR(j,1,n) {
        c[i][j] = sqrt((x[i]-x[j])*(x[i]-x[j])+(y[i]-y[j])*(y[i]-y[j]));
    }
}
int main(){
    cin >> n >> k >> tenure;
    init();
    TabusearchTSP();
    cout<<fOPT<<'\n';
    FOR(i,1,n) cout<<xOPT[i]<<"->"; cout<<1;
    return 0;
}
