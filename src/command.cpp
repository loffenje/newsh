#include <unistd.h>
#include <fstream>
#include <iostream>

#include "command.h"

namespace newsh {
    
    std::pair<std::string, func> findCommand(const std::string &name)
    {
        auto fn = shellCommands.find(name);
        if (fn != shellCommands.end()) {
            return *fn;
        }

        return std::make_pair(name, sh::stub);
    }

    std::string getManUsage(const std::string &name)
    {
        auto fn = manShellCommands.find(name);
        if (fn != manShellCommands.end()) {
            return (*fn).second;
        }
        
        return "Not found\n";
    }

    int dispatchCommand(std::pair<std::string, func> fn, std::vector<std::string> args)
    {
        func executeShellCommand = fn.second;
        
        return executeShellCommand(args);
    }


    /** SHELL FUNCTIONS **/
    namespace sh { 
    
    int man(std::vector<std::string> args)
    {
        if (args.size() < 2) {
            std::cerr << "expected argument to man\n";
            return 1;
        }
        
        std::cout << getManUsage(args[1]) << '\n';

        return 1;
    }
   
    int cd(std::vector<std::string> args)
    {
        if (args.size() < 2) {
            std::cerr << "expected argument to cd\n";
            return 1;
        }
        
        const char *dir = args[1].c_str();
        if (chdir(dir) != 0) {
            perror("newsh cd");
        }

        return 1;
    }

    int cat(std::vector<std::string> args)
    {
        if (args.size() < 2) {
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

    int stub(std::vector<std::string> args)
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
