#include "AudioTrack.h"
#include <iostream>
#include <cstring>
#include <random>

AudioTrack::AudioTrack(const std::string& title, const std::vector<std::string>& artists, 
                      int duration, int bpm, size_t waveform_samples)
    : title(title), artists(artists), duration_seconds(duration), bpm(bpm), 
      waveform_size(waveform_samples) {

    // Allocate memory for waveform analysis
    waveform_data = new double[waveform_size];

    // Generate some dummy waveform data for testing
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(-1.0, 1.0);

    for (size_t i = 0; i < waveform_size; ++i) {
        waveform_data[i] = dis(gen);
    }
    #ifdef DEBUG
    std::cout << "AudioTrack created: " << title << " by " << std::endl;
    for (const auto& artist : artists) {
        std::cout << artist << " ";
    }
    std::cout << std::endl;
    #endif
}

// ========== RULE OF 5 IMPLEMENTATION ==========

AudioTrack::~AudioTrack() {
    #ifdef DEBUG
    std::cout << "AudioTrack destructor called for: " << title << std::endl;
    #endif
    // delete[] for arrays, not just delete
    delete[] waveform_data;
}

AudioTrack::AudioTrack(const AudioTrack& other)
: title(other.title), 
      artists(other.artists), 
      duration_seconds(other.duration_seconds),
      bpm(other.bpm), 
      waveform_size(other.waveform_size)
{
    #ifdef DEBUG
    std::cout << "AudioTrack copy constructor called for: " << other.title << std::endl;
    #endif
    
    // deep copy - allocate new memory and copy all the data
    waveform_data = new double[other.waveform_size];
    for (size_t i = 0; i < waveform_size; i++) {
        waveform_data[i] = other.waveform_data[i];
    }
}

AudioTrack& AudioTrack::operator=(const AudioTrack& other) {
    #ifdef DEBUG
    std::cout << "AudioTrack copy assignment called for: " << other.title << std::endl;
    #endif
    
    // check self-assignment first
    if (this == &other) {
        return *this;
    }
    
    // clean up old memory before getting new data
    delete[] waveform_data;

    title = other.title;
    artists = other.artists;
    duration_seconds = other.duration_seconds;
    bpm = other.bpm;
    waveform_size = other.waveform_size;

    // deep copy the array
    waveform_data = new double[waveform_size];
    for (size_t idx = 0; idx < waveform_size; ++idx) {
        waveform_data[idx] = other.waveform_data[idx];
    }

    return *this;  // for chaining
}

AudioTrack::AudioTrack(AudioTrack&& other) noexcept
    : title(std::move(other.title)),
      artists(std::move(other.artists)),
      duration_seconds(other.duration_seconds),
      bpm(other.bpm),
      waveform_data(other.waveform_data),
      waveform_size(other.waveform_size)
{
    #ifdef DEBUG
    std::cout << "AudioTrack move constructor called for: " << other.title << std::endl;
    #endif
    
    // make source safe to delete by nulling its pointer
    other.waveform_data = nullptr;
    other.waveform_size = 0;
}

AudioTrack& AudioTrack::operator=(AudioTrack&& other) noexcept {
    #ifdef DEBUG
    std::cout << "AudioTrack move assignment called for: " << other.title << std::endl;
    #endif
    
    if (this == &other) {
        return *this;
    }
    
    // clean up what we have
    delete[] waveform_data;
    
    // steal everything from other
    title = std::move(other.title);
    artists = std::move(other.artists);
    duration_seconds = other.duration_seconds;
    bpm = other.bpm;
    waveform_data = other.waveform_data;
    waveform_size = other.waveform_size;
    
    // make other safe to delete
    other.waveform_data = nullptr;
    other.waveform_size = 0;
    
    return *this;
}
