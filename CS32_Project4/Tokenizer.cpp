#include "provided.h"
#include <string>
#include <vector>
using namespace std;

class TokenizerImpl
{
public:
	TokenizerImpl(string separators);
    vector<string> tokenize(const std::string& s) const;
private:
	string sep;
};

TokenizerImpl::TokenizerImpl(string separators)
{
	sep = separators;
}

vector<string> TokenizerImpl::tokenize(const std::string& s) const
{
	string temp = "";
	vector<string> items;
	if (s.size() == 0)
		return items;
	for (int i = 0; i < s.size(); i++) {
		temp = temp + s[i];
		for (int j = 0; j < sep.size(); j++) {
			if (s[i] == sep[j] && temp.size() > 0) {
				if (i == s.size() - 1)
					return items;
				temp.pop_back();
				if (temp.size() > 0)
					items.push_back(temp);
				temp = "";
			}
		}
	}
	items.push_back(temp);
    return items;  // This compiles, but may not be correct
}

//******************** Tokenizer functions ************************************

// These functions simply delegate to TokenizerImpl's functions.
// You probably don't want to change any of this code.

Tokenizer::Tokenizer(string separators)
{
    m_impl = new TokenizerImpl(separators);
}

Tokenizer::~Tokenizer()
{
    delete m_impl;
}

vector<string> Tokenizer::tokenize(const std::string& s) const
{
    return m_impl->tokenize(s);
}
