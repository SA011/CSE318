/*
Author: sa01
Compilation Time: Sun 22 Jan 2023 09:26:53 PM
*/

//#pragma GCC optimize("O3,unroll-loops")
//#pragma GCC target("avx2,bmi,bmi2,lzcnt")
#include <bits/stdc++.h>
using namespace std;

mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());

int getRandom(int l, int r){
	return rng() % (r - l + 1) + l;
}


#include "Course_info.h"
#include "Constructive_heuristics.h"
#include "Penalty.h"
#include "Perturbative_heuristics.h"


AllCourse* loadData(ifstream &crs, ifstream &stu){
	if(!crs.is_open() || !stu.is_open())
		return NULL;
	
	AllCourse *courses = new AllCourse;
	int id, s;
	map<int, Course*> mp;
	while(crs >> id >> s){
		Course *nw = new Course(id, s);
		if(mp.count(id))return NULL;
		mp[id] = nw;
		courses -> add_course(nw);
	}
	
	string line;
	int i = 1;
	while(getline(stu, line)){
		stringstream tok(line);
		vector<Course*> last;
		while(tok >> id){
			for(Course *l : last){
				l -> add_edge(mp[id]);
				mp[id] -> add_edge(l);
			}
			last.push_back(mp[id]);
			mp[id] -> enroll(i);
		}
		i++;
	}
	
	courses -> build();
	
	return courses;
	
}

void showError(){
	cout << "Invalid Command Line Arguments\n";
	cout << "Usage: ./1805106 [cons_heur] [penalty] [filenumber]\n";

	exit(1);
}

Constructive_heuristic* constructives[4];
Perturbative_heuristic* perturbatives[2];
int N = 1e3 + 10;

function<int(int)> penalties[] = {
	linearEvaluation,
	exponentialEvaluation
};

void clearAll(AllCourse *courses){
	for(Course* crs : courses -> all){
		delete crs;
	}
	delete (LargestDegree*)constructives[0];
	delete (SaturationDegree*)constructives[1];
	delete (LargestEnrollment*)constructives[2];
	delete (RandomOrdering*)constructives[3];
	delete (KempeChain_Interchange*)perturbatives[0];
	delete (PairSwap*)perturbatives[1];
	delete courses;
}


void printCourses(AllCourse *courses){
	for(Course* crs : courses -> all){
		cout << setw(4) << setfill('0') << crs -> id << ' ' << crs -> time_slot << "\n";
	}
	
}

void scheduleCourses(AllCourse *courses, Constructive_heuristic* ch, function<int(int)> &penalty){
	Course *cur = NULL;
	
	while((cur = ch -> getNextCourse()) != NULL){
		ch -> assignSlot(cur, cur -> getMex());
	}
	
	int current_penalty = getPenalty(courses, penalty);
	cout << "Time Slots: " << getMaxTimeSlot(courses) << endl;
	cout << "Penalty(After Constructive Heruristic): " << fixed << setprecision(5) << (double)current_penalty / (courses -> mx_stu_id) << endl;
	
	for(int j = 0; j < N; j++)perturbatives[0] -> performSwap(penalty, current_penalty);
	
	cout << "Penalty(After Kempechain Interchange): " << fixed << setprecision(5) << (double)current_penalty / (courses -> mx_stu_id) << endl;
	
	for(int j = 0; j < N; j++)perturbatives[1] -> performSwap(penalty, current_penalty);
	
	cout << "Penalty(After PairSwap): " << fixed << setprecision(5) << (double)current_penalty / (courses -> mx_stu_id) << endl;
	
}

string fileNames[] = {
	"car-f-92",
	"car-s-91",
	"kfu-s-93",
	"tre-s-92",
	"yor-f-83"
};

string HeuristicNames[] = {
	"Largest Degree",
	"Saturation Degree",
	"Largest Enrollment",
	"Random Ordering"
};


int main(int argc, char* argv[]){
	int chosen_constructive = 1, chosen_penalty = 1, L = 0, R = 5;
	
	if(argc > 1){
		chosen_constructive = atoi(argv[1]);
	}
	
	if(argc > 2){
		chosen_penalty = atoi(argv[2]);
	}
	
	if(argc > 3){
		L = atoi(argv[3]);
		R = L + 1;
	}
	
	if(argc > 4){
		N = atoi(argv[4]);
	}
	
	cout << "\n#####\n" << HeuristicNames[chosen_constructive] << "\n#####\n"; 
	for(int i = L; i < R; i++){
		cout << "\n=============================================\n";
		cout << "Scheduling " << fileNames[i] << "\n\n";
		ifstream crs, stu;

		crs.open(fileNames[i] + ".crs");
		stu.open(fileNames[i] + ".stu");

		AllCourse *courses = loadData(crs, stu);

		if(courses == NULL)showError();

		crs.close();
		stu.close();


		//for(auto x : courses -> all[2] -> enrolled_students){
			//cout << x << endl;
		//}

		constructives[0] = new LargestDegree(courses);
		constructives[1] = new SaturationDegree(courses);
		constructives[2] = new LargestEnrollment(courses);
		constructives[3] = new RandomOrdering(courses);
		perturbatives[0] = new KempeChain_Interchange(courses);
		perturbatives[1] = new PairSwap(courses);

		scheduleCourses(courses, constructives[chosen_constructive], penalties[chosen_penalty]);
		//printCourses(courses);

		clearAll(courses);
	}
	return 0;
}
