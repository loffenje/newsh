#include <algorithm>
#include <utility>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <string.h>

#include "newsh.h"
#include "command.h"

#define DELIMIT " \r\t\n\a"

namespace newsh {
    
    static int launch(std::vector<std::string> args)
    {
        pid_t pid;
        int status;
        
        if (args.empty()) return 1;
        
        std::vector<char *> cArgs;
        cArgs.reserve(args.size() + 1);

        std::transform(args.begin(), args.end(), std::back_inserter(cArgs), 
                        [](const std::string &s) { return (char *)s.c_str(); }); //some issue with data() method
        
        cArgs.push_back(nullptr);

        char *cmd = cArgs[0];
        pid = fork();
        if (pid == 0) {
            // child
            if (execvp(cmd, cArgs.data()) == -1) {
                perror("sh");
            }

            exit(EXIT_FAILURE);
        } else if (pid < 0) {
            perror("sh");
        } else {
            do {
                waitpid(pid, &status, WUNTRACED);
            } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        }

        return 1;
    }

    static bool isPrefix(const std::string &src, const std::string &cmp)
    {
        if (src.size() > cmp.size()) return false;

        return std::equal(src.begin(), src.end(), cmp.begin());
    }

    void runShell() 
    {
        std::vector<std::string> args;
        std::string line;
        int status;
        do {
            std::cout << "newsh> ";
            line = readLine();
            args = parseLine(line);
            status = execute(args);
        } while(status > 0);
    }
    
    std::string readLine()
    {
        std::string line;

        std::getline(std::cin, line);
        return line;
    }
    
    std::vector<std::string> parseLine(std::string line)
    {
        std::vector<std::string> tokens;
        std::string delimits = DELIMIT;
        size_t pos1, pos2 = 0;
        for (;;) {
            pos1 = line.find_first_not_of(delimits, pos2);
            if (pos1 == std::string::npos) break; // if empty or all delimits

            pos2 = line.find_first_of(delimits, pos1 + 1);
            if (pos2 != std::string::npos) {
                tokens.push_back(line.substr(pos1, pos2 - pos1));
            } else {
                tokens.push_back(line.substr(pos1));
                break;
            }
        }

        return tokens;
    }

    int execute(std::vector<std::string> args)
    {
        if (args.size() < 1) return 1;

        if (args[0] == "exit" || isPrefix(args[0], "quit")) {
            return 0;
        }

        std::pair<std::string, func> cmd = findCommand(args[0]);
        int status = dispatchCommand(cmd, args);
        if (!status) {
            return launch(args); 
        }
        

        return status;
    }
}
