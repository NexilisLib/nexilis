#include <nexilis/logger/file_handler.hh>
#include <nexilis/logger/file_log.hh>

namespace nexilis
{

logger::Logger FileLog::m_logger;
bool FileLog::m_is_setup = false;

void FileLog::setup()
{
    if (m_is_setup)
    {
        return;
    }

    auto temp_path = Util::getNexilisTempPath();
    auto file_path = (temp_path / "file_log.txt");
    Util::deleteIfExists(file_path);

    auto file_handler = std::make_unique<logger::FileHandler>(logger::FileHandler(file_path.string()));
    m_logger.addHandler(std::move(file_handler));
    m_logger.setAllLevels();
    m_is_setup = true;
}

} // namespace nexilis
