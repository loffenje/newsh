#ifndef NEWSH_H
#define NEWSH_H

#include <string>
#include <vector>

namespace newsh {

    constexpr int NLINE = 512;

    void runShell();
    
    std::string readLine();
    
    std::vector<std::string> parseLine(std::string line);

    int execute(std::vector<std::string> args);
}



#endif
