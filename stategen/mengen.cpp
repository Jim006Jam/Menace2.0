#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unistd.h>
#include <math.h>

using namespace std;
double maxstatesize = 9;
vector <string> fstates;
vector <string> sstates;
vector <vector<int>> fpebs;
vector <vector<int>> spebs;
vector <int> stats;


bool check_win(string s) {
	//Horizontal Wins
	for(int i = 0; i < sqrt(maxstatesize);i++) {
		vector<int> pos;
		int j = i*(sqrt(maxstatesize));
		for(j;j < (sqrt(maxstatesize))*(i+1);j++){
			pos.push_back(j);
		}
		if(maxstatesize == 9) {
			if(((s[pos[0]] == 'x') && (s[pos[1]] == 'x') && (s[pos[2]] == 'x')) || ((s[pos[0]] == 'o') && (s[pos[1]] == 'o') && (s[pos[2]] == 'o'))) {
				return true;
			}
		} else if(maxstatesize == 16) {
			if(((s[pos[0]] == 'x') && (s[pos[1]] == 'x') && (s[pos[2]] == 'x') && (s[pos[3]] == 'x')) || ((s[pos[0]] == 'o') && (s[pos[1]] == 'o') && (s[pos[2]] == 'o') && (s[pos[3]] == 'o'))) {
				return true;
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
			if(((s[pos[0]] == 'x') && (s[pos[1]] == 'x') && (s[pos[2]] == 'x')) || ((s[pos[0]] == 'o') && (s[pos[1]] == 'o') && (s[pos[2]] == 'o'))) {
				return true;
			}
		} else if(maxstatesize == 16) {
			if(((s[pos[0]] == 'x') && (s[pos[1]] == 'x') && (s[pos[2]] == 'x') && (s[pos[3]] == 'x')) || ((s[pos[0]] == 'o') && (s[pos[1]] == 'o') && (s[pos[2]] == 'o') && (s[pos[3]] == 'o'))) {
				return true;
			}
		}
	}
	
	//Diagonal Wins
	vector<int> pos;
	for(int j = 0;j < ((sqrt(maxstatesize)-1)*(sqrt(maxstatesize)+1)+1);j=j+(sqrt(maxstatesize)+1)){
		pos.push_back(j);
	}
	if(maxstatesize == 9) {
		if(((s[pos[0]] == 'x') && (s[pos[1]] == 'x') && (s[pos[2]] == 'x')) || ((s[pos[0]] == 'o') && (s[pos[1]] == 'o') && (s[pos[2]] == 'o'))) {
			return true;
		}
	} else if(maxstatesize == 16) {
		if(((s[pos[0]] == 'x') && (s[pos[1]] == 'x') && (s[pos[2]] == 'x') && (s[pos[3]] == 'x')) || ((s[pos[0]] == 'o') && (s[pos[1]] == 'o') && (s[pos[2]] == 'o') && (s[pos[3]] == 'o'))) {
			return true;
		}
	}
	pos.clear();
	for(int j = sqrt(maxstatesize)-1;j < ((sqrt(maxstatesize)-1)*(sqrt(maxstatesize))+1);j=j+(sqrt(maxstatesize)-1)){
		pos.push_back(j);
	}
	if(maxstatesize == 9) {
		if(((s[pos[0]] == 'x') && (s[pos[1]] == 'x') && (s[pos[2]] == 'x')) || ((s[pos[0]] == 'o') && (s[pos[1]] == 'o') && (s[pos[2]] == 'o'))) {
			return true;
		}
	} else if(maxstatesize == 16) {
		if(((s[pos[0]] == 'x') && (s[pos[1]] == 'x') && (s[pos[2]] == 'x') && (s[pos[3]] == 'x')) || ((s[pos[0]] == 'o') && (s[pos[1]] == 'o') && (s[pos[2]] == 'o') && (s[pos[3]] == 'o'))) {
			return true;
		}
	}
	return false;
}

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

void generate_states() {
	int max = round(maxstatesize);
	char state[max];
	//empty
	string str;
	for(int i = 0; i < maxstatesize; i++) {
		str += '-';
	}
	fstates.push_back(str);
	
	//first move
	for(int i = 0; i < maxstatesize; i ++) {
		for(int j = 0; j < maxstatesize; j ++) {
			state[j] = '-';
		}
		state[i] = 'x';
		string str;
		for(int c = 0; c < maxstatesize; c++) {
			str+=state[c];
		}
		sstates.push_back(str);
	}
	cout << "1" << endl;
	
	// second move
	for(int i = 0; i < maxstatesize; i ++) {
		for (int j = 0; j < maxstatesize; j ++) {
			for(int k = 0; k < maxstatesize; k ++) {
				state[k] = '-';
			}
			state[i] = 'x';
			state[j] = 'o';
			int a = 0;
			for(int k = 0; k < maxstatesize; k++) {
				if(state[k] == 'x' || state[k] == 'o') {
					a++;
				}
			}
			if (a == 2) {
				string str;
				for(int c = 0; c < maxstatesize; c++) {
					str+=state[c];
				}
				fstates.push_back(str);
			}
		}
	}
	cout << "2" << endl;
	
	//third move
	for(int i = 0; i < maxstatesize; i ++) {
		for (int j = 0; j < maxstatesize; j ++) {
			for (int k = 0; k < maxstatesize; k ++) {
				for(int l = 0; l < maxstatesize; l ++) {
					state[l] = '-';
				}
				state[i] = 'x';
				state[j] = 'o';
				state[k] = 'x';
				int a = 0;
				for(int l = 0; l < maxstatesize; l++) {
					if(state[l] == 'x' || state[l] == 'o') {
						a++;
					}
				}
				if (a == 3) {
					string str;
					for(int c = 0; c < maxstatesize; c++) {
						str+=state[c];
					}
					sstates.push_back(str);
				}
			}
		}
	}
	cout << "3" << endl;
	
	//fourth move
	for(int i = 0; i < maxstatesize; i ++) {
		for (int j = 0; j < maxstatesize; j ++) {
			for (int k = 0; k < maxstatesize; k ++) {
				for(int l = 0; l < maxstatesize; l ++) {
					for(int m = 0; m < maxstatesize; m ++) {
						state[m] = '-';
					}
					state[i] = 'x';
					state[j] = 'o';
					state[k] = 'x';
					state[l] = 'o';
					int a = 0;
					for(int m = 0; m < maxstatesize; m++) {
						if(state[m] == 'x' || state[m] == 'o') {
							a++;
						}
					}
					if (a == 4) {
						string str;
						for(int c = 0; c < maxstatesize; c++) {
							str+=state[c];
						}
						fstates.push_back(str);
					}
				}
			}
		}
	}
	cout << "4" << endl;
	
	//fifth move
	for(int i = 0; i < maxstatesize; i ++) {
		for (int j = 0; j < maxstatesize; j ++) {
			for (int k = 0; k < maxstatesize; k ++) {
				for (int l = 0; l < maxstatesize; l ++) {
					for (int m = 0; m < maxstatesize; m ++) {
						for (int n = 0; n < maxstatesize; n ++) {
							state[n] = '-';
						}
						state[i] = 'x';
						state[j] = 'o';
						state[k] = 'x';
						state[l] = 'o';
						state[m] = 'x';
						int a = 0;
						for(int n = 0; n < maxstatesize; n++) {
							if(state[n] == 'x' || state[n] == 'o') {
								a++;
							}
						}
						if (a == 5) {
							string str;
							for(int c = 0; c < maxstatesize; c++) {
								str+=state[c];
							}
							sstates.push_back(str);
						}
					}
				}
			}
		}
	}
	cout << "5" << endl;
	
	//sixth move
	for(int i = 0; i < maxstatesize; i ++) {
		for (int j = 0; j < maxstatesize; j ++) {
			for (int k = 0; k < maxstatesize; k ++) {
				for(int l = 0; l < maxstatesize; l ++) {
					for(int m = 0; m < maxstatesize; m ++) {
						for(int n = 0; n < maxstatesize; n ++) {
							for(int o = 0; o < maxstatesize; o ++) {
								state[o] = '-';
							}
							state[i] = 'x';
							state[j] = 'o';
							state[k] = 'x';
							state[l] = 'o';
							state[m] = 'x';
							state[n] = 'o';
							int a = 0;
							for(int o = 0; o < maxstatesize; o++) {
								if(state[o] == 'x' || state[o] == 'o') {
									a++;
								}
							}
							if (a == 6) {
								string str;
								for(int c = 0; c < maxstatesize; c++) {
									str+=state[c];
								}
								fstates.push_back(str);
							}
						}
					}
				}
			}
		}
	}
	cout << "6" << endl;
	
	//seventh move
	for(int i = 0; i < maxstatesize; i ++) {
		for (int j = 0; j < maxstatesize; j ++) {
			for (int k = 0; k < maxstatesize; k ++) {
				for(int l = 0; l < maxstatesize; l ++) {
					for(int m = 0; m < maxstatesize; m ++) {
						for(int n = 0; n < maxstatesize; n ++) {
							for(int o = 0; o < maxstatesize; o ++) {
								for(int p = 0; p < maxstatesize; p ++) {
									state[p] = '-';
								}
								state[i] = 'x';
								state[j] = 'o';
								state[k] = 'x';
								state[l] = 'o';
								state[m] = 'x';
								state[n] = 'o';
								state[o] = 'x';
								int a = 0;
								for(int p = 0; p < maxstatesize; p++) {
									if(state[p] == 'x' || state[p] == 'o') {
										a++;
									}
								}
								if (a == 7) {
									string str;
									for(int c = 0; c < maxstatesize; c++) {
										str+=state[c];
									}
									sstates.push_back(str);
									cout << str << endl;
								}
							}
						}
					}
				}
			}
		}
	}
	cout << "7" << endl;
	
	if(maxstatesize == 16) {
		//eigth move
		for(int i = 0; i < maxstatesize; i ++) {
			for (int j = 0; j < maxstatesize; j ++) {
				for (int k = 0; k < maxstatesize; k ++) {
					for(int l = 0; l < maxstatesize; l ++) {
						for(int m = 0; m < maxstatesize; m ++) {
							for(int n = 0; n < maxstatesize; n ++) {
								for(int o = 0; o < maxstatesize; o ++) {
									for(int p = 0; p < maxstatesize; p ++) {
										for(int q = 0; q < maxstatesize; q ++) {
											state[q] = '-';
										}
										state[i] = 'x';
										state[j] = 'o';
										state[k] = 'x';
										state[l] = 'o';
										state[m] = 'x';
										state[n] = 'o';
										state[o] = 'x';
										state[p] = 'o';
										int a = 0;
										for(int q = 0; q < maxstatesize; q++) {
											if(state[q] == 'x' || state[q] == 'o') {
												a++;
											}
										}
										if (a == 8) {
											string str;
											for(int c = 0; c < maxstatesize; c++) {
												str+=state[c];
											}
											fstates.push_back(str);
											cout << str << endl;
										}
									}
								}
							}
						}
					}
				}
			}
		}
		cout << "8" << endl;
		
		//ninth move
		for(int i = 0; i < maxstatesize; i ++) {
			for (int j = 0; j < maxstatesize; j ++) {
				for (int k = 0; k < maxstatesize; k ++) {
					for(int l = 0; l < maxstatesize; l ++) {
						for(int m = 0; m < maxstatesize; m ++) {
							for(int n = 0; n < maxstatesize; n ++) {
								for(int o = 0; o < maxstatesize; o ++) {
									for(int p = 0; p < maxstatesize; p ++) {
										for(int q = 0; q < maxstatesize; q++) {
											for(int r = 0; r < maxstatesize; r ++) {
												state[r] = '-';
											}
											state[i] = 'x';
											state[j] = 'o';
											state[k] = 'x';
											state[l] = 'o';
											state[m] = 'x';
											state[n] = 'o';
											state[o] = 'x';
											state[p] = 'o';
											state[q] = 'x';
											int a = 0;
											for(int r = 0; r < maxstatesize; r++) {
												if(state[r] == 'x' || state[r] == 'o') {
													a++;
												}
											}
											if (a == 9) {
												string str;
												for(int c = 0; c < maxstatesize; c++) {
													str+=state[c];
												}
												sstates.push_back(str);
												cout << str << endl;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		cout << "9" << endl;
		
		//tenth move
		for(int i = 0; i < maxstatesize; i ++) {
			for (int j = 0; j < maxstatesize; j ++) {
				for (int k = 0; k < maxstatesize; k ++) {
					for(int l = 0; l < maxstatesize; l ++) {
						for(int m = 0; m < maxstatesize; m ++) {
							for(int n = 0; n < maxstatesize; n ++) {
								for(int o = 0; o < maxstatesize; o ++) {
									for(int p = 0; p < maxstatesize; p ++) {
										for(int q = 0; q < maxstatesize; q++) {
											for(int r = 0; r < maxstatesize; r ++) {
												for(int s = 0; s < maxstatesize; s ++) {
													state[s] = '-';
												}
												state[i] = 'x';
												state[j] = 'o';
												state[k] = 'x';
												state[l] = 'o';
												state[m] = 'x';
												state[n] = 'o';
												state[o] = 'x';
												state[p] = 'o';
												state[q] = 'x';
												state[r] = 'o';
												int a = 0;
												for(int s = 0; s < maxstatesize; s++) {
													if(state[s] == 'x' || state[s] == 'o') {
														a++;
													}
												}
												if (a == 10) {
													string str;
													for(int c = 0; c < maxstatesize; c++) {
														str+=state[c];
													}
													fstates.push_back(str);
													cout << str << endl;
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		cout << "10" << endl;
		
		//eleventh move
		for(int i = 0; i < maxstatesize; i ++) {
			for (int j = 0; j < maxstatesize; j ++) {
				for (int k = 0; k < maxstatesize; k ++) {
					for(int l = 0; l < maxstatesize; l ++) {
						for(int m = 0; m < maxstatesize; m ++) {
							for(int n = 0; n < maxstatesize; n ++) {
								for(int o = 0; o < maxstatesize; o ++) {
									for(int p = 0; p < maxstatesize; p ++) {
										for(int q = 0; q < maxstatesize; q++) {
											for(int r = 0; r < maxstatesize; r ++) {
												for(int s = 0; s < maxstatesize; s ++) {
													for(int t = 0; t < maxstatesize; t ++) {
														state[t] = '-';
													}
													state[i] = 'x';
													state[j] = 'o';
													state[k] = 'x';
													state[l] = 'o';
													state[m] = 'x';
													state[n] = 'o';
													state[o] = 'x';
													state[p] = 'o';
													state[q] = 'x';
													state[r] = 'o';
													state[s] = 'x';
													int a = 0;
													for(int t = 0; t < maxstatesize; t++) {
														if(state[t] == 'x' || state[t] == 'o') {
															a++;
														}
													}
													if (a == 11) {
														string str;
														for(int c = 0; c < maxstatesize; c++) {
															str+=state[c];
														}
														sstates.push_back(str);
														cout << str << endl;
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		cout << "11" << endl;
		
		//twelfth move
		for(int i = 0; i < maxstatesize; i ++) {
			for (int j = 0; j < maxstatesize; j ++) {
				for (int k = 0; k < maxstatesize; k ++) {
					for(int l = 0; l < maxstatesize; l ++) {
						for(int m = 0; m < maxstatesize; m ++) {
							for(int n = 0; n < maxstatesize; n ++) {
								for(int o = 0; o < maxstatesize; o ++) {
									for(int p = 0; p < maxstatesize; p ++) {
										for(int q = 0; q < maxstatesize; q++) {
											for(int r = 0; r < maxstatesize; r ++) {
												for(int s = 0; s < maxstatesize; s ++) {
													for(int t = 0; t < maxstatesize; t ++) {
														for(int u = 0; u < maxstatesize; u ++) {
															state[u] = '-';
														}
														state[i] = 'x';
														state[j] = 'o';
														state[k] = 'x';
														state[l] = 'o';
														state[m] = 'x';
														state[n] = 'o';
														state[o] = 'x';
														state[p] = 'o';
														state[q] = 'x';
														state[r] = 'o';
														state[s] = 'x';
														state[t] = 'o';
														int a = 0;
														for(int u = 0; u < maxstatesize; u++) {
															if(state[u] == 'x' || state[u] == 'o') {
																a++;
															}
														}
														if (a == 12) {
															string str;
															for(int c = 0; c < maxstatesize; c++) {
																str+=state[c];
															}
															fstates.push_back(str);
															cout << str << endl;
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		cout << "12" << endl;
		
		//thirteenth move
		for(int i = 0; i < maxstatesize; i ++) {
			for (int j = 0; j < maxstatesize; j ++) {
				for (int k = 0; k < maxstatesize; k ++) {
					for(int l = 0; l < maxstatesize; l ++) {
						for(int m = 0; m < maxstatesize; m ++) {
							for(int n = 0; n < maxstatesize; n ++) {
								for(int o = 0; o < maxstatesize; o ++) {
									for(int p = 0; p < maxstatesize; p ++) {
										for(int q = 0; q < maxstatesize; q++) {
											for(int r = 0; r < maxstatesize; r ++) {
												for(int s = 0; s < maxstatesize; s ++) {
													for(int t = 0; t < maxstatesize; t ++) {
														for(int u = 0; u < maxstatesize; u ++) {
															for(int v = 0; v < maxstatesize; v ++) {
																state[v] = '-';
															}
															state[i] = 'x';
															state[j] = 'o';
															state[k] = 'x';
															state[l] = 'o';
															state[m] = 'x';
															state[n] = 'o';
															state[o] = 'x';
															state[p] = 'o';
															state[q] = 'x';
															state[r] = 'o';
															state[s] = 'x';
															state[t] = 'o';
															state[u] = 'x';
															int a = 0;
															for(int v = 0; v < maxstatesize; v++) {
																if(state[v] == 'x' || state[v] == 'o') {
																	a++;
																}
															}
															if (a == 13) {
																string str;
																for(int c = 0; c < maxstatesize; c++) {
																	str+=state[c];
																}
																sstates.push_back(str);
																cout << str << endl;
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		cout << "13" << endl;
		
		//fourteenth move
		for(int i = 0; i < maxstatesize; i ++) {
			for (int j = 0; j < maxstatesize; j ++) {
				for (int k = 0; k < maxstatesize; k ++) {
					for(int l = 0; l < maxstatesize; l ++) {
						for(int m = 0; m < maxstatesize; m ++) {
							for(int n = 0; n < maxstatesize; n ++) {
								for(int o = 0; o < maxstatesize; o ++) {
									for(int p = 0; p < maxstatesize; p ++) {
										for(int q = 0; q < maxstatesize; q++) {
											for(int r = 0; r < maxstatesize; r ++) {
												for(int s = 0; s < maxstatesize; s ++) {
													for(int t = 0; t < maxstatesize; t ++) {
														for(int u = 0; u < maxstatesize; u ++) {
															for(int v = 0; v < maxstatesize; v ++) {
																for(int w = 0; w < maxstatesize; w ++) {
																	state[w] = '-';
																}
																state[i] = 'x';
																state[j] = 'o';
																state[k] = 'x';
																state[l] = 'o';
																state[m] = 'x';
																state[n] = 'o';
																state[o] = 'x';
																state[p] = 'o';
																state[q] = 'x';
																state[r] = 'o';
																state[s] = 'x';
																state[t] = 'o';
																state[u] = 'x';
																state[v] = 'o';
																int a = 0;
																for(int w = 0; w < maxstatesize; w++) {
																	if(state[w] == 'x' || state[w] == 'o') {
																		a++;
																	}
																}
																if (a == 14) {
																	string str;
																	for(int c = 0; c < maxstatesize; c++) {
																		str+=state[c];
																	}
																	fstates.push_back(str);
																	cout << str << endl;
																}
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		cout << "14" << endl;
	}
	cout << "States Generated" << endl;
	stats.push_back(fstates.size());
	stats.push_back(sstates.size());
}

void generate_pebs() {
	for(int i = 0; i < fstates.size(); i ++) {
		vector <int> vec;
		vector <string> transforms;
		vec.assign(maxstatesize,0);
		transforms = transform(fstates[i]);
		transforms.push_back(fstates[i]);
		for(int j = 0; j < sstates.size(); j++) {
			int movenum = check_movenum(sstates[j]);
			for(int k = 0; k < transforms.size(); k ++) {
				int movenum2 = check_movenum(transforms[k]);
				vector <int> pos;
				for (int l = 0; l < maxstatesize; l ++) {
					if(movenum == movenum2-1) {
						if((transforms[k][l] != sstates[j][l]) && (fstates[i][l] == '-')) {
							pos.push_back(l);
						}
					}
				}
				if(pos.size() == 1) {
					for(int l = maxstatesize;l > 0; l=l-2) {
						if(movenum2 == l) {
							if(l % 2 == 0) {
								vec[pos[0]] = ((l-2)/2);
							} else {
								vec[pos[0]] = ((l-1)/2);
							}
						}
					}
				}
			}
		}
		fpebs.push_back(vec);
	}
	
	for(int i = 0; i < sstates.size(); i ++) {
		int t = 0;
		int movenum = check_movenum(sstates[i]);
		vector <int> vec;
		vec.assign(maxstatesize,0);
		if(movenum != 2) {
			vector <string> transforms;
			transforms = transform(sstates[i]);
			transforms.push_back(sstates[i]);
			for(int j = 0; j < fstates.size(); j++) {
				int movenum2 = check_movenum(fstates[j]);
				for(int k = 0; k < transforms.size(); k ++) {
					int movenum3 = check_movenum(transforms[k]);
					vector <int> pos;
					for (int l = 0; l < maxstatesize; l ++) {
						if(movenum2 == movenum3-1) {
							if((transforms[k][l] != fstates[j][l]) && (sstates[i][l] == '-')) {
								pos.push_back(l);
							}
						}
					}
					if(pos.size() == 1) {
						for(int l = maxstatesize-1;l > 0; l=l-2) {
							if(movenum3 == l) {
								if(l % 2 == 0) {
									vec[pos[0]] = (l/2);
								} else {
									vec[pos[0]] = ((l+1)/2);
								}
							}
						}
					}
				}
			}
		} else {
			for (int j = 0; j < maxstatesize; j++) {
				if(sstates[i][j] == '-') {
					vec[j] = 1;
				}
			}
		}
		spebs.push_back(vec);
	}
	cout << "Pebs Generated" << endl;
}

void remove_winstates() {
	for (int i = 0; i < fstates.size(); i++) {
		if (check_win(fstates[i]) == true) {
			fstates.erase(fstates.begin()+i);
		}
	}
	for (int i = 0; i < sstates.size(); i++) {
		if (check_win(sstates[i]) == true) {
			sstates.erase(sstates.begin()+i);
		}
	}
}

void remove_dupes() {
	for(int i = 0; i < fstates.size(); i++) {
		for (int j = i; j < fstates.size(); j ++) {
			if((i != j) && (fstates[i].compare(fstates[j]) == 0)) {
				int temp = 0;
				for(int k = i; k < maxstatesize; k++) {
					if(fstates[i][k] != fstates[j][k]) {
						temp++;
					}
				}
				if(temp == 0) {
					fstates.erase(fstates.begin()+j);
				}
			}
		}
	}
	for(int i = 0; i < sstates.size(); i++) {
		for (int j = i; j < sstates.size(); j ++) {
			if((i != j) && (sstates[i].compare(sstates[j]) == 0)) {
				int temp = 0;
				for(int k = i; k < maxstatesize; k++) {
					if(sstates[i][k] != sstates[j][k]) {
						temp++;
					}
				}
				if(temp == 0) {
					sstates.erase(sstates.begin()+j);
				}
			}
		}
	}
}

void remove_transforms() {
	vector <string> transforms;
	for(int i = 0; i < fstates.size(); i++) {
		transforms = transform(fstates[i]);
		for(int j = 0; j < transforms.size(); j++) {
			for(int k = i+1; k < fstates.size(); k++) {
				if (i != k) {
					int temp = 0;
					for(int l = 0; l < maxstatesize; l++) {
						if(transforms[j][l] != fstates[k][l]) {
							temp++;
						}
					}
					if(temp == 0) {
						fstates.erase(fstates.begin()+k);
					}
				}
			}
		}
	}
	for(int i = 0; i < sstates.size(); i++) {
		transforms = transform(sstates[i]);
		for(int j = 0; j < transforms.size(); j++) {
			for(int k = i+1; k < sstates.size(); k++) {
				if (i != k) {
					int temp = 0;
					for(int l = 0; l < maxstatesize; l++) {
						if(transforms[j][l] != sstates[k][l]) {
							temp++;
						}
					}
					if(temp == 0) {
						sstates.erase(sstates.begin()+k);
					}
				}
			}
		}
	}
}

void print_stats() {
	cout << "-------------Fun Facts-------------" << endl;
	cout << "After Complete Generation fstates had " << stats[0] << " states in it" << endl;
	cout << "After Complete Generation sstates had " << stats[1] << " states in it" << endl;
	cout << "After searching for Game Ending states, " << stats[2] << " and " << stats[3] << " states were removed from both state lists respectively" << endl;
	cout << "After searching for Duplicate states, " << stats[4] << " and " << stats[5] << " states were removed from both state lists respectively" << endl;
	cout << "After searching for Transformed states, " << stats[6] << " and " << stats[7] << " states were removed from both state lists respectively" << endl;
	cout << "After all that there are now " << fstates.size() << " and " << sstates.size() << " left in the two state lists respectively" << endl;
}

void write_tofile() {
	ofstream myfile;
	myfile.open ("first_player_menace.txt",std::ofstream::out | std::ofstream::trunc);
	for(int i = 0; i < fstates.size(); i++) {
		myfile << fstates[i] << " ";
		for(int j = 0; j < maxstatesize; j++) {
			if (j == maxstatesize-1) {
				myfile << "[" << fpebs[i][j] << "]";
			} else {
				myfile << "[" << fpebs[i][j] << "]" << " ";
			}
		}if(i != fstates.size()-1) {
			myfile << endl;
		}
	}
	myfile.close();
	myfile.open ("second_player_menace.txt",std::ofstream::out | std::ofstream::trunc);
	for(int i = 0; i < sstates.size(); i++) {
		myfile << sstates[i] << " ";
		for(int j = 0; j < maxstatesize; j++) {
			if (j == maxstatesize-1) {
				myfile << "[" << spebs[i][j] << "]";
			} else {
				myfile << "[" << spebs[i][j] << "]" << " ";	
			}
		}if(i != sstates.size()-1) {
			myfile << endl;
		}
	}
	myfile.close();
}

int main() {
	generate_states();
	for (int i = 0; i < 5; i++) {
		remove_winstates();
	}
	stats.push_back(stats[0] - fstates.size());
	stats.push_back(stats[1] - sstates.size());
	cout << "Win States Removed" << endl;
	for(int i = 0; i < 3; i++) {
		remove_dupes();
	}
	stats.push_back(stats[0] - stats[2] - fstates.size());
	stats.push_back(stats[1] - stats[3] - sstates.size());
	cout << "Dupes Removed" << endl;
	remove_transforms();
	stats.push_back(stats[0] - stats[2] - stats[4] - fstates.size());
	stats.push_back(stats[1] - stats[3] - stats[5] - sstates.size());
	cout << "Transforms Removed" << endl;
	generate_pebs();
	print_stats();
	write_tofile();
	cout << "Done" << endl;
}