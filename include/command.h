#include <unordered_map>
#include <utility>
#include <iostream>
#include <string>

#define UNUSED(x) ((void)(x))

namespace newsh {

    typedef int (* func)(char **);
   
    /** SHELL FUNCTIONS **/
    
    namespace sh {
    
    int stub(char **args);
    
    int man(char **args);

    int cd(char **args);
    
    int cat(char **args);

    }
    /** end **/

    static std::unordered_map<std::string, const char *> manShellCommands {
        {"cd", "change directory to the existing path"},
        {"man", "manual about the given command"},
        {"cat", "write content of file into stdout"}
    };

    static std::unordered_map<std::string, func> shellCommands {
        {"man", sh::man},
        {"cd", sh::cd},
        {"cat", sh::cat}
    };


    std::pair<std::string, func> findCommand(const char *name);
    
    const char *getManUsage(const char *name);
    
    int dispatchCommand(std::pair<std::string, func> fn, char **args);
}
