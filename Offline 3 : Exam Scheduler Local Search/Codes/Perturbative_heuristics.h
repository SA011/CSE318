struct Perturbative_heuristic{
	AllCourse* allcourse;
	
	//constructor
	Perturbative_heuristic(AllCourse* _allcourse){
		allcourse = _allcourse;
	}
	
	virtual bool performSwap(function<int(int)> &eval, int &penalty) = 0;
};
struct KempeChain_Interchange : Perturbative_heuristic{
	
	//constructor
	KempeChain_Interchange(AllCourse* _allcourse) : Perturbative_heuristic(_allcourse){
	}
	void dfs(Course *cur, int nxt, vector<Course*> &A, vector<Course*> &B, vector<int> &vis){
		vis[cur -> id] = 1;
		A.push_back(cur);
		for(Course* u : cur -> adj){
			//assert(u -> time_slot != cur -> time_slot);
			if(u -> time_slot == nxt && !vis[u -> id]){
				dfs(u, cur -> time_slot, B, A, vis);
			}
		}
	}
	bool performSwap(function<int(int)> &eval, int &penalty){
		int V = getRandom(0, allcourse -> num - 1);
		if(allcourse -> all[V] -> adj.size() == 0){
			//V has no neighbour to swap
			return false;
		}
		vector<Course*> A, B;
		vector<int> vis(allcourse -> mx_crs_id + 1);
		int U = getRandom(0, allcourse -> all[V] -> adj.size() - 1);
		Course *it = allcourse -> all[V] -> adj[U];
		
		int clr1 = allcourse -> all[V] -> time_slot;
		int clr2 = (it) -> time_slot;
		dfs(allcourse -> all[V], clr2, A, B, vis);
		
		for(Course* cur : A){
			//assert(cur -> time_slot == clr1);
			cur -> time_slot = clr2;
		}
		for(Course* cur : B){
			
			//assert(cur -> time_slot == clr2);
			cur -> time_slot = clr1;
		}
		
		int nwPenalty = getPenalty(allcourse, eval);
		if(nwPenalty < penalty){
			penalty = nwPenalty;
			return true;
		}
		
		
		for(Course* cur : A){
			cur -> time_slot = clr1;
		}
		for(Course* cur : B){
			cur -> time_slot = clr2;
		}
		
		return false;
		
	}
};

struct PairSwap : Perturbative_heuristic{
	
	//constructor
	PairSwap(AllCourse* _allcourse) : Perturbative_heuristic(_allcourse){
	}
	
	void dfs(Course *cur, int nxt, vector<Course*> &A, vector<Course*> &B, vector<int> &vis){
		if(vis[cur -> id])return;
		vis[cur -> id] = 1;
		A.push_back(cur);
		for(Course* u : cur -> adj){
			//assert(u -> time_slot != cur -> time_slot);
			if(u -> time_slot == nxt){
				dfs(u, cur -> time_slot, B, A, vis);
			}
		}
	}
	
	//bool check(Course *U, Course *V){
		//int clr1 = U -> time_slot;
		//int clr2 = V -> time_slot;
		//for(Course *c : U -> adj){
			//if(c -> time_slot == clr2 && c -> id != V -> id)return false;
		//}
		//for(Course *c : V -> adj){
			//if(c -> time_slot == clr1 && c -> id != U -> id)return false;
		//}
		//return true;
	//}
	bool performSwap(function<int(int)> &eval, int &penalty){
		int U, V, clr1 = 0, clr2 = 0;
		
		for(int i = 0; i < 100 && clr1 == clr2; i++){
			U = getRandom(0, allcourse -> num - 1);
			V = getRandom(0, allcourse -> num - 1);
			clr1 = allcourse -> all[U] -> time_slot;
			clr2 = allcourse -> all[V] -> time_slot;
			
		}
		if(clr1 == clr2)
			return false;
		
		vector<Course*> A, B;
		vector<int> vis(allcourse -> mx_crs_id + 1);
		
		dfs(allcourse -> all[U], clr2, A, B, vis);
		dfs(allcourse -> all[V], clr1, B, A, vis);
		
		for(Course* cur : A){
			//assert(cur -> time_slot == clr1);
			cur -> time_slot = clr2;
		}
		for(Course* cur : B){
			
			//assert(cur -> time_slot == clr2);
			cur -> time_slot = clr1;
		}
		
		int nwPenalty = getPenalty(allcourse, eval);
		if(nwPenalty < penalty){
			penalty = nwPenalty;
			return true;
		}
		
		
		for(Course* cur : A){
			cur -> time_slot = clr1;
		}
		for(Course* cur : B){
			cur -> time_slot = clr2;
		}
		
		return false;
		
	}
};
