//
// Created by Alon Sportes on 23/03/2025.
//

#include "libraries/general_utilities/basic_tools.h"

namespace basic_tools {

// RunShellCommand function ----------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* RunShellCommand function */
std::string RunShellCommand(const std::string& command, const bool suppress_stderr) {
    const std::string full_command = suppress_stderr ? (command + " 2>/dev/null") : command;
    FILE* pipe = popen(full_command.c_str(), "r");
    if (!pipe) { return ""; }

    std::array<char, 256> buffer{};
    std::string output;
    while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe) != nullptr) { output += buffer.data(); }
    pclose(pipe);
    return TrimWhitespace(output);
}
#pragma endregion

// GetRemoteIPFromSSHConnection function ---------------------------------------------------------------------------------------------------------------------------------

#pragma region /* GetRemoteIPFromSSHConnection function */
std::string GetRemoteIPFromSSHConnection(const char* ssh_connection) {
    if (ssh_connection == nullptr) { return ""; }
    std::istringstream sshConnectionStream(ssh_connection);
    std::string remote_ip;
    sshConnectionStream >> remote_ip;
    return remote_ip;
}
#pragma endregion

// ResolveHostNameFromIP function ----------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* ResolveHostNameFromIP function */
bool ResolveHostNameFromIP(const std::string& remote_ip, std::string& host_name) {
    if (remote_ip.empty()) { return false; }
    addrinfo hints{};
    hints.ai_family = AF_UNSPEC;
    hints.ai_flags = AI_NUMERICHOST;
    addrinfo* result = nullptr;
    const int gaiStatus = getaddrinfo(remote_ip.c_str(), nullptr, &hints, &result);
    if (gaiStatus != 0 || result == nullptr) { return false; }
    char hostBuffer[NI_MAXHOST];
    const int nameInfoStatus = getnameinfo(result->ai_addr, result->ai_addrlen, hostBuffer, sizeof(hostBuffer), nullptr, 0, NI_NAMEREQD);
    freeaddrinfo(result);
    if (nameInfoStatus != 0) { return false; }
    host_name = hostBuffer;
    return true;
}
#pragma endregion

// CheckSSHConnection function -------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* CheckSSHConnection function */
void CheckSSHConnection() {
    const char* ssh_connection = std::getenv("SSH_CONNECTION");
    if (ssh_connection) {
        std::cout << env::SYSTEM_COLOR << "Connected via SSH. SSH_CONNECTION: " << env::RESET_COLOR << ssh_connection << std::endl << std::flush;
    } else {
        std::cout << env::SYSTEM_COLOR << "Not connected via SSH." << env::RESET_COLOR << std::endl << std::flush;
    }
}
#pragma endregion

// CheckSSHConnectionAndHost function ------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* CheckSSHConnectionAndHost function */
void CheckSSHConnectionAndHost() {
    const char* ssh_connection = std::getenv("SSH_CONNECTION");
    if (ssh_connection) {
        std::cout << env::SYSTEM_COLOR << "SSH_CONNECTION: " << env::RESET_COLOR << ssh_connection << std::endl << std::flush;
        const std::string remote_ip = GetRemoteIPFromSSHConnection(ssh_connection);
        if (remote_ip.empty()) {
            std::cerr << env::SYSTEM_COLOR << "Could not parse remote IP from SSH_CONNECTION." << env::RESET_COLOR << std::endl << std::flush;
            return;
        }
        std::cout << env::SYSTEM_COLOR << "Remote IP Address: " << env::RESET_COLOR << remote_ip << std::endl << std::flush;
        std::string host_name;
        if (ResolveHostNameFromIP(remote_ip, host_name)) {
            std::cout << env::SYSTEM_COLOR << "Remote Host Name: " << env::RESET_COLOR << host_name << std::endl << std::flush;
        } else {
            std::cerr << env::SYSTEM_COLOR << "Could not resolve host name from IP." << env::RESET_COLOR << std::endl << std::flush;
        }
    } else {
        std::cout << env::SYSTEM_COLOR << "Not connected via SSH." << env::RESET_COLOR << std::endl << std::flush;
    }
}
#pragma endregion

// GetSSHHostName function -----------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* GetSSHHostName function */
std::string GetSSHHostName() {
    const char* ssh_connection = std::getenv("SSH_CONNECTION");
    if (ssh_connection) {
        const std::string remote_ip = GetRemoteIPFromSSHConnection(ssh_connection);
        if (remote_ip.empty()) {
            std::cerr << env::ERROR_COLOR << "Error! " << env::RESET_COLOR << "Could not parse remote IP from SSH_CONNECTION." << std::endl << std::flush;
            return "";
        }
        std::string host_name;
        if (ResolveHostNameFromIP(remote_ip, host_name)) { return host_name; }
        std::cerr << env::ERROR_COLOR << "Error! " << env::RESET_COLOR << "Could not resolve host name from IP." << std::endl << std::flush;
        return "";
    }
    std::cerr << env::ERROR_COLOR << "Error! " << env::RESET_COLOR << "Not connected via SSH." << std::endl << std::flush;
    return "";
}
#pragma endregion

// GetCurrentDirectory function ------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* GetCurrentDirectory function */
std::string GetCurrentDirectory() {
    char pwd[PATH_MAX];
    if (getcwd(pwd, sizeof(pwd)) == nullptr) {
        std::cerr << env::ERROR_COLOR << "Error! " << env::RESET_COLOR << "Error getting current directory." << std::endl << std::endl << std::flush;
        return "";
    }
    return std::string(pwd);
}
#pragma endregion

// LogSkippedHipoFiles function ------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* LogSkippedHipoFiles function */
void LogSkippedHipoFiles(const std::vector<TString>& SkippedHipoChainFiles, int HipoChainLength, const char* filename) {
    std::ofstream outFile(filename);
    if (outFile.is_open()) {
        outFile << "\n===========================================================================================\n";
        outFile << "Skipped HIPO files\n";
        outFile << "===========================================================================================\n\n";
        outFile << "Total number of files in chain: " << HipoChainLength << "\n";
        outFile << "Number of skipped files: " << SkippedHipoChainFiles.size() << "\n\n";
        outFile << "-- Skipped files list ---------------------------------------------------------------------\n\n";
        for (const auto& fname : SkippedHipoChainFiles) { outFile << fname << std::endl; }
        outFile << "\n";
        outFile.close();
        std::cout << env::COMPLETION_COLOR << "Skipped file list saved to: \n" << env::RESET_COLOR << filename << std::endl << std::flush;
    } else {
        ExitWithError(__func__, __FILE__, __LINE__, std::string("Failed to open file:\n") + filename + " for writing!");
    }
}
#pragma endregion

}  // namespace basic_tools
