#include "graph.hpp"
#include <iostream>
using namespace std;

//test of the graph class

int main(){
	graph g(5, false);
	g.fromPrompt();
	BestPathResult *result = g.bestPath(0);
	result->show();
	g.showInfo();
	g.print();
	return 0;
}
