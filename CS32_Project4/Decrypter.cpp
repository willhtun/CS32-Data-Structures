#include "provided.h"
#include <string>
#include <vector>
using namespace std;

class DecrypterImpl
{
public:
    bool load(string filename);
    vector<string> crack(const string& ciphertext);
private:
	void crackHelper(Translator &translate);
	WordList validWords;
	vector<string> cipheredWords;
	vector<string> correctMessages;
	vector<string> alreadyChosenWords;
};

bool DecrypterImpl::load(string filename)
{
	return validWords.loadWordList(filename);
}

vector<string> DecrypterImpl::crack(const string& ciphertext)
{ 
	Translator translate;
	Tokenizer t(" ,.!");
	cipheredWords = t.tokenize(ciphertext);
	/*
	for (int i = 0; i < (int)cipheredWords.size(); i++) {
		possibleMessage.push_back(string(cipheredWords[i].size(), '?'));
	}
	*/

	crackHelper(translate);

	return correctMessages;
}

void DecrypterImpl::crackHelper(Translator &translate) {
	// Step 2 ================================================
	vector<string> possibleWords;
	vector<string> possibleMessage;
	string currentTranslation;
	string current;

	int numOfQ = 0;
	string cipherWord_trans;
	for (int i = 0; i < (int)cipheredWords.size(); i++) {
		cipherWord_trans = translate.getTranslation(cipheredWords[i]);
		int newNumOfQ = 0;
		for (int j = 0; j < (int)cipheredWords[i].size(); j++) {
			if (cipherWord_trans[j] == '?')
				newNumOfQ++;
		}
		if (newNumOfQ > numOfQ) {
			bool canInsert = true;
			for (int j = 0; j < (int)alreadyChosenWords.size(); j++) {
				if (cipheredWords[i] == alreadyChosenWords[j]) {
					canInsert = false;
				}
			}
			if (canInsert) {
				current = cipheredWords[i];
				numOfQ = newNumOfQ; // R28K22AA83D
			}
		}
	}

	alreadyChosenWords.push_back(current);

	for (int i = 0; i < (int)cipheredWords.size(); i++) {
		possibleMessage.push_back(translate.getTranslation(cipheredWords[i]));
	}

	// Step 3
	currentTranslation = translate.getTranslation(current); // ?????sd??d
	
	// Step 4
	possibleWords = validWords.findCandidates(current, currentTranslation);

	// Step 5
	if ((int)possibleWords.size() == 0) {
		alreadyChosenWords.pop_back();
		return; // go back to previous recursive call
	}
	//=============================================================
	// Step 6
	for (int x = 0; x < (int)possibleWords.size(); x++) {
		// 6a
		Translator temp_translate;
		for (int i = 0; i < (int)cipheredWords.size(); i++) {
			temp_translate.pushMapping(cipheredWords[i], translate.getTranslation(cipheredWords[i]));
		}
		if (!temp_translate.pushMapping(current, possibleWords[x])) {
			alreadyChosenWords.pop_back();
			return;
		}
		//temp_translate.pushMapping(current, possibleWords[x]);
		// 6b
		for (int i = 0; i < (int)cipheredWords.size(); i++) {
			possibleMessage[i] = temp_translate.getTranslation(cipheredWords[i]);
			//possibleMessage.push_back(temp_translate.getTranslation(cipheredWords[i]));
		}
		// 6c
		bool hasQuestionMark = false;
		bool allWordsValid = true;
		for (int i = 0; i < (int)possibleMessage.size(); i++) {
			for (int j = 0; j < (int)possibleMessage[i].size(); j++) {
				if (possibleMessage[i][j] == '?') {
					hasQuestionMark = true;
				}
			}
			if (!hasQuestionMark) {
				if (!validWords.contains(possibleMessage[i])) {
					allWordsValid = false;
				}
			}
		}
		if (!allWordsValid) {
			// 6a
			alreadyChosenWords.pop_back();
			return;
		}
		else if (hasQuestionMark) {
			// 6b

			crackHelper(temp_translate);
		}
		else {
			// 6c
			string msg;
			for (int i = 0; i < (int)possibleMessage.size(); i++) {
				msg = msg + possibleMessage[i];
				if (i != (int)possibleMessage.size() - 1) // last word
					msg = msg + " ";
			}
			correctMessages.push_back(msg);
			alreadyChosenWords.pop_back();
			return;
		}
	}
	return;
}

//******************** Decrypter functions ************************************

// These functions simply delegate to DecrypterImpl's functions.
// You probably don't want to change any of this code.

Decrypter::Decrypter()
{
    m_impl = new DecrypterImpl;
}

Decrypter::~Decrypter()
{
    delete m_impl;
}

bool Decrypter::load(string filename)
{
    return m_impl->load(filename);
}

vector<string> Decrypter::crack(const string& ciphertext)
{
   return m_impl->crack(ciphertext);
}
