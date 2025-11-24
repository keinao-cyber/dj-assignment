#include "WAVTrack.h"
#include <iostream>

WAVTrack::WAVTrack(const std::string& title, const std::vector<std::string>& artists, 
                   int duration, int bpm, int sample_rate, int bit_depth)
    : AudioTrack(title, artists, duration, bpm), sample_rate(sample_rate), bit_depth(bit_depth) {

    std::cout << "WAVTrack created: " << sample_rate << "Hz/" << bit_depth << "bit" << std::endl;
}

// ========== TODO: STUDENTS IMPLEMENT THESE VIRTUAL FUNCTIONS ==========

void WAVTrack::load() {
    std::cout << "[WAVTrack::load] Loading WAV: \"" << title
              << "\" at " << sample_rate << "Hz/" << bit_depth 
              << "bit (uncompressed)...\n";

    long long size = static_cast<long long>(duration_seconds) * sample_rate * (bit_depth / 8) * 2;

    std::cout << "  → Estimated file size: " << size << " bytes\n";
    std::cout << "  → Fast loading due to uncompressed format.\n";
}

void WAVTrack::analyze_beatgrid() {
    std::cout << "[WAVTrack::analyze_beatgrid] Analyzing beat grid for: \"" << title << "\"\n";
    
    double beats = (duration_seconds / 60.0) * bpm;
    
    std::cout << "  → Estimated beats: " << static_cast<int>(beats) 
              << "  → Precision factor: 1 (uncompressed audio)\n";
}

double WAVTrack::get_quality_score() const {
   
    double score = 70.0;
        
    if (sample_rate >= 44100) {
        score += 10.0;  
    }
    if (sample_rate >= 96000) {
        score += 5.0;   
    }
    if (bit_depth >= 16) {
        score += 10.0;  
    }
    if (bit_depth >= 24) {
        score += 5.0;
    }
    
    if (score > 100.0) {
        score = 100.0;
    }
    
    return score;
}

PointerWrapper<AudioTrack> WAVTrack::clone() const {
    // TODO: Implement the clone method
    return PointerWrapper<AudioTrack>(new WAVTrack(*this));
}