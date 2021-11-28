#include "graph.hpp"
#include <iostream>
using namespace std;

//here you can use the graph class as you want!!!

int main(){
	graph g(5, true);
	g.fromPrompt();
	g.dijskra(0);
}
