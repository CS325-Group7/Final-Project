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
	int read1;
	int read2;
	int read3;
	int count;
	count = 0;

	ifstream input_in;
	input_in.open(file_in.c_str(), ios::in);

	while(!input_in.eof()){

		// 3 variables on each line of the text files
		// distance, x and y coords

		input_in >> read1 >> read2 >> read3;
		
		count++;
		// make a new city with the x and y coords

		struct Cities city = {read2, read3};
		cityList.push_back(city);

		//count++;
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

	lengths = new int*[verts];

        for(i=0; i<verts; i++){

                lengths[i] = new int[verts];
        }

	for(i=0; i<cityList.size(); i++){
	
		struct Cities curr = cityList[i];
	}
	
	// set up the adjacency list

	adj_list = new vector<int>[verts];	 
}

// set the destructor

TSP::~TSP(){
	
	// initialize variables
	
	int i;

	for(i=0; i<verts; i++){

		delete[] graph[i];
		delete[] lengths[i];
	}	

	delete[] lengths;
	delete[] graph;
	delete[] adj_list;
}

void TSP::fillGraph(){

	int i;
	int j;
	
	for(i=0; i<verts; i++){

		for(j=0; j<verts; j++){

			graph[i][j] = graph[j][i] = calcDistance(cityList[i], cityList[j]);
		}
	}
}

int TSP::calcDistance(struct TSP::Cities cit1, struct TSP::Cities cit2){

	return floor(sqrt(pow(cit1.x-cit2.x,2) + pow(cit1.y-cit2.y,2)));
}

int TSP::getMin(int index[], bool node_in[]){
	
	int min = std::numeric_limits<int>::max();
  	int min_index;
	for (int i = 0; i < verts; i++) {

		if(node_in[i] == false && index[i] < min){
	
			min = index[i];
			min_index = i;
		}	
	}

	return min_index;
}

void TSP::primsMST(){

	int index[verts];
	int parent[verts];
	bool node_in[verts];
	int i;
	//int min = std::numeric_limits<int>::max();
	int min = INT_MAX;
	int minimum;
	int j;

	for(i=0; i<verts; i++){

		//index[i] = std::numeric_limits<int>::max();
		index[i] = INT_MAX;
		node_in[i] = false;
	}

	parent[0] = -1;
	index[0] = 0;
	int u;

	for(i=0; i<verts-1; i++){
	
		u = getMin(index, node_in);
		
		node_in[u] = true;
		
		int v;
		
		for(v=0; v<verts; v++){

			if(graph[u][v] && node_in[v] == false && graph[u][v] < index[v]){

				parent[v] = u;
				index[v] = graph[u][v];
			}
		}										
	}

	// put into the agdjacency list
	
	for(i=0; i<verts; i++){

		if(parent[i] != -1){

			adj_list[i].push_back(parent[i]);;
			adj_list[parent[i]].push_back(i);
		}
	}	
}

void TSP::oddPair(){

	for (int i = 0; i < verts; i++) {

		if((adj_list[i].size()%2) != 0){

			oddPairs.push_back(i);
		}
	}
}

void TSP::matchOdds(){

	std::vector<int>::iterator start, temp;
	//std::vector<int>::iterator temp;
	
	int len;
	int nearest;

	int i;
	
	// find the odds and add to oddPairs
	
	/*for(i=0; i<verts; i++){

		if((adj_list[i].size()%2) != 0){

			oddPairs.push_back(i);
		}
	}*/

	oddPair();
	
	while(!oddPairs.empty()){

		vector<int>::iterator iter;
		iter = oddPairs.begin()+1;
		vector<int>::iterator stop = oddPairs.end();
		start = oddPairs.begin();

		//len = std::numeric_limits<int>::max();	
		len = INT_MAX;
		for(; iter != stop; iter++){

			if(graph[*start][*iter] < len){

				len = graph[*start][*iter];
				nearest = *iter;
				temp = iter;
			}
		}
		
		adj_list[*start].push_back(nearest);
		adj_list[nearest].push_back(*start);
		oddPairs.erase(temp);
		oddPairs.erase(start);
	}
}

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

	path.push_back(begin);

	while(!stack.empty() || temp[check].size()>0){

		if(temp[check].empty()){

			path.push_back(check);
			//check = stack.top();
			check = stack.top();
			stack.pop();
			//begin = check;
		}

		else{

			stack.push(check);
			close = temp[check].back();
			temp[check].pop_back();
			for(i=0; i<temp[close].size(); i++){

				if(temp[close][i] == check){

					temp[close].erase(temp[close].begin()+i);
					break;
				}
			}

			check = close;

		}
	}
	
	path.push_back(check);
}

void TSP::eulerHP(vector<int> &path, int &cost){
	
	// initialize variables
	
	bool node_in[verts];
	int i;
	//cost = 0;
	vector<int>::iterator curr = path.begin();
	vector<int>::iterator next = path.begin() + 1;

	int starting = path.front();

	for(i=0; i<verts; i++){

		node_in[i] = 0;
	}		
	
	cost = 0;

	node_in[starting] = 1;
	
	while(next != path.end()){

		if(!node_in[*next]){

			cost += graph[*curr][*next];
			curr = next;
			node_in[*curr] = 1;
			next = curr +1;
		}	

		else{

			next = path.erase(next);
		}
	}

	cost += graph[*curr][*next];
}

int TSP::findPath(int begin){

	// initialize variables
	
	vector<int> paths;
	int len;

	eulerCircuit(begin, paths);
	eulerHP(paths, len);

	return len;		
}

void TSP::sendFile(){

	// initialize variables
	
	ofstream send_out;
	//vector<int>::iterator iter;

	send_out.open(file_out.c_str(), ios::out);
	send_out << lenPath << endl;
	
	for( vector<int>::iterator iter = cycle.begin(); iter != cycle.end(); ++iter){

		send_out << *iter << endl;
	}	
	
	send_out.close();
}

/*int TSP::tour_size(){
	
	return verts;
}*/
