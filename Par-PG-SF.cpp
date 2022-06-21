#include "Par-PG-SF.hpp"
//#include "SF-Solution.h"
#include "SF-SolutionM.h"

#include "SF-Functions.h"
#include <omp.h>
#include <algorithm>
#include<bits/stdc++.h>
#include <cstdint>

using namespace std;


void familyOfParallelGreedyAlgorithms(int &nVertex, int &nEdges, int &nTerminals, int *&terminals, vector<pair<int,int>> *&adjList, vector<vector<int > > & pairedTerminals ,string algType,string ratio,string numP,int & cleanedSolution)
{
    
    
    // Initialization
    vector<vector<int> > updatedPairedTerminals;
    vector<pair<int,int>> *adjListCopy;
    updatedPairedTerminals=pairedTerminals;
    adjListCopy=adjList;
    int NumProc=stoi(numP);
    int Ratio=stoi(ratio);
    int clusNum=0;

    cout<<"--------------------------------------------"<<endl;
    cout<<"--------------------------------------------"<<endl;

    cout<<"Algorithm Type: "<<algType<<endl;
    cout<<"Ratio: "<<Ratio<<endl;
    cout<<"Number of Edges: "<<nEdges<<endl;   
    cout<<"Number of Vertices: "<<nVertex<<endl;
    cout<<"Number of Terminal Pairs: "<<nTerminals/2<<endl;
    cout<<"Number of Cores: "<<NumProc<<endl;
    cout<<"--------------------------------------------"<<endl;

   
    SteinerForestSolution solutionGraph(nVertex);
    int flag=0;
    int itr=1;
    omp_set_num_threads(NumProc);
    
    while (flag==0)
    {
        if(algType=="1") clusNum=1;
        else if(algType=="Fixed") {clusNum=nTerminals/(2*Ratio);
		if(updatedPairedTerminals.size()<nTerminals/(2*Ratio)){clusNum=updatedPairedTerminals.size();} }
        else if(algType=="Var") clusNum=updatedPairedTerminals.size()/Ratio;
        else if(algType=="All") clusNum=nTerminals/2;
        
	if (updatedPairedTerminals.size()==1) clusNum=1;

        cout<<"*********** Itration "<<itr++<<" *************** \n";
        cout<<"Number of remaining Terminal Pairs: "<<updatedPairedTerminals.size()<<endl;
        cout<<"Number of Pairs to connect: "<<clusNum<<endl;
        
        int* sitesDist=new int[updatedPairedTerminals.size()];
        vector<vector<vector< int > > > allPath;
        int holdIndex=-1;
        int selectedSite=100000000;
        
        vector<vector< int > > selectedEdges;
        for (int i=0; i<updatedPairedTerminals.size(); i++)
        {
            allPath.push_back(vector<vector< int> > () );
        }
        
#pragma omp parallel for
        for (int i=0; i<updatedPairedTerminals.size(); i++)
        {
            vector<vector< int > > path;
            sitesDist[i]=dijkstra(updatedPairedTerminals[i][0], updatedPairedTerminals[i][1], nVertex, adjListCopy, path);
            allPath[i]=path;
        }
        
        // ------------------------------------------------------------------------------------------
        
        vector<pair<int, int> > vec;
        for (int i=0; i<updatedPairedTerminals.size();i++)
        {
            vec.push_back(make_pair(i, sitesDist[i]));
        }
        sort(vec.begin(), vec.end(), sortByVal2);
        vector<int> selectedIndices;
        if(algType=="Cons" && updatedPairedTerminals.size() <clusNum) clusNum=updatedPairedTerminals.size();
        if(clusNum==0 && updatedPairedTerminals.size()>0) clusNum=updatedPairedTerminals.size();
        for(int i=0; i<clusNum; i++)
        {
            for(int j=0; j<allPath[vec[i].first].size();j++)
                selectedEdges.push_back(allPath[vec[i].first][j]);
            selectedIndices.push_back(vec[i].first);
        }
        
        int numOfSelectedEdges=selectedEdges.size();
        int** selectedEdgesArray = new int*[numOfSelectedEdges];
        for(int i=0;i<numOfSelectedEdges;i++)
        {
            selectedEdgesArray[i]=new int[2];
        }
        
#pragma omp parallel for
        for(int i=0;i<numOfSelectedEdges;i++)
        {
            for(int j=0; j<2;j++)
            {
                selectedEdgesArray[i][0]=selectedEdges[i][0];
                selectedEdgesArray[i][1]=selectedEdges[i][1];
            }
        }
        
        //--------------------------------------------------------------------------------------------
        // Zero out this path, i.e. make its length as 0 and include the path in the solution
        
#pragma omp parallel for
        for (int i=0; i<numOfSelectedEdges; i++) {
            int localW=0;
            for (int j=0; j<adjList[selectedEdgesArray[i][0]].size(); j++) {
                if (adjList[selectedEdgesArray[i][0]][j].first==selectedEdgesArray[i][1]) {
                    localW=adjList[selectedEdgesArray[i][0]][j].second;
                }
            }
            if(localW!=0)
            {
                solutionGraph.addEdge(selectedEdgesArray[i][0],selectedEdgesArray[i][1],localW);
            }
            
            vector< pair<int, int> >::iterator j;
            for (j = adjListCopy[selectedEdgesArray[i][0]].begin(); j != adjListCopy[selectedEdgesArray[i][0]].end(); ++j)
            {
                int weight;
                int v = (*j).first;
                if (v==selectedEdgesArray[i][1]) {
                    weight = (*j).second;
                    (*j).second=0;
                }
            }
            for (j = adjListCopy[selectedEdgesArray[i][1]].begin(); j != adjListCopy[selectedEdgesArray[i][1]].end(); ++j)
            {
                int weight;
                int v = (*j).first;
                if (v==selectedEdgesArray[i][0]) {
                    weight = (*j).second;
                    (*j).second=0;
                }
            }
        }
        //}
        
        sort(selectedIndices.begin(), selectedIndices.end());
        for(int i=0; i<clusNum; i++)
        {
            updatedPairedTerminals.erase(updatedPairedTerminals.begin() + selectedIndices[i]-i);
        }
        
        flag=0;
        for (int i=0; i<pairedTerminals.size(); i++)
        {
            if(solutionGraph.BFS(pairedTerminals[i][0],pairedTerminals[i][1]))
            {
                flag=1;
                cout<<i<<endl;
            }
            else
            {
                flag=0;
                break;
            }
        }
        //Free each sub-array
        for(int i = 0; i < numOfSelectedEdges; ++i) {
            delete[] selectedEdgesArray[i];
        }
        //Free the array of pointers
        delete[] selectedEdgesArray;
        delete [] sitesDist;
    } // end of the while loop
    
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
}

bool sortByVal2(const pair<int, int> &a, const pair<int, int> &b) 
{ 
    return (a.second < b.second);
} 

