package main

import (
	"database/sql"
	"fmt"
	_ "github.com/lib/pq"
)

var db *sql.DB

func InitDB() {
	var err error
	connStr := "user=mohdhussain password=1234 dbname=kvdb sslmode=disable"
	db, err = sql.Open("postgres", connStr)
	if err != nil {
		panic(err)
	}

	_, err = db.Exec(`CREATE TABLE IF NOT EXISTS kvstore (
		key TEXT PRIMARY KEY,
		value TEXT
	)`)
	if err != nil {
		panic(err)
	}
	fmt.Println("✅ Database ready")
}

func InsertOrUpdate(key, value string) error {
	_, err := db.Exec(`INSERT INTO kvstore (key,value)
		VALUES ($1,$2)
		ON CONFLICT (key) DO UPDATE SET value=$2;`, key, value)
	return err
}

func ReadFromDB(key string) (string, error) {
	var v string
	err := db.QueryRow("SELECT value FROM kvstore WHERE key=$1;", key).Scan(&v)
	return v, err
}

func DeleteFromDB(key string) error {
	_, err := db.Exec("DELETE FROM kvstore WHERE key=$1;", key)
	return err
}

