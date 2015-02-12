#include "Constants.h"
#include "Trie.h"
#include "http.h"
#include "HSolver.h"
#include <iostream>

using namespace std;


void testTrie() {
	Trie* trie = new Trie();
	trie->insertFromFile(DICT);
	unordered_set<char> emptyCharSet;
	unordered_set<string> emptyStrSet;
	unordered_map<char, int> resultMap;
	trie->regexMatch("____e", emptyCharSet, resultMap, emptyStrSet);
	delete trie;
}

void testAlmostCompleteWord() {
	Trie* trie = new Trie();
	trie->insertFromFile(DICT);
	unordered_set<char> emptyCharSet;
	unordered_set<string> emptyStrSet;
	unordered_map<char, int> resultMap;
	trie->regexMatch("fri__d", emptyCharSet, resultMap, emptyStrSet);
	delete trie;
}

void testSolver() {
	Trie* trie = new Trie();
	trie->insertFromFile(DICT);
	HSolver* solver = new HSolver(trie);
	solver->readStr("h__ are __u");
	cout<<solver->guessChar()<<endl;
	delete trie;
	delete solver;
}

void oneGame(Trie* inputTrie) {
	Trie* trie = inputTrie;
	HSolver* solver = new HSolver(trie);
	string origStr="", gameStr="";
	cout<<endl<<"<<New Game>>"<<endl;
	while(origStr=="") {
		cout<<"Input String: ";
		getline(cin, origStr);
	}
	for(int i=0; i<origStr.size(); i++) {
		if(isValidAlphabet(origStr[i]))
			gameStr += '_';
		else if(origStr[i]==' ')
			gameStr += ' ';
	}
	solver->readStr(gameStr);
	int missed = 0;
	while(gameStr!=origStr) {
		cout<<"Current Status: "<<gameStr<<endl;
		cout<<"The Number of Wrong Guesses sofar: "<<missed<<endl;
		string ask;
		cout<<"Press Enter to continue or Type Q to quit"<<endl;
		getline(cin, ask);
		if(ask=="Q") {
			cout<<"Quit"<<endl;
			delete solver;
			delete inputTrie;
			exit(0);
		} else {
			cout<<"Processing..."<<endl;
			char guessed = solver->guessChar();
			cout<<"Guessed Alphabet: "<<guessed<<endl;
			bool found = false;
			for(int i=0; i<origStr.size(); i++) {
				if(origStr[i] == guessed) {
					gameStr[i] = guessed;
					found = true;
				}
			}
			if(!found) {
				cout<<"Wrong Guess!"<<endl;
				++missed;
			} else {
				cout<<"Correct Guess!"<<endl;
			}
			solver->readStr(gameStr);
		}
	}
	cout<<"Current Status: "<<gameStr<<endl;
	cout<<"Congrats!!"<<endl;
	delete solver;
}

void simulateGame() {
	Trie* trie = new Trie();
	trie->insertFromFile(DICT);
	while(true) {
		oneGame(trie);
	}
}

int main() {
	simulateGame();
}
