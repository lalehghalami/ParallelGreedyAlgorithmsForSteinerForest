#include "PG-SF.hpp"
#include "SF-SolutionM.h"
#include "SF-Functions.h"
#include <omp.h>
#include <algorithm>
#include<bits/stdc++.h>
#include <cstdint>

using namespace std;

void greedyAlgorithm(int &nVertex, int &nEdges, int &nTerminals, int *&terminals, vector<pair<int,int>> *&adjList, vector<vector<int > > & pairedTerminals, int &cleanedSolution)
{
    cout<<"--------------------------------------------"<<endl;
    cout<<"Starting Paird Greedy Algorithm: \n";
    vector<vector<int> > updatedPairedTerminals;
    vector<pair<int,int>> *adjListCopy;
    updatedPairedTerminals=pairedTerminals;
    adjListCopy=adjList;
    
    SteinerForestSolution solutionGraph(nVertex);
    int* sitesDist=new int[nTerminals/2];
    int flag=0;
    
    while (flag==0)
    {
        int holdIndex=-1;
        int selectedSite=10000000;
        vector<vector< int > > selectedEdges;
        for (int i=0; i<updatedPairedTerminals.size(); i++)
        {
            vector<vector< int > > path;
            sitesDist[i]=dijkstra(updatedPairedTerminals[i][0],updatedPairedTerminals[i][1],nVertex, adjListCopy, path);
            if (sitesDist[i]<selectedSite)
            {
                selectedSite=sitesDist[i];
                selectedEdges.clear();
                selectedEdges=path;
                holdIndex=i;
            }
        }
        //--------------------------------------------------------------------------------------------
        // Zero out this path, i.e. make its length as 0 and include the path in the solution
        for (int i=0; i<selectedEdges.size(); i++) {
            int localW=0;
            for (int j=0; j<adjList[selectedEdges[i][0]].size(); j++) {
                if (adjList[selectedEdges[i][0]][j].first==selectedEdges[i][1]) {
                    localW=adjList[selectedEdges[i][0]][j].second;
                }
            }
            
            if(localW!=0) solutionGraph.addEdge(selectedEdges[i][0],selectedEdges[i][1],localW);
            vector< pair<int, int> >::iterator j;
            for (j = adjListCopy[selectedEdges[i][0]].begin(); j != adjListCopy[selectedEdges[i][0]].end(); ++j)
            {
                int weight;
                int v = (*j).first;
                if (v==selectedEdges[i][1]) {
                    weight = (*j).second;
                    (*j).second=0;
                }
            }
            for (j = adjListCopy[selectedEdges[i][1]].begin(); j != adjListCopy[selectedEdges[i][1]].end(); ++j)
            {
                int weight;
                int v = (*j).first;
                if (v==selectedEdges[i][0]) {
                    weight = (*j).second;
                    (*j).second=0;
                }
            }
        }
        
        updatedPairedTerminals.erase(updatedPairedTerminals.begin() + holdIndex); // remove the connected sites from the sites set
        
        flag=0;
        for (int i=0; i<pairedTerminals.size(); i++)
        {
            if(solutionGraph.BFS(pairedTerminals[i][0],pairedTerminals[i][1]))
                flag=1;
            else
            {
                flag=0;
                break;
            }
        }
    } // end of the while loop
    //---------------------------------------------------------------------------------------------------------------------

    updatedPairedTerminals.clear();
cout<<"--------------------------------------------"<<endl;
cout<<"------------ OUT OF WHILE LOOP -------------"<<endl;
cout<<"--------------------------------------------"<<endl;

    //---------------------------------------------------------------------------------------------------------------------
    // Pruning the solution
    // Removes each edge in the solutionGraph, if breaks the connectivity takes it back, by updating its attribute to needed.
    for (int i=0; i<nVertex; i++)
    {
     	for(int j=0; j< nVertex; j++)
        {
            if((solutionGraph.adjSolutionMatrix[i][j].weight!=0)&&(solutionGraph.adjSolutionMatrix[i][j].needed))
            {
             	int holdWeight=solutionGraph.adjSolutionMatrix[i][j].weight;
                solutionGraph.removeEdge(i,j);
                for (int a=0; a<pairedTerminals.size(); a++)
                {
                    if(!(solutionGraph.BFS(pairedTerminals[a][0],pairedTerminals[a][1])))
                    {
                     	solutionGraph.addEdge(i,j,holdWeight);
                    }
                }

            }
	}
    }

    // Compute the cost of the solution by only considering the needed edges in the solution
    for (int i=0; i<pairedTerminals.size(); i++)
    {
     	int sumLocal=0;
        solutionGraph.BFSCleanSolution(pairedTerminals[i][0],pairedTerminals[i][1],sumLocal);
        cleanedSolution=cleanedSolution+sumLocal;
    }

    cout<<"The last line of Parallel PG \n";
    cout <<"Solution cost: "<<cleanedSolution<<endl;
    cout<<"--------------------------------------------"<<endl;
    solutionGraph.deleteSolution();

//////////////////////////////////

    cout<<"The last line of Paird Greedy Algorithm \n";
    cout<<"Solution Seq PG: "<<cleanedSolution<<endl;
    cout<<"--------------------------------------------"<<endl;
//    delete [] sitesDist;
}


bool sortByVal(const pair<int, int> &a, const pair<int, int> &b) 
{ 
    return (a.second < b.second); 
} 

