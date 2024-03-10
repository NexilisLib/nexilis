#include <nexilis/mysql/database.hh>
#include <nexilis/log.hh>

#include <mysql/mysql.h>

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

Database::ConnectionData::ConnectionData(const ConnectionData& other) :
    m_host(other.m_host),
    m_user(other.m_user),
    m_password(other.m_password),
    m_database(other.m_database)
{
}

Database::ConnectionData::ConnectionData(ConnectionData&& other) :
    m_host(std::move(other.m_host)),
    m_user(std::move(other.m_user)),
    m_password(std::move(other.m_password)),
    m_database(std::move(other.m_database))
{
}

Database::ConnectionData& Database::ConnectionData::operator=(const ConnectionData& other)
{
    if (this != &other)
    {
        m_host = other.m_host;
        m_user = other.m_user;
        m_password = other.m_password;
        m_database = other.m_database;
    }
    return *this;
}

Database::ConnectionData& Database::ConnectionData::operator=(ConnectionData&& other)
{
    if (this != &other)
    {
        m_host = std::move(other.m_host);
        m_user = std::move(other.m_user);
        m_password = std::move(other.m_password);
        m_database = std::move(other.m_database);
    }
    return *this;
}

Database::ResultSet::ResultSet(MYSQL_RES* result) :
    m_result(result)
{
}

Database::ResultSet::~ResultSet()
{
    if (m_result)
    {
        mysql_free_result(m_result);
    }
}

std::vector<std::string> Database::ResultSet::getRow()
{
    MYSQL_ROW row = mysql_fetch_row(m_result);
    std::vector<std::string> rowData;

    if (row)
    {
        unsigned int numFields = mysql_num_fields(m_result);
        rowData.reserve(numFields);
        for (unsigned int i = 0; i < numFields; ++i)
        {
            rowData.push_back(row[i] ? row[i] : "NULL");
        }
    }

    return rowData;
}

void Database::ResultSet::print()
{
    while (true)
    {
        std::vector<std::string> row = getRow();
        if (row.empty())
        {
            break;
        }
        for (const auto& value : row)
        {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }
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

bool Database::executeNonQuery(const std::string& query)
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

Database::ResultSet Database::executeQuery(const std::string& query)
{
    if (!m_connection)
    {
        Log::error("Not connected to MySQL server");
        return ResultSet(nullptr);
    }

    if (mysql_query(m_connection, query.c_str()))
    {
        Log::error("Error executing SQL query: ", mysql_error(m_connection));
        return ResultSet(nullptr);
    }

    MYSQL_RES* result = mysql_store_result(m_connection);
    if (!result)
    {
        Log::error("Error storing result set: ", mysql_error(m_connection));
        return ResultSet(nullptr);
    }

    return ResultSet(result);
}


}
