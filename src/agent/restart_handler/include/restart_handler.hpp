#pragma once

#include <boost/asio/awaitable.hpp>

#include <vector>

namespace restart_handler
{
    /// @brief Checks if the Agent is running as a service
    ///
    /// @details Looks for indications that the Agent was started by systemd, launchd or SCM
    /// @return true if the Agent is running as a service, otherwise returns false.
    bool RunningAsService();

    /// @brief Class for handling service restarts.
    class RestartHandler
    {
    public:
        /// @brief A list of command line arguments used to restart the agent process.
        static std::vector<char*> startupCmdLineArgs;

        /// @brief Timeout for the restart operation until force a hard restart.
        static const int timeoutInSecs = 30;

        /// @brief RestartHandler class.
        explicit RestartHandler();

        /// @brief Stores the command-line arguments passed to the agent.
        /// @param argc Number of arguments.
        /// @param argv Array of arguments.
        static void SetCommandLineArguments(int argc, char* argv[])
        {
            for (int i = 0; i < argc; ++i)
            {
                // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
                RestartHandler::startupCmdLineArgs.emplace_back(argv[i]);
            }
            // Add a nullptr to terminate the argument list, as required by execve.
            RestartHandler::startupCmdLineArgs.emplace_back(nullptr);
        }

    };
} // namespace restart_handler
