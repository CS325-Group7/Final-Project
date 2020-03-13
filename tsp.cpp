/********
 * Authors: Kate Galle, Lauren Galle, Peri Cabrales
 * Class: CS 325
 * Assignment: Final Project
 * Date: 3-13-2020
 * Description: contains the definitions for the declarations provided in the tsp.h file
 * ********/

#include "tsp.h"

//using namespace std;

// define the Constructor

TSP::TSP(string in, string out){

	// initialize variables
	
	file_in = in;
	file_out= out;
	int count;
	count = 0;

	ifstream input_in;

	// open file and convert to a c style string
	
	input_in.open(file_in.c_str());

	while(!input_in.eof()){

		// 3 variables on each line of the text files
		// distance, x and y coords

		int read1;
        	int read2;
        	int read3;
		
		input_in >> read1 >> read2 >> read3;
		
		// make a new city with the x and y coords

		struct Cities city = {read2, read3};
		cityList.push_back(city);

		// increment counter so the correct number of vertices 

		count++;
	}		
	
	count--;
	verts = count;
	
	input_in.close();

	graph = new int*[verts];
	
	// iterate through the graph and initialize
	// also iterate through the graph and set up the path lengths
		
	int i;
	int j;

	for(i=0; i<verts; i++){
	
		graph[i] = new int[verts];

		for(j=0; j<verts; j++){

			graph[i][j] = 0;
		}				
	}

	// allocate space for the lengths
	
	lengths = new int*[verts];

        for(i=0; i<verts; i++){

                lengths[i] = new int[verts];
        }

	// set up cityList
	
	for(i=0; i<cityList.size(); i++){
	
		struct Cities curr = cityList[i];
	}
	
	// set up the adjacency list

	adj_list = new vector<int>[verts];	 
}

// fills the graph with the distances

void TSP::fillGraph(){

	// initialize the variables
	
	int i;
	int j;
	int dis;

	int xvar;
	int yvar;

	for(i=0; i<verts; i++){

		for(j=0; j<verts; j++){

			// calculates the x distance

			xvar = cityList[i].x - cityList[j].x;
			
			// calculates the y distance

			yvar = cityList[i].y- cityList[j].y;
			
			// calculates the distance with the equation provided in the assignment

			dis = floor(sqrt(pow(xvar, 2) + pow(yvar, 2)));
			
			// set up graph

			graph[j][i] = dis;			
			graph[i][j] = graph[j][i];
		}
	}
}

// used in the prims MST to find the minimum
// got help for this from https://www.geeksforgeeks.org/prims-minimum-spanning-tree-mst-greedy-algo-5/

int TSP::getMin(int index[], bool node_in[]){

	// initialize the variables
		
	int min = INT_MAX;
  	int min_index;

	for (int i = 0; i < verts; i++) {

		if(node_in[i] != true && index[i] < min){
	
			min = index[i];
			min_index = i;
		}	
	}

	return min_index;
}

// got help for this from https://www.geeksforgeeks.org/prims-minimum-spanning-tree-mst-greedy-algo-5/

void TSP::primsMST(){

	// initialize variables
	
	int index[verts];
	int parent[verts];
	bool node_in[verts];
	int i;
	int j;

	for(i=0; i<verts; i++){

		// set to the maximum or infinity

		index[i] = INT_MAX;
		
		// the node has not yet been included in the MST

		node_in[i] = false;
	}
	
	// set the parent to the root
	
	parent[0] = -1;
	
	// set the index to the first value
	
	index[0] = 0;

	int u;

	for(i=0; i<verts-1; i++){

		// find the min vertex not in the tree, yet

		u = getMin(index, node_in);
		
		// now the node has been included

		node_in[u] = true;
		
		int v;
		
		// iterate through the neighboring vertices that have not been looked at

		for(v=0; v<verts; v++){

			// the node must not be included yet
			// make sure the node is there
			// make sure that the graph is less than the index that is before u

			if(graph[u][v] && (node_in[v] != true) && (graph[u][v] < index[v])){

				// parent node updated

				parent[v] = u;
				
				// index updated

				index[v] = graph[u][v];
			}
			
			// if not, keep looking through vertices

			else{

				continue;
			}
		}										
	}

	// add to the adjacency list
	
	addList(parent);	
}

// used in the prims MST to add values to the 
// adjacency list

void TSP::addList(int parent[]){

	// initialize variables
	
	int i;
	
	for(i=0; i<verts; i++){

                if(parent[i] != -1){


			adj_list[i].push_back(parent[i]);;
                        adj_list[parent[i]].push_back(i);
		}
	}
}

// finds the odd nodes
// adds to subgraph
// finds a matching subgraph
 
void TSP::matchOdds(){

	// initialize variables

	std::vector<int>::iterator start;
	std::vector<int>::iterator temp;
	vector<int>::iterator iter;
	vector<int>::iterator stop;
	
	int len;
	int nearest;

	int i;
	
	// find the odds and add to oddPairs
	
	for(i=0; i<verts; i++){

		if((adj_list[i].size()%2) == 0){

			continue;

		}	

		// if odd, add to subgraph list

		else{
			oddPairs.push_back(i);
		}
	}

	
	while(!oddPairs.empty()){

		start = oddPairs.begin();
		stop = oddPairs.end();

		len = INT_MAX;

		iter = oddPairs.begin()+1;

		// iterate through the odd nodes

		while(iter != stop){

			// if the node is closer in distance

			if(len > graph[*start][*iter]){

				len = graph[*start][*iter];
				nearest = *iter;
				temp = iter;
			}

			iter ++;
		}
		
		// update adjacency list

		adj_list[*start].push_back(nearest);
		adj_list[nearest].push_back(*start);

		// remove from odds

		oddPairs.erase(temp);
		oddPairs.erase(start);
	}
}

// got help for the Euler circuit from https://www.geeksforgeeks.org/hierholzers-algorithm-directed-graph/
// finds the Euler circuit in the graph

void TSP::eulerCircuit(int begin, vector<int> &path){
	
	// initialize variables
	
	vector<int> *temp = new vector<int>[verts];
	int i;
	stack<int> stack;
	int check= begin;
	int close;

	//path.push_back(begin);

	for(i=0; i<verts; i++){
	
		temp[i].resize(adj_list[i].size());
		temp[i] = adj_list[i];
	}

	path.clear();

	// make sure there is something in the stack
	
	while(!stack.empty() || temp[check].size()>0){

		// if there are not any nearby nodes

		if(temp[check].empty() == 1){

			path.push_back(check);
			check = stack.top();
			stack.pop();
		}

		// if there are some neighbors

		else{

			// use backtracking to find the remaining circuit

			stack.push(check);
			close = temp[check].back();
			
			// backtrack

			temp[check].pop_back();

			i=0;
			
			// iterate through the temp variable size

			while(i<temp[close].size()){

				if(temp[close][i] == check){

					temp[close].erase(temp[close].begin()+i);
					break;
				}
			
				// increment i iterator

				i++;
			}

			// current gets neighbor

			check = close;

		}
	}
	
	path.push_back(check);
}

// takes the Euler circuit and turns it into 
// a Hamiltonian path

void TSP::eulerHP(vector<int> &path, int &cost){
	
	// initialize variables
	
	bool node_in[verts];
	int i;
	
	vector<int>::iterator curr = path.begin();
	vector<int>::iterator next = path.begin() + 1;

	int starting = path.front();

	// takes away the visited nodes
	
	for(i=0; i<verts; i++){

		node_in[i] = 0;
	}		
	
	// start the cost of the path off at 0
	
	cost = 0;

	// the root is visted so gets a 1 to represent
	
	node_in[starting] = 1;
	
	while(next - path.end() != 0){

		if(node_in[*next] == 0){

			cost += graph[*curr][*next];
			curr = next;
			node_in[*curr] = true;
			next = curr +1;
		}	

		else{

			next = path.erase(next);
		}
	}

	cost += graph[*curr][*next];
}

// adds the information to the .tour files

void TSP::sendFile(){

	// initialize variables
	
	ofstream send_out;
	vector<int>::iterator iter;
	
	send_out.open(file_out.c_str(), ios::out);
	
	// adds an extra line to the file to be the length of the total path
	
	send_out << lenPath << endl;
	
	// iterates through the circuit to print out the distances taken
	// to the file
	
	iter = cycle.begin();

	while(iter != cycle.end()){

		send_out << *iter << endl;
		
		iter++;
	}	
	
	send_out.close();
}

//set destructor

TSP::~TSP(){

	// initialize variables
	
	int i;

        for(i=0; i<verts; i++){

                delete[] graph[i];
                delete[] lengths[i];
        }

	// finish clearing memory
	
        delete[] lengths;
        delete[] graph;
        delete[] adj_list;
}
