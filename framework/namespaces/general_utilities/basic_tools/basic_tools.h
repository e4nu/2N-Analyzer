//
// Created by Alon Sportes on 23/03/2025.
//

#ifndef BASIC_TOOLS_H
#define BASIC_TOOLS_H

#include <arpa/inet.h>  // for inet_ntoa
#include <limits.h>     // for PATH_MAX
#include <netdb.h>      // for gethostbyaddr
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>  // for getcwd
//
#include <cmath>
#include <cstdlib>  // for getenv
#include <cstring>  // for strtok, strcpy
#include <iostream>
#include <sstream>  // for std::ostringstream
#include <string>

// // using namespace std;

namespace basic_tools {

// CheckSSHConnection function ------------------------------------------------------------------------------------------------------------------------------------------

void CheckSSHConnection();

// CheckSSHConnectionAndHost function -----------------------------------------------------------------------------------------------------------------------------------

void CheckSSHConnectionAndHost();

// GetSSHHostName function ----------------------------------------------------------------------------------------------------------------------------------------------

std::string GetSSHHostName();

// quit function --------------------------------------------------------------------------------------------------------------------------------------------------------

void quit() { exit(0); }

// GetCurrentDirectory function -----------------------------------------------------------------------------------------------------------------------------------------

std::string GetCurrentDirectory();

// BoolToChar function --------------------------------------------------------------------------------------------------------------------------------------------------

inline const char *const BoolToChar(bool b);

// BoolToString function ------------------------------------------------------------------------------------------------------------------------------------------------

std::string BoolToString(bool b);

// ToStringWithPrecision function ---------------------------------------------------------------------------------------------------------------------------------------

template <typename A>
std::string ToStringWithPrecision(const A a_value, const int n = 2);

// FindSubstring function -----------------------------------------------------------------------------------------------------------------------------------------------

inline bool FindSubstring(const std::string &string1, const std::string &string2);

// GetBeamEnergyFromString function -------------------------------------------------------------------------------------------------------------------------------------

double GetBeamEnergyFromString(const std::string &sn);

// GetBeamEnergyFromDouble function -------------------------------------------------------------------------------------------------------------------------------------

std::string GetBeamEnergyFromDouble(const double &BeamE);

};  // namespace basic_tools

#endif  // BASIC_TOOLS_H
