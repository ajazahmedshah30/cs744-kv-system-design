# cs744-kv-system-design (Project Phase 1)

A simple **HTTP-based Key-Value Store** built in **Go**, as part of the CS744 Distributed Systems Project.  
This system provides REST APIs to create, read, and delete key-value pairs,  
stores data persistently in **PostgreSQL**, and uses an in-memory cache for faster reads.  

---

##  Project Overview

**Architecture**

 
Client (curl / browser)
      ↓
HTTP Server (Go)
 ├── Request Handlers (/create, /read, /delete)
 ├── In-Memory Cache (LRU)
 └── Database Connector (PostgreSQL)


This Phase 1 submission demonstrates a functionally correct, concurrent system that supports basic CRUD operations, caching, and persistent storage.

---

##  Setup Instructions

###  For Both Linux and macOS

It needs **Go** and **PostgreSQL** installed.  
Follow the OS-specific instructions below (if dont have them).

---

### Linux Setup

#### 1. Install Dependencies
```bash
sudo apt update
sudo apt install golang-go postgresql
```

#### 2. Start PostgreSQL
```bash
sudo service postgresql start
```

#### 3. Create Database
```bash
sudo -u postgres psql
CREATE DATABASE kvdb;
\q
```

---

### macOS Setup

#### 1. Install Homebrew (if not installed)
```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

#### 2. Install Go and PostgreSQL
```bash
brew install go postgresql
```

#### 3. Start PostgreSQL
```bash
brew services start postgresql
```

#### 4. Create Database
```bash
psql postgres
CREATE DATABASE kvdb;
\q
```

---

### Common Steps (After Installation)

#### 1. Clone the Repository
```bash
git clone https://github.com/<username>/kv_server.git
cd kv_server
```

#### 2. Initialize Go Modules
```bash
go mod init kv_server
go get github.com/lib/pq
```

#### 3. Update Database Credentials (if needed)
Open **db.go** and make sure this line matches your local setup:
```go
connStr := "user=postgres password=1234 dbname=kvdb sslmode=disable"
```
>  On macOS, replace `"user=postgres"` with your Mac username if needed:
> ```go
> connStr := "user=<your_mac_username> dbname=kvdb sslmode=disable"
> ```

---

##   Run the Server

```bash
go run main.go cache.go db.go
```

Expected output:
```
  Database ready
  KV-Server running on port 8080 …
```

---

##   Test the APIs

You can use **curl** from the terminal or a tool like **Postman**.

### 1 Create a Key-Value Pair
```bash
curl "localhost:8080/create?key=1&value=hello"
```
Output:
```
  stored 1:hello
```

### 2 Read the Key (First Time – Cache Miss)
```bash
curl "localhost:8080/read?key=1"
```
Output:
```
  Cache Miss (from DB): hello
```

### 3 Read Again (Cache Hit)
```bash
curl "localhost:8080/read?key=1"
```
Output:
```
  Cache Hit: hello
```

### 4 Delete the Key
```bash
curl "localhost:8080/delete?key=1"
```
Output:
```
  Deleted key: 1
```

---

##  What’s Working (Phase 1 Goals)

-  HTTP server with `/create`, `/read`, and `/delete` routes  
-  Persistent PostgreSQL backend  
-  Thread-safe in-memory cache (LRU)  
-  Concurrency handled automatically by Go’s HTTP server  
-  Fully functional, ready-to-demo system  

---

##  Next Steps (for Phase 2)

In the next phase, you’ll:
- Add a load generator to simulate concurrent clients  
- Measure throughput and latency under different workloads  
- Identify CPU-bound vs. I/O-bound bottlenecks  
- Plot performance graphs and analyze results  

---

##  Author
**Your Name**  
CS744 – Distributed & Cloud Systems Project  
Indian Institute of Technology (Bombay)

---

###  Notes

- **To restart PostgreSQL**
  - Linux: `sudo service postgresql restart`  
  - macOS: `brew services restart postgresql`
- **Check if it’s running**
  ```bash
  psql -l
  ```
- **Stop PostgreSQL**
  - Linux: `sudo service postgresql stop`  
  - macOS: `brew services stop postgresql`

---

  This README covers **both Linux and macOS**,  
and matches the **Phase 1  is ** perfectly:  
a functionally correct, concurrent, and persistent Key-Value Server ready for demo and submission.

