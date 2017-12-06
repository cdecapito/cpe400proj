CPE 400 PROJECT - ENERGY CONSERVATION USING OSPF

Contributors: Carli DeCapito, Eugene Nelson, Jervyn Suguitan
Course: CPE 400 - Network Communications
University of Nevada, Reno

Date: December 6, 2017
////////////////////////////////////////////////////////////////////////
The purpose of this program is to simulate OSPF routing protocol with a focus on energy consumption. This simulation uses dijkstra's algorithm to save the shortest path from all nodes to the sink node. This program tests the evaulation metrics (end-to-end delay, throughput, and energy consumption) of different sized networks (10, 50, 75, 100 nodes). Static graphs of node sizes 10, 50, 75 and 100 are provided in links10.txt, links50.txt, links75.txt, and links100.txt respectively. To run this simulation and for further details about the files provided, see the information below. 
///////////////////////////////////////////////////////////////////////

HOW TO COMPILE: 
		~ make
		~ ./main

TESTING PURPOSES:
	Different graph sizes are provided in links10.txt, links50.txt, links75.txt, and links100.txt. The program will prompt the user to enter the file of the graph. At this point please enter one of the above mentioned graphs. If you wish to use your own graph, please see the links#_node.txt format below. 

	The program also provides the user the option to output the details of the graph, including: node link relationships, current energy, and energy consumption variables. 


BRIEF FILE DESCRIPTIONS:
		-makefile
		-main.cpp
			simulation implementation
		-objects.cpp 
			holds object definitions: minHeap, Graph, AdjList
		-functions.cpp

		-links#_nodes.txt- holds information about the graph; includes how many nodes, how many sink nodes, and what their relationships are. Used in main to read in information about the network.  
		#_nodes provided: 10,50,75,100
					format- 
					#_nodes (not including sink)
					#_sink nodes
					node_index #_neighbors neighbor_node_index 
									
