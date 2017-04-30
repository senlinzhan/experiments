#ifndef _UTILS_H_
#define _UTILS_H_


#include <string>
#include <vector>
#include <queue>

// remove leading and trailing whitespace characters from the string
std::string &strip(std::string &str, const std::string &spaces = " \t\n"); 

// returns a vector of all the words in the string, these words are separated by the delimiters
// if return_tokens is true, then we will also return these delimiters in the result
std::vector<std::string> split_tokens(const std::string &str, const std::string &delimiters = " \t\n", bool return_tokens = false);

// return true if str starts with the prefix, otherwise return false
bool starts_with(const std::string &str, const std::string &prefix);

// return true if str ends with the postfix, otherwise return false
bool ends_with(const std::string &str, const std::string &postfix);

// pop and return one element from the queue
template <typename T>
T queue_pop(std::queue<T> &q)
{
     T elem = q.front();
     q.pop();
     return elem;
}


#endif /* _UTILS_H_ */
