// rl.h

#ifndef RL_H
#define RL_H

#include <vector>
#include <string>

/*
===============================================================================

    7th Guest - RL Parser

    This header file contains the function prototype for parsing an RL file
    to get the VDX file locations.

===============================================================================
*/

struct RLEntry {
    std::string filename;
    size_t offset;
    size_t length;
};

std::vector<RLEntry> parseRLFile(const std::string &rlFilename);

#endif // RL_H