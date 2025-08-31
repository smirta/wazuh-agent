#include <centralized_configuration.hpp>

#include <boost/asio.hpp>

#include <config.h>
#include <filesystem_wrapper.hpp>
#include <logger.hpp>

#include <chrono>
#include <filesystem>
#include <random>

namespace
{
    std::string CreateTmpFilename()
    {
        constexpr int MIN_VALUE = 1000;
        constexpr int MAX_VALUE = 9999;
        std::random_device rd;
        std::mt19937 generator(rd());
        std::uniform_int_distribution<int> distribution(MIN_VALUE, MAX_VALUE);
        const int random = distribution(generator);

        auto now = std::chrono::high_resolution_clock::now();
        auto timestamp = now.time_since_epoch().count();

        return std::to_string(timestamp) + "_" + std::to_string(random);
    }
} // namespace

namespace centralized_configuration
{
    CentralizedConfiguration::CentralizedConfiguration(SetGroupIdFunctionType setGroupIdFunction,
                                                       GetGroupIdFunctionType getGroupIdFunction,
                                                       DownloadGroupFilesFunctionType downloadGroupFilesFunction,
                                                       ValidateFileFunctionType validateFileFunction,
                                                       ReloadModulesFunctionType reloadModulesFunction,
                                                       std::shared_ptr<IFileSystemWrapper> fileSystemWrapper)
        : m_setGroupIdFunction(std::move(setGroupIdFunction))
        , m_getGroupIdFunction(std::move(getGroupIdFunction))
        , m_downloadGroupFilesFunction(std::move(downloadGroupFilesFunction))
        , m_validateFileFunction(std::move(validateFileFunction))
        , m_reloadModulesFunction(std::move(reloadModulesFunction))
        , m_fileSystemWrapper(fileSystemWrapper ? fileSystemWrapper
                                                : std::make_shared<file_system::FileSystemWrapper>())
    {
        if (m_setGroupIdFunction == nullptr || m_getGroupIdFunction == nullptr ||
            m_downloadGroupFilesFunction == nullptr || m_validateFileFunction == nullptr ||
            m_reloadModulesFunction == nullptr)
        {
            throw std::runtime_error("SetGroupIdFunction, GetGroupIdFunction, DownloadGroupFilesFunction, "
                                     "ValidateFileFunction, and ReloadModulesFunction must be provided.");
        }
    }

} // namespace centralized_configuration
