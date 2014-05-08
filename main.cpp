/*
 * main.cpp
 *
 *  Created on: 02-May-2014
 *      Author: sp
 */
#include<iostream>
#include<cstdio>
#include<stdlib.h>
#include "util.h"
using namespace std;
int main(int argc, char** argv){
	if (argc != 2){
		cout<<"Usage: solver <path_to_dimacs_file>\n";
		return -1;
	}

	char * in;
	long length;
	int vars, clauses;
	FILE * f = fopen (argv[1], "rb");

	if (f){
	  fseek (f, 0, SEEK_END);
	  length = ftell (f);
	  fseek (f, 0, SEEK_SET);
	  in = (char*)malloc(length);
	  if (in){
		fread (in, 1, length-1, f);
		in[length-1]=-1;
	  }else{
		  cout<<"Fatal Error: Could not allocate memory buffer of length "<<length<<endl;
		  fclose(f);
		  return -1;
	  }
	  fclose (f);
	}else{
		cout<<"Fatal Error: Could not read from file.";
		return -1;
	}
	vector<vector<int> > formula;
	pair<int, int> returnedPair=dimacsToHypergraph(in,"graph.txt",formula);
	vars=returnedPair.first;
	clauses=returnedPair.second;
	cout<<"length of formula="<<formula.size()<<endl;
		for(int i=0;i<formula.size();i++){
			for(int j=0;j<formula[i].size();j++){
				cout<<formula[i][j]<<" ";
			}
			cout<<endl;
		}
	int retVal=system("shmetis graph.txt 2 5");
	if(retVal==-1){
		cout<<"Fatal Error: Could not execute shmetis"<<endl;
		return -1;
	}
	char* parts=readFile("graph.txt.part.2");
	vector<vector<vector<int> > > partitions(2);
	for(int i=0;;i++){
		if (*parts=='\0'){
			break;
		}
		int part=parseInt(parts);
		cout<<"Clause number "<<i<<" is going to partition "<<part<<endl;
		partitions[part].push_back(formula[i]);
	}
	for(int i=0;i<partitions.size();i++){
		cout<<"partition number "<<i<<endl;
		for(int j=0;j<partitions[i].size();j++){
			for(int k=0;k<partitions[i][j].size();k++){
				cout<<partitions[i][j][k]<<" ";
			}
			cout<<endl;
		}
	}
	cout<<"here"<<endl;
	vector<vector<bool> > literalInPartition(2, vector<bool>(vars, false));
	for(int i=0;i<partitions.size();i++){
			for(int j=0;j<partitions[i].size();j++){
				for(int k=0;k<partitions[i][j].size();k++){
					int lit=partitions[i][j][k];
					literalInPartition[i][abs(lit)-1]=true;
				}
			}
		}
	for(int i=0;i<2;i++){
		for(int j=0;j<vars;j++){
			cout<<literalInPartition[i][j]<<" ";
		}
		cout<<endl;
	}
}
