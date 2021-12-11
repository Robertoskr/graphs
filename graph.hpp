#include<iostream>
#include "unionfind.hpp"
#include "minheap.hpp"
#include <queue>
using namespace std;


/*
    GRAPH IMPLEMENTATIONS WITH FAMOUS ALGORITHMS ASOCIATED AND COMPLEMENT CLASSES;
    author: 
	Roberto Kalinovskyy
	email:
    roberto.kalinovskyy@gmail.com
*/


bool edgecompare(vector<int> &a, vector<int> &b){
    //a basic helper function for sorting the vectors in kruskal algorithm
	int size = a.size() -1;
	return a[size] < b[size];
}

//used int table, and graph::bestPath 
struct cell{
	int cost;
	int parent;
};
//used as result in besPath(int target); method
class BestPathResult{
    public:
        BestPathResult(int _n);
        BestPathResult* newResult(){
            BestPathResult* result = new BestPathResult(n);
            for(int i = 0; i < result->n; i++){
                result->parents[i] = parents[i];
            }
            result->cost = cost;
            return result;
        }
        void show(); //show the information of the result;
    private:
        int *parents;
        int cost;
        int n;
friend class graph;
};

BestPathResult::BestPathResult(int _n){
    parents = new int[_n];
    cost = 0;
    n = _n;
}

void BestPathResult::show(){
    cout << "THE RESULT IS: " << cost << endl;
    cout << n;
    while(n--){
        cout << "THE PARENT OF : " << n << " IS : " << parents[n] << endl;
    }
}


class edgenode{
	public:
        edgenode(int to, int w);
		edgenode(int to, int w, int cap);
    private:
		int y;
		int weight;
		edgenode *next;
		//used for network flows algorithms
		int capacity; //the max capacity of this edge; (different from the weight)
		int flow; //the actual flow of the edge;
friend class graph;
friend class flownode;
};

edgenode::edgenode(int to, int w){
	y = to;
	weight = w;
}

edgenode::edgenode(int to, int w, int cap){
	y = to;
	weight = w;
	capacity = cap;
}


class graph{
	public:
        graph(int n, bool _directed); //the constructor / initialization function
        void addEdge(int from, int to, int weight); //the driver function for inserting an edge
		void addEdge(int from, int to, int weight, int cap); //the driver function for inserting an edge
		void print(); //for printing the graph
		void dfs(int start); //the dfs traversal algorithm
		void dijskra(int from); //the famous dijsktra algorithm for finding the best path from one node to all(in weighted graphs)
		//function used in kruskal algorithm, return a vector of vectors, denoting the edges of the graph
		vector<vector<int>> tovector(); //transform the graph adjajency list , to a vector, of vectors
		int kruskal(); //kruskal minimum spaning tree algorithm, (for finding minimum spaning trees)
		BestPathResult* bestPath(int target); //driver and entry function for the traveling salesman problem
		void showInfo(); //show the information obtained in other algorithms to the prompt
        void fromPrompt(); //create a new graph with info from the prompt line (cmd)
		void fromPromptFlows(); //create a new graph from prompt for network flow algorithms
		void bfs(int start); //the bfs graph algorithm
		void processEdgeAfter(edgenode *actual); //process an edge after the traversal of the edge in bfs and dfs
		void processEdgeBefore(edgenode *actual); //process an edge before the traversal of the edge in the bfs and dfs
		void unweightedPath(int from, int to);
		int maximumFlowFulkerson(int from , int to); //find the maximum flow from one node to another;
		int** toMatrix(); //converts the current adjancecy list to a matrix based graph
		int** floydWarshall(); //floyd wharsall dp algorithm for computing all sortest paths
	private:
        BestPathResult* _bestPath(int target, int start, bool *visited); //the actual logic of traveling salesman problem
        bool* newVisited(bool old[], int pos); //return a new visited array, used for traversing a new time int the _bestPath() function
		bool allVisited(bool *visited); //return if a visited array is full (if is full is visited == true)
        void _addEdge(int from, int to, int weight, bool flow, int cap=0); //the actual logic for adding an edge

        //the data that the graph is holding for its algorithms
        edgenode *edges[1000];
		int nvertices;
		int nedges;
		int parent[1000]; //parent of vertex y
		int distance[1000]; //distance to vertex y from vertex x
		int entry[1000]; //entry of the vertex
		int exit[1000]; //exit of the vertex
		bool discovered[1000];
		bool processed[1000];
		bool intree[1000];
		int time = 0; //used for dfs or bfs
		bool directed;
};

graph::graph(int n, bool _directed = true){ //ask the number of vertices for initialization
	nvertices = n;
	directed = _directed;
	for(int i = 0; i < n; i++){
		parent[i] = -1;
		intree[i] = false;
		distance[i] = 10000000;
		entry[i] = -1;
		exit[i] = -1;
		discovered[i] = false;
		processed[i] = false;
	}
}

void graph::addEdge(int from, int to, int weight){
	_addEdge(from, to, weight, false);
	if(directed == false)
		_addEdge(to, from , weight, false);
}

void graph::addEdge(int from, int to, int weight, int cap){
	_addEdge(from, to, weight, true, cap);
	if(directed == false)
		_addEdge(to, from, weight, true, cap);
}

void graph::_addEdge(int from, int to, int weight, bool flow, int cap=0){
	edgenode *p;
	if(!flow)
		p = new edgenode(to, weight);
	else
		p = new edgenode(to, weight, cap);
	p->next = edges[from];
	edges[from] = p;
	nedges++;
}


void graph::print(){
	for(int i = 0 ; i < nvertices; i++){
		edgenode *p;
		if(edges[i] != NULL){
			p = edges[i];
			cout << i << "->";
			while(p != NULL){
				cout << "{" << p->y << ", " << p->weight << "}";
				p = p->next;
			}
			cout << '\n';
		}
	}
}

void processEdgeAfter(edgenode *actual){
	//this function is free to customize depending of your neecesity
	return;
}

void processEdgeBefore(edgenode *actual){
	//this function is free to customize depenfing of your needings
	return;
}

void graph::bfs(int start){
	queue<int> q;
	edgenode *actual;
	q.push(start);
	int actualvertice;
	while(!q.empty()){
		actualvertice = q.front();
		discovered[actualvertice] = true;
		q.pop();
		actual = edges[actualvertice];
		//processEdgeBefore(actual);
		while(actual != NULL){ //traverse on the actual node;
			if(!discovered[actual->y]){
				q.push(actual->y);
				parent[actual->y] = actualvertice;
			}
			actual = actual->next;
		}
		//processEdgeAfter(actual);
		processed[actualvertice] = true;
	}
}

void graph::unweightedPath(int from, int to){
	bfs(from);
	int actual = to;
	while(actual != from){
		if(actual == -1)
			cout << "theres isnt a path from " << from << " to " << to << endl;
		cout << actual << endl;
		actual = parent[actual];
	}
	if(actual != -1)
		cout << actual << endl;
}


void graph::dfs(int start){
	edgenode *p;
	edgenode *tmp;
	p = edges[start];
	discovered[start] = true;
	entry[start] = time;
	time++;
	if(p != NULL){
		tmp = p;
		while(tmp != NULL){ //traverse all the edges of this vertex (start)
			if(!discovered[tmp->y]){ //if not discovered go and discover this vertex (dfs(tmp->y))
				//processEdgeAfter(tmp);
				parent[tmp->y] = start; 
				dfs(tmp->y);
			}
			tmp = tmp->next;
		}
	}
	//processEdgeBefore(tmp);
	processed[start] = true;
	exit[start] = time;
}

void graph::dijskra(int from){
	edgenode *p;
	edgenode *tmp;
	priorityq heap;
	int v = from; //start node
	distance[v] = 0;
	while(!intree[v] && v != -1){
		cout << v << " distance of v is : " << distance[v] << '\n';
		intree[v] = true;
		p = edges[v];
		while(p != NULL){
			if(distance[p->y] > p->weight + distance[v]){
				distance[p->y] = p->weight + distance[v];
				parent[p->y] = v;
				heap.push(p->y, distance[p->y]);
			}
			p = p->next;
		}
		v = heap.pop()[0]; //the next less cost edge;
	}
}

int graph::kruskal(){
	union_find un(nvertices);
	int w = 0;//weight of the minimum spanning tree
	//wee ned to sort the edges of our graph, but how we can sort it? if wee have it with 
	//a strage structure of nodes and pointers, lets spend nlogn for sorting it and then linear time for creating the minimum spanning tree
	vector<vector<int>> edg = tovector();
	sort(edg.begin(), edg.end(), &edgecompare);
	int i = 0;
	int intree = 0;
	while(intree != nvertices-1){
		if(un.find(edg[i][0]) != un.find(edg[i][1])){
				cout << "edge in the mst: " << edg[i][0] << edg[i][1] << edg[i][2] << '\n';
				w += edg[i][2];
				un.Union(edg[i][0], edg[i][1]);
				intree++;
		}
		i++;
	}
	return w;
}

vector<vector<int>> graph::tovector(){
    vector<vector<int>> result;
	edgenode *p;
	for(int i = 0; i < nvertices; i++){
		if(edges[i] != NULL){
			p = edges[i];
			while(p != NULL){
				vector<int> actual{i, p->y, p->weight};
				result.push_back(actual);
				p = p->next;
			}
		}
	}
	return result;
}

bool *graph::newVisited(bool old[], int pos){
    bool *result = new bool[nvertices];
    for(int i = 0; i < nvertices; i++){
        result[i] = old[i];
    }
    result[pos] = true;
    return result;
}

bool graph::allVisited(bool *visited){
    for(int i = 0; i < nvertices; i++)
		if(!visited[i])
			return false;
	return true;
}


BestPathResult* graph::bestPath(int target){
    BestPathResult *result;
    BestPathResult *actualresult;
    edgenode *start = edges[target];
    bool visited[nvertices];
    int *cost = new int[2]{0,0};
    int *actualcost;
    int bestpath;
    for( ; start != NULL ; start = start->next){
    	actualresult = _bestPath(target, start->y, visited);
    	actualresult->cost += start->weight;
    	if(actualresult->cost < result->cost || result->cost == 0){
    		result = actualresult;
    		bestpath = start->y;
    	}
    }
    result->parents[target] = bestpath;
    return result;
}

BestPathResult* graph::_bestPath(int target, int start, bool* visited){
    BestPathResult *result = new BestPathResult(nvertices);
	if(start == target){ //base case of the algorithm
		visited[start] = true;
		if(allVisited(visited)){
			visited[start] = false;
			result->cost = 0;
			return result;
		}
		else{
            visited[start] = false;
			result->cost = -1;
			return result;
		}
	}
	edgenode *p = edges[start]; //for traversing this actual node of the graph
    BestPathResult *actualresult;
    BestPathResult *nextresult = result->newResult(); //the result passed to this recursive function
	int bestpath; //the bestpath from this actual node to get to the target node;
	bool *newvisited = newVisited(visited, start);
	for( ; p != NULL; p = p->next){
		if(!visited[p->y]){//p is not visited , do the things with i
			actualresult = _bestPath(target, p->y, newvisited);
			if(actualresult->cost != -1){
				actualresult->cost += p->weight;
				if(actualresult->cost < result->cost || result->cost == 0){
					result = actualresult;
					bestpath = p->y;
				}
			}
		}
	}
    //what if we do not have avaiable vertices?
    if(result->cost == 0){
        result->cost = -1;
        return result;
    }
	result->parents[start] = bestpath;
	return result;
}

int** graph::toMatrix(){
	int **resultGraph;
	resultGraph = new int*[nvertices];
	for(int i = 0 ; i < nvertices; i++){
		resultGraph[i] = new int[nvertices];
		for(int b = 0; b < nvertices; b++){
			resultGraph[i][b] = -1;
		}
	}
	edgenode *tmp;
	for(int i = 0; i < nvertices; i++){
		tmp = edges[i];
		while(tmp != NULL){
			resultGraph[i][tmp->y] = tmp->weight;
		}
	}
	return resultGraph;
}

int** graph::floydWarshall(){
	int **graph = toMatrix();
	int **dist = toMatrix(); //initialize the result matrix as the same as the input matrix(the actual matrix)
	int i, j, k;
	for(k = 0; k < nvertices; k++){
		for(i = 0; i < nvertices; i++){
			for(j = 0; j < nvertices; j++){
				if(dist[i][j] > (dist[i][k] + dist[k][j]) && dist[i][j] != -1 && dist[i][k] != -1){
					dist[i][j] = dist[i][k] + dist[k][j];
				}
			}
		}
	}
	return dist;
}

int graph::maximumFlowFulkerson(int from, int to){
	return 0;
}

void graph::showInfo(){
	cout << "DISCOVERED\n";
	for(int i = 0; i < nvertices; i++){
		cout << discovered[i] << ", ";
	}
	cout << endl;
	cout << "PROCESSED\n";
	for(int i = 0; i < nvertices; i++){
		cout << processed[i] << ", ";
	}
	cout << endl;
	cout << "entry\n";
	for(int i = 0; i < nvertices; i++){
		cout << entry[i] << ", ";
	}
	cout << endl;
	cout << "exit\n";
	for(int i = 0; i < nvertices; i++){
		cout << exit[i] << ", ";
	}
	cout << endl;
	cout << "parent\n";
	for(int i = 0; i < nvertices; i++){
		cout << parent[i] << ", ";
	}
}

void graph::fromPrompt(){
	int n;
	cin >> n;
	while(n--){
		int from, to , weight;
		cin >> from >> to >> weight;
		addEdge(from, to, weight);
	}
}

void graph::fromPromptFlows(){
	int n;
	cin >> n;
	while(n--){
		int from, to , weight, cap;
		cin >> from >> to >> weight >> cap;
		addEdge(from, to, weight);
	}
}