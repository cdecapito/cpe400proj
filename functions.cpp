// Program Information /////////////////////////////////////////////////////////
/**
  * @file functions.cpp
  *
  * @brief function simulation implementation  
  * 
  * @details implements simulation
  *
    * @version  1.10 Carli DeCapito
  *           Dijsktra's Algorithm 11/22/17
  *
  *           1.01 Carli DeCapito
  *           Node Creation 11/20/17
  *
  *           1.00 Carli DeCapito
  *           Original Documentation (11/13/17)
  *
  * @note None
  */

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include "objects.cpp"

using namespace std;



int INFINITY = 999;

void createRandomEnergy( float arr[], int totalSize, int sinkSize );
bool create_links( Graph &graph, int totalSize, int sinkSize );
struct MinHeapNode *newMinHeapNode( int v, float energyConsumption );
struct MinHeap *createMinHeap( int capacity );
void swapMinHeapNode( struct MinHeapNode **a, struct MinHeapNode ** b );
void minHeapify( struct MinHeap *minHeap, int index );
bool isEmpty( struct MinHeap* minHeap );
struct MinHeapNode* extractMin( struct MinHeap *minHeap );
void decreaseKey( struct MinHeap *minHeap, int v, float energyConsumption );
bool isInMinHeap( struct MinHeap *minHeap, int v );
void printArr( float arr[], int n );
void dijkstra( Graph * graph, int src );
void printSolution( float dist[], int V, int parent[] );
void printPath( int parent[], int j);

/**
 * @brief create_links
 *
 * @details reads graph info from file
 *          
 * @pre file exists and graph size is initialized
 *
 * @post graph contains network information, return bool if successful
 *
 * @par Algorithm 
 *      using fstream objects, read from file
 * 		using graph function addEdge, add edges to graph
 *      
 * @exception None
 *
 * @param [in] totalSize includes information about how many nodes total
 *			   sinkSize includes information about how many sinks in the graph
 *
 * @param [out] graph contains information about the network
 *
 * @return bool
 *
 * @note None
 */
 
bool create_links( Graph &graph, int totalSize, int sinkSize )
{
	ifstream fin;
	int vertex;
	int edges;
	int pointer;
	int temp;
	float energy[ totalSize ];
	fin.open( "links1.txt" );

	createRandomEnergy( energy, totalSize, sinkSize );

	if( !fin.good() )
	{
		return false;
	}
	fin >> temp;
	fin >> temp;

	for ( int index = 0; index < totalSize; index++ )
	{
		fin >> vertex;
		if( vertex == index )
		{
			fin >> edges;
			for ( int subIndex = 0; subIndex < edges; subIndex++ )
			{
				fin >> pointer;
				graph.addEdge( index, 100.00, energy[ index ], 100.0, energy[ pointer ], pointer );
			}
		}
	}

	return true;
}


/**
 * @brief createRandomEnergy
 *
 * @details creates random energy
 *          
 * @pre totalSize and sinKSize are already initalized
 *
 * @post arr[] holds random energy floats
 *
 * @par Algorithm 
 *      loop through and use rand()
 *      
 * @exception None
 *
 * @param [in] totalSize provides int for total number of nodes
 *			   sinkSize provides int for total number of sink nodes
 *
 * @param [out] arr provides array of floats for energy consumption
 *
 * @return None
 *
 * @note None
 */
 
void createRandomEnergy( float arr[], int totalSize, int sinkSize )
{

	srand( time( 0 ) );
	for ( int index = 0; index < totalSize; index++ )
	{
		if( index < sinkSize )
		{
			arr[ index ] = 0;
		}
		else
		{
			arr[ index ] = ( float ) ( ( rand() % 100 ) + 1 ) / (float) 100.0;
		}
	}
}


/**
 * @brief newMinHeapNode
 *
 * @details creates new minHeapNode
 *          
 * @pre None
 *
 * @post minHeapNode is returned
 *
 * @par Algorithm 
 *      Using assignment operand, initialize minHeapNode
 *      
 * @exception None
 *
 * @param [in] v provides int for an index
 *			   energyConsumption provides a float for the "distance"
 *
 * @param [out] None
 *
 * @return MinHeapNode*
 *
 * @note None
 */
struct MinHeapNode * newMinHeapNode( int v, float energyConsumption )
{
	MinHeapNode * minHeapNode = new MinHeapNode;
	minHeapNode->v = v;
	minHeapNode->energyConsumption = energyConsumption;
	return minHeapNode;
}


/**
 * @brief createMinHeap
 *
 * @details utility function to create the minHeap
 *          
 * @pre none
 *
 * @post minHeap is initalized and returned
 *
 * @par Algorithm 
 *      using assignment operand, intialize minHEap
 *      
 * @exception None
 *
 * @param [in] capacity provides an int for max number of nodes
 *
 * @param [out] None
 *
 * @return MinHeap* 
 *
 * @note None
 */
struct MinHeap * createMinHeap( int capacity )
{
	MinHeap *minHeap = new MinHeap;
	minHeap->position = new int [ capacity ];
	minHeap->size = 0;
	minHeap->capacity = capacity;
	minHeap->array = new MinHeapNode*[ capacity ];
	return minHeap;
}

/**
 * @brief swapMinHeapNode
 *
 * @details swaps two nodes
 *          
 * @pre none
 *
 * @post nodes a and b are swapped
 *
 * @par Algorithm 
 *      assignment operators
 *      
 * @exception None
 *
 * @param [in] a provides pointer to a node
 *			   b provides pointer to b node
 *
 * @param [out] 
 *
 * @return None
 *
 * @note None
 */
void swapMinHeapNode( struct MinHeapNode **a, struct MinHeapNode ** b )
{
	MinHeapNode *t = *a;
	*a = *b;
	*b = t;
}


/**
 * @brief minHeapify
 *
 * @details heapify a given index, updates position of nodes when they are swapped
 * 			position is needed to decreaseKey()
 *          
 * @pre minHeap exists
 *
 * @post heap is heapified
 *
 * @par Algorithm 
 *      saves smallest index
 *      
 * @exception None
 *
 * @param [in] index provides index in heap
 *			   minHeap provides pointer to minHeap
 *
 * @param [out] 
 *
 * @return None
 *
 * @note None
 */
void minHeapify( struct MinHeap *minHeap, int index )
{
	int smallest, left, right;
	smallest = index;
	left = 2 * index + 1;
	right = 2 * index + 2;

	if( left < minHeap->size &&
		minHeap->array[ left ]->energyConsumption < minHeap->array[ smallest ]->energyConsumption )
	{
		smallest = left;
	}
	if( right < minHeap->size &&
		minHeap->array[ right ]->energyConsumption < minHeap->array[ smallest ]->energyConsumption )
	{
		smallest = right;
	}
	if( smallest != index )
	{
		//store nodes to be swapped
		MinHeapNode *smallestNode = minHeap->array[ smallest ];
		MinHeapNode *indexNode = minHeap->array[ index ];

		//swap positions
		minHeap->position[ smallestNode->v ] = index;
		minHeap->position[ indexNode->v ] = smallest;

		//swap nodes
		swapMinHeapNode( &minHeap->array[ smallest ], &minHeap->array[ index ]);

		minHeapify( minHeap, smallest );
	}
}


/**
 * @brief isEmpty
 *
 * @details utility function checks if minHeap is empty
 *          
 * @pre minHeap exists
 *
 * @post returns true if empty, false if not
 *
 * @par Algorithm 
 *      bool 
 *      
 * @exception None
 *
 * @param [in] minHeap provides pointer to minHeap
 *
 * @param [out] 
 *
 * @return None
 *
 * @note None
 */
bool isEmpty( struct MinHeap* minHeap )
{
	if( minHeap->size == 0 )
	{
		return true;
	}
	return false;
}


/**
 * @brief extractMin
 *
 * @details utility function to extract min node from heap
 *          
 * @pre MinHeap exists
 *
 * @post min node is extracted and returned
 *
 * @par Algorithm 
 *    	using poitners extract.
 *      
 * @exception None
 *
 * @param [in] minHeap provides minHeap pointer
 *
 * @param [out] 
 *
 * @return MinHeapNode
 *
 * @note None
 */
struct MinHeapNode* extractMin( struct MinHeap *minHeap )
{
	if( isEmpty( minHeap ) )
	{
		return NULL;
	}

	//store root
	MinHeapNode *root = minHeap->array[ 0 ];

	//replace rootnode with lastnode
	MinHeapNode *lastNode = minHeap->array[ minHeap->size - 1 ];
	minHeap->array[ 0 ] = lastNode;

	//update position at last node
	minHeap->position[ root->v ] = minHeap->size - 1;
	minHeap->position[ lastNode->v ] = 0;


	//reduce heap size and heapify root
	--minHeap->size;
	minHeapify( minHeap, 0 );

	return root;
}

/**
 * @brief decreaseKey
 *
 * @details decrease dist value of given vertex v.
 *			uses position of minheap to get the curr index of node
 *          
 * @pre MinHeap exists
 *
 * @post keydecreased
 *
 * @par Algorithm 
 *      
 *      
 * @exception None
 *
 * @param [in] minheap provides pointer to minHeap
 *			   v provides int index in minHeap
 *			   energyConsumption provides float 
 *
 * @param [out] 
 *
 * @return None
 *
 * @note None
 */
void decreaseKey( struct MinHeap *minHeap, int v, float energyConsumption )
{
	//get index of v in heap array
	int i = minHeap->position[ v ];

	//get node and update energy value
	minHeap->array[ i ]->energyConsumption = energyConsumption;

	//traverse while complete tree is not heapified
	while ( i &&
		 	minHeap->array[ i ]->energyConsumption < minHeap->array[ ( i - 1 )/ 2 ]->energyConsumption )
	{
		//swap this node with its parent
		minHeap->position[ minHeap->array[ i ]->v ] = ( i - 1 )/2;
		minHeap->position[ minHeap->array[ ( i - 1 ) / 2 ] ->v ] = i;
		swapMinHeapNode( &minHeap->array[ i ], &minHeap->array[ ( i - 1 )/2 ]);

		//move to parent index
		i = ( i - 1 ) /2;
	}
}

/**
 * @brief isInMinHeap
 *
 * @details returns bool if v is in minHeap
 *          
 * @pre minHeap exsists
 *
 * @post returns true if in heap, false otherwise
 *
 * @par Algorithm 
 *      bool variable
 *      
 * @exception None
 *
 * @param [in] MinHeap provides minHeap pointer ]
 *			   v provides index value of int
 *
 * @param [out] 
 *
 * @return bool
 *
 * @note None
 */
bool isInMinHeap( struct MinHeap *minHeap, int v )
{
	if( minHeap->position[ v ] < minHeap->size )
	{
		return true;
	}
	return false;
}


/**
 * @brief printArr
 *
 * @details prints distance arr
 *          
 * @pre arr exists
 *
 * @post outputed to mointor
 *
 * @par Algorithm 
 *      cout using for loop
 *      
 * @exception None
 *
 * @param [in] n provides size of arr
 *
 * @param [out] arr provides distance arr
 *
 * @return None
 *
 * @note None
 */
void printArr( float arr[], int n )
{
	cout << "Vertex" << "		Distance from Source " << endl;
	for ( int index = 0; index < n; index++ )
	{
		cout << index << "		" << arr[ index ] << endl;
	}
}


/**
 * @brief dijsktra
 *
 * @details uses dijsktra's algorithm to find shortest path
 *          
 * @pre graph exists
 *
 * @post save shortest path
 *
 * @par Algorithm 
 *      
 *      
 * @exception None
 *
 * @param [in] graph provides graph of nodes
 *			   src provides int index of source node
 *
 * @param [out] 
 *
 * @return None
 *
 * @note None
 */
void dijkstra( Graph * graph, int src )
{
    //V = number of totalNodes
	int V = graph->V; 

    int parent[V];
    //int key[V];
	float dist[ V ];//distance values used to pick min energy consumption

	struct MinHeap *minHeap = createMinHeap( V );

	//initialize min heap with all vertices, dist value to INFINITY
	for( int v = 0; v < V; v++ )
	{
        parent[0] = -1;        

		dist[ v ] = INFINITY;
		minHeap->array[ v ] = newMinHeapNode( v, dist[ v ] );
		minHeap->position[ v ] = v;
	}

	//make dist value of src vertex as 0 so is extracted first
	minHeap->array[ src ] = newMinHeapNode( src, dist[ src ] );
	minHeap->position[ src ] = src;
	dist[ src ] = 0;
	decreaseKey( minHeap, src, dist[ src ] );

	//initalize min heap size to V
	minHeap->size = V;
	//while min heap is not empty, min heap contains all nodes whose
	//shortest dist is not yet finalized
	while( !isEmpty( minHeap ))
	{
		//extract minimum vertex w/ minimum distance value
		MinHeapNode *minHeapNode = extractMin( minHeap );
		int u = minHeapNode->v;

		//traverse adj vertices of u and update distance values
		Node *temp = graph->arr[ u ].head;
		while( temp != NULL )
		{

			int v = temp->dest;

			//if shortest distance to u is not finalized yet
			//and if dist to v through u is less than prev calculated dist
			if( isInMinHeap( minHeap, v ) &&
				dist[ u ] != INFINITY &&
				temp->energyConsumption + dist[ u ] < dist[ v ] )
			{
                parent[ v] = u;
				dist[ v ] = dist[ u ] + temp->energyConsumption;

				//update distance value in minheap
				decreaseKey( minHeap, v, dist[ v ] );
			}
			//update current energy
			temp->currentEnergy -= temp->energyConsumption;
			//next node
			temp = temp->next;
		}
	}
	//print shortest distance

    //minheap int pos.  Shows the direction from a node to sink
    
   // cout << V << " before loops" << endl;
/*
    cout << minHeap -> size << endl;
    for(int index = 0;index < V; index++ )
    {
        
        for( int j = 0; j < minHeap[ index ].size; j++ )
        {
            //cout << minHeap[ index ].array[ j ] << " ";
            //cout << minHeap->array[index][j].v << " ";
        } 
        cout << endl;
    }
*/
   	cout << endl << endl;

    printSolution( dist, V, parent );

    cout << endl << endl;
	//printArr( dist, V );
}


void printSolution( float dist[], int V, int parent[] )
{
    int src = 0;

    //formating for reading the paths
    cout << "Vertex     " << "Distance     " << "Path" << endl;
    for( int i = 1; i < V; i++ )
    {
        cout << endl << src << " -> " << i << "      "<< dist[i] << "          " << src;
        printPath(parent, i );
    }

}


void printPath( int parent[], int j )
{
    if( parent[j] == -1)
        return;
    
    printPath(parent, parent[j] );
    cout<< j;


}










