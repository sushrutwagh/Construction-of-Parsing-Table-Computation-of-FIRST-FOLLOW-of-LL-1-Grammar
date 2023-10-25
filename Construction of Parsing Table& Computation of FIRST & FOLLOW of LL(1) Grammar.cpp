#include<bits/stdc++.h>

using namespace std;
//Map for storing the Production Rules
map<string,vector<string>>production;


// map of type <<pair<NT,first>,production rules deriving that first>>
map<pair<string,string>,vector<string>>NTs_first_rules;

vector<string> temp;
//Recursive Function for deriving a terminal if a non_terminal symbol is encountered as FIRST
string rec;
string recurs(string s){
	for(int i=0;i<production[s].size();i++){
		if(production[s][i][0] >= 'A' && production[s][i][0] <= 'Z'){
			if((production[s][i][0] + production[s][i][1]) == (production[s][i][0] + '\'')){
				string r;
				r.push_back(production[s][i][0]);
				r.push_back(production[s][i][1]);
				(recurs(r));
			}
			else{
				string r;
				r.push_back(production[s][i][0]);
				(recurs(r));
			}
		}
		else{
			rec.push_back(production[s][i][0]);
		}
	}
	return rec;
}
map<string, string> first_ans;
void first(string s){
	string ans = "";
	for(int i=0;i<production[s].size();i++){
		if(production[s][i][0] >= 'A' and production[s][i][0] <= 'Z'){
			if((production[s][i][0] + production[s][i][1]) == (production[s][i][0] + '\'')){
				string r;
				r.push_back(production[s][i][0]);
				r.push_back(production[s][i][1]);
				ans += (recurs(r));
				pair<string,string>x = {s,recurs(r)};
				NTs_first_rules[x].push_back(r);
			}
			else{
				string r;
				r.push_back(production[s][i][0]);
				ans += (recurs(r));
				pair<string,string>x = {s,recurs(r)};
				NTs_first_rules[x].push_back(r);
			}
		}
		else{
		    string r = "";
		    r.push_back(production[s][i][0]);
		    ans.push_back(production[s][i][0]);
		    string temp = "";
		    temp.push_back(production[s][i][0]);
				pair<string,string>x = {s,temp};
				NTs_first_rules[x].push_back(temp);
		}
	}
	first_ans[s] = ans;


	// Printing First of all Non Terminals
	cout<<"First("<<s<<")-> {";
	for(int i=0;i<ans.length();++i){
		cout<<ans[i];
		if(i!=ans.length()-1)
		{
		    cout<<",";
		}
	}
	cout<<"}"<<endl;
	rec = "";
}




map<string,string > follow_ans;

void follow(string s){
	for(map<string,vector<string> >::iterator it = production.begin();it!=production.end();it++){
		for(int i=0;i<it->second.size();i++){
			for(int j=0;j<it->second[i].length();j++){
				if(s.length()>1 and it->second[i][j]==s[0] and j!=it->second[i].length()-1 and it->second[i][j+1] == '\''){
					j++;
					if(j==it->second[i].length()-1) follow_ans[s] += follow_ans[it->first];
					else if(it->second[i][j+1] <='Z' and it->second[i][j+1]>='A'){
                        string temp;
			            temp.push_back(it->second[i][j+1]);
                        if(it->second[i][j+2] == '\'') temp.push_back(it->second[i][j+2]);
                        follow_ans[s] += first_ans[temp];
                        follow_ans[s] += follow_ans[it->first];
					}else if(it->second[i][j+1] != '\'') follow_ans[s].push_back(it->second[i][j+1]);
				}
				else if(s.length()==1 and s[0] == it->second[i][j]){
                    if(j==it->second[i].length()-1) follow_ans[s] += follow_ans[it->first];
					else if(it->second[i][j+1] <='Z' and it->second[i][j+1]>='A'){
                        string temp; temp.push_back(it->second[i][j+1]);
                        if(it->second[i][j+2] == '\'') temp.push_back(it->second[i][j+2]);
                        follow_ans[s] += first_ans[temp];
                        follow_ans[s] += follow_ans[it->first];
					}else if(it->second[i][j+1] != '\'') follow_ans[s].push_back(it->second[i][j+1]);
				}
			}
		}
	}
}






//Split input string and push into the "Production" map
void createProduction(string &s){
	string non_terminalinal = "",terminal="";
	int i;
	for(i=0;i<s.length();++i){
		if(s[i]!='-'){
			non_terminalinal.push_back(s[i]);
		}else break;
	}
	i=i+2;
	for(i=i;i<s.length();++i){
		if(s[i]=='|'){
			production[non_terminalinal].push_back(terminal);
			terminal="";
		}
		else{
		    terminal.push_back(s[i]);
		}
	}
	production[non_terminalinal].push_back(terminal);
	temp.push_back(non_terminalinal);
}



void printAllRules(){
	cout<<"\n\nProduction Rules: "<<endl;
	for(map<string,vector<string> >::iterator it = production.begin();it!=production.end();it++){
		cout<<it->first<<"->";
		for(int i=0;i<it->second.size();i++){
			cout<<it->second[i];
			if(i!=it->second.size()-1) cout<<"|";
		}
		cout<<endl;
	}
	cout<<"\n\n"<<endl;
}

int main() {
	cout<<"Enter Production Rules(Type 'end' at last to break):"<<endl;
	string s;
	while(cin>>s and s!="end"){
		createProduction(s);
	}
	printAllRules();


	// Printing FOLLOWs
    cout<<"**********"<<endl;
    cout<<"* FIRSTS *"<<endl;
    cout<<"**********"<<endl;

	for(auto it:production){
		first(it.first);
	}

// 	for(map<string,string >::iterator it = first_ans.begin();it!=first_ans.end();it++){
// 		//cout<<it->first<<"->"<<it->second<<endl;
// 	}
// 	for(int i=0;i<temp.size();++i){
// 		//cout<<temp[i]<<endl;
// 	}
	follow_ans[temp[0]]+="$";
	for(int i=0;i<temp.size();++i){
		follow(temp[i]);
	}
	map<string,set<char> > f;
	cout<<endl<<endl<<endl<<endl;
	for(map<string,string >::iterator it = follow_ans.begin();it!=follow_ans.end();it++){
		for(int i=0;i<it->second.length();i++){
            f[it->first].insert(it->second[i]);
		}
	}


	// Printing FOLLOWs
    cout<<"***********"<<endl;
    cout<<"* FOLLOWS *"<<endl;
    cout<<"***********"<<endl;
	for(map<string,set<char>>::iterator it = f.begin();it!=f.end();it++){
        cout<<"Follow("<<it->first<<")->{";
        int count = 0;
        for(set<char>::iterator st = it->second.begin();st!=it->second.end();st++){
            if(count!=0){
                cout<<",";
            }
            if(*st!='#'){
                cout<<*st;
                count++;
            }
        }
        cout<<"}"<<endl;
	}
	cout<<endl<<endl<<endl<<endl;






set<string>NT;
set<char>T;


// for finding all Non terminals and inserting them into a set
for(auto it = first_ans.begin(); it!=first_ans.end();it++){
	NT.insert(it->first);
}
for(auto it = follow_ans.begin(); it!=follow_ans.end();it++){
	NT.insert(it->first);
}


// for finding all terminals and inserting them into a set
for(auto it = first_ans.begin(); it!=first_ans.end();it++){
    string temp = it->second;
    for(int i=0;i<temp.size();i++){
        if(temp[i]!='#'){
            T.insert(temp[i]);
        }
    }
}
for(auto it = follow_ans.begin(); it!=follow_ans.end();it++){
    string temp = it->second;
    for(int i=0;i<temp.size();i++){
        if(temp[i]!='#'){
            T.insert(temp[i]);
        }
    }
}






// for(auto it = NT.begin(); it!=NT.end();it++){
//     cout<<*it<<" ";
// }

// cout<<endl;

// for(auto it = T.begin(); it!=T.end();it++){
//     cout<<*it<<" ";
// }





vector<vector<string>>parsing_table;
//pushing column names(Terminals) into the first row of the vector
vector<string>column_names;
column_names.push_back("ZZZZZ");
for(auto it = T.begin(); it!=T.end();it++){
    string s;
    s.push_back(*it);
    column_names.push_back(s);
}
parsing_table.push_back(column_names);

// Now pushing Non terminals as first element of each row vector
for(auto it = NT.begin(); it!=NT.end();it++){
    vector<string>row_names;
    string s;
    s = s + (*it);
    row_names.push_back(s);
    parsing_table.push_back(row_names);
}






// Initializing Productions in Parsing table
for(int i=1;i<parsing_table.size();i++){
    int rowsize = parsing_table[0].size();
    for(int j=1;j<rowsize;j++){
        parsing_table[i].push_back("*");
    }
    cout<<endl;
}





//for finding and mapping production rules to be pushed in the parsing table acc to firsts of each NTs
for(auto it:NTs_first_rules){
    pair<string,string>temp = it.first;
    // cout<<"NT = "<<temp.first<<" "<<"First = "<<temp.second<<" "<<"Rules = "<<" ";
    vector<string>v = it.second;
    for(auto itr:v){
        // Comparring each NT and rule with the corresponding Firsts froduced and pushing them in the table accordingly.
    	for(map<string,vector<string>>::iterator x = production.begin();x!=production.end();x++){
    		for(int i=0;i<x->second.size();i++){
    	        string rule_check="";
    	        rule_check.push_back(x->second[i][0]);
        	    if(temp.first==x->first && itr==rule_check){
        	        string ftemp = temp.second;
        	        map<string,int>unique_ftemp;
        	        for(int count=0;count<ftemp.size();count++){
        	            string tempstr = "";
        	            tempstr.push_back(ftemp[count]);
        	            unique_ftemp[tempstr]++;
        	        }
        	        for(auto idx:unique_ftemp){
        	            if(idx.first!="#"){
                	        int row = 1;
                	        int column = 1;
                	        for(int r=1;r<parsing_table.size();r++){
                	            if(x->first==parsing_table[r][0]){
                	                row = r;
                	            }
                	        }
                	        for(int c=1;c<parsing_table[0].size();c++){
                	            if(idx.first==parsing_table[0][c]){
                	                column = c;
                	            }
                	        }
                	        string answer = x->first + "->" + x->second[i];
                	        parsing_table[row][column] = answer;
            	        }
            	        else{
                        	set<char>followstoinsert = f[x->first];
                        	for(auto itrx=followstoinsert.begin();itrx!=followstoinsert.end();itrx++){
                        	    string checker = "";
                        	    checker.push_back(*itrx);
                    	        int row = 1;
                    	        int column = 1;
                    	        for(int r=1;r<parsing_table.size();r++){
                    	            if(x->first==parsing_table[r][0]){
                    	                row = r;
                    	            }
                    	        }
                    	        for(int c=1;c<parsing_table[0].size();c++){
                    	            if(checker==parsing_table[0][c]){
                    	                column = c;
                    	            }
                    	        }
                    	        string answer = x->first + "->" + idx.first;
                    	        parsing_table[row][column] = answer;
                        	}
            	        }
        	        }
        	    }
    		}
    	}
    }
}




// Printing Patrsing Table
cout<<"*****************"<<endl;
cout<<"* PARSING TABLE *"<<endl;
cout<<"*****************"<<endl;
for(auto it:parsing_table){
    vector<string>temp = it;
    for(auto itr:temp){
        cout<<itr<<"\t\t";
    }
    cout<<endl;
}

	return 0;
}






