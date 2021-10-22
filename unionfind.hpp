#include <vector>
#include <iostream>
using namespace std;

class union_find{
	public:
		union_find(int n){
			v.resize(n);
			for(int i = 0; i < n; i++){
				v[i] = i;
			}
		}

		int find(int x){
			if(x == v[x])
				return x;

			return v[x] = find(v[x]);
		}

		void Union(int x, int y){
			int px = find(x);
			int py = find(y);
			if (px != py){
				v[px] = v[y];
			}
		}

		int size(){
			int ans = 0;
			for(int i = 0; i < v.size(); i++){
				if(v[i] == i) i++;
			}
			return ans;
		}
	private:
		vector<int> v;

};
