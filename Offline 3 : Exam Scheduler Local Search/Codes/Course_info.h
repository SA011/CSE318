struct Course{
	int id;													//id of the course
	int student;										//number of students
	//int sat_degree;								//saturation degree
	set<int> neig_color;						//unique neighbour colors	
	int degree;											//number of adjacent courses
	int time_slot;									//time_slot assigned to it
	unordered_set<Course*> temp;		//courses that conflict with this course (temporary set to avoid duplication)
	vector<Course*> adj;						//courses that conflict with this course
	vector<int> enrolled_students;	//id of enrolled student
	//constructor
	Course(int _id, int _stu) : id(_id), student(_stu),
		degree(0), time_slot(0){
	}
	
	//adds a conflction
	bool add_edge(Course *crs){
		int added = temp.insert(crs).second;
		degree += added;
		return added;
	}
	
	void enroll(int stu){
		enrolled_students.push_back(stu);
	}
	//returns minimum possible time slot starting from 1
	int getMex(){
		vector<int> all(degree);
		for(Course* crs : adj){
			if(crs -> time_slot > 0 && crs -> time_slot <= degree)
				all[crs -> time_slot - 1] = 1;
		}
		int ans = 0;
		while(ans < degree && all[ans] == 1)
			ans++;
		return ans + 1;
	}
	
};


struct AllCourse{
	int num;												//number of course
	int mx_crs_id;									//mx id of course
	int mx_stu_id;									//mx id of student
	vector<Course*> all;						//array of courses
	
	//constructor
	AllCourse() : num(0){
	}
	
	void add_course(Course *crs){
		num++;
		mx_crs_id = max(mx_crs_id, crs -> id);
		all.push_back(crs);
	}
	
	void build(){
		for(Course *c : all){
			for(int stu : c -> enrolled_students)mx_stu_id = max(mx_stu_id, stu);
			for(Course *d : c -> temp)c -> adj.push_back(d);
			c -> temp.clear();
		}
	}
};
