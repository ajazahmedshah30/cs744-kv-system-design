# cs744-kv-system-design 

This project is a simple HTTP-based Key-Value Store written in C++ for the CS744 Distributed Systems course.  
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
 \-- Database (SQLite)
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

If everything works, you should see:
```
Server running on http://localhost:8080
```

---

## Testing the APIs

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

First read will show "Cache Miss", and repeated reads will show "Cache Hit".

---

## How It Works

- The server listens on port 8080.  
- Each request runs in its own thread.  
- The cache stores recent key-value pairs and removes the least recently used when full.  
- The database ensures data is persistent between runs.

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

Expected behavior:
- First read -> Cache miss  
- Second read -> Cache hit  

---

## Summary

This project implements a small and concurrent HTTP-based key-value store.  
It provides a basic understanding of how caching and persistence work in web systems.

---

Author: Ajaz Ahmed Shah  
CS744 - DECS 
IIT Bombay
