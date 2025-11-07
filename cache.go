package main

import (
	"container/list"
	"sync"
)

type Cache struct {
	capacity int
	items    map[string]*list.Element
	order    *list.List
	mu       sync.Mutex
}

type entry struct {
	key, value string
}

func NewCache(cap int) *Cache {
	return &Cache{
		capacity: cap,
		items:    make(map[string]*list.Element),
		order:    list.New(),
	}
}

func (c *Cache) Get(key string) (string, bool) {
	c.mu.Lock()
	defer c.mu.Unlock()
	if e, ok := c.items[key]; ok {
		c.order.MoveToFront(e)
		return e.Value.(*entry).value, true
	}
	return "", false
}

func (c *Cache) Put(key, value string) {
	c.mu.Lock()
	defer c.mu.Unlock()

	if e, ok := c.items[key]; ok {
		e.Value.(*entry).value = value
		c.order.MoveToFront(e)
		return
	}
	e := c.order.PushFront(&entry{key, value})
	c.items[key] = e

	if c.order.Len() > c.capacity {
		last := c.order.Back()
		if last != nil {
			c.order.Remove(last)
			delete(c.items, last.Value.(*entry).key)
		}
	}
}

func (c *Cache) Delete(key string) {
	c.mu.Lock()
	defer c.mu.Unlock()
	if e, ok := c.items[key]; ok {
		c.order.Remove(e)
		delete(c.items, key)
	}
}

