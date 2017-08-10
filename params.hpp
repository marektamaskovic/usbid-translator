#ifndef PARAMS_H
#define PARAMS_H

#include "types.hpp"


int parseParams(int argc, char *argv[], params_t &params);
void printParams(const params_t &p);
void printHelp(void);
void wrongParams(const std::string &msg);

#endif