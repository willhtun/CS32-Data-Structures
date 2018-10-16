#include "provided.h"
#include "MyHash.h"
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
using namespace std;

class WordListImpl
{
public:
    bool loadWordList(string filename);
    bool contains(string word) const;
    vector<string> findCandidates(string cipherWord, string currTranslation) const;
private:
	string getPattern(string &word) const {
		MyHash<string, string> usedChar;
		string temp = "";
		char latestChar = 'A';
		for (int i = 0; i < (int)word.size(); i++) {
			if (usedChar.find({ word[i] }) != nullptr) {
				temp += *(usedChar.find({ word[i] }));
			}
			else { // ==nullptr
				temp += latestChar;
				usedChar.associate({ word[i] }, { latestChar });
				latestChar++;
			}
		}
		return temp;
	}
	MyHash<string, vector<string>> wordMap;
};

bool WordListImpl::loadWordList(string filename)
{
	ifstream infile(filename);
	if (!infile) {
		return false;
	}
	string s;
	int debug_count = 0;
	while (infile >> s) {
		//getPattern(string)
		//usedChar.reset();
		vector<string> newLoad;
		if (wordMap.find(getPattern(s)) != nullptr)
			newLoad = *wordMap.find(getPattern(s));
		newLoad.push_back(s);
		wordMap.associate(getPattern(s), newLoad);
		debug_count++;
	}
	return true;
}

bool WordListImpl::contains(string word) const
{
	vector<string> group = *wordMap.find(getPattern(word));
	if ((find(group.begin(), group.end(), word)) != group.end())
		return true;
	return false;
}

vector<string> WordListImpl::findCandidates(string cipherWord, string currTranslation) const
{ 
	vector<string> possibleWords;
	if (cipherWord.size() != currTranslation.size())
		return vector<string>();

	vector<string> allPossibleWords = *wordMap.find(getPattern(cipherWord));
	vector<string>::iterator it;
	it = allPossibleWords.begin();
	while (it != allPossibleWords.end()) {
		bool match = true;
		for (int i = 0; i < (int)(*it).size(); i++) {
			if (cipherWord[i] == '?' && cipherWord[i] == '\'')
				return vector<string>();
			if (currTranslation[i] != '?' && currTranslation[i] != '\'') {
				if ((*it)[i] != currTranslation[i]) {
					match = false;
					i = (int)(*it).size();
				}
			}
			else if (currTranslation[i] == '\'') {
				if (cipherWord[i] != '\'')
					return vector<string>();
			}
			
		}
		if (match)
			possibleWords.push_back(*it);
		it++;
	}
	return possibleWords;
}

//***** hash functions for string, int, and char *****

unsigned int hasher(const std::string& s)
{
    return std::hash<std::string>()(s);
}

unsigned int hasher(const int& i)
{
    return std::hash<int>()(i);
}

unsigned int hash(const char& c)
{
    return std::hash<char>()(c);
}

//******************** WordList functions ************************************

// These functions simply delegate to WordListImpl's functions.
// You probably don't want to change any of this code.

WordList::WordList()
{
    m_impl = new WordListImpl;
}

WordList::~WordList()
{
    delete m_impl;
}

bool WordList::loadWordList(string filename)
{
    return m_impl->loadWordList(filename);
}

bool WordList::contains(string word) const
{
    return m_impl->contains(word);
}

vector<string> WordList::findCandidates(string cipherWord, string currTranslation) const
{
   return m_impl->findCandidates(cipherWord, currTranslation);
}

//=====================================================================
