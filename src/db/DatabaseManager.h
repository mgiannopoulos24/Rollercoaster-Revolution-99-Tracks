#ifndef RR99_DB_DATABASEMANAGER_H
#define RR99_DB_DATABASEMANAGER_H

#include <string>
#include <functional>
#include <sqlite3.h>
#include <vector>
#include <map>

namespace rr99 {

class DatabaseManager {
public:
    static DatabaseManager& instance();

    bool initialize(const std::string& dbPath);
    void createTables();
    void close();

    bool execute(const std::string& sql);
    
    bool queryExec(const std::string& sql, std::function<void(sqlite3_stmt*)> callback);

    sqlite3* getDb() { return m_db; }

private:
    DatabaseManager() = default;
    ~DatabaseManager() { close(); }

    sqlite3* m_db = nullptr;
    bool m_initialized = false;
};

} // namespace rr99

#endif // RR99_DB_DATABASEMANAGER_H
