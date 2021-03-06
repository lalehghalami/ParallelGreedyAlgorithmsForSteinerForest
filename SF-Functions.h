#ifndef SF_Functions_h
#define SF_Functions_h

#include <stdio.h>
#include <vector>
#include <string>

using namespace std;

// Reading file
void creatInputPath(string &strInputFile,string path,string nFile);
void creatOutputPath(string &strOutputFile, string nFile, string alg, string algType, string ratio, string proc,string run);
void readInputFileAdjList(string str, int &nVertex, int &nEdges, int &nTerminals, int *&terminals, vector<pair<int,int> > *&adj, vector<vector<int > > & sites, vector<vector<int > > & E,double &prob, double &avEdges);
void readInputFileToAdjMatrix(string str, int &nVertex, int &nEdges, int &nTerminals, int *&terminals, vector<vector<int> > & adjMatrix, vector<vector<int > > & sites, vector<vector<int > > & E, double &prob, double &avEdges);


//Greedy

int dijkstra(int source, int sink,int nVertex, vector<pair<int,int> > *&adj, vector<vector< int > > &path);
int dijkstraWithAdjMatrix(int source, int sink,int nVertex, vector<vector<int> > &adjMatrix, vector<vector< int > > &path);
void floydWarshall (int &nVertex, vector<pair<int,int> > *&adjList,vector<vector<int > > & distResult, vector<vector<int> > & pathTracker);

int parallelDijkstraWithAdjMatrix(int source, int sink,int nVertex, vector<vector<int> > &adjMatrix, vector<vector< int > > &path);
// Tree Metric

vector<int> intersection(vector<int> &v1, vector<int> &v2);
int findPath(vector<pair<int,int> > *&adjList ,vector<vector<int > > & pathMatrix, int nVertex);
int pow2roundup (int x);
#endif /* SteinerFunctions_h */
