#include <time.h>
#include <sys/time.h>
#include <sstream>
#include <string>
#include <string.h>

#include "SF-Solution.h"
#include "SF-SolutionM.h"

#include "SF-Functions.h"
#include "PG-SF.hpp"
#include "Par-PG-SF.hpp"
//#include "Par-PD-SF.hpp"
//#include "GL-SF.hpp"
//#include "PD-SF.hpp"
//#include "TM-SF.hpp"

using namespace std;

int main(int argc, char* argv[])
{
    
    string strInputFile,strOutputFile,path,nFile,alg,algType,ratio, numP,run;
    double prob=0;
    double avEdges=0;
    int nVertex,nEdges,nTerminals;
    vector<pair<int,int>> *adjList;
    vector<vector<int>> adjMatrix;
    vector<vector<int>> E;
    int *terminals;
    vector<vector<int> > pairedTerminals;
    struct timeval time_start,time_end;
    double durationAlg;
    int initialSolution=0;
    int cleanedSolution=0;
    int diameter=0;
    int numProc=0;

    if(argc<9)
    {
        cout << "Not enough parameters have been passed. \n";
        cin.get();
        exit(0);
    }
    else
    {
        strInputFile=argv[1];
        strOutputFile=argv[2];
        path=argv[3];
        
        nFile=argv[4];
        alg=argv[5];
        algType =argv[6];
	ratio=argv[7];
	numP=argv[8];
	run=argv[9];
    }
    
   if(alg=="PG")
   {
        algType="";
        ratio="1";
        numP="1";
   }
    
    creatInputPath(strInputFile,path,nFile);
    creatOutputPath(strOutputFile,nFile,alg,algType,ratio,numP,run);

	cout<<"strInputFile: "<<strInputFile<<endl;
	cout<<"strOutputFile: "<<strOutputFile<<endl;
cout<<alg<<endl;
    ofstream outputFile;
    outputFile.open(strOutputFile);
    
//    if (alg=="ParPD")
//    {
//        readInputFileToAdjMatrix(strInputFile, nVertex, nEdges, nTerminals, terminals, adjMatrix, pairedTerminals, E,prob, avEdges);
//        gettimeofday(&time_start, NULL);
//        cout<<"********************* File  "<<nFile<<"  is read for Parallel Approx PD algorithm. ********************** \n";
//        cout<<"Calling Parallel Primal Dual Algorithm \n";
//        parallelPDAlgorithm(nVertex, nEdges, nTerminals, terminals, adjMatrix, pairedTerminals,E, algType,ratio,numP, cleanedSolution);
//    cout<<"m="<<nEdges<<endl;
//    cout<<"T="<<nTerminals<<endl;
//
//    }
//
//    else
    if (alg=="FamPar")
    {
        readInputFileAdjList(strInputFile, nVertex, nEdges, nTerminals, terminals, adjList, pairedTerminals, E,prob, avEdges);
        gettimeofday(&time_start, NULL);
        cout<<"********************* File  "<<nFile<<"  is read for Family of Parallel Greedy Algorithms. ********************** \n";
        cout<<"Calling A Family of Parallel Greedy Algorithms:\n";
        
        familyOfParallelGreedyAlgorithms(nVertex, nEdges, nTerminals, terminals, adjList, pairedTerminals,algType,ratio,numP, cleanedSolution);
    }
    else if (alg=="PG")
    {
        readInputFileAdjList(strInputFile, nVertex, nEdges, nTerminals, terminals, adjList, pairedTerminals, E,prob, avEdges);
        gettimeofday(&time_start, NULL);
        cout<<"********************* File  "<<nFile<<"  is read for PG Algorithm. ********************** \n";
        cout<<"Calling Paird Greedy Algorithm \n";
        
        greedyAlgorithm(nVertex, nEdges, nTerminals, terminals, adjList, pairedTerminals, cleanedSolution);
    }
//    else if (alg=="PD")
//    {
//        
//        readInputFileToAdjMatrix(strInputFile, nVertex, nEdges, nTerminals, terminals, adjMatrix, pairedTerminals, E,prob, avEdges);
//        gettimeofday(&time_start, NULL);
//        cout<<"********************* File  "<<nFile<<"  is read for PD algorithms. ********************** \n";
//        cout<<"Calling Primal Dual Algorithm \n";
//        
//        PrimalDualAlgorithm(nVertex, nEdges, nTerminals, terminals, adjMatrix, pairedTerminals, E,cleanedSolution,initialSolution);
//    }
//    else if (alg=="PDNoPQ")
//    {
//        readInputFileToAdjMatrix(strInputFile, nVertex, nEdges, nTerminals, terminals, adjMatrix, pairedTerminals, E,prob, avEdges);
//        gettimeofday(&time_start, NULL);
//        cout<<"********************* File  "<<nFile<<"  is read for PD-No-PQ algorithms. ********************** \n";
//        cout<<"Calling Primal Dual No PQ Algorithm \n";
//        PrimalDualAlgorithmNoPQ(nVertex, nEdges, nTerminals, terminals, adjMatrix, pairedTerminals, E,cleanedSolution,initialSolution);
//	cout<<"m="<<nEdges<<endl;
//	cout<<"T="<<nTerminals<<endl;        
//        
//    }
//    else if (alg=="GL")
//    {
//        readInputFileAdjList(strInputFile, nVertex, nEdges, nTerminals, terminals, adjList, pairedTerminals, E,prob, avEdges);
//        gettimeofday(&time_start, NULL);
//        cout<<"********************* File  "<<nFile<<"  is read for GL Algorithm. ********************** \n";
//        cout<<"Calling Gluttonous Algorithm \n";
//        gluttonousAlgorithm(nVertex, nEdges, nTerminals, terminals, adjList, pairedTerminals, cleanedSolution, initialSolution);
//        
//    }
//    else if (alg=="TM")
//    {
//        readInputFileAdjList(strInputFile, nVertex, nEdges, nTerminals, terminals, adjList, pairedTerminals, E,prob, avEdges);
//        gettimeofday(&time_start, NULL);
//        cout<<"********************* File  "<<nFile<<"  is read for TM Algorithm. ********************** \n";
//        cout<<"Calling Tree Metric Algorithm \n";
//        treeMetricAlgorithm(nVertex, nEdges, nTerminals, terminals, adjList, pairedTerminals, cleanedSolution, diameter);
//        
//    }
    else
    {
        cout<<"The requested algorithm is not recegnized \n";
    }
    gettimeofday(&time_end, NULL);
    durationAlg = (time_end.tv_sec * 1000000 + time_end.tv_usec) - (time_start.tv_sec * 1000000 + time_start.tv_usec);
    cout<< "Total Time: "<< durationAlg<<endl;
    // Output:
    outputFile<< nFile<<"\n"<<nVertex<<"\n"<<nEdges<<"\n"<<nTerminals<<"\n";
    outputFile<< alg<<"\n"<<algType<<"\n"<<ratio<<"\n"<<numP<<"\n"<<cleanedSolution<<"\n"<<durationAlg<<endl;
    cout<<"--------------------------------------------"<<endl;   
    cout<<"Computing the solution is complete! \n";
     
cout<<"******************************************************************** \n";
    
    return 0;
}

