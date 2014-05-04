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
	//cout<<in<<endl;
	dimacsToHypergraph(in,"graph.txt");
}
