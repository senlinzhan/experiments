#include "utils.hpp"
#include <iostream>
#include <queue>
#include <errno.h>

// the global variable for storing errno code
extern int errno;


// below are some user defined error code
// --------------------------------------

// represents for syntax error
#define ECMDSYNTAX -1

// --------------------------------------



class shell
{
public:
    // read at least one command from the input
    bool read_commands();

    // test if the shell has more commands 
    bool has_command();

    // execute exactly one command
    bool execute_command();

private:
    // return true if the syntax of that command is valid
    // otherwise return false
    bool check_syntax(const std::string &cmd);

    // save commands for executing
    std::queue<std::string> commands_;
};

  
