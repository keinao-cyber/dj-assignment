#include "LRUCache.h"
#include <iostream>

LRUCache::LRUCache(size_t capacity)
    : slots(capacity), max_size(capacity), access_counter(0) {}

bool LRUCache::contains(const std::string& track_id) const {
    return findSlot(track_id) != max_size;
}

AudioTrack* LRUCache::get(const std::string& track_id) {
    size_t idx = findSlot(track_id);
    if (idx == max_size) return nullptr;
    return slots[idx].access(++access_counter);
}

/**
 * TODO: Implement the put() method for LRUCache
 */
 bool LRUCache::put(PointerWrapper<AudioTrack> track) {
    // check if track is valid
    if (!track) {
        return false;
    }
    
    // case of existing track
    std::string title = track->get_title();
    size_t existing_idx = findSlot(title);
    if (existing_idx != max_size) {
        slots[existing_idx].access(++access_counter);
        return false; 
    }
    
    // case of new track
    bool eviction_occurred = false;
    
    // check if cache is full and evict LRU if necessary
    if (isFull()) {
        evictLRU();
        eviction_occurred = true;
    }
    
    // find an empty slot and store the track
    size_t empty_idx = findEmptySlot();
    if (empty_idx != max_size) {
        slots[empty_idx].store(std::move(track), ++access_counter);
    }
    
    return eviction_occurred;
}

bool LRUCache::evictLRU() {
    size_t lru = findLRUSlot();
    if (lru == max_size || !slots[lru].isOccupied()) return false;
    slots[lru].clear();
    return true;
}

size_t LRUCache::size() const {
    size_t count = 0;
    for (const auto& slot : slots) if (slot.isOccupied()) ++count;
    return count;
}

void LRUCache::clear() {
    for (auto& slot : slots) {
        slot.clear();
    }
}

void LRUCache::displayStatus() const {
    std::cout << "[LRUCache] Status: " << size() << "/" << max_size << " slots used\n";
    for (size_t i = 0; i < max_size; ++i) {
        if(slots[i].isOccupied()){
            std::cout << "  Slot " << i << ": " << slots[i].getTrack()->get_title()
                      << " (last access: " << slots[i].getLastAccessTime() << ")\n";
        } else {
            std::cout << "  Slot " << i << ": [EMPTY]\n";
        }
    }
}

size_t LRUCache::findSlot(const std::string& track_id) const {
    for (size_t i = 0; i < max_size; ++i) {
        if (slots[i].isOccupied() && slots[i].getTrack()->get_title() == track_id) return i;
    }
    return max_size;

}

/**
 * TODO: Implement the findLRUSlot() method for LRUCache
 */
 size_t LRUCache::findLRUSlot() const {
    size_t lru_index = max_size;
    uint64_t min_time = UINT64_MAX;
    
    // find the least recently used slot in cache
    for (size_t i = 0; i < max_size; i++) {
        if (slots[i].isOccupied()) {
            if (lru_index == max_size || slots[i].getLastAccessTime() < min_time) {
                min_time = slots[i].getLastAccessTime();
                lru_index = i;
            }
        }
    }

    // return the index of the least recently used slot (max_size if no occupied slots)
    return lru_index;  
}

size_t LRUCache::findEmptySlot() const {
    for (size_t i = 0; i < max_size; ++i) {
        if (!slots[i].isOccupied()) return i;
    }
    return max_size;
}

void LRUCache::set_capacity(size_t capacity){
    if (max_size == capacity)
        return;
    //udpate max size
    max_size = capacity;
    //update the slots vector
    slots.resize(capacity);
}