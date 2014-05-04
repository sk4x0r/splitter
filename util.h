/*
 * util.h
 *
 *  Created on: 02-May-2014
 *      Author: sp
 */

#ifndef UTIL_H_
#define UTIL_H_
#include<stdlib.h>
#include<string.h>
#include<cstdio>
#include<vector>
#include<iostream>
#include <fstream>
using namespace std;

template<class B>
static void skipWhitespace(B& in) {
	while ((*in >= 9 && *in <= 13) || *in == 32)
		++in;
}

template<class B>
static void skipLine(B& in) {
	//cout<<"inside skipline"<<endl;
	for (;;) {
		if (*in == -1) //TODO: check *in for EOF
			return;
		if (*in == '\n') {
			++in;
			//cout<<"leaving skipline"<<endl;
			return;
		}
		++in;
	}
	//cout<<"leaving skipline"<<endl;
}

template<class B>
static int parseInt(B& in) {
	//cout << "inside parseInt" << endl;
	//cout<<"first char of int="<<*in<<endl;
	int val = 0;
	bool neg = false;
	skipWhitespace(in);
	if (*in == '-')
		neg = true, ++in;
	else if (*in == '+')
		++in;
	if (*in < '0' || *in > '9')
		fprintf(stderr, "PARSE ERROR in parseInt! Unexpected char: %c %d\n",
				*in, *in), exit(3);
	while (*in >= '0' && *in <= '9')
		val = val * 10 + (*in - '0'), ++in;
	return neg ? -val : val;
}

template<class B>
static bool eagerMatch(B& in, const char* str) {
	for (; *str != '\0'; ++str, ++in)
		if (*str != *in)
			return false;
	return true;
}

template<class B>
static vector<int> readClause(B& in) {
	vector<int> lits;
	cout << "inside readClause" << endl;
	int parsed_lit;
	lits.clear();
	for (;;) {
		parsed_lit = parseInt(in);
		cout << "parsed_lit=" << parsed_lit << endl;
		if (parsed_lit == 0)
			break;
		cout << "pushing " << parsed_lit << endl;
		cout << "lits.size()=" << lits.size() << endl;
		lits.push_back(parsed_lit);
		cout << "pushed" << endl;
	}
	cout << "returning from readClause" << endl;
	return lits;
}

template<class B>
void dimacsToHypergraph(B& in, char* out) {
	vector<int> lits; //TODO: initialize lits to size 3
	vector<vector<int> > graph;
	int vars = 0;
	int clauses = 0;
	int cnt = 0;

	FILE * f = fopen(out, "wb");
	if (!f) {
		printf("Fatal Error: Could not write to file.");
		exit(-1);
	}

	for (;;) {
		cout << *in << endl;
		if (*in == -1) {
			cout << "EOF found" << endl;
			break;
		}
		skipWhitespace(in);
		if (*in == 'p') {
			cout << "found p" << endl;
			if (eagerMatch(in, "p cnf")) {
				vars = parseInt(in);
				clauses = parseInt(in);
				//initialize graph
				for (int i = 0; i < vars; i++) {
					graph.resize(vars);
				}
				cout << "vars=" << vars << endl << "clauses=" << clauses
						<< endl;
			} else {
				printf(
						"PARSE ERROR in dimacsToHypergraph! Unexpected char: %c\n",
						*in), exit(3);
			}
		} else if (*in == 'c') {
			skipLine(in);
		} else {
			cnt++;
			cout << "reading clauses" << endl;
			lits = readClause(in);
			cout << "size of graph=" << graph.size() << endl;
			for (int i = 0; i < 3; i++) {
				int idx = lits[i];
				if (idx < 0) {
					idx = -1 * idx;
					idx = idx - 1;
				} else {
					idx = idx - 1;
				}
				cout << "pushing count " << cnt << " at position " << idx
						<< endl;
				graph[idx].push_back(cnt);
			}
		}

	}
	cout << "size of graph=" << graph.size() << endl;

	//writing to file
	ofstream outfile;
	outfile.open(out);
	outfile << vars << " " << clauses << endl;
	for (int i = 0; i < graph.size(); i++) {
		for (int j = 0; j < graph[i].size(); j++) {
			printf("%d ", graph[i][j]);
			outfile<<graph[i][j]<<" ";
		}
		printf("\n");
		outfile<<endl;
	}
	outfile.close();
}
#endif /* UTIL_H_ */
