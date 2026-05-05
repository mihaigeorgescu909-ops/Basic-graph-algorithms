/// Algoritmi grafuri

/// biblioteci utilizate:

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <utility>
#include <queue>
#include <functional>

/// macro-uri:
#define inf (1<<30)
#define pii pair<int,int>

using namespace std;

/// Pentru a reprezenta grafurile in memorie vom folosi liste de adiacente
/// implementate folosind containerul std::vector

struct graf_neponderat // graful in care muchiile nu au cost
{
    int n,m;
    vector < vector <int> > v;
    inline void read() // citire
    {
        cin>>n>>m;
        v.assign(n+1, {});
        int x,y;
        while(m--)
        {
            cin>>x>>y;
            v[x].emplace_back(y);
            v[y].emplace_back(x);
        }
    }
    /// DFS
    inline void dfs(int start) // algoritm recursiv de parcurgere "depth-first"
    {
        vector <int> viz(n+1,0);
        function <void(int)> f=[&](int nod)
        {
            viz[nod]=1;
            for(int i : v[nod])
                if(!viz[i]) f(i);
        };
        f(start);
    }

    /// BFS
    inline void bfs(int start) // algoritm iterativ de parcurgere "breadth-first"
    {
        vector <int> dp(n+1,0);
        queue <int> q;  // se va folosi o structura de tip coada pentru a salva nodurile ce urmeaza a fi vizitate
        for(int i=1; i<=n; i++)
            dp[i]=inf;
        q.push(start);
        dp[start]=0;
        while(!q.empty())
        {
            int p=q.front(); q.pop();
            for(int i : v[p]) if(dp[i]>dp[p]+1)
                {
                    dp[i]=dp[p]+1;
                    q.push(i);
                }
        }
    }
};

struct graf_ponderat // graful in care muchiile au cost
{
    int n,m;
    vector < vector <pii> > v;
    inline void read()
    {
        cin>>n>>m;
        v.assign(n+1, {});
        int x,y,c;
        while(m--)
        {
            cin>>x>>y>>c;
            v[x].emplace_back(y,c);
            v[y].emplace_back(x,c);
        }
    }
    /// Roy-Warshall pentru grafuri cu numar mic de noduri
    vector < vector <int> > d;
    inline void roy() // algoritm in complexitate O(n^3) pentru calcularea tuturor distantelor minime din graf
    {
        d.assign(n+1, vector <int>(n+1,0));
        for(int i=1; i<=n; i++)
            for(int j=1; j<=n; j++)
                d[i][j]=inf;
        for(int i=1; i<=n; i++)
            for(auto [j,c] : v[i]) d[i][j]=d[j][i]=c;
        for(int k=1; k<=n; k++)
            for(int i=1; i<=n; i++)
                for(int j=1; j<=n; j++)
                    if(d[i][j]>d[i][k]+d[k][j])
                        d[i][j]=d[i][k]+d[k][j];
    }

    /// Dijkstra
    inline void dijkstra(int start)
    {
        vector <int> dp(n+1,0);
        priority_queue < pii, vector <pii>, greater <pii> > pq; // se foloseste o structura de tip priority queue pentru a obtine complexitatea O(n*log(n))
        pq.push({0,start});
        for(int i=1; i<=n; i++)
            dp[i]=inf;
        dp[start]=0;
        while(!pq.empty())
        {
            auto [d,p]=pq.top(); pq.pop(); // nodurile aflate la distanta minima fata de sursa vor fi vizitate primele, pentru a evita vizitarea aceluiasi nod de mai multe ori decat este necesar
            if(d>=dp[p]) continue;
            for(auto [i,c] : v[p]) if(dp[i]>dp[p]+c)
                {
                    dp[i]=dp[p]+c;
                    pq.push({dp[i],i});
                }
        }
    }
};

struct arbore
{
    int n,t[20][200005];
    inline void read()
    {
        cin>>n;
        int x;
        for(int i=2; i<=n; i++)
        {
            cin>>x;
            t[0][i]=x;
        }
    }
    /// t[X][Y] = ascendentul lui Y aflat la distanta 2^X
    inline void lgpow()
    {
        for(int i=1; i<20; i++)
            for(int j=1; j<=n; j++)
                t[i][j]=t[i-1][t[i-1][j]]; // ridicarea la putere a structurii t
    }
    inline int salt(int x, int k)
    {
        for(int i=20; i>=0; i--)
            if(k & (1<<i)) x=t[i][x]; // salturi de lungime putere de 2
        return x;
    }
};

graf_neponderat G1;
graf_ponderat G2;
arbore A;

int main()
{
    /// exemple de apel
    G1.read();
    G2.read();
    A.read();
    G1.dfs(1);
    G1.bfs(1);
    G2.roy();
    return 0;
}
