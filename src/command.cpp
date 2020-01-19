#include <unistd.h>
#include <fstream>

#include "command.h"

namespace newsh {
    
    std::pair<std::string, func> findCommand(const char *name)
    {
        auto fn = shellCommands.find(name);
        if (fn != shellCommands.end()) {
            return *fn;
        }

        return std::make_pair(name, sh::stub);
    }

    const char *getManUsage(const char *name)
    {
        auto fn = manShellCommands.find(name);
        if (fn != manShellCommands.end()) {
            return (*fn).second;
        }
        
        return "Not found\n";
    }

    int dispatchCommand(std::pair<std::string, func> fn, char **args)
    {
        func executeShellCommand = fn.second;
        
        return executeShellCommand(args);
    }


    /** SHELL FUNCTIONS **/
    namespace sh { 
    
    int man(char **args)
    {
        if (!args[1]) {
            std::cerr << "expected argument to man\n";
            return 1;
        }
        
        std::cout << getManUsage(args[1]) << '\n';

        return 1;
    }
   
    int cd(char **args)
    {
        if (!args[1]) {
            std::cerr << "expected argument to cd\n";
            return 1;
        }
        
        if (chdir(args[1]) != 0) {
            perror("newsh cd");
        }

        return 1;
    }

    int cat(char **args)
    {
        if (!args[1]) {
            std::cerr << "expected argument to cat\n";
            return 1;
        }

        std::string name = args[1];
        std::ifstream file(name, std::ios::binary);
        std::string line;
        while (std::getline(file, line, '\n')) {
            std::cout << line << '\n';
        }
        
        return 1;
    }

    int stub(char **args)
    { 

#if 1
        UNUSED(args); 
#else
        std::cout << args[0] << " undefined, find help\n";
#endif

        return 0;
    }

    }

    /** end **/
}
