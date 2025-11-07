#pragma once
#include <sqlite3.h>
#include <string>
#include <iostream>

class Database {
private:
    sqlite3 *db;

public:
    Database(const std::string &filename) {
        if (sqlite3_open(filename.c_str(), &db)) {
            std::cerr << "Error opening database\n";
            db = nullptr;
        } else {
            const char *sql = "CREATE TABLE IF NOT EXISTS kvstore (key TEXT PRIMARY KEY, value TEXT);";
            char *errMsg = nullptr;
            sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);
            if (errMsg) sqlite3_free(errMsg);
        }
    }

    ~Database() {
        if (db) sqlite3_close(db);
    }

    bool insert_or_update(const std::string &key, const std::string &value) {
        std::string sql = "INSERT INTO kvstore (key,value) VALUES(?,?) "
                          "ON CONFLICT(key) DO UPDATE SET value=excluded.value;";
        sqlite3_stmt *stmt;
        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        sqlite3_bind_text(stmt, 1, key.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, value.c_str(), -1, SQLITE_STATIC);
        bool ok = (sqlite3_step(stmt) == SQLITE_DONE);
        sqlite3_finalize(stmt);
        return ok;
    }

    bool read(const std::string &key, std::string &value) {
        std::string sql = "SELECT value FROM kvstore WHERE key=?;";
        sqlite3_stmt *stmt;
        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        sqlite3_bind_text(stmt, 1, key.c_str(), -1, SQLITE_STATIC);
        bool ok = false;
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            value = (const char *)sqlite3_column_text(stmt, 0);
            ok = true;
        }
        sqlite3_finalize(stmt);
        return ok;
    }

    bool remove(const std::string &key) {
        std::string sql = "DELETE FROM kvstore WHERE key=?;";
        sqlite3_stmt *stmt;
        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        sqlite3_bind_text(stmt, 1, key.c_str(), -1, SQLITE_STATIC);
        bool ok = (sqlite3_step(stmt) == SQLITE_DONE);
        sqlite3_finalize(stmt);
        return ok;
    }
};
