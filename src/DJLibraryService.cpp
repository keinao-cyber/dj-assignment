#include "DJLibraryService.h"
#include "SessionFileParser.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>
#include <filesystem>


DJLibraryService::DJLibraryService(const Playlist& playlist) 
    : playlist(playlist) {}

DJLibraryService::~DJLibraryService() {
    for (AudioTrack* track : library) {
        delete track;
    }
    library.clear();
}
/**
 * @brief Load a playlist from track indices referencing the library
 * @param library_tracks Vector of track info from config
 */
 void DJLibraryService::buildLibrary(const std::vector<SessionConfig::TrackInfo>& library_tracks) {
    for (const auto& track_info : library_tracks) {
        AudioTrack* track = nullptr;
        
        // extra_param1 = bitrate after research it's better to implement this way
        // the type safety is enforced when creating the actual AudioTrack objects
        if (track_info.type == "MP3") {
            track = new MP3Track(
                track_info.title,
                track_info.artists,
                track_info.duration_seconds,
                track_info.bpm,
                track_info.extra_param1  // bitrate
            );
        } else if (track_info.type == "WAV") {
            // extra_param1 = sample_rate, extra_param2 = bit_depth
            track = new WAVTrack(
                track_info.title,
                track_info.artists,
                track_info.duration_seconds,
                track_info.bpm,
                track_info.extra_param1,  // sample_rate
                track_info.extra_param2   // bit_depth
            );
        }
        
        if (track) {
            library.push_back(track);
        }
    }
    
    std::cout << "[INFO] Track library built: " << library.size() 
 << " tracks loaded" << std::endl;
}

/**
 * @brief Display the current state of the DJ library playlist
 * 
 */
void DJLibraryService::displayLibrary() const {
    std::cout << "=== DJ Library Playlist: " 
              << playlist.get_name() << " ===" << std::endl;

    if (playlist.is_empty()) {
        std::cout << "[INFO] Playlist is empty.\n";
        return;
    }

    // Let Playlist handle printing all track info
    playlist.display();

    std::cout << "Total duration: " << playlist.get_total_duration() << " seconds" << std::endl;
}

/**
 * @brief Get a reference to the current playlist
 * 
 * @return Playlist& 
 */
Playlist& DJLibraryService::getPlaylist() {
    // Your implementation here
    return playlist;
}

/**
 * TODO: Implement findTrack method
 * 
 * HINT: Leverage Playlist's find_track method
 */
AudioTrack* DJLibraryService::findTrack(const std::string& track_title) {
    return playlist.find_track(track_title);
}

void DJLibraryService::loadPlaylistFromIndices(const std::string& playlist_name, 
    const std::vector<int>& track_indices) {
std::cout << "[INFO] Loading playlist: " << playlist_name << std::endl;

// Clear old playlist data before loading new one
playlist.clear();
playlist = Playlist(playlist_name);

// add tracks from indices
for (int idx : track_indices) {
    if (idx < 1 || idx > static_cast<int>(library.size())) {
        std::cout << "[WARNING] Invalid track index: " << idx << std::endl;
        continue;
    }

    AudioTrack* og_track = library[idx - 1];

    // clone the track
    PointerWrapper<AudioTrack> cloned_track = og_track->clone();
    if (!cloned_track) {
        std::cerr << "[ERROR] Failed to clone track: " << og_track->get_title() << std::endl;
        continue;
    }

    // load and analyze the track
    cloned_track->load();
    cloned_track->analyze_beatgrid();

    // Add to playlist (releases ownership to playlist)
    AudioTrack* track_ptr = cloned_track.release();
    playlist.add_track(track_ptr);
}

std::cout << "[INFO] Playlist loaded: " << playlist_name << " (" << playlist.get_track_count() << " tracks)" << std::endl;
}

/**
 * TODO: Implement getTrackTitles method
 * @return Vector of track titles in the playlist
 */
std::vector<std::string> DJLibraryService::getTrackTitles() const {
    std::vector<std::string> titles;
    std::vector<AudioTrack*> tracks = playlist.getTracks();
    
    for (AudioTrack* track : tracks) {
        if (track) {
            titles.push_back(track->get_title());
        }
    }
    
    return titles;
}