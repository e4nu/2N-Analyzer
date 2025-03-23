//
// Created by Alon Sportes on 23/03/2025.
//

#ifndef BASIC_TOOLS_H
#define BASIC_TOOLS_H

#include <arpa/inet.h>  // for inet_ntoa
#include <netdb.h>      // for gethostbyaddr
#include <netinet/in.h>
#include <sys/socket.h>

#include <cstdlib>  // for getenv
#include <cstring>  // for strtok
#include <iostream>
#include <string>

using namespace std;

namespace basic_tools {
// CheckSSHConnection function ------------------------------------------------------------------------------------------------------------------------------------------

// Function to check if the program is running over SSH and print connection info
void CheckSSHConnection() {
    const char *ssh_connection = std::getenv("SSH_CONNECTION");
    if (ssh_connection) {
        std::cout << "Connected via SSH. SSH_CONNECTION: " << ssh_connection << std::endl;
    } else {
        std::cout << "Not connected via SSH." << std::endl;
    }
}

// CheckSSHConnectionAndHost function -----------------------------------------------------------------------------------------------------------------------------------

void CheckSSHConnectionAndHost() {
    // Get the SSH_CONNECTION environment variable
    const char *ssh_connection = std::getenv("SSH_CONNECTION");
    if (ssh_connection) {
        std::cout << "SSH_CONNECTION: " << ssh_connection << std::endl;

        // Extract the remote IP address (first part of SSH_CONNECTION)
        char *remote_ip = strtok(const_cast<char *>(ssh_connection), " ");
        if (remote_ip) {
            std::cout << "Remote IP Address: " << remote_ip << std::endl;

            // Convert the IP address to a sockaddr_in structure
            struct sockaddr_in sa;
            sa.sin_family = AF_INET;
            inet_pton(AF_INET, remote_ip, &(sa.sin_addr));

            // Use gethostbyaddr to get the host name from the IP address
            struct hostent *host = gethostbyaddr(&(sa.sin_addr), sizeof(struct in_addr), AF_INET);
            if (host) {
                std::cout << "Remote Host Name: " << host->h_name << std::endl;
            } else {
                std::cerr << "Could not resolve host name from IP." << std::endl;
            }
        }
    } else {
        std::cout << "Not connected via SSH." << std::endl;
    }
}

// GetSSHHostName function ----------------------------------------------------------------------------------------------------------------------------------------------

std::string GetSSHHostName() {
    // Get the SSH_CONNECTION environment variable
    const char *ssh_connection = std::getenv("SSH_CONNECTION");
    if (ssh_connection) {
        // Extract the remote IP address (first part of SSH_CONNECTION)
        char *remote_ip = strtok(const_cast<char *>(ssh_connection), " ");
        if (remote_ip) {
            // Convert the IP address to a sockaddr_in structure
            struct sockaddr_in sa;
            sa.sin_family = AF_INET;
            inet_pton(AF_INET, remote_ip, &(sa.sin_addr));

            // Use gethostbyaddr to get the host name from the IP address
            struct hostent *host = gethostbyaddr(&(sa.sin_addr), sizeof(struct in_addr), AF_INET);
            if (host) {
                // Return the host name as a string
                return std::string(host->h_name);
            } else {
                std::cerr << "Could not resolve host name from IP." << std::endl;
                return "";
            }
        }
    }

    // If not connected via SSH, return an empty string
    std::cerr << "Not connected via SSH." << std::endl;
    return "";
}

// GetCurrentDirectory function -----------------------------------------------------------------------------------------------------------------------------------------

string GetCurrentDirectory() {
    char pwd[PATH_MAX];
    getcwd(pwd, sizeof(pwd));

    string WorkingDirectory = pwd;

    return WorkingDirectory;
}

// BoolToChar function --------------------------------------------------------------------------------------------------------------------------------------------------

/* Usage: convert bool variables to char */
inline const char *const BoolToChar(bool b) { return b ? "true" : "false"; }

// BoolToString function ------------------------------------------------------------------------------------------------------------------------------------------------

/* Usage: convert bool variables to string */
string BoolToString(bool b) { return b ? "true" : "false"; }

// ToStringWithPrecision function ---------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
string ToStringWithPrecision(const T a_value, const int n = 2) {
    std::ostringstream out;
    out.precision(n);
    out << std::fixed << a_value;
    return out.str();
}

// FindSubstring function -----------------------------------------------------------------------------------------------------------------------------------------------

inline bool FindSubstring(const string &string1, const string &string2) { return string1.find(string2) != string::npos; }

// GetBeamEnergyFromString function -------------------------------------------------------------------------------------------------------------------------------------

double GetBeamEnergyFromString(const string &sn) {
    double be = 0.;

    if (FindSubstring(sn, "598636MeV") || FindSubstring(sn, "598636mev") || FindSubstring(sn, "598636") || FindSubstring(sn, "5986MeV") || FindSubstring(sn, "5986mev") ||
        FindSubstring(sn, "5986") || FindSubstring(sn, "6GeV") || FindSubstring(sn, "6Gev") || FindSubstring(sn, "6gev")) {
        be = 5.98636;
    } else if (FindSubstring(sn, "402962MeV") || FindSubstring(sn, "402962mev") || FindSubstring(sn, "402962") || FindSubstring(sn, "4029MeV") || FindSubstring(sn, "4029mev") ||
               FindSubstring(sn, "4029") || FindSubstring(sn, "4GeV") || FindSubstring(sn, "4Gev") || FindSubstring(sn, "4gev")) {
        be = 4.02962;
    } else if (FindSubstring(sn, "207052MeV") || FindSubstring(sn, "207052mev") || FindSubstring(sn, "207052") || FindSubstring(sn, "2070MeV") || FindSubstring(sn, "2070mev") ||
               FindSubstring(sn, "2070") || FindSubstring(sn, "2GeV") || FindSubstring(sn, "2Gev") || FindSubstring(sn, "2gev")) {
        be = 2.07052;
    }

    return be;
}

}  // namespace basic_tools

#endif  // BASIC_TOOLS_H
