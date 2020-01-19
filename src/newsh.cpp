#include <algorithm>
#include <new>
#include <utility>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>

#include "newsh.h"
#include "command.h"

#define DELIMIT " \r\t\n\a"

namespace newsh {
    

    static int launch(char **args)
    {
        pid_t pid;
        int status;

        pid = fork();
        if (pid == 0) {
            // child
            if (execvp(args[0], args) == -1) {
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
        char **args;
        char *line;
        int status;
        do {
            std::cout << "newsh> ";
            line = readLine();
            args = parseLine(line);
            status = execute(args);

            delete[] args;

        } while(status > 0);
    }
    
    char *readLine()
    {
        char *line = nullptr;
        size_t bufSize = 0;
        getline(&line, &bufSize, stdin);

        return line;
    }
    
    char **parseLine(char *line)
    {
        int bufSize = NLINE;
        int position = 0;
        char **tokens = new (std::nothrow) char *[bufSize * sizeof(char *)];
        if (!tokens) {
            std::cerr << "ERR: allocation of buffer\n";
            exit(EXIT_FAILURE);
        }

        char *token = strtok(line, DELIMIT);
        while (token != NULL) {
            tokens[position++] = token;
            
            if (position >= bufSize) {
                bufSize += NLINE;
                int length = bufSize * sizeof(char *);
                char **buf = new (std::nothrow) char *[length];
                if (!buf) {
                    std::cerr << "ERR: reallocation of buffer\n";
                    delete[] buf;
                    delete[] tokens;
                    exit(EXIT_FAILURE);
                }

                std::copy_n(tokens, length, buf);
                delete[] tokens;
                tokens = buf;
            }

            token = strtok(NULL, DELIMIT);
        }
        
        tokens[position] = NULL;

        return tokens;
    }

    int execute(char **args)
    {

        if (!args[0]) return 1;

        if (args[0] == std::string{"exit"} || isPrefix(args[0], "quit")) {
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
