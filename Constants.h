#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <iostream>

#ifdef _WIN32
	#include <unordered_map>
#else
	#include <tr1/unordered_map>
#endif
#include <string>
using namespace std;
using namespace std::tr1;
	
	const char* DICT = "small_dictionary.txt";
	const char CHARS_IN_FREQ_ORDER[] = {'e', 's', 'i', 'a', 'r', 'n', 't', 'o', 'l', 'c', 'd', 'u', 'p', 'm', 'g', 'h', 'b', 'y', 'f', 'v', 'k', 'w', 'z', 'x', 'q', 'j'};

	typedef struct Node {
		Node() {
			isWord = false;
		}
		unordered_map<char, Node*> children;
		bool isWord;
	} Node;

	bool isValidAlphabet(char c) {
		if((c>='a' && c<='z') || (c>='A' && c<='Z'))
			return true;
		else
			return false;
	}

	int getPriority(char c) {
		for(int i=0; i<26; i++) {
			if(CHARS_IN_FREQ_ORDER[i] == c)
				return i;
		}
		return 26;
	}

	string toLowerCase(string s) {
		string result = "";
		for(int i=0; i<s.size(); i++) {
			if(s[i]>='A' && s[i]<='Z')
				result += s[i] - ('A'-'a');
			else
				result += s[i];
		}
		return result;
	}
#endif
