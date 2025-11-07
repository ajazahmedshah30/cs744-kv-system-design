#!/bin/bash

echo " Starting cs744-kv-system-design Tests..."
sleep 1

# Step 1: CREATE Tests
echo -e "\n[CREATE TESTS]"
curl -s "localhost:8080/create?key=1&value=hello"
curl -s "localhost:8080/create?key=2&value=world"
curl -s "localhost:8080/create?key=3&value=testdata"

# Step 2: READ Tests (Cache Hit)
echo -e "\n[READ TESTS - Cache Hit]"

curl -s "localhost:8080/read?key=1"   # Cache hit expected

# Step 3: DELETE Tests
echo -e "\n[ DELETE TESTS]"
curl -s "localhost:8080/delete?key=2"
curl -s "localhost:8080/read?key=2"   # Should not exist

# Step 4: VERIFY DATABASE PERSISTENCE
echo -e "\n[VERIFY DATABASE CONTENTS]"
sqlite3 kvstore.db "SELECT * FROM kvstore;"

# Step 5: CONCURRENCY TEST (Simulate Multiple Clients)
echo -e "\n[CONCURRENCY TEST - Multiple Clients Writing Concurrently]"
for i in {1..5}; do
  curl -s "localhost:8080/create?key=concurrent$i&value=data$i" &
done
wait

echo "Database content count after concurrency test:"
sqlite3 kvstore.db "SELECT COUNT(*) FROM kvstore;"

echo -e "\nAll Tests Completed Successfully!"
