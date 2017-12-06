// Program Information /////////////////////////////////////////////////////////
/**
  * @file objects.cpp
  *
  * @brief struct and class definitions     
  * 
  * @details holds definitions for Graph and MinHeap
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
//header files
#include <iostream>

using namespace std;

//PACKET OBJECT DEFINITIONS
struct PacketInfo{
	int nodeNum; //source node
	int numberOfPackets;  //number of packets being sent
};


// GRAPH OBJECT DEFINITIONS
struct Node{
  float currentEnergy;  //current health 
  float energyConsumption;  //health consumption per packet transfer
  int dest;   //destination node
  struct Node* next;  //next node pointer
  int forwardSignalTo;
};

struct AdjList
{
  struct Node *head;  //head of list
};

class Graph{ 
    
public: 
    int V; //size (how many nodes)
    struct AdjList *arr; //array

  //constructor
	Graph( int V ) 
    	{
      		this->V = V;
      		arr = new AdjList[ V ];
      		for ( int index = 0; index < V; index++ )
	      	{
				arr[ index ].head = NULL;
	      	}
    	}

    //create new node
  	Node * newNode( int dest )
    	{
    		Node *newNode = new Node;
    		newNode->dest = dest;
    		newNode->next = NULL;
	
    		return newNode;
    	}

  //add neighbors
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


  //output graph
	void printGraph()
   	{
   		int index;
   		for ( index = 0; index < V; index++ )
   		{
   			Node *temp = arr[ index ].head;

   			cout << endl << "Adjacency list of vertex " << index << endl;
   			cout << "current energy: " << temp->currentEnergy << endl;
   			cout << "energy consumption: " << temp->energyConsumption << endl;

   			cout << "head"; 			
			while( temp )
   			{
   				cout << " -> " << temp->dest;
   				temp = temp->next;
   			}
   			cout << endl;
   		}
   	}

   	bool sendSignal( int src )
   	{
   		if( src == 0 )
   			return true;
   		
   		Node *temp = arr[ src ].head;
   		temp->currentEnergy -= temp->energyConsumption;

      for( int i = 0; i < 100000; i++ )
        ;

   		sendSignal( temp->forwardSignalTo );
   		return true;
   	}
};


// MINHEAP OBJ DEFINITIONS
struct MinHeapNode
{
  int v; //index
  float energyConsumption; 
};

struct MinHeap
{
  int size;
  int capacity;
  int *position;
  struct MinHeapNode ** array;
};

