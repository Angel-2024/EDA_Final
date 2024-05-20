#include<iostream>
#include<vector>
#include<array>
#include<fstream>
#include<utility>
#include<algorithm>
#include<windows.h>
#include<queue>
using namespace std;
// Math
int pwr2(int a){int ans = 1; for(int i=0;i<a;i++){ans*=2;} return ans;}
int log2(int a){int ans = 0; while(a!=0){a/=2; ans+=1;} return ans;}
// Printer
void print(vector<int> a){for(int i=0;i<a.size();i++){cout<<a[i]<<" ";}cout<<endl;}
void print(vector<char> a){for(int i=0;i<a.size();i++){cout<<a[i];}cout<<endl;}
void print(vector<bool> a){for(int i=0;i<a.size();i++){cout<<a[i];}cout<<endl;}
void print(vector<vector<int>> a){for(int i=0;i<a.size();i++){print(a[i]);}}
void print(vector<vector<char>> a){for(int i=0;i<a.size();i++){print(a[i]);}}
void print(vector<vector<bool>> a){for(int i=0;i<a.size();i++){print(a[i]);}}
// Truth to Binary
vector<char> Binary(int a, int varNum){
	vector<char> bin;
	while(varNum>1){
		varNum /= 2;
		if(a/varNum==0){bin.push_back('0');}
		else{bin.push_back('1'); a -= varNum;}
	}
	return bin;
}
// Merge Minterms
bool equalto(vector<char> a, vector<char> b, int digit){
	for(int i=0;i<digit;i++){
		if(a[i]!=b[i]){return false;}
	} return true;
}
void mergeMinterms(vector<vector<char>> &a){
	vector<vector<char>> Result;
	for(int i=0;i<a.size();){
		if(i!=a.size()-1){
			if(equalto(a[i],a[i+1],a[i].size()-1)){
				a[i][a[i].size()-1] = '-';
				Result.push_back(a[i]); i+=2;
			}
			else{Result.push_back(a[i]); i+=1;}			
		}
		else{Result.push_back(a[i]); i+=1;}
	} a = Result;
}
void rotate(vector<vector<char>> &a, int varNum){
	for(int i=0;i<a.size();i++){
		a[i].push_back(a[i][varNum]); a[i][varNum] = 'X';
	} sort(a.begin(),a.end());
}
void merge(vector<vector<char>> &a){
	int varNum = a[0].size();
	for(int i=0;i<varNum;i++){
		mergeMinterms(a); rotate(a,i);
	}
	for(int i=0;i<a.size();i++){
		for(int j=varNum-1;j>=0;j--){
			a[i][j] = a[i][j+varNum]; a[i].pop_back();
		}
	}
}
void reOrder(vector<vector<vector<char>>> &a, vector<char> &varName){
	vector<vector<int>> order;
	for(int i=0;i<varName.size();i++){order.push_back({0,i});}
	for(int i=0;i<a.size();i++){
		for(int j=0;j<a[i].size();j++){
			for(int k=0;k<a[i][j].size();k++){
				if(a[i][j][k]!='-'){order[k][0]-=1;}
			}
		}
	}
	sort(order.begin(),order.end());
	vector<vector<vector<char>>> b = a; vector<char> VarName = varName;
	for(int i=0;i<b.size();i++){
		for(int j=0;j<b[i].size();j++){
			for(int k=0;k<b[i][j].size();k++){
				b[i][j][k] = a[i][j][order[k][1]];
			}
		} sort(b[i].begin(),b[i].end());
	}
	for(int i=0;i<VarName.size();i++){VarName[i] = varName[order[i][1]];}
	a = b; varName = VarName;
}
// Struct
struct edge{int start, end; bool inv; char op;};
void print(edge n){
	cout<<n.start<<" -> "<<n.end<<"  \t"<<n.inv<<endl;
}
edge New(int Start, int End, bool Inv, char Op){
	edge Newedge; Newedge.start = Start; Newedge.end = End; Newedge.inv = Inv; Newedge.op = Op; return Newedge;
}
void print(vector<edge> n){for(int i=0;i<n.size();i++){print(n[i]);}}
// Write AO Tree
void AO(vector<vector<char>> a, vector<edge> &data, int root, int &current, int layer, int left, int right){
	int neg = left; int pos = right;
	for(int i=left;i<=right; i++){if(a[i][layer]!='-'){neg = i;break;}}
	if(a[right][layer]=='-'){neg = right+1;}
	for(int i=right;i>=left; i--){if(a[i][layer]!='1'){pos = i;break;}}
	if(a[left][layer]=='1'){pos = left-1;}
	bool L=(neg>left); bool M=(pos>=neg); bool R=(right>pos);
	if(L&&M&&R){
		int Lroot = current+3; int Mroot = current+4; int Rroot = current+5;
		data.push_back(New(root,current,false,'+'));
		data.push_back(New(root,current+1,false,'+'));
		data.push_back(New(current,current+3,false,'+'));
		data.push_back(New(current,current+2,false,'+'));
		data.push_back(New(current+1,-layer-2,false,'*'));
		data.push_back(New(current+1,current+5,false,'*'));
		data.push_back(New(current+2,-layer-2,true,'*'));
		data.push_back(New(current+2,current+4,false,'*'));
		current += 6;
		AO(a, data, Lroot, current, layer+1, left, neg-1);
		AO(a, data, Mroot, current, layer+1, neg, pos);
		AO(a, data, Rroot, current, layer+1, pos+1, right);
	}
	else if(L&&M){
		int Lroot = current+1; int Mroot = current+2;
		data.push_back(New(root,current+1,false,'+'));
		data.push_back(New(root,current,false,'+'));
		data.push_back(New(current,-layer-2,true,'*'));
		data.push_back(New(current,current+2,false,'*'));
		current += 3;
		AO(a, data, Lroot, current, layer+1, left, neg-1);
		AO(a, data, Mroot, current, layer+1, neg, pos);		
	}
	else if(L&&R){
		int Lroot = current+1; int Rroot = current+2;
		data.push_back(New(root,current+1,false,'+'));
		data.push_back(New(root,current,false,'+'));
		data.push_back(New(current,-layer-2,false,'*'));
		data.push_back(New(current,current+2,false,'*'));
		current += 3;
		AO(a, data, Lroot, current, layer+1, left, neg-1);
		AO(a, data, Rroot, current, layer+1, pos+1, right);
	}
	else if(M&&R){
		int Mroot = current+2; int Rroot = current+3;
		data.push_back(New(root,current,false,'+'));
		data.push_back(New(root,current+1,false,'+'));
		data.push_back(New(current,-layer-2,true,'*'));
		data.push_back(New(current,current+2,false,'*'));
		data.push_back(New(current+1,-layer-2,false,'*'));
		data.push_back(New(current+1,current+3,false,'*'));
		current += 4;
		AO(a, data, Mroot, current, layer+1, neg, pos);
		AO(a, data, Rroot, current, layer+1, pos+1, right);
	}
	else if(L){
		if(layer<a[0].size()){
			int Lroot = root;
			AO(a, data, Lroot, current, layer+1, left, neg-1);
		}
		//else{data.push_back(New(root,-1,false,'*'));}
	}
	else if(M){
		if(layer<a[0].size()){
			int Mroot = current;
			data.push_back(New(root,-layer-2,true,'*'));
			data.push_back(New(root,current,false,'*'));
			current += 1;
			AO(a, data, Mroot, current, layer+1, neg, pos);			
		}
		//else{data.push_back(New(root,-1,false,'*'));}
	}
	else if(R){
		if(layer<a[0].size()){
			int Rroot = current;
			data.push_back(New(root,-layer-2,false,'*'));
			data.push_back(New(root,current,false,'*'));
			current += 1;
			AO(a, data, Rroot, current, layer+1, pos+1, right);			
		}
		//else{data.push_back(New(root,-1,false,'*'));}
	}	
}
bool compare_se(edge i, edge j){
	if(i.start>j.start){return true;}
	if(i.start<j.start){return false;}
	return i.end>j.end;
}
bool compare_se2(edge i, edge j){
	if(i.start>j.start){return false;}
	if(i.start<j.start){return true;}
	return i.end<j.end;
}
void assignINV(vector<edge> &data){
	vector<bool> reference;
	for(int i=0;i<data.size();i++){
		reference.push_back(false);
	}
	for(int i=0;i<data.size();i++){
		if(data[i].op=='+'){
			reference[data[i].start] = true;
		}
	}
	for(int i=0;i<data.size();i++){
		if(reference[data[i].start]==true){
			data[i].inv = !data[i].inv;
		}
		if(data[i].end >= 0){
			if(reference[data[i].end]==true){
				data[i].inv = !data[i].inv;
			}
		}
	}
}
void MIAIG(vector<edge> &data, vector<int> &outputs){
	vector<int> reference; int initsize = data.size();
	for(int i=0;i<data.size();i++){
		reference.push_back(i);
	}
	for(int i=0;i<data.size();i++){
		if(data[i].end>=0 && data[i].inv==false){
			reference[data[i].end] = data[i].start;
		}
	}
	for(int i=0;i<data.size();i++){
		while(reference[data[i].start]!=data[i].start){
			data[i].start = reference[data[i].start];
		}
		if(data[i].end>=0){
			while(reference[data[i].end]!=data[i].end){
				data[i].end = reference[data[i].end];
			}
		}
	}
	for(int i=0;i<data.size();i++){
		if(data[i].start==data[i].end){
			data[i].start = -100; data[i].end = -100;
		}
	}
	sort(data.begin(),data.end(),compare_se);
	while(data[data.size()-1].start<0){data.pop_back();}
	vector<bool> exist; vector<int> newpos; int init = -1;
	for(int i=0;i<initsize;i++){exist.push_back(false);}
	for(int i=0;i<data.size();i++){exist[data[i].start]=true;}
	for(int i=0;i<exist.size();i++){init += exist[i]; newpos.push_back(init);}
	for(int i=0;i<outputs.size();i++){outputs[i] = newpos[outputs[i]];}
	for(int i=0;i<data.size();i++){
		data[i].start = newpos[data[i].start];
		if(data[i].end>=0){data[i].end = newpos[data[i].end];}
	} sort(data.begin(),data.end(),compare_se2);
}
vector<int> Fanout(vector<edge> data){
	int maxno = 0; vector<int> token;
	for(int i=0;i<data.size();i++){
		if(maxno<data[i].start){maxno = data[i].start;}
	}
	for(int i=0;i<=maxno;i++){token.push_back(0);}
	for(int i=0;i<data.size();i++){if(data[i].end>=0){token[data[i].end] += 1;}}
	return token;
}
vector<int> Topological(vector<edge> data){
	sort(data.begin(),data.end(),compare_se2);
	vector<int> lookup = {0}; vector<int> token; int size = 0;
	for(int i=0;i<data.size();i++){
		if(data[i].start != data[i-1].start){lookup.push_back(i);}
	} lookup.push_back(data.size());
	int maxno = 0; vector<int> result;
	for(int i=0;i<data.size();i++){
		if(maxno<data[i].start){maxno = data[i].start;}
	}
	for(int i=0;i<=maxno;i++){token.push_back(0);}
	for(int i=0;i<data.size();i++){if(data[i].end>=0){token[data[i].end] += 1;}}
	while(size<=maxno){
		for(int i=maxno;i>=0;i--){
			if(token[i]==0){
				token[i] = 1; size += 1; result.push_back(i);
				for(int j=lookup[i];j<lookup[i+1];j++){
					if(data[j].end>=0){token[data[j].end] -= 1;}
				}
			}
		}
	}
	return result;
}
bool positive(vector<char> a, vector<char> b){
	for(int i=0;i<a.size();i++){
		if(b[i]!='X'&&a[i]!=b[i]){
			return false;
		}
	}
	return true;
}
bool negative(vector<char> a, vector<char> b){
	for(int i=0;i<a.size();i++){
		if(b[i]!='X'&&a[i]==b[i]){
			return false;
		}
	}
	return true;
}
bool fanin(vector<vector<edge>> data_start, int a, int b){
	vector<bool> marked;
	if(a==b){return true;}
	for(int i=0;i<=data_start.size();i++){marked.push_back(false);}
	queue<int> remain; remain.push(b);
	while(!remain.empty()){
		for(int i=0;i<data_start[remain.front()].size();i++){
			if(data_start[remain.front()][i].end>=0){
				if(data_start[remain.front()][i].end==a){return true;}
				if(marked[data_start[remain.front()][i].end]==false){
					remain.push(data_start[remain.front()][i].end);
					marked[data_start[remain.front()][i].end]==true;
				}
			}
		}
		remain.pop();
	}
	return false;
}
void TruthTable(vector<edge> data, vector<int> Topological, vector<char> varName, vector<int> outputs, int iteration){
	vector<char> empty1; vector<vector<char>> Internal; vector<vector<char>> Primary; int maxno = Topological.size();
	for(int j=0;j<pwr2(varName.size());j++){empty1.push_back('1');}
	
	for(int i=0;i<maxno;i++){Internal.push_back(empty1);}
	for(int i=0;i<varName.size();i++){Primary.push_back(empty1);}
	for(int i=0;i<varName.size();i++){
		for(int k=0;k<pwr2(varName.size());k++){
			if(k%pwr2(varName.size()-varName[i]+65)<pwr2(varName.size()-varName[i]+64)){
				Primary[i][k]='0';
			}
		}		
	}
	vector<vector<edge>> data_start; vector<edge> emptydata;
	for(int i=0;i<maxno;i++){data_start.push_back(emptydata);}
	for(int i=0;i<data.size();i++){
		data_start[data[i].start].push_back(data[i]);
	}
	for(int i=Topological.size()-1;i>=0;i--){
		for(int j=0;j<data_start[Topological[i]].size();j++){
			for(int k=0;k<pwr2(varName.size());k++){
				if(data_start[Topological[i]][j].end<0){
					if(Primary[-data_start[Topological[i]][j].end-2][k]=='0' ^ data_start[Topological[i]][j].inv){
						Internal[Topological[i]][k] = '0';
					}
				}
				if(data_start[Topological[i]][j].end>=0){
					if(Internal[data_start[Topological[i]][j].end][k]=='0' ^ data_start[Topological[i]][j].inv){
						Internal[Topological[i]][k] = '0';
					}
				}
			}
		}
	}
	vector<vector<bool>> care; vector<bool> bool1; 
	for(int k=0;k<pwr2(varName.size());k++){bool1.push_back(false);}
	for(int i=0;i<Internal.size();i++){care.push_back(bool1);}
	for(int i=0;i<outputs.size();i++){
		for(int k=0;k<pwr2(varName.size());k++){
			care[outputs[i]][k] = true;
		}
	}
	for(int i=0;i<Topological.size();i++){
		for(int k=0;k<pwr2(varName.size());k++){
			if(Internal[Topological[i]][k]=='1' && care[Topological[i]][k]){
				for(int j=0;j<data_start[Topological[i]].size();j++){
					if(data_start[Topological[i]][j].end>=0){
						care[data_start[Topological[i]][j].end][k] = true;
					}
				}
			}
			if(Internal[Topological[i]][k]=='0' && care[Topological[i]][k]){
				bool zero = false;
				for(int j=0;j<data_start[Topological[i]].size();j++){
					if(data_start[Topological[i]][j].end<0){
						if(Primary[-data_start[Topological[i]][j].end-2][k]=='0' ^ data_start[Topological[i]][j].inv){
							zero = true;
						}
					}
					if(data_start[Topological[i]][j].end>=0){
						if(Internal[data_start[Topological[i]][j].end][k]=='0' ^ data_start[Topological[i]][j].inv){
							if(zero == false){
								care[data_start[Topological[i]][j].end][k] = true; zero = true; 
							}
						}
					}
				}
			}
		}
	}
	vector<vector<char>> Internal_dc = Internal;
	for(int i=0;i<Internal_dc.size();i++){
		for(int j=0;j<Internal_dc[i].size();j++){
			if(care[i][j]==false){
				Internal_dc[i][j] = 'X';
			}
		}
	}
	for(int i=0;i<Internal.size();i++){
		if(fanin(data_start,Topological[i],Topological[iteration])==false){
			if(positive(Internal[Topological[i]],Internal_dc[Topological[iteration]])){
				cout<<Topological[i]<<" can be connected to "<<Topological[iteration]<<" without invert"<<endl;
			}
			if(negative(Internal[Topological[i]],Internal_dc[Topological[iteration]])){
				cout<<Topological[i]<<" can be connected to "<<Topological[iteration]<<" with invert"<<endl;
			}			
		}
	}
}
int main(){
	// Input and Write
	ifstream truth("Inputs/ex00.truth");
	int size = 1000000; char num[size]; vector<vector<char>> Truth; vector<char> empty1;
	while(truth.getline(num,size)){
		Truth.push_back(empty1);
		for(int i=0;num[i]=='0'||num[i]=='1';i++){
			Truth[Truth.size()-1].push_back(num[i]);
		}
    }
    // Truth to Minterm
    vector<vector<vector<char>>> Minterm3; vector<vector<char>> empty2;
    for(int i=0;i<Truth.size();i++){
    	Minterm3.push_back(empty2);
    	for(int j=0;j<Truth[i].size();j++){
    		if(Truth[i][j]=='1'){Minterm3[i].push_back(Binary(j,Truth[i].size()));}
		}
	}
	// Merge Minterm
	for(int i=0;i<Minterm3.size();i++){merge(Minterm3[i]);}
	int varNum = Minterm3[0][0].size();
	vector<char> varName; vector<int> inputs;
	for(int i=0;i<varNum;i++){varName.push_back(i+65);}
	reOrder(Minterm3,varName);
	vector<edge> data; vector<int> outputs;
	int root = 0; int current = 1;
	for(int i=0;i<Minterm3.size();i++){
		outputs.push_back(root);
		data.push_back(New(root,root+1,false,'*'));
		root += 1; current += 1;
		AO(Minterm3[i], data, root, current, 0, 0, Minterm3[i].size()-1);
		root = current; current = root+1;
	}
	for(int i=0;i<=varName.size();i++){
		inputs.push_back(-i-1);
	}
	assignINV(data);
	MIAIG(data,outputs);
	vector<int> Top = Topological(data);
	for(int i=0;i<Topological(data).size();i++){
		TruthTable(data, Top, varName, outputs, i);
	}
	print(outputs);
}
