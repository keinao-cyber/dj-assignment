#include "MixingEngineService.h"
#include <iostream>
#include <memory>


/**
 * TODO: Implement MixingEngineService constructor
 */
MixingEngineService::MixingEngineService()
    : active_deck(0), auto_sync(false), bpm_tolerance(0)
{
    decks[0] = nullptr;
    decks[1] = nullptr;
    std::cout << "[MixingEngineService] Initialized with 2 empty decks."  << std::endl;
}

/**
 * TODO: Implement MixingEngineService destructor
 */
 MixingEngineService::~MixingEngineService() {
    std::cout << "[MixingEngineService] Cleaning up decks..." << std::endl;
    
    for (size_t i = 0; i < 2; ++i) {
        if (decks[i] != nullptr) {
            delete decks[i];
            decks[i] = nullptr;
        }
    }
}


/**
 * TODO: Implement loadTrackToDeck method
 * @param track: Reference to the track to be loaded
 * @return: Index of the deck where track was loaded, or -1 on failure
 */
 int MixingEngineService::loadTrackToDeck(const AudioTrack& track) {
    std::cout << "\n=== Loading Track to Deck ===" << std::endl;
    
    // clone track polymorphically using wrapper in PointerWrapper for safety
    PointerWrapper<AudioTrack> cloned = track.clone();
    if (!cloned) {
        std::cerr << "[ERROR] Track: \"" << track.get_title() 
                  << "\" failed to clone" << std::endl;
        return -1;
    }
    
    // identify target deck (the inactive one
    
    size_t target;
    bool is_first_track = (decks[0] == nullptr && decks[1] == nullptr);
    
    if (is_first_track) {
        target = 0;
    } else {
        target = 1 - active_deck;
    }
    std::cout << "[Deck Switch] Target deck: " << target << std::endl;
    
    // unload target deck if occupied
    if (decks[target] != nullptr) {
        delete decks[target];
        decks[target] = nullptr;
    }
    
    // preper track
    cloned->load();
    cloned->analyze_beatgrid();
    
    // if there's an active deck and sync is enabled, check if the tracks can be mixed
    if (decks[active_deck] != nullptr && auto_sync) {
        if (!can_mix_tracks(cloned)) {
            sync_bpm(cloned);
        }
    }
    
    // assign the track to the deck
    decks[target] = cloned.release();
    std::cout << "[Load Complete] '" << decks[target]->get_title() 
              << "' is now loaded on deck " << target << std::endl;
    
    // unload the previous active deck (instant transition - only if not first track)
    if (!is_first_track && decks[active_deck] != nullptr) {
        std::cout << "[Unload] Unloading previous deck " << active_deck 
                  << " (" << decks[active_deck]->get_title() << ")" << std::endl;
        delete decks[active_deck];
        decks[active_deck] = nullptr;
    }
    
    // switch the active deck
    active_deck = target;
    std::cout << "[Active Deck] Switched to deck " << active_deck << std::endl;
    
    return target;
}

/**
 * @brief Display current deck status
 */
void MixingEngineService::displayDeckStatus() const {
    std::cout << "\n=== Deck Status ===\n";
    for (size_t i = 0; i < 2; ++i) {
        if (decks[i])
            std::cout << "Deck " << i << ": " << decks[i]->get_title() << "\n";
        else
            std::cout << "Deck " << i << ": [EMPTY]\n";
    }
    std::cout << "Active Deck: " << active_deck << "\n";
    std::cout << "===================\n";
}

/**
 * TODO: Implement can_mix_tracks method
 * 
 * Check if two tracks can be mixed based on BPM difference.
 * 
 * @param track: Track to check for mixing compatibility
 * @return: true if BPM difference <= tolerance, false otherwise
 */
bool MixingEngineService::can_mix_tracks(const PointerWrapper<AudioTrack>& track) const {
    // check if the active deck has a track
    if (decks[active_deck] == nullptr) {
        return false;
    }
    
    // Verify new track is valid
    if (!track) {
        return false;
    }
    
    // get bpm values
    int curr_bpm = decks[active_deck]->get_bpm();
    int new_bpm = track->get_bpm();
    
    // calculate diff
    int diff = std::abs(curr_bpm - new_bpm);
    
    // return trueif difference is less than or equal to bpm_tolerance
    return diff <= bpm_tolerance;
}

/**
 * TODO: Implement sync_bpm method
 * @param track: Track to synchronize with active deck
 */
 void MixingEngineService::sync_bpm(const PointerWrapper<AudioTrack>& track) const {
    // validity check
    if (decks[active_deck] == nullptr || !track) {
        return;
    }
    
    int original_bpm = track->get_bpm();
    int active_bpm = decks[active_deck]->get_bpm();
    
    // average
    int average_bpm = (original_bpm + active_bpm) / 2;
    
    // update new track's bpm
    const_cast<AudioTrack*>(track.get())->set_bpm(average_bpm);
    
    std::cout << "[Sync BPM] Syncing BPM from " << original_bpm 
              << " to " << average_bpm << std::endl;
}
