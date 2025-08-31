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
