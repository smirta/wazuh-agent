#include <imultitype_queue.hpp>
#include <message_queue_utils.hpp>

#include <vector>

boost::asio::awaitable<std::tuple<int, std::string>>
GetMessagesFromQueue(std::shared_ptr<IMultiTypeQueue> multiTypeQueue,
                     MessageType messageType,
                     const size_t messagesSize,
                     std::function<std::string()> getMetadataInfo)
{
    std::string output;

    if (getMetadataInfo != nullptr)
    {
        output = getMetadataInfo();
    }

    const auto messages = co_await multiTypeQueue->getNextBytesAwaitable(messageType, messagesSize, "", "");
    for (const auto& message : messages)
    {
        output += (message.metaData.empty() ? "" : "\n" + message.metaData) +
                  (message.data.dump() == "{}" ? "" : "\n" + message.data.dump());
    }

    co_return std::tuple<int, std::string> {static_cast<int>(messages.size()), output};
}

void PopMessagesFromQueue(std::shared_ptr<IMultiTypeQueue> multiTypeQueue, MessageType messageType, int numMessages)
{
    multiTypeQueue->popN(messageType, numMessages);
}

void PushCommandsToQueue(std::shared_ptr<IMultiTypeQueue> multiTypeQueue, const std::string& commands)
{
    const auto jsonObj = nlohmann::json::parse(commands);

    if (jsonObj.contains("commands") && jsonObj["commands"].is_array())
    {
        std::vector<Message> messages;

        for (const auto& command : jsonObj["commands"])
        {
            messages.emplace_back(MessageType::COMMAND, command);
        }

        if (!messages.empty())
        {
            multiTypeQueue->push(messages);
        }
    }
}

void PopCommandFromQueue(std::shared_ptr<IMultiTypeQueue> multiTypeQueue)
{
    multiTypeQueue->pop(MessageType::COMMAND);
}
