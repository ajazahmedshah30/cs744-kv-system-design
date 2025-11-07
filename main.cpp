#include "httplib.h"
#include "cache.hpp"
#include "db.hpp"
#include <iostream>
#include <mutex>

using namespace std;
using namespace httplib;

LRUCache<string, string> cache(1000);  // cache capacity
Database db("kvstore.db");
mutex mtx;  // thread safety for DB operations

int main() {
    Server svr;

    // CREATE or UPDATE key-value
    svr.Get("/create", [&](const Request &req, Response &res) {
        auto key = req.get_param_value("key");
        auto value = req.get_param_value("value");
        if (key.empty() || value.empty()) {
            res.status = 400;
            res.set_content("Missing key or value", "text/plain");
            return;
        }
        {
            lock_guard<mutex> lock(mtx);
            db.insert_or_update(key, value);
        }
        cache.put(key, value);
        res.set_content("stored " + key + ":" + value + "\n", "text/plain");
    });

    // READ key-value
    svr.Get("/read", [&](const Request &req, Response &res) {
        auto key = req.get_param_value("key");
        if (key.empty()) {
            res.status = 400;
            res.set_content("Missing key", "text/plain");
            return;
        }

        string value;
        if (cache.get(key, value)) {
            res.set_content("Cache Hit: " + value + "\n", "text/plain");
            return;
        }

        {
            lock_guard<mutex> lock(mtx);
            if (db.read(key, value)) {
                cache.put(key, value);
                res.set_content("Cache Miss : " + value + "\n", "text/plain");
                return;
            }
        }

        res.status = 404;
        res.set_content("Key not found: " + key + "\n", "text/plain");
    });

    // DELETE key-value (fixed version)
    svr.Get("/delete", [&](const Request &req, Response &res) {
        auto key = req.get_param_value("key");
        if (key.empty()) {
            res.status = 400;
            res.set_content("Missing key", "text/plain");
            return;
        }

        string value;
        bool exists = false;

        // Check in cache first
        if (cache.get(key, value)) {
            exists = true;
        } else {
            lock_guard<mutex> lock(mtx);
            string temp;
            if (db.read(key, temp)) exists = true;
        }

        if (!exists) {
            res.status = 404;
            res.set_content("Key not found: " + key + "\n", "text/plain");
            return;
        }

        // Proceed with deletion
        {
            lock_guard<mutex> lock(mtx);
            db.remove(key);
        }
        cache.remove(key);
        res.set_content("Deleted key: " + key + "\n", "text/plain");
    });

    cout << "Server running on http://localhost:8080" << endl;
    svr.listen("0.0.0.0", 8080);

    return 0;
}
