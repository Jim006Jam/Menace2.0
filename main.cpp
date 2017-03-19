#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <math.h>
#include <sstream>

using namespace std;
int maxstatesize = 9;
vector <string> fstates;
vector <string> sstates;
vector <vector<int>> fpebs;
vector <vector<int>> spebs;

//To do list
//perf_move()


//Basic Functions
string rotate(string s) {
	string a;
	for(int i = 0; i < sqrt(maxstatesize); i ++) {
		int j = maxstatesize-sqrt(maxstatesize)+i;
		for(j; j > -1; j = j - sqrt(maxstatesize)) {
			a += s.substr(j,1);
		}
		j++;
	}
	return a;
}

string reflectx(string s) {
	string a;
	for(int i = 0; i < sqrt(maxstatesize); i ++) {
		int j = maxstatesize-(sqrt(maxstatesize)*(i+1));
		for(j; j < maxstatesize-(sqrt(maxstatesize)*i);j++) {
			a += s.substr(j,1);
		}
	}
	return a;
}

string reflecty(string s) {
	string a;
	for(int i = 0; i < sqrt(maxstatesize); i ++) {
		int j = (sqrt(maxstatesize)*(i+1))-1;
		for(j; j > (maxstatesize-(sqrt(maxstatesize)*(sqrt(maxstatesize)-i))-1);j--) {
			a += s.substr(j,1);
		}
	}
	return a;
}

vector <string> transform(string s) {
	vector <string> transforms;
	string a = s;
	for(int i = 0; i < 3; i ++) {
		string b = rotate(a);
		transforms.push_back(b);
		a = b;
	}
	transforms.push_back(reflecty(s));
	transforms.push_back(reflectx(s));
	a = reflectx(s);
	transforms.push_back(rotate(a));
	for(int i = 0; i < 3; i ++) {
		string b = rotate(a);
		a = b;
	}
	transforms.push_back(a);
	return transforms;
}

int check_movenum(string s) {
	int temp = 0;
	for(int i = 0; i < maxstatesize; i ++) {
		if(s[i] == '-') {
			temp++;
		}
	}
	return temp;
}


//Loading, Saving and Printing
void load_files() {
	string::size_type sz;
	//Reading in first player states and probs
	string line;
	ostringstream oss;
	oss << "first_player_menace" << sqrt(maxstatesize) << ".txt";
	ifstream firstfile (oss.str());
	oss.str(string());
	oss << "second_player_menace" << sqrt(maxstatesize) << ".txt";
	ifstream secondfile (oss.str());
	if (firstfile.is_open()) {
		while(firstfile.eof() == 0) {
			getline(firstfile,line,' ');
			string s = line;
			fstates.push_back(s);
			int i = 0;
			vector <int> vec;
			for(int i = 0; i < maxstatesize; i++) {
				int a;
				getline(firstfile,line,'[');
				getline(firstfile,line,']');
				try {
					a = stoi(line,&sz);
				} catch (invalid_argument&) {
					//Do Nothing just catch error
				}
				vec.push_back(a);
			}
			fpebs.push_back(vec);
			getline(firstfile,line,'\n');
		}
		if(fstates.size() == 1) {
			fstates.erase(fstates.begin());
			fpebs.erase(fpebs.begin());
		}
	} else {
		cout << "Could not open file" << endl;
	}
	//Reading in second player states and probs
	if (secondfile.is_open()) {
		while(secondfile.eof() == 0) {
			getline(secondfile,line,' ');
			string s = line;
			sstates.push_back(s);
			int i = 0;
			vector <int> vec;
			for(int i = 0; i < maxstatesize; i++) {
				int a;
				getline(secondfile,line,'[');
				getline(secondfile,line,']');
				try {
					a = stoi(line,&sz);
				} catch (invalid_argument&) {
					//Do Nothing just catch error
				}
				vec.push_back(a);
			}
			spebs.push_back(vec);
			getline(secondfile,line,'\n');
		}
		if(sstates.size() == 1) {
			sstates.erase(sstates.begin());
			spebs.erase(spebs.begin());
		}
	} else {
		cout << "Could not open file" << endl;
	}
	secondfile.close();
}

void save_files() {
	ofstream myfile;
	ostringstream oss;
	oss << "first_player_menace" << sqrt(maxstatesize) << ".txt";
	myfile.open (oss.str(),std::ofstream::out | std::ofstream::trunc);
	for(int i = 0; i < fstates.size(); i++) {
		myfile << fstates[i] << " ";
		for(int j = 0; j < maxstatesize; j++) {
			myfile << "[" << fpebs[i][j] << "]" << " ";
		} if(i != fstates.size()-1) {
			myfile << endl;
		}
	}
	myfile.close();
	oss.str(string());
	oss << "second_player_menace" << sqrt(maxstatesize) << ".txt";
	myfile.open (oss.str(),std::ofstream::out | std::ofstream::trunc);
	for(int i = 0; i < sstates.size(); i++) {
		myfile << sstates[i] << " ";
		for(int j = 0; j < maxstatesize; j++) {
			myfile << "[" << spebs[i][j] << "]" << " ";
		} if(i != sstates.size()-1) {
			myfile << endl;
		}
	}
	myfile.close();
}

void print_menu(int menu) {
	if (menu == 0) {
		cout << "Which Version Would You Like To Play?" << endl;
		cout << "1 - 3x3" << endl;
		cout << "2 - 4x4" << endl;
		cout << "3 - Exit" << endl;
	} else if (menu == 1) {
		cout << "Main Menu" << endl;
		cout << "1 - Train Menace" << endl;
		cout << "2 - Play Menace" << endl;
		cout << "3 - Test The AI's" << endl;
		cout << "4 - Exit" << endl;
	} else if (menu == 2) {
		cout << "Choose an opponent" << endl;
		cout << "1 - \"Perfect Player\" - Not Implemented Yet" << endl;
		cout << "2 - \"Logical Player\"" << endl;
		cout << "3 - Randomly Placing Player" << endl;
		cout << "4 - Itself (MENACE VS MENACE)" << endl;
		cout << "5 - Back" << endl;
	}
}

void print_grid(string s){
	int cell_num = 0;
	for (int i = 0; i < sqrt(maxstatesize); i++) {
		for (int j = 0; j < sqrt(maxstatesize); j++) {
			cout << "|";
			if (s[(i*sqrt(maxstatesize))+j] == 'x') {
				cout << "X";
			}
			else if (s[i*sqrt(maxstatesize)+j] == 'o') {
				cout << "O";
			} else {
				cout << " ";
			}
			cout << "|";
		}
		cout << endl;
	}
	cout << endl;
}

void print_states() {
	cout << endl << "-----THESE ARE THE FIRST PLAYER STATES-----" << " " << fstates.size() << endl;
	if(fstates.size() == 0) {
		cout << "There are none yet" << endl;
	} else {
		for (int i = 0; i < fstates.size(); i ++) {
			cout << fstates[i] << " |";
			for (int j = 0; j < maxstatesize; j ++) {
				cout << fpebs[i][j] << "|";
			}
			cout << endl;
		}
	}
	
	cout << endl << "-----THESE ARE THE SECOND PLAYER STATES-----" << " " << sstates.size() << endl;
	if(sstates.size() == 0) {
		cout << "There are none yet" << endl;
	} else {
		for (int i = 0; i < sstates.size(); i ++) {
			cout << sstates[i] << " |";
			for (int j = 0; j < maxstatesize; j ++) {
				cout << spebs[i][j] << "|";
			}
			cout << endl;
		}
	}
}

void remove_minus() {
	for (int i = 0; i < fstates.size(); i ++) {
		for(int j = 0; j < maxstatesize; j ++) {
			if(fpebs[i][j] < 0) {
				fpebs[i][j] = 0;
			}
		}
	}
	for (int i = 0; i < sstates.size(); i ++) {
		for(int j = 0; j < maxstatesize; j ++) {
			if(spebs[i][j] < 0) {
				spebs[i][j] = 0;
			}
		}
	}
}


//Checking and Adjusting values
int check_index(string s,char x) {
	int index = -1;
	vector <string> transforms;
	vector <int> total;
	transforms = transform(s);
	transforms.push_back(s);
	if(x == 'x') {
		for (int i = 0; i < transforms.size(); i ++) {
			for(int j = 0; j < fstates.size(); j ++) {
				if(transforms[i] == fstates[j]) {
					index = j;
				}
			}
		}
	} else if (x == 'o'){
		for (int i = 0; i < transforms.size(); i ++) {
			for(int j = 0; j < sstates.size();j++) {
				if(transforms[i] == sstates[j]) {
					index = j;
				}
			}
		}
	}
	return index;
}

int check_game(string s) {
	//Horizontal Wins
	for(int i = 0; i < sqrt(maxstatesize);i++) {
		vector<int> pos;
		int j = i*(sqrt(maxstatesize));
		for(j;j < (sqrt(maxstatesize))*(i+1);j++){
			pos.push_back(j);
		}
		if(maxstatesize == 9) {
			if ((s[pos[0]] == 'x') && (s[pos[1]] == 'x') && (s[pos[2]] == 'x')) {
				return -1;
			}
			else if((s[pos[0]] == 'o') && (s[pos[1]] == 'o') && (s[pos[2]] == 'o')) {
				return -2;
			}
		} else if(maxstatesize == 16) {
			if ((s[pos[0]] == 'x') && (s[pos[1]] == 'x') && (s[pos[2]] == 'x') && (s[pos[3]] == 'x')) {
				return -1;
			}
			else if((s[pos[0]] == 'o') && (s[pos[1]] == 'o') && (s[pos[2]] == 'o') && (s[pos[3]] == 'o')) {
				return -2;
			}
		}
	}
	
	//Vertical Wins
	for(int i = 0; i < sqrt(maxstatesize);i++) {
		vector<int> pos;
		for(int j = i;j < maxstatesize;j=j+sqrt(maxstatesize)){
			pos.push_back(j);
		}
		if(maxstatesize == 9) {
			if ((s[pos[0]] == 'x') && (s[pos[1]] == 'x') && (s[pos[2]] == 'x')) {
				return -1;
			}
			else if((s[pos[0]] == 'o') && (s[pos[1]] == 'o') && (s[pos[2]] == 'o')) {
				return -2;
			}
		} else if(maxstatesize == 16) {
			if ((s[pos[0]] == 'x') && (s[pos[1]] == 'x') && (s[pos[2]] == 'x') && (s[pos[3]] == 'x')) {
				return -1;
			}
			else if((s[pos[0]] == 'o') && (s[pos[1]] == 'o') && (s[pos[2]] == 'o') && (s[pos[3]] == 'o')) {
				return -2;
			}
		}
	}
	
	//Diagonal Wins
	vector<int> pos;
	for(int j = 0;j < ((sqrt(maxstatesize)-1)*(sqrt(maxstatesize)+1)+1);j=j+(sqrt(maxstatesize)+1)){
		pos.push_back(j);
	}
	if(maxstatesize == 9) {
			if ((s[pos[0]] == 'x') && (s[pos[1]] == 'x') && (s[pos[2]] == 'x')) {
				return -1;
			}
			else if((s[pos[0]] == 'o') && (s[pos[1]] == 'o') && (s[pos[2]] == 'o')) {
				return -2;
			}
		} else if(maxstatesize == 16) {
			if ((s[pos[0]] == 'x') && (s[pos[1]] == 'x') && (s[pos[2]] == 'x') && (s[pos[3]] == 'x')) {
				return -1;
			}
			else if((s[pos[0]] == 'o') && (s[pos[1]] == 'o') && (s[pos[2]] == 'o') && (s[pos[3]] == 'o')) {
				return -2;
			}
		}
	pos.clear();
	for(int j = sqrt(maxstatesize)-1;j < ((sqrt(maxstatesize)-1)*(sqrt(maxstatesize))+1);j=j+(sqrt(maxstatesize)-1)){
		pos.push_back(j);
	}
	if(maxstatesize == 9) {
		if ((s[pos[0]] == 'x') && (s[pos[1]] == 'x') && (s[pos[2]] == 'x')) {
			return -1;
		}
		else if((s[pos[0]] == 'o') && (s[pos[1]] == 'o') && (s[pos[2]] == 'o')) {
			return -2;
		}
	} else if(maxstatesize == 16) {
		if ((s[pos[0]] == 'x') && (s[pos[1]] == 'x') && (s[pos[2]] == 'x') && (s[pos[3]] == 'x')) {
			return -1;
		}
		else if((s[pos[0]] == 'o') && (s[pos[1]] == 'o') && (s[pos[2]] == 'o') && (s[pos[3]] == 'o')) {
			return -2;
		}
	}
	if(check_movenum(s) == 0) {
		return -3;
	}
}

int get_winmove(string s,vector<int> pos,char x) {
	if(x == 'x') {
		if(maxstatesize == 9) {
			if ((s[pos[1]] == 'x') && (s[pos[2]] == 'x')) {
				return pos[0];
			} else if ((s[pos[0]] == 'x') && (s[pos[2]] == 'x')) {
				return pos[1];
			} else if ((s[pos[0]] == 'x') && (s[pos[1]] == 'x')) {
				return pos[2];
			}
		} else if(maxstatesize == 16) {
			if ((s[pos[1]] == 'x') && (s[pos[2]] == 'x') && (s[pos[3]] == 'x')) {
				return pos[0];
			} else if ((s[pos[0]] == 'x') && (s[pos[2]] == 'x') && (s[pos[3]] == 'x')) {
				return pos[1];
			} else if ((s[pos[0]] == 'x') && (s[pos[1]] == 'x') && (s[pos[3]] == 'x')) {
				return pos[2];
			} else if ((s[pos[0]] == 'x') && (s[pos[1]] == 'x') && (s[pos[2]] == 'x')) {
				return pos[3];
			}
		}
		return -1;
	} else if (x == 'o') {
		if(maxstatesize == 9) {
			if ((s[pos[1]] == 'o') && (s[pos[2]] == 'o')) {
				return pos[0];
			} else if ((s[pos[0]] == 'o') && (s[pos[2]] == 'o')) {
				return pos[1];
			} else if ((s[pos[0]] == 'o') && (s[pos[1]] == 'o')) {
				return pos[2];
			}
		} else if(maxstatesize == 16) {
			if ((s[pos[1]] == 'o') && (s[pos[2]] == 'o') && (s[pos[3]] == 'o')) {
				return pos[0];
			} else if ((s[pos[0]] == 'o') && (s[pos[2]] == 'o') && (s[pos[3]] == 'o')) {
				return pos[1];
			} else if ((s[pos[0]] == 'o') && (s[pos[1]] == 'o') && (s[pos[3]] == 'o')) {
				return pos[2];
			} else if ((s[pos[0]] == 'o') && (s[pos[1]] == 'o') && (s[pos[2]] == 'o')) {
				return pos[3];
			}
		}
		return -1;
	}

}

int detect_win(string s, char x) {
	int winmove = 0;
	for(int i = 0; i < sqrt(maxstatesize);i++) {
		vector<int> pos;
		int j = i*(sqrt(maxstatesize));
		for(j;j < (sqrt(maxstatesize))*(i+1);j++){
			pos.push_back(j);
		}
		if(x == 'x') {
			winmove = get_winmove(s,pos,'x');
		} else if(x == 'o') {
			winmove = get_winmove(s,pos,'o');
		}
		if(winmove > -1) {
			return winmove;
		}
	}
	for(int i = 0; i < sqrt(maxstatesize);i++) {
		vector<int> pos;
		for(int j = i;j < maxstatesize;j=j+sqrt(maxstatesize)){
			pos.push_back(j);
		}
		if(x == 'x') {
			winmove = get_winmove(s,pos,'x');
		} else if(x == 'o') {
			winmove = get_winmove(s,pos,'o');
		}
		if(winmove > -1) {
			return winmove;
		}
	}
	vector<int> pos;
	for(int j = 0;j < ((sqrt(maxstatesize)-1)*(sqrt(maxstatesize)+1)+1);j=j+(sqrt(maxstatesize)+1)){
		pos.push_back(j);
	}
	if(x == 'x') {
			winmove = get_winmove(s,pos,'x');
		} else if(x == 'o') {
			winmove = get_winmove(s,pos,'o');
		}
	if(winmove > -1) {
		return winmove;
	}
	pos.clear();
	for(int j = sqrt(maxstatesize)-1;j < ((sqrt(maxstatesize)-1)*(sqrt(maxstatesize))+1);j=j+(sqrt(maxstatesize)-1)){
		pos.push_back(j);
	}
	if(x == 'x') {
			winmove = get_winmove(s,pos,'x');
		} else if(x == 'o') {
			winmove = get_winmove(s,pos,'o');
		}
	if(winmove > -1) {
		return winmove;
	}
	return -1;
}

void generate_pebs(string s) {
	vector <int> vec;
	vector <string> transforms;
	transforms.clear();
	vec.assign(maxstatesize,0);
	int x = 0;
	int o = 0;
	for(int i = 0; i < maxstatesize; i ++) {
		if(s[i] == 'x') {
			x++;
		} else if(s[i] == 'o') {
			o++;
		}
	}
	int movenum = check_movenum(s);
	for(int i = 0; i < maxstatesize; i ++) {
		string temp = s;
		if(s[i] == '-') {
			if(maxstatesize % 2 == 0) {
				if(movenum % 2 == 0) {
					vec[i] = ((movenum-2)/2);
				} else {
					vec[i] = ((movenum-1)/2);
				}
			} else {
				if(movenum % 2 == 0) {
					vec[i] = (movenum/2);
				} else {
					vec[i] = ((movenum+1)/2);
				}
			}
			if(x == o) {
				temp[i] = 'x';
			} else {
				temp[i] = 'o';
			}
		}
		int t = 0;
		for(int j = 0; j < transforms.size(); j ++) {
			if(temp == transforms[j]) {
				t++;
			}
		}
		if(t == 0) {
			vector <string> comps = transform(temp);
			for (int j = 0; j < 7; j++) {
				transforms.push_back(comps[j]);
				transforms.push_back(temp);
			}
		} else {
			vec[i] = 0;
		}
	}
	if(x == o) {
		fpebs.push_back(vec);
	} else {
		spebs.push_back(vec);
	}
}

void sort_lists() {
	int t = 0;
	do {
		int t = 0;
		for(int i = 0; i < fstates.size(); i ++ ) {
			for(int j = i; j < fstates.size(); j ++) {
				if((check_movenum(fstates[i]) < check_movenum(fstates[j])) && (i != j)) {
					string temp = fstates[i];
					vector <int> pebs;
					pebs.assign(maxstatesize,0);
					for(int k = 0; k < maxstatesize; k ++) {
						pebs[k] = fpebs[i][k];
						fpebs[i][k] = fpebs[j][k];
						fpebs[j][k] = pebs[k];
					}
					fstates[i] = fstates[j];
					fstates[j] = temp;
					t++;
				}
			}
		}
	} while(t != 0);
	do {
		t = 0;
		for(int i = 0; i < sstates.size(); i ++ ) {
			for(int j = i; j < sstates.size(); j ++) {
				if((check_movenum(sstates[i]) < check_movenum(sstates[j])) && (i != j)) {
					string temp = sstates[i];
					vector <int> pebs;
					pebs.assign(maxstatesize,0);
					for(int k = 0; k < maxstatesize; k ++) {
						pebs[k] = spebs[i][k];
						spebs[i][k] = spebs[j][k];
						spebs[j][k] = pebs[k];
					}
					sstates[i] = sstates[j];
					sstates[j] = temp;
					t++;
				}
			}
		}
	} while(t != 0);
}

void adjust_pebs(vector <int> moves, vector <string> states,int result,int men) {
	int i;
	int ii;
	int increment;
	if(check_movenum(states[0]) != check_movenum(states[1])+1) {
		i = 0;
		ii = 0;
		increment = 1;
	} else {
		i = 1;
		ii = 0;
		increment = 2;
	}
	if(men == 2) {
		for(i; i < states.size(); i = i + increment) {
			int in;
			int newmove;
			string state;
			int index = check_index(states[i],'o');
			vector <string> transforms = transform(states[i]);
			transforms.push_back(states[i]);
			for(int j = 0; j < transforms.size(); j ++) {
				if(transforms[j] == sstates[index]) {
					in = j;
				}
			}
			for(int j = 0; j < maxstatesize; j ++) {
				if(j == moves[i]) {
					state+= 'o';
				} else {
					state+= '-';
				}
			}
			transforms = transform(state);
			transforms.push_back(state);
			for(int j = 0; j < maxstatesize; j ++) {
				if(transforms[in][j] == 'o') {
					newmove = j;
				}	
			}
			int movenum = check_movenum(states[i]);
			if(result == 0) {
				if(maxstatesize % 2 == 0) {
					if((movenum == maxstatesize) || (movenum == maxstatesize - 2)) {
						spebs[index][newmove] -= 1;
					} else if((movenum == maxstatesize -4) || (movenum == maxstatesize - 6)) {
						spebs[index][newmove] -= 2;
					} else {
						spebs[index][newmove] -= 3;
					}
				} else {
					if((movenum == maxstatesize) || (movenum == maxstatesize - 2)) {
						spebs[index][newmove] -= 1;
					} else if(movenum == maxstatesize -4) {
						spebs[index][newmove] -= 2;
					} else {
						spebs[index][newmove] -= 3;
					}
				}
			} else if(result == 1) {
				spebs[index][newmove] += 3;
			} else if(result == 2) {
				spebs[index][newmove] += 1;
			}
		}
	} else if (men == 1) {
		for(ii; ii < states.size(); ii = ii + increment) {
			int in;
			int newmove;
			string state;
			int index = check_index(states[ii],'x');
			vector <string> transforms = transform(states[ii]);
			transforms.push_back(states[ii]);
			for(int j = 0; j < transforms.size(); j ++) {
				if(transforms[j] == fstates[index]) {
					in = j;
				}
			}
			for(int j = 0; j < maxstatesize; j ++) {
				if(j == moves[ii]) {
					state+= 'x';
				} else {
					state+= '-';
				}
			}
			transforms = transform(state);
			transforms.push_back(state);
			for(int j = 0; j < maxstatesize; j ++) {
				if(transforms[in][j] == 'x') {
					newmove = j;
				}	
			}
			int movenum = check_movenum(states[ii]);
			if(result == 0) {
				if(maxstatesize % 2 == 0) {
					if((movenum == maxstatesize) || (movenum == maxstatesize - 2)) {
						fpebs[index][newmove] -= 1;
					} else if((movenum == maxstatesize -4) || (movenum == maxstatesize - 6)) {
						fpebs[index][newmove] -= 2;
					} else {
						fpebs[index][newmove] -= 3;
					}
				} else {
					if((movenum == maxstatesize) || (movenum == maxstatesize - 2)) {
						fpebs[index][newmove] -= 1;
					} else if(movenum == maxstatesize -4) {
						fpebs[index][newmove] -= 2;
					} else {
						fpebs[index][newmove] -= 3;
					}
				}
			} else if(result == 1) {
				fpebs[index][newmove] += 3;
			} else if(result == 2) {
				fpebs[index][newmove] += 1;
			}
		}
	}
}



//Player and AI Moves
int user_move(string s,char x) {
	string decision;
	string::size_type sz;
	int decisionint;
	int size = check_movenum(s);
	bool movemade = false;
	while (movemade == false) {
		cout << "1-" << maxstatesize <<" :";
		getline(cin, decision);
		try {
			decisionint = stoi(decision,&sz);
		} catch (invalid_argument&) {
			//Do nothing, just catch teh error.
		}
		if (s[decisionint-1] == '-') {
			if (x == 'x') {
				return decisionint-1;
			} else {
				return decisionint-1;
			}
		} else {
			cout << "Please Choose A Valid Move" << endl;
		}
	}
};

int men_move(string s,char x) {
	int index;
	vector <int> total;
	vector <int> movelist;
	vector <string> transforms;
	int size = check_movenum(s);
	if(size == 1) {
		for(int i = 0; i < maxstatesize; i ++) {
			if (s[i] == '-') {
				return i;
			}
		}
	} else {
		if(x == 'x') {
			index = check_index(s,x);
			if((index == -1) && (check_movenum(s) > 1)) {
				fstates.push_back(s);
				generate_pebs(s);
				index = fstates.size()-1;
			}
			int t = 0;
			for (int i = 0; i < maxstatesize; i++) {
				if(fpebs[index][i] > 0) {
					t = t + fpebs[index][i];
					total.push_back(t);
					movelist.push_back(i);
				}
			}
			if(t == 0) {
				for(int i = 0; i < maxstatesize; i ++) {
					if (s[i] == '-') {
						return i;
					}
				}
			}
			transforms = transform(fstates[index]);
			transforms.push_back(fstates[index]);
			int in = 0;
			for(int i = 0; i < transforms.size(); i ++) {
				if(s == transforms[i]) {
					in = i;
				}
			}
			int chance = rand() %t;
			string temp = "";
			for(int i = 0; i < maxstatesize; i++) {
				temp += '-';
			}
			for(int i = 0; i < total.size(); i ++) {
				if(i == 0) {
					if((chance >= 0) && (chance < total[i])) {
						if(in == 7) {
							return movelist[i];
						} else {
							temp[movelist[i]] = 'x';
							transforms = transform(temp);
							for(int j = 0; j < maxstatesize; j++) {
								if(transforms[in][j] == 'x') {
									return j;
								}
							}
						}
					}
				} else {
					if ((chance >= total[i-1]) && (chance < total[i])) {
						if(in == 7) {
							return movelist[i];
						} else {
							temp[movelist[i]] = 'x';
							transforms = transform(temp);
							for(int j = 0; j < maxstatesize; j++) {
								if(transforms[in][j] == 'x') {
									return j;
								}
							}
						}
					}
				}
			}
		} else if (x == 'o'){
			index = check_index(s,x);
			if(index == -1) {
				sstates.push_back(s);
				generate_pebs(s);
				index = sstates.size()-1;
			}
			int t = 0;
			for (int i = 0; i < maxstatesize; i++) {
				if(spebs[index][i] > 0) {
					t = t + spebs[index][i];
					total.push_back(t);
					movelist.push_back(i);
				}
			}
			if(t == 0) {
				for(int i = 0; i < maxstatesize; i ++) {
					if (s[i] == '-') {
						return i;
					}
				}
			}
			transforms = transform(sstates[index]);
			transforms.push_back(sstates[index]);
			int in = 0;
			for(int i = 0; i < transforms.size(); i ++) {
				if(s == transforms[i]) {
					in = i;
				}
			}
			int chance = rand() %t;
			string temp = "";
			for(int i = 0; i < maxstatesize; i++) {
				temp += '-';
			}
			for(int i = 0; i < total.size(); i ++) {
				if(i == 0) {
					if((chance >= 0) && (chance < total[i])) {
						if(in == 7) {
							return movelist[i];
						} else {
							temp[movelist[i]] = 'o';
							transforms = transform(temp);
							for(int j = 0; j < maxstatesize; j++) {
								if(transforms[in][j] == 'o') {
									return j;
								}
							}
						}
					}
				} else {
					if ((chance >= total[i-1]) && (chance < total[i])) {
						if(in == 7) {
							return movelist[i];
						} else {
							temp[movelist[i]] = 'o';
							transforms = transform(temp);
							for(int j = 0; j < maxstatesize; j++) {
								if(transforms[in][j] == 'o') {
									return j;
								}
							}
						}
					}
				}
			}
		}
	}
} 

//This is Gonna be the hardest part
/*int perf_move(string s,char x) {
	bool movemade = false;
	while(movemade == false) {
		if(x == 'x') {
			// Go for the win
			int move = detect_win(cells,'x');
			if((move > -1) && (move < 9)) {
				return move;
			}
			// Block
			move = detect_win(cells,'o');
			if((move > -1) && (move < 9)) {
				return move;
			}
			while(movemade == false) {
				int tactic = rand() %1;
				if(tactic == 0) {
					if(check_movenum(cells) == 9) {
						return 4;
					}
					if(check_movenum(cells) == 7) {
						if(cells[1].get_naught() == 1) {
							return 6;
						} else if(cells[3].get_naught() == 1) {
							return 8;
						} else if(cells[5].get_naught() == 1) {
							return 0;
						} else if(cells[7].get_naught() == 1) {
							return 2;
						} else if(cells[0].get_naught() == 1) {
							return 8;
						} else if(cells[2].get_naught() == 1) {
							return 6;
						} else if(cells[6].get_naught() == 1) {
							return 2;
						} else if(cells[8].get_naught() == 1) {
							return 0;
						}
					}
					if(check_movenum(cells) == 5) {
						if(cells[1].get_naught() == 1) {
							if(cells[0].get_cross() == 1) {
								return 6;
							} else if(cells[2].get_cross() == 1) {
								return 8;
							}
						} else if(cells[3].get_naught() == 1) {
							if(cells[0].get_cross() == 1) {
								return 2;
							} else if(cells[6].get_cross() == 1) {
								return 8;
							}
						} else if(cells[5].get_naught() == 1) {
							if(cells[2].get_cross() == 1) {
								return 0;
							} else if(cells[8].get_cross() == 1) {
								return 6;
							}
						} else if(cells[7].get_naught() == 1) {
							if(cells[6].get_cross() == 1) {
								return 0;
							} else if(cells[8].get_cross() == 1) {
								return 2;
							}
						}
					}
					vector <int> moves;
					for(int i = 0; i < maxstatesize; i ++) {
						if((cells[i].get_cross() == 0) && cells[i].get_naught() == 0) {
							moves.push_back(i);
						}
					}
					move = rand() % moves.size();
					return moves[move];
				} else if(tactic == 1) {
					if(check_movenum(cells) == 9) {
						move = rand() % 4;
						if(move == 1) {
							return 0;
						} else if(move == 2) {
							return 2;
						} else if(move == 3) {
							return 6;
						} else if(move == 4) {
							return 8;
						}
					} else if(check_movenum(cells) == 7) {
						if(cells[0].get_cross() == 1) {
							if(cells[2].get_naught() == 1 || cells[1].get_naught() == 1) {
								return 6;
							} else if(cells[4].get_naught() == 1) {
								return 8;
							} else {
								return 2;
							}
						} else if(cells[2].get_cross() == 1) {
							if(cells[0].get_naught() == 1 || cells[1].get_naught() == 1) {
								return 8;
							} else if(cells[4].get_naught() == 1) {
								return 6;
							} else {
								return 0;
							}
						} else if(cells[6].get_cross() == 1) {
							if(cells[8].get_naught() == 1 || cells[7].get_naught() == 1) {
								return 0;
							} else if(cells[4].get_naught() == 1) {
								return 2;
							} else {
								return 8;
							}
						} else if(cells[8].get_cross() == 1) {
							if(cells[6].get_naught() == 1 || cells[7].get_naught() == 1) {
								return 2;
							} else if(cells[4].get_naught() == 1) {
								return 0;
							} else {
								return 6;
							}
						}
					} else if(check_movenum(cells) == 5) {
						if((cells[0].get_naught() == 1) || (cells[2].get_naught() == 1) || (cells[6].get_naught() == 1) || (cells[8].get_naught() == 1)) {
							for(int i = 0; i < maxstatesize; i = i+2) {
								if((cells[i].get_cross() == 0) && (cells[i].get_naught() == 0) && (i != 4)) {
									return i;
								}
							}
						} else if((cells[1].get_naught() == 1) && (cells[3].get_naught() == 1)) {
							return 8;
						} else if((cells[1].get_naught() == 1) && (cells[5].get_naught() == 1)) {
							return 6;
						} else if((cells[3].get_naught() == 1) && (cells[7].get_naught() == 1)) {
							return 2;
						} else if((cells[5].get_naught() == 1) && (cells[7].get_naught() == 1)) {
							return 0;
						}
					}
					vector <int> moves;
					for(int i = 0; i < maxstatesize; i ++) {
						if((cells[i].get_cross() == 0) && cells[i].get_naught() == 0) {
							moves.push_back(i);
						}
					}
					move = rand() % moves.size();
					if((cells[moves[move]].get_cross() == 0) && (cells[moves[move]].get_naught() == 0)) {
						return moves[move];
					}
				}
			}
		} else if(x == 'o') {
			// Go for the win
			int move = detect_win(cells,'o');
			if((move > -1) && (move < 9)) {
				return move;
			}
			// Block
			move = detect_win(cells,'x');
			if((move > -1) && (move < 9)) {
				return move;
			}
			while(movemade == false) {
				if(check_movenum(cells) == 8) {
					if(cells[4].get_cross() == 1) {
						move = rand() % 3;
						if(move == 0) {
							return 0;
						} else if(move == 1) {
							return 2;
						} else if(move == 2) {
							return 6;
						} else if(move == 3) {
							return 8;
						}
					} else {
						return 4;
					}
				} else if (check_movenum(cells) == 6) {
					if(cells[4].get_cross() == 1) {
						if((cells[0].get_cross() == 0) && (cells[0].get_naught() == 0)) {
							return 0;
						} else if ((cells[2].get_cross() == 0) && (cells[2].get_naught() == 0)) {
							return 2;
						} else if ((cells[6].get_cross() == 0) && (cells[6].get_naught() == 0)) {
							return 6;
						} else if ((cells[8].get_cross() == 0) && (cells[8].get_naught() == 0)) {
							return 8;
						}
					} else if(cells[4].get_naught() == 1) {
						if((cells[1].get_cross() == 0) && (cells[7].get_cross() == 0)) {
							return 1;
						} else if((cells[3].get_cross() == 0) && (cells[5].get_cross() == 0)) {
							return 3;
						} else if((cells[5].get_cross() == 0) && (cells[3].get_cross() == 0)) {
							return 5;
						} else if((cells[7].get_cross() == 0) && (cells[1].get_cross() == 0)) {
							return 7;
						} else {
							if((cells[1].get_cross() == 1) && (cells[3].get_cross() == 1)) {
								return 0;
							} else if((cells[1].get_cross() == 1) && (cells[5].get_cross() == 1)) {
								return 2;
							} else if((cells[7].get_cross() == 1) && (cells[3].get_cross() == 1)) {
								return 6;
							} else if((cells[7].get_cross() == 1) && (cells[5].get_cross() == 1)) {
								return 8;
							}
						}
					} else {
						if(cells[0].get_naught() == 1) {
							return 1;
						} else if(cells[2].get_naught() == 1) {
							return 1;
						} else if(cells[6].get_naught() == 1) {
							return 7;
						} else if(cells[8].get_naught() == 1) {
							return 7;
						}
					}
				} else if(check_movenum(cells) == 4) {
					if((cells[1].get_cross() == 1) && (cells[3].get_cross() == 1) && (cells[0].get_cross() == 0) && (cells[0].get_naught() == 0)) {
						return 0;
					} else if((cells[1].get_cross() == 1) && (cells[5].get_cross() == 1) && (cells[2].get_cross() == 0) && (cells[2].get_naught() == 0)) {
						return 2;
					} else if((cells[7].get_cross() == 1) && (cells[3].get_cross() == 1) && (cells[6].get_cross() == 0) && (cells[6].get_naught() == 0)) {
						return 6;
					} else if((cells[7].get_cross() == 1) && (cells[5].get_cross() == 1) && (cells[8].get_cross() == 0) && (cells[8].get_naught() == 0)) {
						return 8;
					}
				}
				vector <int> moves;
				for(int i = 0; i < maxstatesize; i ++) {
					if((cells[i].get_cross() == 0) && cells[i].get_naught() == 0) {
						moves.push_back(i);
					}
				}
				move = rand() % moves.size();
				if((cells[moves[move]].get_cross() == 0) && (cells[moves[move]].get_naught() == 0)) {
					return moves[move];
				}
			}
		}
	}
}*/

int log_move(string s,char x) {
	bool movemade = false;
	while(movemade == false) {
		if(x == 'x') {
			// Go for the win
			int move = detect_win(s,'x');
			if((move > -1) && (move < maxstatesize) && (s[move] == '-')) {
				return move;
			}
			// Block
			move = detect_win(s,'o');
			if((move > -1) && (move < maxstatesize) && (s[move] == '-')) {
				return move;
			}
			while(movemade == false) {
				move = rand() %maxstatesize;
				if(s[move] == '-') {
					return move;
				}
			}
		} else if(x == 'o') {
			// Go for the win
			int move = detect_win(s,'o');
			if((move > -1) && (move < maxstatesize) && (s[move] == '-')) {
				return move;
			}
			// Block
			move = detect_win(s,'x');
			if((move > -1) && (move < maxstatesize) && (s[move] == '-')) {
				return move;
			}
			while(movemade == false) {
				move = rand() % maxstatesize;
				if(s[move] == '-') {
					return move;
				}
			}
		}
	}
}

int rand_move(string s) {
	bool movemade = false;
	while(movemade == false) {
		int r = rand() % maxstatesize;
		if(s[r] == '-') {
			return r;
		}
	}
}


//Main Game Functions
int game(string p1, string p2) {
	vector <int> menmoves;
	vector <string> menstates;
	bool movemade = false;
	string state = "";
	for(int i = 0; i < maxstatesize; i++) {
		state += '-';
	}
	for(int i = 0; i < maxstatesize; i++) {
		if((i%2 == 0) || (i == 0)) {
			int move;
			if(p1 == "Menace") {
				while(movemade == false) {
					move = men_move(state,'x');
					if(state[move] == '-') {
						break;
					}
				}
				if(i < maxstatesize -2) {
					menstates.push_back(state);
				}
				state[move] = 'x';
				menmoves.push_back(move);
				print_grid(state);
			} else if(p1 == "Perfect") {
				//move = perf_move(state,'x');
				state[move] = 'x';
				print_grid(state);
			} else if(p1 == "Logical") {
				move = log_move(state,'x');
				state[move] = 'x';
				print_grid(state);
			} else if(p1 == "Random") {
				move = rand_move(state);
				state[move] = 'x';
				print_grid(state);
			} else if(p1 == "Player") {
				if(i == 0) {
					cin.ignore();
				}
				move = user_move(state,'x');
				state[move] = 'x';
				print_grid(state);
			}
		} else {
			int move;
			if(p2 == "Menace") {
				while(movemade == false) {
					move = men_move(state,'o');
					if(state[move] == '-') {
						break;
					}
				}
				if(i < maxstatesize -2) {
					menstates.push_back(state);
				}
				state[move] = 'o';
				menmoves.push_back(move);
				print_grid(state);
			} else if(p2 == "Perfect") {
				//move = perf_move(state,'o');
				state[move] = 'o';
				print_grid(state);
			} else if(p2 == "Logical") {
				move = log_move(state,'o');
				state[move] = 'o';
				print_grid(state);
			} else if(p2 == "Random") {
				move = rand_move(state);
				state[move] = 'o';
				print_grid(state);
			} else if(p2 == "Player") {
				if(i == 1) {
					cin.ignore();
				}
				move = user_move(state,'o');
				state[move] = 'o';
				print_grid(state);
			}
		}
		if(check_game(state) == -1) {
			cout << p1 << " Wins!" << endl;
			if(p1 == "Menace") {
				sort_lists();
				adjust_pebs(menmoves,menstates,1,1);
			} else if(p2 == "Menace") {
				sort_lists();
				adjust_pebs(menmoves,menstates,0,2);
			}
			return 1;
		} else if(check_game(state) == -2) {
			cout << p2 << " Wins!" << endl;
			if(p2 == "Menace") {
				sort_lists();
				adjust_pebs(menmoves,menstates,1,2);
			} else if(p1 == "Menace") {
				sort_lists();
				adjust_pebs(menmoves,menstates,0,1);
			}
			return 2;
		} else if(check_game(state) == -3) {
			cout << "It's a Draw!" << endl;
			if(p1 == "Menace") {
				sort_lists();
				adjust_pebs(menmoves,menstates,2,1);
			} else if(p2 == "Menace") {
				sort_lists();
				adjust_pebs(menmoves,menstates,2,2);
			}
			return 3;
		}
	}
}

void auto_play(int player) {
	string a = "";
	int response;
	int win = 0;
	int draw = 0;
	cout << "How Many Games do you want to auto play?" << endl;
	cin >> response;
	if(player == 1) {
		for(int i = 0; i < response; i ++) {
			int r = rand() %100;
			int temp;
			if(r < 50) {
				cout << "Menace Goes First" << endl;
				//temp = game("Menace","Perfect");
				if(temp == 1) {
					win++;
				} else if(temp == 3) {
					draw++;
				}
			} else {
				cout << "Perfect Goes First" << endl;
				//temp = game("Perfect","Menace");
				if(temp == 2) {
					win++;
				} else if(temp == 3) {
					draw++;
				}
			}
			remove_minus();
		}
	} else if(player == 2) {
		for(int i = 0; i < response; i ++) {
			int r = rand() %100;
			int temp;
			if(r < 50) {
				cout << "Menace Goes First" << endl;
				temp = game("Menace","Logical");
				if(temp == 1) {
					win++;
				} else if(temp == 3) {
					draw++;
				}
			} else {
				cout << "Logical Goes First" << endl;
				temp = game("Logical","Menace");
				if(temp == 2) {
					win++;
				} else if(temp == 3) {
					draw++;
				}
			}
			remove_minus();
		}
	} else if(player == 3) {
		for(int i = 0; i < response; i ++) {
			int r = rand() %100;
			int temp;
			if(r < 50) {
				cout << "Menace Goes First" << endl;
				temp = game("Menace","Random");
				if(temp == 1) {
					win++;
				} else if(temp == 3) {
					draw++;
				}
			} else {
				cout << "Random Goes First" << endl;
				temp = game("Random","Menace");
				if(temp == 2) {
					win++;
				} else if(temp == 3) {
					draw++;
				}
			}
			remove_minus();
		}
	} else if(player == 4) {
		for(int i = 0; i < response; i ++) {
			int r = rand() %100;
			int temp;
			if(r < 50) {
				cout << "Menace 1 Goes First" << endl;
				temp = game("Menace","Menace");
				if(temp == 1) {
					win++;
				} else if(temp == 3) {
					draw++;
				}
			} else {
				cout << "Menace 2 Goes First" << endl;
				temp = game("Menace","Menace");
				if(temp == 2) {
					win++;
				} else if(temp == 3) {
					draw++;
				}
			}
			remove_minus();
		}
		a = "1 ";
	}
	cout << "A Total of " << response << " games were played" << endl;
	cout << "Menace " << a <<"Won - " << win << " games, and Drew - " << draw << " games" << endl;
	save_files();
}

int main() {
	srand(time(0));
	while(0 == 0) {
		print_menu(0);
		int response;
		cin >> response;
		if(response == 1) {
			maxstatesize = 9;
			break;
		} else if(response == 2) {
			maxstatesize = 16;
			break;
		} else if(response == 3) {
			exit(0);
		} else {
			cout << "Please Enter A Valid Response" << endl;
		}
	}
	load_files();
	while(0 == 0) {
		print_menu(1);
		int response;
		cin >> response;
		if(response == 1) {
			print_menu(2);
			while (0 == 0) {
				int response2;
				cin >> response2;
				if(response2 == 1) {
					auto_play(1);
					break;
				} else if (response2 == 2) {
					auto_play(2);
					break;
				} else if (response2 == 3) {
					auto_play(3);
					break;
				} else if (response2 == 4) {
					auto_play(4);
					break;
				} else if(response2 == 5) {
					break;
				} else {
					cout << "Please Enter A Valid Response" << endl;
				}
			}
		} else if (response == 2) {
			int r = rand() %100;
			if(r < 50) {
				cout << "Menace Goes First" << endl;
				game("Menace","Player");
			} else {
				cout << "Player Goes First" << endl;
				game("Player","Menace");
			}
		} else if (response == 3) {
			print_menu(2);
			while (0 == 0) {
				int response2;
				cin >> response2;
				if(response2 == 1) {
					int r = rand() %100;
					if(r < 50) {
						cout << "Perfect Player Goes First" << endl;
						//game("Perfect","Player");
					} else {
						cout << "Player Goes First" << endl;
						//game("Player","Perfect");
					}
					break;
				} else if (response2 == 2) {
					int r = rand() %100;
					if(r < 50) {
						cout << "Logical Player Goes First" << endl;
						game("Logical","Player");
					} else {
						cout << "Player Goes First" << endl;
						game("Player","Logical");
					}
					break;
				} else if (response2 == 3) {
					int r = rand() %100;
					if(r < 50) {
						cout << "Random Goes First" << endl;
						game("Random","Player");
					} else {
						cout << "Player Goes First" << endl;
						game("Player","Random");
					}
					break;
				} else if(response2 == 4) {
					break;
				} else {
					cout << "Please Enter A Valid Response" << endl;
					print_menu(1);
				}
			}
		} else if (response == 4) {
			save_files();
			break;
		} else if (response == 5) {
			print_states();
		} else if (response == 6) {
			cout << "Space Holder" << endl;
		} else {
			cout << "Please Enter A Valid Response" << endl;
		}
	}
}