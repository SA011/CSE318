int linearEvaluation(int n){
	//if(n <= 0){
		//cout << n << endl;
	//}
	//assert(n > 0);
	if(n > 5)return 0;
	return 2 * (5 - n);
}

int exponentialEvaluation(int n){
	//if(n <= 0){
		//cout << n << endl;
	//}
	assert(n > 0);
	if(n > 5)return 0;
	return 1 << (5 - n);
}

int getPenalty(AllCourse *crs, function<int(int)> &eval){
		vector<Course*> temp = crs -> all;
		sort(temp.begin(), temp.end(), [&](const Course* A, const Course* B){
			return A -> time_slot < B -> time_slot;
		});
		
		vector<deque<int>> last_slot(crs -> mx_stu_id + 1);
		int ans = 0;
		for(Course *now : temp){
			for(int student : now -> enrolled_students){
				while(!last_slot[student].empty() && now -> time_slot - last_slot[student].front() > 5)
					last_slot[student].pop_front();
				for(int x : last_slot[student])ans += eval(now -> time_slot - x);
				
				last_slot[student].push_back(now -> time_slot);
			}
		}
		//cout << ans << endl;
		return ans;
}
//int getPenalty(AllCourse *crs, function<int(int)> &eval){
		//vector<Course*> temp = crs -> all;
		//sort(temp.begin(), temp.end(), [&](const Course* A, const Course* B){
			//return A -> time_slot < B -> time_slot;
		//});
		
		//vector<int> last_slot(crs -> mx_stu_id + 1, -5);
		//int ans = 0;
		//for(Course *now : temp){
			//for(int student : now -> enrolled_students){
				//ans += eval(now -> time_slot - last_slot[student]);
				//last_slot[student] = now -> time_slot;
			//}
		//}
		
		//return ans;
//}

int getMaxTimeSlot(AllCourse *courses){
	int ans = 0;
	for(Course *c : courses -> all)ans = max(ans, c -> time_slot);
	return ans;
}
