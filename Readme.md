# cs744-kv-system-design 

This project is a simple HTTP-based Key-Value Store written in C++.  
It supports basic create, read, and delete operations over HTTP. Data is stored in a SQLite database, and an in-memory cache is used to improve performance.

---

## Overview

### System Architecture

```
Client (curl / browser)
      |
      v
HTTP Server (C++)
 |-- Handlers (/create, /read, /delete)
 |-- Cache (LRU)
      |
      v
Database (SQLite)

```

The client sends HTTP requests, the server handles them concurrently using threads, and the SQLite database ensures persistence.  
The cache helps serve frequently accessed keys faster.

---

## Setup

### On macOS
```bash
brew install g++ make sqlite3 cpp-httplib
```

### On Linux
```bash
sudo apt update
sudo apt install g++ make libsqlite3-dev
```

No extra setup is needed since SQLite is embedded.

---

## Build and Run

```bash
make
./kv_server
```

If working, we should see:
```
Server running on http://localhost:8080
```

---

## Testing

You can use curl to test the basic operations.

**Create**
```bash
curl "localhost:8080/create?key=1&value=hello"
```

**Read**
```bash
curl "localhost:8080/read?key=1"
```

**Delete**
```bash
curl "localhost:8080/delete?key=1"
```

---


## Quick Demo

```bash
make
./kv_server &
curl "localhost:8080/create?key=1&value=hi ajaz"
curl "localhost:8080/read?key=1"
curl "localhost:8080/read?key=1"
curl "localhost:8080/delete?key=1"
```

---

## Summary

This project implements a small and concurrent HTTP-based key-value store.  
It provides a basic understanding of how caching and persistence work in web systems.

---
