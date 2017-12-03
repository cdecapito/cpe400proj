// Program Information /////////////////////////////////////////////////////////
/**
  * @file main.cpp
  *
  * @brief main driver     
  * 
  * @details creates network used to test OSPF algorith for energy consumption
  *	         Some functions adapted from geeksforgeeks.com
  *
  * @version 
  *			 1.10 Carli DeCapito
  *			 Dijsktra's Algorithm 11/22/17
  *
  *			 1.01 Carli DeCapito
  *			 Node Creation 11/20/17
  *
  *			 1.00 Carli DeCapito
  *			 Original Documentation (11/13/17)
  *
  * @note None
  */

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include "functions.cpp"

using namespace std;

int main()
{

	int nodeSize; 
	int sinkSize;
	int totalSize;
	bool success;
	ifstream fin;

	fin.open( "links1.txt" );

	if( !fin.good() )
	{
		return 0;
	}
	else 
	{
		fin >> nodeSize;
		fin >> sinkSize;
		totalSize = nodeSize + sinkSize;

		cout << "TOT" << totalSize << endl;
	}
	fin.close();

	Graph graph( totalSize );

	success = create_links( graph, totalSize, sinkSize );

	if( !success )
	{
		cout << "Invalid graph file. Exiting now. " << endl;
		return 0;
	}

	graph.printGraph();

	dijkstra( &graph, 0 );

	graph.printGraph();

	return 0;
}
