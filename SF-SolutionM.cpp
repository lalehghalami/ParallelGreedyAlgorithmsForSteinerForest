#include "SF-SolutionM.h"
#include <omp.h>

using namespace std;


SteinerForestSolution::SteinerForestSolution(int V)
{
    this->V = V;
    
    adjSolutionMatrix = new node*[V];
    
    for(int i=0;i<V;i++)
        adjSolutionMatrix[i]=new node[V];
    
    node vNode(0,false);
// For Seq
#pragma omp parallel for
    for (int i=0; i<V; i++) {
        for (int j=0; j<V; j++) {
            adjSolutionMatrix[i][j]=vNode;
        }
    }
}

void SteinerForestSolution::addEdge(int v, int w, int wei)
{
    node vNode(wei,true);
    adjSolutionMatrix[v][w]=vNode;
    adjSolutionMatrix[w][v]=vNode;
}


void SteinerForestSolution::removeEdge(int v, int w)
{
    adjSolutionMatrix[v][w].needed=false;
    adjSolutionMatrix[w][v].needed=false;
}

void SteinerForestSolution::edgeZeroOut(int v, int w)
{
    adjSolutionMatrix[v][w].weight=0;
    adjSolutionMatrix[w][v].weight=0;
}

void SteinerForestSolution::deleteSolution()
{
    //Free each sub-array
    for(int i = 0; i < V; ++i) {
        delete[] adjSolutionMatrix[i];
    }
    //Free the array of pointers
    delete[] adjSolutionMatrix;
}

bool SteinerForestSolution::BFS(int s,int t)
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
        
        for (int i=0; i< V; i++)
        {
            if ((!visited[i]) && (adjSolutionMatrix[u][i].weight!=0) && (adjSolutionMatrix[u][i].needed))
            {
                visited[i] = true;
                myQueue.push(i);
                if (i==t) return true;
            }
        }
    }
    return false;
}

void SteinerForestSolution::BFSCleanSolution(int s,int t, int &sum)
{
    //    cout<<"----------- Calling BFSCleanSolution \n";
    // in this algorithm, it finds the path between each pair in the initial solution and zero out the path for the next pair
    sum=0;
    int u;
    bool *visited = new bool[V];
    int *pred = new int[V];
    int *We = new int[V];

// For Seq
#pragma omp parallel for    
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
        for (int i=0; i< V; i++)
        {
            if ((!visited[i]) &&  (adjSolutionMatrix[u][i].needed))
            {
                visited[i] = true;
                pred[i]=u;
                We[i]=adjSolutionMatrix[u][i].weight;
                myQueue.push(i);
                if (i==t) {
                    queue<int> empty;
                    swap(myQueue,empty);
                    break;
                }
            }
        }
    }
    vector<vector<int> > pathVec;
    
    int currentV=t;
    while (pred[currentV]!=-1) {
        sum+=We[currentV];
        for (int i=0; i< V; i++)
        {
            if ((i==pred[currentV])&&(adjSolutionMatrix[currentV][i].needed)) {
                vector<int> tPath;
                tPath.push_back(currentV);
                tPath.push_back(i);
                pathVec.push_back(tPath);
                edgeZeroOut(currentV,i);
            }
        }
        for (int i=0; i< V; i++)
        {
            if (   (i==currentV) && (adjSolutionMatrix[currentV][i].needed)) {
                edgeZeroOut(currentV,i);
            }
        }
        currentV=pred[currentV];
    }
}
