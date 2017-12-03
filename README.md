CPE 400 PROJECT - ENERGY CONSERVATION USING OSPF

////////////////////////////////////////////////

HOW TO COMPILE: 
		~ make
		~ ./main

////////////////////////////////////////////////

FILES INCLUDED:
	main.cpp

	objects.cpp 

	functions.cpp

	links.txt- holds information about the graph; includes how many nodes, 
	how many sink nodes, and what their relationships are. Used in main to
	read in information about the network.  
				format- 
				#_nodes (not including sink)
				#_sink nodes
				node_index #_neighbors neighbor_node_index 
								
	packets.txt
