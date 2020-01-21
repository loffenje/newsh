#include <unordered_map>
#include <utility>
#include <string>
#include <vector>

#define UNUSED(x) ((void)(x))

namespace newsh {

    typedef int (* func)(std::vector<std::string> args);
   
    /** SHELL FUNCTIONS **/
    namespace sh {
    
    int stub(std::vector<std::string> args);
    
    int man(std::vector<std::string> args);

    int cd(std::vector<std::string> args);
    
    int cat(std::vector<std::string> args);

    }
    /** end **/

    static std::unordered_map<std::string, std::string> manShellCommands {
        {"cd", "change directory to the existing path"},
        {"man", "manual about the given command"},
        {"cat", "write content of file into stdout"}
    };

    static std::unordered_map<std::string, func> shellCommands {
        {"man", sh::man},
        {"cd", sh::cd},
        {"cat", sh::cat}
    };


    std::pair<std::string, func> findCommand(const std::string &name);
    
    std::string getManUsage(const std::string &name);
    
    int dispatchCommand(std::pair<std::string, func> fn, std::vector<std::string> args);
}
