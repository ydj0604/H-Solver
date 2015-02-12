#ifndef HSOLVER_H
#define HSOLVER_H

#include "Trie.h"
#include "Constants.h"
#include <vector>
#include <string>

#ifdef _WIN32
	#include <unordered_map>
#else
	#include <tr1/unordered_map>
#endif

using namespace std;
using namespace std::tr1;

class HSolver {
public:
	HSolver(Trie* trie) {
		m_trie = trie;
	}

	void readStr(const string& s) {
		resetWords();
		int wordBegin = 0;
		int numUnknownChars = 0;
		for(int i=0; i<s.size(); ++i) {
			if(s[i]==' ') {
				string currWord = s.substr(wordBegin, i-wordBegin);
				if(currWord != "") {
					m_words.push_back(currWord);
					m_words_unknown.push_back(numUnknownChars);
				}
				wordBegin = i+1;
				numUnknownChars = 0;
			} else if(i==s.size()-1) {
				string lastWord = s.substr(wordBegin, i-wordBegin+1);
				if(lastWord != "") {
					m_words.push_back(lastWord);
					if(s[i] == '_')
						++numUnknownChars;
					m_words_unknown.push_back(numUnknownChars);
				}
			} else {
				if(s[i] == '_')
					++numUnknownChars;
			}
		}
	}

	char guessChar() {
		if(m_words.size()==0)
			return ' '; //invalid case

		//take a random guess for the first time
		if(m_guessedChars.size() == 0) {
			char randomChar = getRandomChar();
			m_guessedChars.insert(randomChar); //record the alphabet to be guessed
			return randomChar;
		}

		//given the input string, calculate the frequency of each character using a trie and store it in the map
		unordered_map<char, int> alphabetToCountMap;
		unordered_set<string> matchedWordSet; //used to avoid counting letters from the same word
		for(int i=0; i<m_words.size(); ++i) {
			if(m_words_unknown[i]==0)
				continue;
			char strongGuess = m_trie->regexMatch(m_words[i], m_guessedChars, alphabetToCountMap, matchedWordSet);
			if(isValidAlphabet(strongGuess) && m_guessedChars.count(strongGuess)==0) { //the word found is a strong candidate
				m_guessedChars.insert(strongGuess);
				return strongGuess;
			}
		}

		//if there is no alphabet found, use a random guess based on general frequency order
		if(alphabetToCountMap.size() == 0) {
			char randomChar = getRandomChar();
			m_guessedChars.insert(randomChar); //record the alphabet to be guessed
			return randomChar;
		}

		//given the alphabet distribution, find the most frequent character
		unordered_map<char, int>::iterator itr = alphabetToCountMap.begin();
		char mostFreqChar = itr->first;
		while(itr != alphabetToCountMap.end()) {
			if(itr->second > alphabetToCountMap[mostFreqChar]) {
				mostFreqChar = itr->first;
			} else if(itr->second == alphabetToCountMap[mostFreqChar]) { //if it's a tie, use the general ranking
				int p1 = getPriority(itr->first), p2 = getPriority(mostFreqChar);
				mostFreqChar = p1<p2? itr->first: mostFreqChar;
			}
			++itr;
		}

		if(m_guessedChars.count(mostFreqChar) > 0) { //should never reach here; this is an error
			char tempChar = getRandomChar();
			m_guessedChars.insert(tempChar);
			cout<<"error case: "<<mostFreqChar<<" replaced with "<<tempChar<<endl;
			return tempChar;
		}
		m_guessedChars.insert(mostFreqChar); //record the alphabet to be guessed
		return mostFreqChar;
	}

	char getRandomChar() {
		for(int i=0; i<26; i++) {
			if(m_guessedChars.count(CHARS_IN_FREQ_ORDER[i])==0) {
				return CHARS_IN_FREQ_ORDER[i];
			}
		}
		return ' ';
	}

	void resetGuessedWords() {
		m_guessedChars.clear();
	}

	void resetWords() {
		m_words.clear();
		m_words_unknown.clear();
	}

private:
	vector<string> m_words; //stores all the words
	vector<int> m_words_unknown; //stores the number of unknown characters for each word
	unordered_set<char> m_guessedChars;
	Trie* m_trie;
};

#endif
