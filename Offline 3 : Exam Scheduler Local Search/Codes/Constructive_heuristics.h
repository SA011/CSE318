
struct Constructive_heuristic{
	AllCourse* allcourse;
	
	//constructor
	Constructive_heuristic(AllCourse* _allcourse){
		allcourse = _allcourse;
	}
	
	virtual Course* getNextCourse() = 0;
	virtual void assignSlot(Course* crs, int slot) = 0;
};

///////////////////////////////////////////////////////////////////////

struct LargestDegree : Constructive_heuristic{
	vector<Course*> orderedCourses;
	
	LargestDegree(AllCourse* _allcourse) : Constructive_heuristic(_allcourse){
		orderedCourses = allcourse -> all;
		
		//sorting
		sort(orderedCourses.begin(), orderedCourses.end(), [&](const Course* a, const Course* b){
			return a -> degree < b -> degree;
		});
		
		//random shuffle same degree
		
		int last = 0;
		for(int i = 0; i < orderedCourses.size(); i++){
			if(orderedCourses[i] -> degree != orderedCourses[last] -> degree){
				//random ordering [last, i)
				random_shuffle(orderedCourses.begin() + last, orderedCourses.begin() + i);
				
				last = i;
			}
		}
		//last part [last, n)
		random_shuffle(orderedCourses.begin() + last, orderedCourses.end());
	}
	
	Course* getNextCourse(){
		if(orderedCourses.empty())return NULL;
		Course* ret = orderedCourses.back();
		orderedCourses.pop_back();
		return ret;
	}
	
	void assignSlot(Course* crs, int slot){
		crs -> time_slot = slot;
	}
};

///////////////////////////////////////////////////////////////////////

struct CoursePaircmp
{
  bool operator()(const pair<int, Course*> &A, const pair<int, Course*> &B) const  { 
		if(A.first == B.first)return A.second -> degree < B.second -> degree;
		return A.first < B.first;
	}
};

struct SaturationDegree : Constructive_heuristic{
	priority_queue<pair<int, Course*>, vector<pair<int, Course*>>, CoursePaircmp> pq;
	
	SaturationDegree(AllCourse* _allcourse) : Constructive_heuristic(_allcourse){
		for(Course *crs : allcourse -> all){
			pq.emplace(crs -> neig_color.size(), crs);
		}
	}
	
	Course* getNextCourse(){
		int sat_d;
		Course *crs = NULL;
		while(!pq.empty()){
			tie(sat_d, crs) = pq.top();
			pq.pop();
			if(crs -> time_slot == 0 && 
					crs -> neig_color.size() == sat_d)break;
			
			crs = NULL;
		}
		return crs;
		
	}
	
	void assignSlot(Course* crs, int slot){
		crs -> time_slot = slot;
		for(Course* neig : crs -> adj){
			int added = neig -> neig_color.insert(slot).second;
			if(neig -> time_slot == 0 && added)
				pq.emplace(neig -> neig_color.size(), neig);
		}
	}
};

///////////////////////////////////////////////////////////////////////

struct LargestEnrollment : Constructive_heuristic{
	vector<Course*> orderedCourses;
	
	LargestEnrollment(AllCourse* _allcourse) : Constructive_heuristic(_allcourse){
		orderedCourses = allcourse -> all;
		
		//sorting
		sort(orderedCourses.begin(), orderedCourses.end(), [&](const Course* a, const Course* b){
			if(a -> student == b -> student)
				return a -> degree < b -> degree;
			return a -> student < b -> student;
		});
		
	}
	
	Course* getNextCourse(){
		if(orderedCourses.empty())return NULL;
		Course* ret = orderedCourses.back();
		orderedCourses.pop_back();
		return ret;
	}
	
	void assignSlot(Course* crs, int slot){
		crs -> time_slot = slot;
	}
};

///////////////////////////////////////////////////////////////////////

struct RandomOrdering : Constructive_heuristic{
	vector<Course*> orderedCourses;
	
	RandomOrdering(AllCourse* _allcourse) : Constructive_heuristic(_allcourse){
		orderedCourses = allcourse -> all;
		
		random_shuffle(orderedCourses.begin(), orderedCourses.end());
	}
	
	Course* getNextCourse(){
		if(orderedCourses.empty())return NULL;
		Course* ret = orderedCourses.back();
		orderedCourses.pop_back();
		return ret;
	}
	
	void assignSlot(Course* crs, int slot){
		crs -> time_slot = slot;
	}
};

///////////////////////////////////////////////////////////////////////

