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
 	ifstream fin;   
	bool success;
	int nodeSize; 
	int sinkSize;
	int totalSize;
    int packetInstructions;
    int **pathArray;
    PacketInfo* packetInfoArray;




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

    //read in packetInfo
    fin.open("packet.txt");
    if( !fin.good() )
	{
		return 0;
	}
	else 
	{
		fin >> packetInstructions;

        packetInfoArray = new PacketInfo[ packetInstructions ];

        for( int index = 0; index < packetInstructions; index ++ )
        {
            fin >> packetInfoArray[ index ].nodeNum;
            fin >> packetInfoArray[ index ].numberOfPackets;
        }
		

	}




	Graph graph( totalSize );

	success = create_links( graph, totalSize, sinkSize );

	if( !success )
	{
		cout << "Invalid graph file. Exiting now. " << endl;
		return 0;
	}

	graph.printGraph();


    // init 2D array with -1
    // they are initialized with -1 to differentiate a path with 
    // empty space
     
    pathArray = new int* [totalSize];
    for( int i = 0; i < totalSize ; i++ )
    {
        pathArray[ i ] = new int[ totalSize ];
    }
    
    //initialize 2D array
    init2Darray( pathArray, -1, totalSize );

	dijkstra( &graph, 0, pathArray );

/////////////




    //algorithm that inputs the path into the pathArray
    // the forloop starts with 1 because you dont need to write a path 
    // from 0 node to 0


    //printing out test code
    cout << endl << "In the main yo" << endl;
    for( int i = 1; i < totalSize; i++ )
    {
        for( int j = 0; j < totalSize; j++ )
        {
            if( pathArray[i][j] == -1 )
            {
                //quit the for loop
                j = totalSize;
            }
            else
            {
                cout << pathArray[i][j] << "  ";
            }
        }   
        cout << endl;
    }


    testNetwork( &graph, pathArray, packetInfoArray, packetInstructions );


////////
	//graph.printGraph();

	return 0;
}
