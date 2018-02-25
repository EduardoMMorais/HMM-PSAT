#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED
#include <cstdlib>
#include <string>
#include <iostream>
#include <sstream>

#include <vector>
#include <map>
#include <set>

#define VERSION 0.02

#define EPSILON 1.0e-12


using namespace std;

typedef string Pos;

template <class T>
ostream & operator<<(ostream &out, const vector<T> &vec) {
	int w = out.width();
    for( unsigned int i=0; i<vec.size(); i++ )
        out << setw(w) << left << vec.at(i) << " ";
    return out;
}

template <class T,class S>
ostream & operator<<(ostream &out, pair<T,S> &x) {
    out << "<" << x.first << "," << x.second << ">";
    return out;
}

template <class T,class S>
ostream & operator<<(ostream &out, map<T,S> &m)
{
    class map<T,S>::iterator it;
    for( it=m.begin(); it != m.end(); it++ )
        out << *it << " ";
    return out;
}

template <class T>
unsigned manhattanDistance( const vector<T>& v1, const vector<T>& v2 )
{
	unsigned s = v1.size();
	if( s != v2.size() )
		return -1;

	unsigned dist=0;
	for( unsigned i=0; i<s; ++i )
		if( v1[i] != v2[i] )
			++dist;
	return dist;
}



//-------------------------------------------
// Verbosity treatment
//-------------------------------------------
const int VERBOSITY_THRESHOLD = 3;
inline ostream& verb( int x )
	{ return (x >= VERBOSITY_THRESHOLD)? cout : nocout;}


#endif // UTIL_H_INCLUDED
