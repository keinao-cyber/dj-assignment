#include "DJControllerService.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>

DJControllerService::DJControllerService(size_t cache_size)
    : cache(cache_size) {}
/**
 * TODO: Implement loadTrackToCache method
 */
int DJControllerService::loadTrackToCache(AudioTrack& track) {
    // check if track is already in cache
    if (cache.contains(track.get_title())) {
        cache.get(track.get_title());
        return 1; // return 1 for HIT
    }
    
    // else, clone the track
    PointerWrapper<AudioTrack> cloned = track.clone();
    if (!cloned) {
        return 0; // return 0 for MISS
    }
    
    // use load() and analyze_beatgrid()
    cloned->load();
    cloned->analyze_beatgrid();
    
    // move the cloned track to cache
    bool evicted = cache.put(std::move(cloned));
    
    // return -1 if eviction, 0 if simple MISS
    return evicted ? -1 : 0;
}

void DJControllerService::set_cache_size(size_t new_size) {
    cache.set_capacity(new_size);
}

//implemented
void DJControllerService::displayCacheStatus() const {
    std::cout << "\n=== Cache Status ===\n";
    cache.displayStatus();
    std::cout << "====================\n";
}

/**
 * TODO: Implement getTrackFromCache method
 */
AudioTrack* DJControllerService::getTrackFromCache(const std::string& track_title) {
    return cache.get(track_title);
}
