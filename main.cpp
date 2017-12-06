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
#include <vector>
#include <cstdlib>
#include <ctime>
#include <sys/time.h>
#include "functions.cpp"

using namespace std;

long getWaitTime( timeval& start );

int main()
{

	int nodeSize; 
	int sinkSize;
	int totalSize;
	char filename[100];
	char temp ;
	bool success;
	bool printDijkstra = false;
	long initialEnergy;
	double finalEnergy;
	ifstream fin;

	cout << "Enter filename containing graph information: ";
	cin >> filename;

	cout << "Would you like to output graph information (y/n): ";
	cin >> temp;

	if( temp == 'y' )
	{
		printDijkstra = true;
	}

	srand( time( NULL ) );

 	fin.open( filename );

	if( !fin.good() )
	{
		return 0;
	}
	else 
	{
		fin >> nodeSize;
		cout << nodeSize << endl;
		fin >> sinkSize;
		cout << sinkSize << endl;
		totalSize = nodeSize + sinkSize;
	}
	
	fin.close();
	
	Graph graph( totalSize );
	success = create_links( graph, filename, totalSize, sinkSize );
	
	if( !success )
	{
		cout << "Invalid graph file. Exiting now. " << endl;
		return 0;
	}

	//graph.printGraph();

	if( printDijkstra )
	{
		graph.printGraph();
	}
	cout << "Creating links using Dijkstra's" << endl;
	dijkstra( &graph, 0 );
	cout << "Links created" << endl;
	

	timeval startTime;
	gettimeofday( &startTime, NULL );
	initialEnergy = nodeSize * 100;

	// Initialize varaibles for calculating simulation metrics.
	int randomSignal;
	int itterator = 0;
	long waitTime;
	long totalEndToEndTime;
	timeval startEndToEnd;
	vector< long > endToEndTime;
	
	// Run simulation
	while( checkStatus( graph, sinkSize ) )
	{
		randomSignal = rand() % ( nodeSize ) + sinkSize;
		gettimeofday( &startEndToEnd, NULL );
		graph.sendSignal( randomSignal );
		endToEndTime.push_back( getWaitTime( startEndToEnd ) );
		itterator++;
	}

	endToEndTime.push_back( -1 ); // Used to find the last end to end time

	waitTime = getWaitTime( startTime );

	cout << " after " << itterator << " signals" << endl << endl;
	cout << "Total simulation time: " << waitTime << " usec" << endl << endl;
	cout << "Total throughput: " << (itterator/(float)waitTime) << " signals/usec" << endl << endl;

	finalEnergy = 0;
	for( itterator = sinkSize; itterator < totalSize; itterator++ )
		finalEnergy += graph.arr[ itterator ].head->currentEnergy;

	totalEndToEndTime = 0;
	for( itterator = 0; endToEndTime[ itterator ] != -1; itterator++ )
		totalEndToEndTime += endToEndTime[ itterator ];

	cout << "Energy consumption: " << initialEnergy - finalEnergy << endl;
	float percentageEnergy = (( initialEnergy - finalEnergy ) / initialEnergy ) * 100;
	cout << "Overall energy consumption (percentage): " << percentageEnergy << "%" << endl << endl;

	cout << "Average End To End time: " << (totalEndToEndTime/(float)itterator) << " usec/signal" << endl << endl;
	cout << "End simulation." << endl;

	return 0;
}
