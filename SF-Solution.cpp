#include "SF-Solution.h"
using namespace std;

steinerForest::steinerForest(int V)
{
    this->V = V;
    adj = new vector<node>[V];
}

void steinerForest::addEdge(int v, int w, int wei)
{
    node vNode(v,wei);
    node WNode(w, wei);
    adj[v].push_back(WNode); // Add w to v’s list.
    adj[w].push_back(vNode); // Add v to w’s list.
}

void steinerForest::removeEdge(int v, int w)
{
    for(int i=0;i<adj[v].size();i++)
    {	
	if(adj[v][i].index==w)
	{	
		adj[v].erase(adj[v].begin()+i);
	//	adj[v][i].weight=0;
	//	adj[v][i].needed=false;
	}
    }
    for(int i=0;i<adj[w].size();i++)
    { 
        if(adj[w][i].index==v)
        { 
		adj[w].erase(adj[w].begin()+i);
             //   adj[w][i].weight=0;
            //    adj[w][i].needed=false;
        }
    }
}

void steinerForest::edgeZeroOut(int v, int w)
{
    for(int i=0;i<adj[v].size();i++)
    { 
        if(adj[v][i].index==w)
        { 
           adj[v][i].weight=0;
        }
    }
    for(int i=0;i<adj[w].size();i++)
    {
     	if(adj[w][i].index==v)
        {
           adj[w][i].weight=0;
        }
    }
}


bool steinerForest::BFS(int s,int t)
{
    int u;
    bool *visited = new bool[V];
    for(int i = 0; i < V; i++)
    visited[i] = false;
    
    queue<int> myQueue;
    visited[s] = true;
    myQueue.push(s);
    
    while(!myQueue.empty())
    {
        u = myQueue.front();
        myQueue.pop();
        
        for (int i=0; i< adj[u].size(); i++)
        {
            if ((!visited[adj[u][i].index]) && (adj[u][i].needed))
            {
                visited[adj[u][i].index] = true;
                myQueue.push(adj[u][i].index);
                if (adj[u][i].index==t) return true;
            }
        }
    }
    return false;
}

void steinerForest::BFSCleanSolution(int s,int t, int &sum)
{
    // in this algorithm, it finds the path between each pair in the initial solution and zero out the path for the next pair
    sum=0;
    int u;
    bool *visited = new bool[V];
    int *pred = new int[V];
    int *We = new int[V];
    
    for(int i = 0; i < V; i++){
        visited[i] = false;
        pred[i]=-1;
        We[i]=0;
    }
    
    queue<int> myQueue;
    visited[s] = true;
    myQueue.push(s);
    
    while(!myQueue.empty())
    {
        u = myQueue.front();
        myQueue.pop();
        for (int i=0; i< adj[u].size(); i++)
        {
            if ((!visited[adj[u][i].index]) && (adj[u][i].needed))
            {
                visited[adj[u][i].index] = true;
                pred[adj[u][i].index]=u;
                We[adj[u][i].index]=adj[u][i].weight;
                myQueue.push(adj[u][i].index);
                if (adj[u][i].index==t) {
                    queue<int> empty;
                    swap(myQueue,empty);
                    break;
                }
            }
        }
    }
    vector<vector<int> > pathVec;
/*    
cout<<"We: \n";
   for(int i=0;i<V;i++)
        {
                cout<<We[i]<<" ";
        }
cout<<endl;
*/
    int currentV=t;
    while (pred[currentV]!=-1) {
        sum+=We[currentV];
//       	cout<<"currentV "<<currentV<<": "<<We[currentV]<<endl;

        for (int i=0; i< adj[currentV].size(); i++)
        {
            if ((adj[currentV][i].index==pred[currentV])&&(adj[currentV][i].needed)) {
                vector<int> tPath;
                tPath.push_back(currentV);
                tPath.push_back(adj[currentV][i].index);
//                cout<<"("<<currentV<<","<<adj[currentV][i].index<<")"<<endl;
                pathVec.push_back(tPath);
                edgeZeroOut(currentV,adj[currentV][i].index);
          //      adj[currentV][i].weight=0;
            }
        }
        for (int i=0; i< adj[pred[currentV]].size(); i++)
        {
            if ((adj[pred[currentV]][i].index==currentV)&&(adj[currentV][i].needed)) {
               edgeZeroOut(currentV,adj[currentV][i].index);
               // adj[pred[currentV]][i].weight=0;
            }
        }
        currentV=pred[currentV];
    }

}
