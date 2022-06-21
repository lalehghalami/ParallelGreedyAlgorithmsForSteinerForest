#ifndef SF_SolutionM_h
#define SF_SolutionM_h
#include <iostream>
#include <queue>

using namespace std;

class SteinerForestSolution
{
public:
    
    SteinerForestSolution(int V);
    void addEdge(int v, int w,int wei);
    void removeEdge(int v, int w);
    bool BFS(int s,int t);
    void BFSCleanSolution(int s,int t, int &sum);
    void deleteSolution();
    void edgeZeroOut(int v, int w);

    class node
    {
     	public:
        node()
	{};
        node(int w, bool neededl)
        {
            needed=neededl;
            weight=w;
        }
        bool needed;
        int weight;

    };

    int V;
    node** adjSolutionMatrix;
};

#endif /* SF_Solution_h */
