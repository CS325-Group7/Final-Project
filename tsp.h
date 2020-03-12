/********
 * Authors: Kate Galle, Lauren Galle, Peri Cabrales
 * Class: CS 325
 * Assignment: Final Project
 * Date: 3-13-2020
 * Description: Contains the function declarations for the algorithm's TSP class
 * ******/

// include libraries

#include <stdio.h>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <iomanip>
#include <queue>
#include <stack>
#include <vector>
#include <climits>
#include <stdio.h>
#include <limits>

using namespace std;

#ifndef TSP_H
#define TSP_H

class TSP{
	private:
		
		// create a struct to represent each city

		struct Cities{
			int x;
			int y;
		};
		
		// initialize other variables

		// holds the odd node pairs

		vector<int>oddPairs;
		
		string file_in;
		string file_out;

	public:

		// set constructor

		TSP(string in, string out);

		// set destructor

		~TSP();

		// initialize public vaiables

		// create an adjacency list as a data structure

		vector<int>*adj_list;

		// create a graph 

		int **graph;

		int verts;
		int lenPath;

		vector<Cities> cityList;
		vector<int> cycle;

		int **lengths;
	
		// set public functions
		
		void fillGraph();
		int getMin(int index[], bool node_in[]);
		void primsMST();
		void matchOdds();
		void addList(int parent[]);
		void eulerCircuit(int begin, vector<int> &path);
		void eulerHP(vector<int> &path, int &cost);
		void sendFile();
		int tour_size();
};
#endif
