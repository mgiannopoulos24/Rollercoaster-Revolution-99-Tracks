#include "DatabaseManager.h"

#include <iostream>

namespace rr99 {

DatabaseManager& DatabaseManager::instance() {
    static DatabaseManager instance;
    return instance;
}

bool DatabaseManager::initialize(const std::string& dbPath) {
    if (m_initialized)
        return true;

    if (sqlite3_open(dbPath.c_str(), &m_db) != SQLITE_OK) {
        std::cerr << "Failed to open database: " << sqlite3_errmsg(m_db) << std::endl;
        return false;
    }

    m_initialized = true;
    return true;
}

void DatabaseManager::createTables() {
    execute("CREATE TABLE IF NOT EXISTS schema_version ("
            "  version INTEGER PRIMARY KEY"
            ");");
    execute("CREATE TABLE IF NOT EXISTS levels ("
            "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "  unlocked BOOLEAN DEFAULT 0,"
            "  best_score INTEGER DEFAULT 0,"
            "  tokens_collected INTEGER DEFAULT 0,"
            "  stars INTEGER DEFAULT 0"
            ");");
    execute("CREATE TABLE IF NOT EXISTS scores ("
            "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "  level_id INTEGER,"
            "  score INTEGER,"
            "  date TEXT,"
            "  train_type TEXT,"
            "  FOREIGN KEY(level_id) REFERENCES levels(id)"
            ");");
    execute("CREATE TABLE IF NOT EXISTS ghost_runs ("
            "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "  level_id INTEGER,"
            "  mode TEXT,"
            "  input_data BLOB,"
            "  timestamp TEXT,"
            "  FOREIGN KEY(level_id) REFERENCES levels(id)"
            ");");
    execute("CREATE TABLE IF NOT EXISTS settings ("
            "  key TEXT PRIMARY KEY,"
            "  value TEXT"
            ");");
    execute("CREATE TABLE IF NOT EXISTS achievements ("
            "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "  unlocked BOOLEAN DEFAULT 0,"
            "  date TEXT"
            ");");
}

void DatabaseManager::close() {
    if (m_db) {
        sqlite3_close(m_db);
        m_db = nullptr;
    }
    m_initialized = false;
}

bool DatabaseManager::execute(const std::string& sql) {
    char* errMsg = nullptr;
    if (sqlite3_exec(m_db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "SQL Error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

bool DatabaseManager::queryExec(const std::string& sql, std::function<void(sqlite3_stmt*)> callback) {
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Prepare Error: " << sqlite3_errmsg(m_db) << std::endl;
        return false;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        callback(stmt);
    }

    sqlite3_finalize(stmt);
    return true;
}

} // namespace rr99
