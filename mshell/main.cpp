#include <iostream>
#include "mshell.hpp"
using namespace std;

int main()
{
    shell mshell;
    
    while (true)
    {
      if (!mshell.read_commands())
	{
	    switch(errno) {
	    case ECMDSYNTAX:
		break;
	    default:
		cerr << "error" << endl;
		break;
	    }
	    continue;
	}
	

	if (!mshell.execute_command())
	{
	}
    }


    return 0;
}

