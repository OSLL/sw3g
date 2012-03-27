#ifndef UTIL_H
#define UTIL_H

/**
  * @file util.h
  * Various utility methods
  */

#include <string>
#include <iostream>
#include <stdio.h>

/**
  * @brief Executes @p cmd and returns its @b stdout as string.
  * @param cmd - the system command to execute
  * @return command result, or "ERROR" if could not execute the
  * command
  */
std::string exec(const char* cmd) {
    const int BUFFER_SIZE = 128;

    FILE* pipe = popen(cmd, "r");
    if (!pipe) return "ERROR";
    char buffer[BUFFER_SIZE];
    std::string result = "";
    while(!feof(pipe)) {
        if(fgets(buffer, BUFFER_SIZE, pipe) != NULL)
                result += buffer;
    }
    pclose(pipe);
    return result;
}

#endif // UTIL_H
