#ifdef HAS_MYSQL_CLIENT_LIBRARY

#ifndef NEXILIS_MYSQL_DATABASE_HH
#define NEXILIS_MYSQL_DATABASE_HH

#include <string>
#include <vector>

// Forward declarations.
struct st_mysql;
struct st_mysql_res;

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

        /// Copy constructor.
        ConnectionData(const ConnectionData& other);

        /// Move constructor.
        ConnectionData(ConnectionData&& other);

        /// Copy assignment operator.
        ConnectionData& operator=(const ConnectionData& other);

        /// Move assignment operator.
        ConnectionData& operator=(ConnectionData&& other);

    public:
        /// Getters.
        std::string getHost()
        {
            return m_host;
        }
        std::string getUser()
        {
            return m_user;
        }
        std::string getPassword()
        {
            return m_password;
        }
        std::string getDatabase()
        {
            return m_database;
        }

    private:
        std::string m_host;
        std::string m_user;
        std::string m_password;
        std::string m_database;
    };

    class ResultSet
    {
    public:
        /// Constructor.
        ResultSet(st_mysql_res* result);

        /// Destructor.
        ~ResultSet();

        /// Get data of the query.
        std::vector<std::string> getRow();

        /// Helper function to print the contents of the query.
        void print();

    private:
        st_mysql_res* m_result;
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

    /// For query operations that don't return anything.
    /// For example INSERT, UPDATE, DELETE, etc.
    bool executeNonQuery(const std::string& query);

    /// For queries that return results, like SELECT.
    ResultSet executeQuery(const std::string& query);

private:
    st_mysql* m_connection;
    ConnectionData m_connectionData;
};

} // namespace nexilis::mysql

#endif
#endif
