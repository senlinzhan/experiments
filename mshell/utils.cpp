#include "utils.hpp"
#include <algorithm>
#include <unordered_set>
#include <iostream>

using namespace std;

// return true if elem is in container, otherwise return false
// the container type may be set, unordered_set, map or unordered_map
template <typename Container, typename T>
bool set_contains(const Container &container, const T &elem)
{
    return container.find(elem) != container.cend();
}

// remove leading and trailing whitespace characters from the string
string &strip(string &str, const string &spaces)
{
    const unordered_set<char> white_spaces(spaces.cbegin(), spaces.cend());

    auto not_spaces = [&white_spaces](char c) {
        return !set_contains(white_spaces, c);
    };
    
    str.erase(str.begin(), find_if(str.begin(), str.end(), not_spaces));
    str.erase(find_if(str.rbegin(), str.rend(), not_spaces).base(), str.end());
    return str;
}

// returns a vector of all the words in the string, these words are separated by the delimiters
// if return_tokens is true, then we will also return these delimiters in the result
vector<string> split_tokens(const string &str, const string &delimiters, bool return_tokens)
{
    vector<string> tokens;
    const unordered_set<char> delims(delimiters.cbegin(), delimiters.cend());
    
    auto is_delimiter = [&delims](char c) {
        return set_contains(delims, c);
    };

    auto first = str.cbegin();
    decltype(first) last;
    
    while ((last = find_if(first, str.cend(), is_delimiter)) != str.cend())
    {
        // store the non-empty token
        if (first != last)
	{
	    tokens.push_back(string(first, last));	    
	}
	
	while (last != str.cend() && is_delimiter(*last)) 
	{
	    // store the token if we need
	    if (return_tokens)
	    {
	        tokens.push_back(string(last, last + 1));
	    }
	    ++last;
	}
	first = last;	
    }
    
    if (first != last) 
    {
        tokens.push_back(string(first, last));
    }
    
    return tokens;
}

// return true if str starts with the prefix, otherwise return false
bool starts_with(const string &str, const string &prefix)
{
    auto first = str.cbegin(), second = prefix.cbegin(); 
    for (; first != str.cend() && second != prefix.cend() && *first == *second; ++first, ++second)
    {
    }
    
    return second == prefix.cend();
}


// return true if str ends with the postfix, otherwise return false
bool ends_with(const string &str, const string &postfix)
{
    auto first = str.crbegin(), second = postfix.crbegin(); 
    for (; first != str.crend() && second != postfix.crend() && *first == *second; ++first, ++second)
    {
    }
    
    return second == postfix.crend();    
}
