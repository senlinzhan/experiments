#include "mshell.hpp"
#include <algorithm>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <memory>
#include <cstring>

extern char **environ;

using namespace std;

// read at least one command from the input
bool shell::read_commands()
{ 
    // if the shell has more commands for executing, then we just return 
    if (has_command())
    {
	return true;
    }

    string line, command;
    
    while (getline(cin, line))
    {
        strip(line);
	if (line.empty())
	{
	    continue;
	}
	
	if (ends_with(line, "\\"))
	{
	    line.pop_back();
	    command += line;
	    cout << "> ";
	    cout.flush();
	    continue;
	}
	command += line;
	break;
    }
    
    // check the syntax of that command
    if (!check_syntax(command))
    {
        errno = ECMDSYNTAX;
	return false;
    }

    auto commands = split_tokens(command, ";");
    for (string &cmd: commands)
    {
        strip(cmd);
	if (!cmd.empty())
	{
	    commands_.push(cmd);
	}
    }

    return true;
}  



bool shell::check_syntax(const string &cmd)
{
    // TODO
    return true;
}

bool shell::has_command()
{
    return !commands_.empty();
}

bool shell::execute_command()
{   
    if (commands_.empty())
    {
	return false;
    }

    auto cmd = queue_pop(commands_);
    
    if (cmd == "exit")
    {
        exit(0);
    }

    bool background = false;
    if (cmd.back() == '&')
    {
	background = true;
	cmd.pop_back();
    }

    auto tokens = split_tokens(cmd);
    
    unique_ptr<char *[]> argv{ new char*[tokens.size() + 1] };

    for (decltype(tokens)::size_type i = 0; i < tokens.size(); ++i)
    {
        argv[i] = const_cast<char *>(tokens[i].c_str());
    }
    argv[tokens.size()] = nullptr;

    auto pid = fork();
    if (pid < 0) 
    {
        cerr << "fork() error" << endl;
	exit(1);
    }
    else if(pid == 0)
    {
	auto execute_path = "/bin/" + string(argv[0]);
        if (::execve(execute_path.c_str(), argv.get(), environ) < 0)
	{
	    cerr << argv[0] << ": command not found: " << ::strerror(errno) << endl;
	    exit(1);
	}
    }
    else
    {
        if (background) 
	{
	    cout << pid << " " << cmd << endl;
	}
	else
	{
	    if( waitpid( pid, nullptr, 0 ) < 0 ) 
	    {
	      cerr << "waitpid() error" << endl;
	      exit(1);
	    }               
	}
    }
     
    return true;    
}
