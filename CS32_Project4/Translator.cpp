#include "provided.h"
#include "MyHash.h"
#include <string>
using namespace std;

class TranslatorImpl
{
public:
    bool pushMapping(string ciphertext, string plaintext);
    bool popMapping();
    string getTranslation(const string& ciphertext) const;
private:
	char plain[26] = { '?','?','?','?','?','?','?','?','?','?','?','?','?','?','?','?','?','?','?','?','?','?','?','?','?','?' };
	MyHash<int, string> pushOrder;
	int numPushed = 0;
};

bool TranslatorImpl::pushMapping(string ciphertext, string plaintext)
{
	/*
	if (ciphertext.size() != plaintext.size()) // and many other constraints <--------------------OOO
		return false;
	for (int i = 0; i < (int)plaintext.size(); i++) {
		if (plaintext[i] != '?') {
			for (int j = 0; j < 26; j++) {
				if (plaintext[i] == plain[j]) {
					if (ciphertext[i] - 97 != j)
						return false;
				}
				if (ciphertext[i] - 97 != j) {
					if (plaintext[i] != plain[j])
						return false;
				}
			}
		}
	}
	for (int i = 0; i < (int)ciphertext.size(); i++) {
		plain[ciphertext[i] - 97] = plaintext[i];
	}
	return true;
	*/
	//==================================
	bool allLettersValid = true;
	if (ciphertext.size() != plaintext.size()) // and many other constraints <--------------------OOO
		return false;
	for (int i = 0; i < (int)ciphertext.size(); i++) {
		if (plain[ciphertext[i] - 97] != '?') { // ciphertext[i] already mapped to some letter
			if (plain[ciphertext[i] - 97] != plaintext[i]) // The some letter is not consistent with plaintext. Wrong potential word
				return false;
		}
		else if (plain[ciphertext[i] - 97] == '?') { //ciphertext[i] not mapped to anything
			bool canPush = true;
			for (int j = 0; j < 26; j++) {
				if (plaintext[i] == plain[j]) {  // already mapped/appeared in plain[]
					canPush = false;
					return false;
				}
			}
		}
	}
	for (int i = 0; i < (int)ciphertext.size(); i++) {
		for (int j = 0; j < 26; j++) {
			plain[ciphertext[i] - 97] = plaintext[i];
		}
	}
	numPushed++;
	pushOrder.associate(numPushed, ciphertext);
	return true;
}

bool TranslatorImpl::popMapping()
{ 
	if (pushOrder.getNumItems() == 0)
		return false;
	string lettersToRemove = *pushOrder.find(numPushed);
	for (int i = 0; i < (int)lettersToRemove.size(); i++) {

	}
}

string TranslatorImpl::getTranslation(const string& ciphertext) const
{
	string translated = "";
	for (int i = 0; i < (int)ciphertext.size(); i++) {
		translated = translated + "?";
		if (plain[ciphertext[i] - 97] != '?') {
			translated.pop_back();
			translated = translated + plain[ciphertext[i] - 97];
		}
	}
	return translated;
}

//******************** Translator functions ************************************


// These functions simply delegate to TranslatorImpl's functions.
// You probably don't want to change any of this code.

Translator::Translator()
{
    m_impl = new TranslatorImpl;
}

Translator::~Translator()
{
    delete m_impl;
}

bool Translator::pushMapping(string ciphertext, string plaintext)
{
    return m_impl->pushMapping(ciphertext, plaintext);
}

bool Translator::popMapping()
{
    return m_impl->popMapping();
}

string Translator::getTranslation(const string& ciphertext) const
{
    return m_impl->getTranslation(ciphertext);
}
