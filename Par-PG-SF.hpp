#ifndef Par_PG_SF_hpp
#define Par_PG_SF_hpp

#include <fstream>
#include <vector>

using namespace std;


void familyOfParallelGreedyAlgorithms(int &nVertex, int &nEdges, int &nTerminals, int *&terminals, vector<pair<int,int>> *&adjList, vector<vector<int > > & pairedTerminals ,string algType,string ratio,string numP,int & cleanedSolution);

bool sortByVal2(const pair<int, int> &a,const pair<int, int> &b);


#endif /* PG_SF_hpp */
