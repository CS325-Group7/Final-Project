/********
 * Authors: Kate Galle, Lauren Galle, Peri Cabrales
 * Class: CS 325
 * Assignment: Final Project
 * Date: 3-13-2020
 * Description: handles the main function that reads in command line input and calls the
 * functions for the Christofides algorithm
 * *******/

#include "tsp.h"
#include <ctime>
#include <iostream>

int main(int argc, char** argv){

	//start timer for the algorithm

	clock_t start, end;

	//hold the total time at the end

	double time;

	//get current time as the start

	start = clock();

	// initialize variables

	string in;
	string out;
	int tourSize;
	int opt;
	opt = INT_MAX;
	int index;
	long i;
	int fin;

	out = argv[1];
	in = out;

	// make appropriate format
	
	out.append(".tour");

	TSP tour(in, out);

	// size is the number of vertices
	
	tourSize = tour.verts;

	// call Christofides functions
	
	tour.fillGraph();
	cout << "finished filling"<< endl;
	
	tour.primsMST();
	cout << "finshed mst"<< endl;

	tour.matchOdds();
	cout << "finished finding odds" << endl;

	for(i=0; i<tourSize; i++){

		// find the best path length

		vector<int> paths;
		tour.eulerCircuit(i, paths);
		int len;
		tour.eulerHP(paths, len);
		
		fin = len;

		// find the start

		tour.lengths[i][0] = i;
		
		// find the end

		tour.lengths[i][1] = fin;

		// find the best length there is

		if(tour.lengths[i][1] < opt){

			index = tour.lengths[i][0];
		
			// update the optimal

			opt = tour.lengths[i][1];
		}
	}

	// make the path for the best tour found
	
	tour.eulerCircuit(index, tour.cycle);
	tour.eulerHP(tour.cycle, tour.lenPath);

	cout << "Final length: " << tour.lenPath << endl;

	//end timer for the algorithm

	end = clock();

	//hold the difference between the two times

	double difference = ((double)end)-((double)start);

	//calculate the difference between now and the starting time

	time = difference / (double(CLOCKS_PER_SEC)/(double(1000))); //clocks_per_sec will give the number of seconds per tick of the clock to help find the overall seconds

	cout<<"Time (Milliseconds): "<<time<<endl;

	// send the output to the files
	
	tour.sendFile();

	return 0;
}
