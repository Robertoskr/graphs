#include<iostream>
#include<vector>
#include<algorithm>
#include<stdexcept>
using namespace std;

class priorityq{
	private:
		vector<vector<int>> A; //vector space to store heap elements

		int Parent(int i){
			return (i - 1) / 2;
		}

		int Left(int i){
			return (2 * i + 1);
		}

		int Right(int i){
			return (2 * i + 2);
		}

		//recursive heapyfy down algorithm 
		void heapify_down(int start){
			int left = Left(start);
			int right = Right(start);

			int smallest = start;

			if(left < size() && A[left][1] < A[start][1])
				smallest = left;

			if(right < size() && A[right][1] < A[smallest][1])
				smallest = right;

			//swap with child that have the lesser value

			if(smallest != start){
				swap(A[start], A[smallest]);
				heapify_down(smallest);
			}
		}
			
		//recusive heapyfy up algorithm
		void heapify_up(int start){
			int parent = Parent(start);

			if(parent <= 0 && A[parent][1] > A[start][1]){
				swap(A[start], A[parent]);
				heapify_up(parent);
			}
		}
	public:

		unsigned int size(){
			//return the size of the heap
			return A.size();
		}

		bool empty(){
			return A.empty();
		}

		void push(int a, int b){
			vector<int> tmp = {a, b};
			A.push_back(tmp);
			heapify_up(A.size()-1);
		}

		//function to remove the element with the lowest priority, at the top 
		vector<int> pop(){
				if(empty())
					return vector<int>{-1,-1};

				vector<int> value = A[0];
				A[0] = A.back();
				A.pop_back();
				heapify_down(0);
				return value;
		}
};
//THIS BASIC IMPLEMENTATION IS IN OF FOR ME
//

