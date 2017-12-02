// Program Information /////////////////////////////////////////////////////////
/**
  * @file main.cpp
  *
  * @brief main driver     
  * 
  * @details calls implementation
  *
  * @version 1.00 Carli DeCapito
  *			 Original Documentation (11/13/17)
  *
  * @note None
  */

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
 #include <limits.h>

using namespace std;

int INFINITY = 999;

//////////////////////////
//// Graph Definition ////
//////////////////////////

struct PacketInfo{
	int nodeNum;
	int numberOfPackets;
};

struct Node{
  float currentEnergy;
  float energyConsumption;
  int dest;
  struct Node* next;
};

struct AdjList
{
  struct Node *head;
};

class Graph{ 
    
public: 
    int V;
    struct AdjList *arr;
	Graph( int V )
    	{
      		this->V = V;
      		arr = new AdjList[ V ];
      		for ( int index = 0; index < V; index++ )
	      	{
				arr[ index ].head = NULL;
	      	}
    	}

  	Node * newNode( int dest )
    	{
    		Node *newNode = new Node;
    		newNode->dest = dest;
    		newNode->next = NULL;
	
    		return newNode;
    	}

	void addEdge( int src, float currentEnergy1, float energyConsumption1, float currentEnergy2, float energyConsumption2, int dest )
	{
		//hello packets
   		Node *tempNode = newNode( dest );
   		tempNode->currentEnergy = currentEnergy1;
   		tempNode->energyConsumption = energyConsumption1;
   		tempNode->next = arr[ src ].head;
   		arr[ src ].head = tempNode;

   		//hello packets
		tempNode = newNode( src );
		tempNode->currentEnergy = currentEnergy2;
   		tempNode->energyConsumption = energyConsumption2;
		tempNode->next = arr[ dest ].head;
		arr[ dest ].head = tempNode;
		
   	}

	void printGraph()
   	{
   		int index;
   		for ( index = 0; index < V; index++ )
   		{
   			Node *temp = arr[ index ].head;

   			cout << endl << "Adjacency list of vertex " << index << endl;
   			cout << "current energy: " << temp->currentEnergy << endl;
   			cout << "energy consumption: " << temp->energyConsumption << endl;

   			cout << "head "; 			
			while( temp )
   			{
   				cout << "-> " << temp->dest;
   				temp = temp->next;
   			}
   			cout << endl;
   		}
   	}
};

struct MinHeapNode
{
	int v;
	float energyConsumption;
};

struct MinHeap
{
	int size;
	int capacity;
	int *position;
	struct MinHeapNode ** array;
};


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


int main()
{

	int nodeSize; 
	int sinkSize;
	int totalSize;
	bool success;
	ifstream fin;

	fin.open( "links.txt" );

	if( !fin.good() )
	{
		return 0;
	}
	else 
	{
		fin >> nodeSize;
		fin >> sinkSize;
		totalSize = nodeSize + sinkSize;
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

	return 0;
}

bool create_links( Graph &graph, int totalSize, int sinkSize )
{
	ifstream fin;
	int vertex;
	int edges;
	int pointer;
	int temp;
	float energy[ totalSize ];
	fin.open( "links.txt" );

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
		fin >> edges;
		for ( int subIndex = 0; subIndex < edges; subIndex++ )
		{
			fin >> pointer;
			graph.addEdge( index, 100.00, energy[ index ], 100.0, energy[ pointer ], pointer );
		}
	}

	return true;
}



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

struct MinHeapNode * newMinHeapNode( int v, float energyConsumption )
{
	MinHeapNode * minHeapNode = new MinHeapNode;
	minHeapNode->v = v;
	minHeapNode->energyConsumption = energyConsumption;
	return minHeapNode;
}

struct MinHeap * createMinHeap( int capacity )
{
	MinHeap *minHeap = new MinHeap;
	minHeap->position = new int [ capacity ];
	minHeap->size = 0;
	minHeap->capacity = capacity;
	minHeap->array = new MinHeapNode*[ capacity ];
	return minHeap;
}

void swapMinHeapNode( struct MinHeapNode **a, struct MinHeapNode ** b )
{
	MinHeapNode *t = *a;
	*a = *b;
	*b = t;
}

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
		MinHeapNode *smallestNode = minHeap->array[ smallest ];
		MinHeapNode *indexNode = minHeap->array[ index ];

		minHeap->position[ smallestNode->v ] = index;
		minHeap->position[ indexNode->v ] = smallest;

		swapMinHeapNode( &minHeap->array[ smallest ], &minHeap->array[ index ]);

		minHeapify( minHeap, smallest );
	}
}

bool isEmpty( struct MinHeap* minHeap )
{
	if( minHeap->size == 0 )
	{
		return true;
	}
	return false;
}

struct MinHeapNode* extractMin( struct MinHeap *minHeap )
{
	if( isEmpty( minHeap ) )
	{
		return NULL;
	}

	MinHeapNode *root = minHeap->array[ 0 ];

	MinHeapNode *lastNode = minHeap->array[ minHeap->size - 1 ];
	minHeap->array[ 0 ] = lastNode;

	minHeap->position[ root->v ] = minHeap->size - 1;
	minHeap->position[ lastNode->v ] = 0;

	--minHeap->size;
	minHeapify( minHeap, 0 );

	return root;
}

void decreaseKey( struct MinHeap *minHeap, int v, float energyConsumption )
{
	int i = minHeap->position[ v ];

	minHeap->array[ i ]->energyConsumption = energyConsumption;

	while ( i &&
		 	minHeap->array[ i ]->energyConsumption < minHeap->array[ ( i - 1 )/ 2 ]->energyConsumption )
	{
		minHeap->position[ minHeap->array[ i ]->v ] = ( i - 1 )/2;
		minHeap->position[ minHeap->array[ ( i - 1 ) / 2 ] ->v ] = i;
		swapMinHeapNode( &minHeap->array[ i ], &minHeap->array[ ( i - 1 )/2 ]);

		i = ( i - 1 ) /2;
	}
}

bool isInMinHeap( struct MinHeap *minHeap, int v )
{
	if( minHeap->position[ v ] < minHeap->size )
	{
		return true;
	}
	return false;
}

void printArr( float arr[], int n )
{
	cout << "Vertex" << "		Distance from Source " << endl;
	for ( int index = 0; index < n; index++ )
	{
		cout << index << "		" << arr[ index ] << endl;
	}
}

void dijkstra( Graph * graph, int src )
{
	int V = graph->V; 
	float dist[ V ];

	struct MinHeap *minHeap = createMinHeap( V );

	for( int v = 0; v < V; v++ )
	{
		dist[ v ] = INFINITY;
		minHeap->array[ v ] = newMinHeapNode( v, dist[ v ] );
		minHeap->position[ v ] = v;
	}

	minHeap->array[ src ] = newMinHeapNode( src, dist[ src ] );
	minHeap->position[ src ] = src;
	dist[ src ] = 0;
	decreaseKey( minHeap, src, dist[ src ] );

	minHeap->size = V;

	while( !isEmpty( minHeap ))
	{
		MinHeapNode *minHeapNode = extractMin( minHeap );
		int u = minHeapNode->v;

		Node *temp = graph->arr[ u ].head;
		while( temp != NULL )
		{
			int v = temp->dest;

			if( isInMinHeap( minHeap, v ) &&
				dist[ u ] != INFINITY &&
				temp->energyConsumption + dist[ u ] < dist[ v ] )
			{
				dist[ v ] = dist[ u ] + temp->energyConsumption;


				decreaseKey( minHeap, v, dist[ v ] );
			}
			temp = temp->next;
		}
	}
	printArr( dist, V );
}