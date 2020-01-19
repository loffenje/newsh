#ifndef NEWSH_H
#define NEWSH_H

#include <string>
#include <vector>

namespace newsh {

    constexpr int NLINE = 512;

    void runShell();
    
    char *readLine();
    
    char **parseLine(char *line);

    int execute(char **args);
}



#endif
