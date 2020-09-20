#include <iostream>
#include <vector>
#include <queue>
#include <set>
using namespace std;

int main()
{
    
        int n;
        cin>>n;
        string name[n]; /* array for storing names */
        int wins[n]; /* array for storing wins */
        int loss[n]; /* array for storing loss */
        int rem[n];  /* array for storing total remaining matches */
        int infgraph[n][n]; /* matrix for storing matches between i and jth team */
 
        int totalrem = 0; /* Counts the overall remaining matches */
        int maxwins = 0; /* Stores index of the team having maximum wins */ 
        set <int> s; /* store wins+rem matches of a team in increasing order */
        
	/* In the next loop, all the information is stored in the corresponding arrays */

        for(int i=0;i<n;i++)
        {
            cin>>name[i];
            cin>>wins[i];
            cin>>loss[i];
            cin>>rem[i];

            if(wins[i]>maxwins)
            maxwins=wins[i];

            for(int j=0;j<n;j++)
            {
                cin>>infgraph[i][j];
                totalrem+=infgraph[i][j];
            }
        }
        
	/* In the following loop, all the values of (wins + remaining) matches is stored in the set whose sum is greater than the maxwins value. */

        for(int i=0;i<n;i++)
        {
            int tot = wins[i] + rem[i];
            if(tot>=maxwins)
            s.insert(tot);
        }

        totalrem/=2; /* Total remaining matches is halved as each match is counted twice */

        int point = -1; /* This stores the value at which the next for loop breaks */

        bool visit[n]; /* This stores the visited nodes of graph when the for loop breaks */

	/* The next loop iterates on the values of set and calculate whether the max value of total matches(wins + rem) at which a team is eliminated. */
 
    for(auto it = s.begin();it!=s.end();it++)
    {
        vector <pair<int,int > > adj[(n*(n+1))/2 + 2]; /* adjacency list is created to perform Breadth first search (BFS) used in maxflow */
        int k =n+1;

	/* In next loops, adjacency list is filled according to the input */

        for(int i=0;i<n;i++)
        {
            for(int j=i+1;j<n;j++)
            {
                adj[0].push_back(make_pair(k,infgraph[i][j]));
                adj[k].push_back(make_pair(i+1,INT_MAX));
                adj[k].push_back(make_pair(j+1,INT_MAX));
                k++;
            }
        }
        for(int i=1;i<=n;i++)
        adj[i].push_back(make_pair(k,*it-wins[i-1]));
  
    int maxflow = 0; /* Stores maxflow of the graph */
    bool visited[(n*(n+1))/2 + 2]; /* Stores visited nodes during the end of maxflow, used for calculating min cut */
	
    /* The next loop is an infinite loop which breaks when there is no path left from source to sink and maxflow can't be increased further */

    while(1>0)
    {
        queue <int> q; /* Queue is used for performing BFS for searching for a path from source to sink */
        for(int i=0;i<=k;i++)
        visited[i] = false;
        int parent[(n*(n+1))/2 + 2]; /* parent array is created which stores the parent(previous node) index of each node visited in a path */ 
        parent[0] = -1; /* parent of source is initialised with negative value */
	
	/* In next lines, BFS is performed to find an augmenting path from source to sink */
        q.push(0); 
        visited[0] = true;
        while(!q.empty())
        {
            int s = q.front();
            for(int i=0;i<adj[s].size();i++)
            {
                int itr = adj[s][i].first;
                int wt = adj[s][i].second;
                if(wt>0 && !visited[itr])
                {
                    
                    q.push(itr);
                    parent[itr] = s;
                    visited[itr] = true;
                }
            }
            q.pop();
            
        }

	/* In the following if statement, we have checked whether we have got a path by checking the sink in the visited array. If it is there, we have calculated the minimum flow in the path and reduced the capacities by flow and increased opposite edges capacity by flow in the residual graph. The maxflow is increased by flow amount. */
        if(visited[k])
        {
            int i = k;
            
            int minflow = INT_MAX;
            while(i>0)
            {
                for(int j=0;j<adj[parent[i]].size();j++)
                {
                    int itr = adj[parent[i]][j].first;
                    if(itr == i)
                    {
                        int wt = adj[parent[i]][j].second;
                        if(wt<minflow)
                        minflow = wt;
                        break;
                    }
           
                }
                i = parent[i];
            }
            i = k;
            while(i>0)
            {
                for(int j=0;j<adj[parent[i]].size();j++)
                {
                    int itr = adj[parent[i]][j].first;
                    if(itr == i)
                    {
                        adj[parent[i]][j].second-=minflow;
                        break;
                    }
           
                }
                bool flag = true;
                for(int j=0;j<adj[i].size();j++)
                {
                    int itr = adj[i][j].first;
                    if(itr == parent[i])
                    {
                        flag = false;
                        adj[i][j].second+=minflow;
                        break;
                    }
           
                }
                if(flag)
                adj[i].push_back(make_pair(parent[i],minflow));
                i = parent[i];
            }
            maxflow+= minflow;
        }
        else
        break;
     }
	
     /* we have stored the point of set where maxflow exceeds the total remaining which shows that all edges from source is satisfied and we broke the loop */

     if(maxflow>=totalrem)
     {
         point = *it;
         break;
     }
     
     for(int i=0;i<n;i++)
     visit[i] = visited[i+1];
    }
    

    	vector <int> eliminated; /* stores all the teams which are eliminated */
        vector <int> v; /* stores the teams of mincut used for explanations of eliminated teams */

	/* In the next loop, the eliminated teams is found by checking their total matches(wins + rem) is greater than the point or not. The point is calculated using the above for loop */

        for(int i=0;i<n;i++)
        {
           if(wins[i]+rem[i]<point)
           eliminated.push_back(i);
           else if(visit[i])
           v.push_back(i);
        }
    
	/* In the next loop, the output is printed as asked in the question */

        for(int it = 0;it<eliminated.size();it++)
        {
            cout<<name[eliminated[it]]<<" is eliminated."<<"\n";
                cout<<"They can win at most "<<wins[eliminated[it]]<<" + "<<rem[eliminated[it]]<< " = "<<wins[eliminated[it]]+rem[eliminated[it]] <<" games."<<"\n";
                if(v.size()>1)
                {
                    int totalgame = 0;
                    for(int i=0;i<v.size()-1;i++)
                    {
                        cout<<name[v[i]]<<", ";
                        totalgame+= wins[v[i]];
                    }
                    totalgame+=wins[v[v.size()-1]];
                    cout<<"and "<<name[v[v.size()-1]]<<" have won a total of "<<totalgame<<" games."<<"\n";
                    int remgame=0;
                    for(int i=0;i<v.size();i++)
                    {
                        for(int j = i+1;j<v.size();j++)
                        remgame+=infgraph[v[i]][v[j]];
                    }
                    cout<<"They play each other "<<remgame<<" times."<<"\n";
                    cout<<"So on average, each of the teams in this group wins "<<totalgame+remgame<<"/"<<v.size()<< " = "<< float(totalgame+remgame)/float(v.size())<<" games."<<"\n";
                }
                else if(v.size()==1)
                {
                    cout<<name[v[0]]<<" has won a total of "<<wins[v[0]]<<" games."<<"\n";
                    cout<<"They play each other "<<0<<" times."<<"\n";
                    cout<<"So on average, each of the teams in this group wins "<<wins[v[0]]<<"/"<<1<< " = "<<wins[v[0]]<<" games."<<"\n";
                }
                cout<<"\n";
        }
    return 0;
}