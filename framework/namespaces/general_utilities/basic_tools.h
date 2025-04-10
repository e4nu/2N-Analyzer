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

#include <cstdlib>  // for getenv
#include <cstring>  // for strtok, strcpy
#include <fstream>
#include <iostream>
#include <sstream>  // for std::ostringstream
#include <string>

namespace basic_tools {

// CheckSSHConnection function ------------------------------------------------------------------------------------------------------------------------------------------

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
    const char *ssh_connection = std::getenv("SSH_CONNECTION");
    if (ssh_connection) {
        std::cout << "SSH_CONNECTION: " << ssh_connection << std::endl;

        // Copy to a mutable buffer before using strtok
        char ssh_conn_copy[256];
        strncpy(ssh_conn_copy, ssh_connection, sizeof(ssh_conn_copy) - 1);
        ssh_conn_copy[sizeof(ssh_conn_copy) - 1] = '\0';  // Ensure null termination

        char *remote_ip = strtok(ssh_conn_copy, " ");
        if (remote_ip) {
            std::cout << "Remote IP Address: " << remote_ip << std::endl;

            struct sockaddr_in sa;
            sa.sin_family = AF_INET;
            inet_pton(AF_INET, remote_ip, &(sa.sin_addr));

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
    const char *ssh_connection = std::getenv("SSH_CONNECTION");
    if (ssh_connection) {
        char ssh_conn_copy[256];
        strncpy(ssh_conn_copy, ssh_connection, sizeof(ssh_conn_copy) - 1);
        ssh_conn_copy[sizeof(ssh_conn_copy) - 1] = '\0';  // Ensure null termination

        char *remote_ip = strtok(ssh_conn_copy, " ");
        if (remote_ip) {
            struct sockaddr_in sa;
            sa.sin_family = AF_INET;
            inet_pton(AF_INET, remote_ip, &(sa.sin_addr));

            struct hostent *host = gethostbyaddr(&(sa.sin_addr), sizeof(struct in_addr), AF_INET);
            if (host) {
                return std::string(host->h_name);
            } else {
                std::cerr << "Could not resolve host name from IP." << std::endl;
                return "";
            }
        }
    }

    std::cerr << "Not connected via SSH." << std::endl;
    return "";
}

// quit function --------------------------------------------------------------------------------------------------------------------------------------------------------

void quit() { exit(0); }

// GetCurrentDirectory function -----------------------------------------------------------------------------------------------------------------------------------------

std::string GetCurrentDirectory() {
    char pwd[PATH_MAX];
    if (getcwd(pwd, sizeof(pwd)) == nullptr) {
        std::cerr << "Error getting current directory." << std::endl;
        return "";
    }

    return std::string(pwd);
}

// BoolToChar function --------------------------------------------------------------------------------------------------------------------------------------------------

inline const char *const BoolToChar(bool b) { return b ? "true" : "false"; }

// BoolToString function ------------------------------------------------------------------------------------------------------------------------------------------------

std::string BoolToString(bool b) { return b ? "true" : "false"; }

// ToStringWithPrecision function ---------------------------------------------------------------------------------------------------------------------------------------

template <typename A>
std::string ToStringWithPrecision(const A a_value, const int n = 2) {
    std::ostringstream out;
    out.precision(n);
    out << std::fixed << a_value;
    return out.str();
}

// FindSubstring function -----------------------------------------------------------------------------------------------------------------------------------------------

inline bool FindSubstring(const std::string &string1, const std::string &string2) { return string1.find(string2) != std::string::npos; }

// GetBeamEnergyFromString function -------------------------------------------------------------------------------------------------------------------------------------

double GetBeamEnergyFromString(const std::string &sn) {
    double BeamE_double = 0.;

    if (FindSubstring(sn, "598636MeV") || FindSubstring(sn, "598636mev") || FindSubstring(sn, "598636") || FindSubstring(sn, "5986MeV") || FindSubstring(sn, "5986mev") ||
        FindSubstring(sn, "5986") || FindSubstring(sn, "6GeV") || FindSubstring(sn, "6Gev") || FindSubstring(sn, "6gev")) {
        BeamE_double = 5.98636;
    } else if (FindSubstring(sn, "402962MeV") || FindSubstring(sn, "402962mev") || FindSubstring(sn, "402962") || FindSubstring(sn, "4029MeV") || FindSubstring(sn, "4029mev") ||
               FindSubstring(sn, "4029") || FindSubstring(sn, "4GeV") || FindSubstring(sn, "4Gev") || FindSubstring(sn, "4gev")) {
        BeamE_double = 4.02962;
    } else if (FindSubstring(sn, "207052MeV") || FindSubstring(sn, "207052mev") || FindSubstring(sn, "207052") || FindSubstring(sn, "2070MeV") || FindSubstring(sn, "2070mev") ||
               FindSubstring(sn, "2070") || FindSubstring(sn, "2GeV") || FindSubstring(sn, "2Gev") || FindSubstring(sn, "2gev")) {
        BeamE_double = 2.07052;
    }

    return BeamE_double;
}

// GetBeamEnergyFromDouble function -------------------------------------------------------------------------------------------------------------------------------------

std::string GetBeamEnergyFromDouble(const double &BeamE) {
    std::string BeamE_str;

    if (std::round(BeamE) == 6) {
        BeamE_str = "5986MeV";
    } else if (std::round(BeamE) == 4) {
        BeamE_str = "4029MeV";
    } else if (std::round(BeamE) == 2) {
        BeamE_str = "2070MeV";
    }

    return BeamE_str;
}

// GetBeamEnergyFromDouble function -------------------------------------------------------------------------------------------------------------------------------------

bool fileExists(const char *filename) {
    std::ifstream file(filename);
    return file.good();
}

// LogSkippedHipoFiles function -----------------------------------------------------------------------------------------------------------------------------------------

// This function logs the skipped HIPO files to a text file.
void LogSkippedHipoFiles(std::vector<TString> SkippedHipoChainFiles, const int &HipoChainLength, const char *filename) {
    std::ofstream outFile(filename);

    if (outFile.is_open()) {
        outFile << "\n===========================================================================\n";
        outFile << "Skipped HIPO files\n";
        outFile << "===========================================================================\n\n";

        outFile << "Total number of files in chain: " << HipoChainLength << "\n";
        outFile << "Number of skipped files: " << SkippedHipoChainFiles.size() << "\n\n";

        outFile << "-- Skipped files list -----------------------------------------------------\n\n";

        for (const auto &fname : SkippedHipoChainFiles) { outFile << fname << std::endl; }

        outFile << "\n";

        outFile.close();

        std::cout << "\033[32mSkipped file list saved to:\033[0m\n" << filename << std::endl;
    } else {
        std::cerr << "\033[31mbasic_tools::LogSkippedHipoFiles: ERROR! Failed to open file:\033[0m\n" << filename << "\n\033[31mfor writing!\033[0m\n";
    }
}

};  // namespace basic_tools

#endif  // BASIC_TOOLS_H
