#include <bits/stdc++.h>
using namespace std;

using MAT = vector<vector<int>>;
namespace Npuzzle{
	int countInversion(MAT &v){
		int ans = 0, n = v.size();
		for(int i = 0; i < n; i++){
			for(int j = 0; j < n; j++){
				if(v[i][j] == 0)continue;
				for(int k = 0; k < i; k++){
					for(int l = 0; l < n; l++)ans += (v[k][l] > v[i][j]);
				}
				for(int k = 0; k < j; k++)ans += (v[i][k] > v[i][j]);
			}
		}
		return ans;
	}

	pair<int, int> getBlank(MAT &v){
		int n = v.size();
		for(int i = 0; i < n; i++){
			for(int j = 0; j < n; j++){
				if(v[i][j] == 0){
					return {i, j};
				}
			}
		}
		return {0, 0};
	}

	bool isSolvable(MAT &v){
		vector<int> cnt(v.size() * v.size());
		for(int i = 0; i < v.size(); i++){
			for(int j = 0; j < v.size(); j++){
				if(v[i][j] < 0 || v[i][j] >= v.size() * v.size())return false;
				if(cnt[v[i][j]])return false;
				cnt[v[i][j]] = 1;
			}
		}
		if(v.size() & 1){
			return (countInversion(v) & 1) == 0;
		}
		int blank = getBlank(v).first;
		return (blank & 1) != (countInversion(v) & 1);
	}
	int T;
	int explored = 0, expanded = 0;
	const int N = 2e7 + 5;
	MAT config[N];
	pair<int, int> blank[N];
	map<MAT, int> indx;
	int visited[N];
	int dis[N];
	int heuristicValue[N];
	int par[N];
	function<int(MAT&)> heuristic;
	int n;
	const int INF = 1e9;

	int get(MAT &mat, int X = -1, int Y = -1){
		if(!indx.count(mat)){
			indx[mat] = T;

			config[T] = mat;
			if(X == -1)
				blank[T] = getBlank(mat);
			else
				blank[T] = {X, Y};
			heuristicValue[T] = (heuristic(mat));

			visited[T] = (0);

			dis[T] = (INF);

			par[T] = (-1);
			return T++;
		}

		return indx[mat];
	}

	void init(){
		T = 0;
		explored = expanded = 0;
		MAT temp(n, vector<int>(n, 0));
		for(int i = 0; i < n; i++){
			for(int j = 0; j < n; j++){
				temp[i][j] = i * n + j + 1;
			}
		}
		temp[n - 1][n - 1] = 0;
		get(temp);

	}
	int xi[] = {1, -1, 0, 0};
	int yi[] = {0, 0, 1, -1};
	int Rxi[] = {-1, 1, 0, 0};
	int Ryi[] = {0, 0, -1, 1};
	string pat = "UDLR";
	int move(int ind, int dir){
		int curX, curY;
		tie(curX, curY) = blank[ind];

		int nwX = curX + xi[dir];
		int nwY = curY + yi[dir];
		if(nwX < 0 || nwX >= n || nwY < 0 || nwY >= n)return ind;
		MAT nw = config[ind];
		swap(nw[curX][curY], nw[nwX][nwY]);
		return get(nw, nwX, nwY);
	}

	int moveRev(int ind, int dir){
		int curX, curY;
		tie(curX, curY) = blank[ind];

		int nwX = curX + Rxi[dir];
		int nwY = curY + Ryi[dir];
		if(nwX < 0 || nwX >= n || nwY < 0 || nwY >= n)return ind;
		MAT nw = config[ind];
		swap(nw[curX][curY], nw[nwX][nwY]);
		return get(nw);
	}

	string getPattern(int pr = false){
		int cur = 0;
		string ans;
		vector<MAT> all(1, config[cur]);
		while(par[cur] != -1){
			ans += pat[par[cur]];
			cur = moveRev(cur, par[cur]);
			if(pr)all.push_back(config[cur]);
		}
		reverse(ans.begin(), ans.end());
		if(pr){
			reverse(all.begin(), all.end());
			for(auto x : all){
				for(auto y : x){
					for(auto z : y)cout << z << ' ';
					cout << endl;
				}
				cout << "=============" << endl;
			}
		}
		return ans;
	}

	string solve(MAT &src, function<int(MAT&)> func){
			n = src.size();
			heuristic = func;

			init();

			//priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
			priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>, greater<tuple<int, int, int>>> pq;
			int sr = get(src);
			explored = 1;
			dis[sr] = 0;
			if(sr == 0){
				return "";
			}
			pq.emplace(dis[sr] + heuristicValue[sr], heuristicValue[sr], sr);
			while(!pq.empty()){
				int f, d, u;
				tie(f, d, u) = pq.top();
				pq.pop();
				if(visited[u])continue;
				visited[u] = 1;
				expanded++;
				for(int i = 0; i < 4; i++){
					int nx = move(u, i);
					if(visited[nx] == 0 && dis[nx] > dis[u] + 1){
						explored += (dis[nx] == INF);
						dis[nx] = dis[u] + 1;
						par[nx] = i;
						if(nx == 0){
							//goal state
							return getPattern();
						}
						pq.emplace(dis[nx] + heuristicValue[nx], heuristicValue[nx], nx);
					}
				}
			}

			assert(false);


	}
}

int manhatten(MAT &v){
	int n = v.size();
	int ans = 0;
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			int x = v[i][j] - 1;
			if(x == -1)continue;
			ans += abs(x / n - i) + abs(x % n - j);
		}
	}
	return ans;
}

int hamming(MAT &v){
	int n = v.size();
	int ans = 0;
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			int x = v[i][j] - 1;
			if(x == -1)continue;
			ans += (abs((x / n) - i) + abs((x % n) - j) > 0);
		}
	}
	return ans;
}

int main(){
	int n;
	cout << "Enter the number of rows: ";
	cin >> n;
	MAT v(n, vector<int>(n, 0));

	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			while(isspace(cin.peek()))cin.ignore();
			if(cin.peek() == '*'){
				char c;
				cin >> c;
				v[i][j] = 0;
			}else
				cin >> v[i][j];
		}
	}

	if(!Npuzzle::isSolvable(v)){
		cout << "Not Solvable\n";
		return 0;
	}
	cout << "Solvable\n";
	cout << "Choose Heuristics: \n 1.Manhatten \n 2.Humming\n";
	int t;
	cin >> t;
	while(t <= 0 || t > 2){
		cout << "Wrong Choice\n";
		cout << "Choose Heuristics: \n 1.Manhatten \n 2.Humming\n";
		cin >> t;
	}
	string ans;
	if(t == 1)
		ans = Npuzzle::solve(v, manhatten);
	else
		ans = Npuzzle::solve(v, hamming);
	Npuzzle::getPattern(true);
	cout << "OPTIMAL SOLUTION: " << ans.size() << "\n";
	//cout << ans << endl;
	cout << "EXPLORED NODES: " <<  Npuzzle::explored << "\n";
	cout << "EXPANDED NODES: " <<  Npuzzle::expanded << "\n";
}


/*

4
13 3 5 14
12 15 2 10
8 9 1 11
7 6 4 0

3
8 5 2
4 7 1
6 3 *
*/
