#include<iostream>
#include "unionfind.hpp"
#include "minheap.hpp"
using namespace std;
//graphs implementations, with adjajency list, and adjajency matrix

//the clasess are intentionally left all public for more simplicity in the algorithms

class edgenode{
	public:
		edgenode(int to, int w){
			y = to;
			weight = w;
		}
		
		int y;
		int weight;
		edgenode *next;
};


class graph{
	public:
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
		graph(int n){ //ask the number of vertices for initialization
			nvertices = n;
			for(int i = 0; i < n; i++){
				parent[i] = -1;
				intree[i] = false;
				distance[i] = 10000000;
				entry[i] = -1;
				exit[i] = -1;
				discovered[i] = false;
				processed[i] = false;
			}
		}//initialization
		
		//adding a edge
		void addEdge(int from, int to, int weight){
			edgenode *p = new edgenode(to, weight);
			p->next = edges[from];
			edges[from] = p;
			nedges++;
		}

		void print(){
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

		void dfs(int start){
			edgenode *p;
			edgenode *tmp;
			p = edges[start];
			discovered[start] = true;
			entry[start] = time;
			time++;
			if(p != NULL){
				tmp = p;
				while(tmp != NULL){
					if(!discovered[tmp->y]){
						parent[tmp->y] = start; 
						dfs(tmp->y);
					}
					tmp = tmp->next;
				}
			}
			processed[start] = true;
			exit[start] = time;
		}

		void dijskra(int from){ //from wich vertex do you want to start the algorithm
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
				v = heap.pop()[0];
			}
		}

		//function used in kruskal algorithm, return a vector of vectors, denoting the edges of the graph
		vector<vector<int>> tovector(){
			vector<vector<int>> result;
			edgenode *p;
			for(int i = 0; i < nvertices; i++){
				if(edges[i] != NULL){
					p = edges[i];
					while(p != NULL){
						vector<int> actual = {i, p->y, p->weight};
						result.push_back(actual);
						p = p->next;
					}
				}
			}
			return result;
		}

		bool edgecompare(vector<int> &a, vector<int> &b){
			return a[2] < b[2];
		}

		int kruskal(){
			union_find un(nvertices);
			int w = 0;//weight of the minimum spanning tree
			//wee ned to sort the edges of our graph, but how we can sort it? if wee have it with 
			//a strage structure of nodes and pointers, lets spend nlogn for sorting it and then linear time for creating the minimum spanning tree
			vector<vector<int>> edg = tovector();
			sort(edg.begin(), edg.end(), &graph::edgecompare);
			for(int i = 0; i < nvertices-1; i++){
				if(un.find(edg[i][0]) != un.find(edg[i][1])){
						printf("edge in minimum spaning tree (%d, %d)\n", edg[i][0], edg[i][1]);
						w += edg[i][2];
						un.Union(edg[i][0], edg[i][1]);
				}
			}
			return w;
		}

		void showinfo(){
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
};

int main(){
	graph g(7);
	g.addEdge(0,1, 3);
	g.addEdge(0,2, 3);
	g.addEdge(0, 4, 10);
	g.addEdge(1,2, 3);
	g.addEdge(1, 5, 2);
	g.addEdge(2,3, 3);
	g.addEdge(3,4, 3);
	g.addEdge(4,0, 3);
	g.addEdge(4,1, 3);
	g.addEdge(4,3, 3);
	g.addEdge(5, 6, 1);
	g.addEdge(6, 4, 2);
	g.print();
	cout << g.kruskal();
	return 0;
}
