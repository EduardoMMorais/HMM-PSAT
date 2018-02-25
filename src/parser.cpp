#include <iostream>
#include <algorithm>
#include <cstdlib>
#include "parser.h"

pair<string, string> Parser::getNextToken() {
    string wordtag, word, tag;
    file >> wordtag;
    while (wordtag.size() == 0 && ! file.eof())
        file >> wordtag;
    size_t pos = wordtag.find('/');
    if (pos != string::npos) {
        word.assign(wordtag, 0, pos);
        tag.assign(wordtag, pos+1, wordtag.size());
        if (word == "" || tag == "")
            return getNextToken();
        if (tag == ".")
            status = EOL;
        else
            status = 0;

        // To lower case
        mbtolower(word);
    }
    if (file.eof())
        status = EOF;

    return pair<string,string>(word, tag);
}

void Parser::mbtolower(string &mbstring) const {
    // Convert multibyte to widechar
    wchar_t *wstr = new wchar_t[mbstring.size()+1];
    size_t wlen = mbstowcs(wstr, mbstring.c_str(), mbstring.size()+1);

    //To lower
    transform(wstr, wstr+wlen, wstr, std::towlower);

    // Convert back to multibyte string
    char* mbcstr = new char[mbstring.size() * sizeof(wchar_t) / sizeof(char) + 1];
    wcstombs(mbcstr, wstr, mbstring.size() * sizeof(wchar_t) / sizeof(char) + 1);
    mbstring.assign(mbcstr);

    delete[] wstr;
    delete[] mbcstr;
}
