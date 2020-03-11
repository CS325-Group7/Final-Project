/********
 * Authors: Kate Galle, Lauren Galle, Peri Cabrales
 * Class: CS 325
 * Assignment: Final Project
 * Date: 3-13-2020
 * Description: handles the main function that reads in command line input and calls the 
 * functions for the Christofides algorithm
 * *******/

#include "tsp.h"
#include <iostream>

int main(int argc, char** argv){
	
	// initialize variables
	
	string in;
	string out;
	int tourSize;
	int opt;
	opt = INT_MAX;
	int index;
	long i;
	int fin;

	in = out = argv[1];
	out.append(".tour");

	TSP tour(in, out);
	tourSize = tour.tour_size();	

	cout << "fill"<< endl;	
	tour.fillGraph();
	cout << "mst"<< endl;
	tour.primsMST();
	cout << "odds"<<endl;
	tour.matchOdds();

	for(i=0; i<tourSize; i++){

		cout << "eulert"<<endl;
		//tour.eulerCircuit(index, tour.cycle);	
		cout << "ham"<<endl;
		//tour.eulerHP(tour.cycle, tour.lenPath);
		cout << "best"<<endl;

		fin = tour.findPath(i);

		tour.lengths[i][0] = i;
		tour.lengths[i][1] = fin;		
		
		if(tour.lengths[i][1] < opt){

			index = tour.lengths[i][0];
			opt = tour.lengths[i][1];
		}
	}

	cout << "euler"<<endl;	
	tour.eulerCircuit(index, tour.cycle);
	tour.eulerHP(tour.cycle, tour.lenPath);

	tour.sendFile();	

	return 0;
}
