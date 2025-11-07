package main

import (
	"fmt"
	"log"
	"net/http"
)

// global cache (initialized in main)
var cache *Cache

func main() {
	InitDB()                // connect to DB
	cache = NewCache(1000)  // capacity = 1000 entries

	http.HandleFunc("/create", createHandler)
	http.HandleFunc("/read", readHandler)
	http.HandleFunc("/delete", deleteHandler)
	http.HandleFunc("/ping", func(w http.ResponseWriter, r *http.Request) {
		fmt.Fprint(w, "pong")
	})

	fmt.Println("🚀 KV-Server running on port 8080 …")
	log.Fatal(http.ListenAndServe(":8080", nil))
}

// ---------- Handlers ----------

func createHandler(w http.ResponseWriter, r *http.Request) {
	key := r.URL.Query().Get("key")
	value := r.URL.Query().Get("value")
	if key == "" || value == "" {
		http.Error(w, "missing key or value", http.StatusBadRequest)
		return
	}

	cache.Put(key, value)                       // update cache
	if err := InsertOrUpdate(key, value); err != nil {
		http.Error(w, err.Error(), 500)
		return
	}
	fmt.Fprintf(w, "✅ stored %s:%s\n", key, value)
}

func readHandler(w http.ResponseWriter, r *http.Request) {
	key := r.URL.Query().Get("key")
	if key == "" {
		http.Error(w, "missing key", http.StatusBadRequest)
		return
	}

	// try cache
	if val, ok := cache.Get(key); ok {
		fmt.Fprintf(w, "💾 Cache Hit: %s\n", val)
		return
	}

	// fallback to DB
	val, err := ReadFromDB(key)
	if err != nil {
		http.Error(w, "key not found", 404)
		return
	}
	cache.Put(key, val)
	fmt.Fprintf(w, "📦 Cache Miss (from DB): %s\n", val)
}

func deleteHandler(w http.ResponseWriter, r *http.Request) {
	key := r.URL.Query().Get("key")
	if key == "" {
		http.Error(w, "missing key", http.StatusBadRequest)
		return
	}

	cache.Delete(key)
	if err := DeleteFromDB(key); err != nil {
		http.Error(w, err.Error(), 500)
		return
	}
	fmt.Fprintf(w, "🗑️ deleted key: %s\n", key)
}

