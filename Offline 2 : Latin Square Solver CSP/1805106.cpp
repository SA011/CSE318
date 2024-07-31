#include <bits/stdc++.h>
using namespace std;

#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt")

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
const int N = 16;
using pii = pair<int, int>;
using ull = unsigned int;
using ll = long long;

int mat[N][N];
ull row[N], col[N];
int rowAssign[N], colAssign[N];
int temp[N];
int cnt[N];
ll nodes = 0, backtracks = 0;
set<pii> baki;
bool checkrow(int r, int n){
		for(int i = 1; i <= n; i++)temp[i] = 0;
		for(int i = 0; i < n; i++){
			if(mat[r][i] && temp[mat[r][i]])return false;
			temp[mat[r][i]] = 1;
		}
		return true;
}
bool checkcol(int c, int n){
		for(int i = 1; i <= n; i++)temp[i] = 0;
		for(int i = 0; i < n; i++){
			if(mat[i][c] && temp[mat[i][c]])return false;
			temp[mat[i][c]] = 1;
		}
		return true;
}

bool check(int n){
	for(int i = 0; i < n; i++)
		if(!checkrow(i, n) || !checkcol(i, n))return false;
	return true;
}
void assignCalc(int n){
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			rowAssign[i] += (mat[i][j] != 0);
			colAssign[j] += (mat[i][j] != 0);
			if(mat[i][j] == 0)baki.emplace(i, j);
			else cnt[mat[i][j]]++;
		}
	}
}

pii vah5(int n){
	if(baki.empty())
		return {-1, -1};
	int i, j;
	for(auto x : baki){
		tie(i, j) = x;
		int u = n - __builtin_popcount(row[i] | col[j]);
		if(u <= 0)return {-2, -2};
	}		
	int u = rng() % baki.size();
	auto it = baki.begin();
	advance(it, u);
	return *it;
}
pii vah4(int n){
	int nom = 1, denom = 0;
	pii ret = {-1, -1};
	int i, j;
	for(auto x : baki){
		tie(i, j) = x;
		int u = 2 * n - rowAssign[i] - colAssign[j] - 1;		//VAH2
		int v = n - __builtin_popcount(row[i] | col[j]);  //VAH1
		if(v == 0)return {-2, -2};
		if(v * denom <= u * nom){
			denom = u;
			nom = v;
			ret = x;
		}
	}
	return ret;

}
pii vah3(int n){
	int mn = N, mx = -1;
	pii ret = {-1, -1};
	int i, j;
	for(auto x : baki){
		tie(i, j) = x;
		int u = 2 * n - rowAssign[i] - colAssign[j] - 1;		//VAH2
		int v = n - __builtin_popcount(row[i] | col[j]);  //VAH1
		if(v == 0)return {-2, -2};
		if(v < mn || v == mn && u > mx){
			mn = v;
			mx = u;
			ret = x;
		}
	}
	return ret;

}
pii vah2(int n){
	int mx = -1;
	pii ret = {-1, -1};
	int i, j;
	for(auto x : baki){
		tie(i, j) = x;
		int u = 2 * n - rowAssign[i] - colAssign[j] - 1;
		if((row[i] | col[j]) == (1 << n) - 1)return {-2, -2};
		assert(u >= 0);
		if(u >= mx){
			mx = u;
			ret = x;
		}
	}
	return ret;

}
pii vah1(int n){
	int mn = N;
	pii ret = {-1, -1};
	int i, j;
	for(auto x : baki){
		tie(i, j) = x;
		int u = n - __builtin_popcount(row[i] | col[j]);
		if(u <= 0)return {-2, -2};
		if(u < mn){
			mn = u;
			ret = x;
		}
	}		
	return ret;
}


function<pii(int n)> vah[5] = {
	vah1, vah2, vah3, vah4, vah5
};
vector<int> get_order(int r, int c, int n){
	vector<int> ret;
	ull x = row[r] | col[c];
	for(int i = 0; i < n; i++)if(!(x >> i & 1))ret.push_back(i + 1);
	//int cnt2[n];
	//for(int i = 0; i < n; i++)cnt2[i] = 0;
	//for(int i = 0; i < n; i++)for(int j = 0; j < n; j++)cnt2[j] += (mat[r][i] != 0) && !((row[r] & col[i]) >> j & 1);
	//for(int i = 0; i < n; i++)for(int j = 0; j < n; j++)cnt2[j] += (mat[i][c] != 0) && !((row[i] & col[c]) >> j & 1);
	
	
	sort(ret.begin(), ret.end(), [&](int a, int b){
		return cnt[a] < cnt[b];
		//return pii(cnt2[a - 1], cnt[a]) < pii(cnt2[b - 1], cnt[b]);
	});
	return ret;
}

bool backtrack(int n, int vh){
	int r, c;
	tie(r, c) = vah[vh](n);
	nodes++;
	//cout << r << ' ' << c << endl;
	if(r == -1){
		//complete;
		return true;
	}
	if(r == -2){
		backtracks++;
		return false;
	}
	baki.erase(baki.find(pii(r, c)));
	rowAssign[r]++;
	colAssign[c]++;
	
	for(int x : get_order(r, c, n)){
		mat[r][c] = x;
		cnt[x]++;
		if(checkcol(c, n) && checkrow(r, n)){
			if(backtrack(n, vh))return true;
		}else{
			backtracks++;
			nodes++;
		}
		cnt[x]--;
	}
	
	mat[r][c] = 0;
	rowAssign[r]--;
	colAssign[c]--;
	baki.emplace(r, c);
	return false;
}
bool forwardCheck(int n, int vh){
	nodes++;
	int r, c;
	tie(r, c) = vah[vh](n);
	if(r == -1){
		//complete;
		return true;
	}
	if(r == -2){
		backtracks++;
		return false;
	}
	baki.erase(baki.find(pii(r, c)));
	
	rowAssign[r]++;
	colAssign[c]++;
	for(int x : get_order(r, c, n)){
		mat[r][c] = x;
		cnt[x]++;
		//
		row[r] ^= (ull)1 << x - 1;
		col[c] ^= (ull)1 << x - 1;
		//assert(checkcol(c, n) && checkrow(r, n));
		//
		if(forwardCheck(n, vh))return true;
		//
		row[r] ^= (ull)1 << x - 1;
		col[c] ^= (ull)1 << x - 1;
		cnt[x]--;
		//
	}
	
	mat[r][c] = 0;
	rowAssign[r]--;
	colAssign[c]--;
	baki.emplace(r, c);
	return false;
}
bool forwardChecker(int n, int vh){
	//preparing domain
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++)if(mat[i][j]){
			row[i] |= ((ull)1 << mat[i][j] - 1);
			col[j] |= ((ull)1 << mat[i][j] - 1);
		}
	}
	return forwardCheck(n, vh);
}

function<bool(int,int)> solver[2] = {
	backtrack, forwardChecker
};

int main(int argc, char* argv[]){
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	int vh = 0, solve = 1;
	if(argc >= 2){
		solve = atoi(argv[1]);
	}
	if(argc >= 3){
		vh = atoi(argv[2]);
	}
	int n;
	cin >> n;
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++)cin >> mat[i][j];
	}
	
	clock_t st = clock();
	assignCalc(n);
	
	
	if(check(n) && solver[solve](n, vh)){
		for(int i = 0; i < n; i++){
			for(int j = 0; j < n; j++)cout << setw(2) << mat[i][j] << ' ';
			cout << "\n";
		}
	}else{
		cout << "UNSOLVABLE\n";
	}
	
	clock_t en = clock();
	cout << "NUMBER OF NODES: " << nodes << "\n";
	cout << "NUMBER OF BACKTRACKS " << backtracks << "\n";
	cout << "ELAPSED TIME: " << fixed << setprecision(6) << (en - st) * 1000.0 / CLOCKS_PER_SEC << "ms\n";
	
}
/*
3
0 0 1
0 0 2
0 0 0
*/
