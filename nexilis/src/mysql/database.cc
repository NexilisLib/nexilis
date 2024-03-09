#include <nexilis/mysql/database.hh>

namespace nexilis::mysql
{

Database::ConnectionData::ConnectionData(const std::string& host, const std::string& user,
        const std::string& password, const std::string& database) :
        m_host(host),
        m_user(user),
        m_password(password),
        m_database(database)
{
}

Database::Database(const Database::ConnectionData& connectionData) :
    m_connectionData(connectionData)
{
    m_connection = mysql_init(nullptr);

    if (!m_connection)
    {
        Log::error("Error initializing MySQL connection");
        return;
    }

    if (mysql_real_connect(m_connection, m_connectionData.getHost().c_str(),
                m_connectionData.getUser().c_str(), m_connectionData.getPassword().c_str(),
                m_connectionData.getDatabase().c_str(), 0, nullptr, 0) == nullptr)
    {
        Log::error("Error connecting to MySQL server: ", mysql_error(m_connection));
        mysql_close(m_connection);
        m_connection = nullptr;
    }
}

Database::Database(Database&& other) :
    m_connection(std::move(other.m_connection)),
    m_connectionData(std::move(other.m_connectionData))
{
}

Database::~Database()
{
    if (m_connection)
    {
        mysql_close(m_connection);
    }
}

Database& Database::operator=(Database&& other)
{
    if (this != &other)
    {
        m_connection = std::move(other.m_connection);
        m_connectionData = std::move(other.m_connectionData);
    }
    return *this;
}

bool Database::executeQuery(const std::string& query)
{
    if (!m_connection)
    {
        Log::error("Not connected to MySQL server");
        return false;
    }

    if (mysql_query(m_connection, query.c_str()))
    {
        Log::error("Error executing SQL query: ", mysql_error(m_connection));
        return false;
    }
    return true;
}


}
