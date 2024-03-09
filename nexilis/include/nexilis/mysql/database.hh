#ifndef NEXILIS_MYSQL_DATABASE_HH
#define NEXILIS_MYSQL_DATABASE_HH

#include <nexilis/log.hh>

#include <mysql/mysql.h>

namespace nexilis::mysql
{
/// Database class holds context to mysql or mariadb database.
/// Database is not copyable, is is movable.
/// This class should be ideally used in context of a reference.
class Database
{
public:
    class ConnectionData
    {
    public:
        /// Constructor.
        ConnectionData(const std::string& host, const std::string& user, const std::string& password, const std::string& database);

        /// TODO copy, move initialization.

        /// Getters
        std::string getHost() { return m_host; }
        std::string getUser() { return m_user; }
        std::string getPassword() { return m_password; }
        std::string getDatabase() { return m_database; }

    private:
        std::string m_host;
        std::string m_user;
        std::string m_password;
        std::string m_database;
    };

    /// Constructor.
    Database(const ConnectionData& connectionData);

    /// Destructor.
    ~Database();

    /// Deleted copy constructor.
    Database(const Database& other) = delete;

    /// Move constructor.
    Database(Database&& other);

    /// Deleted copy assignment operator.
    Database& operator=(const Database& other) = delete;

    /// Move assignment operator.
    Database& operator=(Database&& other);

    bool executeQuery(const std::string& query);
private:
    MYSQL* m_connection;
    ConnectionData m_connectionData;
};


}

#endif

