#ifndef TRIE_H
#define TRIE_H

#include "Constants.h"
#ifdef _WIN32
	#include <unordered_map>
	#include <unordered_set>
#else
	#include <tr1/unordered_map>
	#include <tr1/unordered_set>
#endif
#include <fstream>
#include <string>
#include <iostream>
#include <stdlib.h>

using namespace std;
using namespace std::tr1;

class Trie { //stores words in lower-case
public:
	Trie() {
		m_root = new Node();
		m_size = 0;
	}

	~Trie() {
		clearTrie(m_root);
	}

	void clearTrie(Node* n) {
		if(n==NULL)
			return;
		unordered_map<char, Node*>::iterator itr = n->children.begin();
		while(itr != n->children.end()) {
			clearTrie(itr->second);
			++itr;
		}
		delete n;
	}

	void insertFromFile(const char* fileName) {
		ifstream dictFile;
		dictFile.open(fileName);
		if(!dictFile.is_open())
			exit(1);
		string currWord;
		while(getline(dictFile, currWord)) {
			if(currWord != "")
				insertWord(currWord);
		}
		cout<<"Trie Dictionary Size: "<<m_size<<" words"<<endl;
		dictFile.close();
	}

	void insertWord(string word) {
		Node* curr = m_root;
		word = toLowerCase(word);
		for(int i=0; i<word.size(); ++i) {
			if(curr->children.count(word[i])>0) {
				curr = curr->children[word[i]];
			} else {
				(curr->children)[word[i]] = new Node();
				curr = curr->children[word[i]];
			}
		}
		curr->isWord = true; //mark that it's a word
		++m_size;
	}

	char regexMatch(string word, const unordered_set<char>& guessedCharSet, unordered_map<char, int>& charToCountMap, unordered_set<string>& matchedWordSet) { //stores alphabet distribution in charToCountMap
		if(m_root == NULL)
			return ' ';
		unordered_set<char> emptyCharSet;
		string emptyStr = "";
		int numFoundWord = 0;
		char strongGuess = ' ';
		regexMatchDFS(m_root, word, emptyStr, emptyCharSet, guessedCharSet, charToCountMap, matchedWordSet, numFoundWord, strongGuess);
		if(numFoundWord == 1 && strongGuess != ' ')
			return strongGuess;
		else
			return ' ';
	}

	void regexMatchDFS(Node* n, string& origWord, string& currWord, unordered_set<char>& insertedCharSet, const unordered_set<char>& guessedCharSet, unordered_map<char, int>& charToCountMap, unordered_set<string>& matchedWordSet, int& numFoundWord, char& strongGuess) {
		if(origWord.size()==0) { //base case
			if(n->isWord && matchedWordSet.count(currWord)==0) { //a word satisfying the given regex is found; if the word has been found before, skip it
				//cout<<currWord<<":"<<endl;
				unordered_set<char>::iterator itr = insertedCharSet.begin();
				while(itr != insertedCharSet.end()) {
					//cout<<" "<<*itr;
					if(charToCountMap.count(*itr) > 0) {
						charToCountMap[*itr] ++;
					} else {
						charToCountMap[*itr] = 1;
					}
					++itr;
				}
				//cout<<endl;
				++numFoundWord;
				strongGuess = *(insertedCharSet.begin());
				matchedWordSet.insert(currWord);
			}
			return;
		}

		if(origWord[0]=='_') { //if it's '_', search exhaustively
			unordered_map<char, Node*>::iterator itr = n->children.begin();
			while(itr != n->children.end()) {
				if(guessedCharSet.count(itr->first) > 0) {//to avoid using the already guessed character to make a guess for an unknown character
					++itr;
					continue;
				} else if(!isValidAlphabet(itr->first)) { //skip invalid characters
					++itr;
					continue;
				}
				origWord = origWord.substr(1);
				currWord = currWord += itr->first;
				bool alreadyExists = insertedCharSet.count(itr->first)>0? true: false;
				if(!alreadyExists)
					insertedCharSet.insert(itr->first);
				regexMatchDFS(itr->second, origWord, currWord, insertedCharSet, guessedCharSet, charToCountMap, matchedWordSet, numFoundWord, strongGuess);
				origWord = '_' + origWord;
				currWord = currWord.substr(0, currWord.size()-1);
				if(!alreadyExists)
					insertedCharSet.erase(itr->first);
				++itr;
			}
		} else { //if it's an existing alphabet, traverse down the trie
			if(n->children.count(origWord[0]) > 0) {
				char nextChar = origWord[0];
				origWord = origWord.substr(1);
				currWord += nextChar;
				regexMatchDFS(n->children[nextChar], origWord, currWord, insertedCharSet, guessedCharSet, charToCountMap, matchedWordSet, numFoundWord, strongGuess);
				origWord = nextChar + origWord;
				currWord = currWord.substr(0, currWord.size()-1);
			}
		}
	}

private:
	Node* m_root;
	int m_size;
};

#endif
