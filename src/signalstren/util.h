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

/**
  * Reads a single line from the input stream.
  */
std::string read_line(istream &ifs) {
    std::string value;
    getline(ifs, value);

    return value;
}

/**
  * Reads a single line from the input stream and
  * parses it as an optional double value.
  * "---" signifies no value.
  *
  * Returns parsed double value, or NaN, if there
  * is no value.
  *
  * @throws runtime_error - unparsable value
  */
double read_double_opt(istream &ifs) {
    std::string str;
    getline(ifs, str);

    double value = numeric_limits<double>::quiet_NaN();
    if (str != "---") {
        stringstream str_stream(str);
        str_stream >> value;
        if (!str_stream) {
            throw runtime_error("cannot parse double value");
        }
    }

    return value;
}

/**
  * Reads a required double value from the input stream.
  * @throws runtime_error - unparsable value
  */
double read_double_req(istream &ifs) {
    double value;
    ifs >> value;

    if (!ifs) {
        throw runtime_error("cannot read required double value");
    }
    return value;
}

#endif // UTIL_H
