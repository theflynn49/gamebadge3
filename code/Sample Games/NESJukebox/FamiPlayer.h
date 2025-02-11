#ifndef FAMIPLAYER_H
#define FAMIPLAYER_H
#pragma once

#include "SdFat.h"

#define PULSE1      6                           // GPIO for the Pulse Wave Channel #1
#define PULSE2      8                           // GPIO for the Pulse Wave Channel #2
#define TRIANGLE    10                          // GPIO for the Triangle Wave Channel
#define NOISE       12                          // GPIO for the Noise Channel
#define BUF_SZ      32                          // Size of the buffer (in notes). 60 notes = 1 second of audio
#define NUM_TRACKS  8                           // Maximum number of tracks to hold
#define HEAD_SZ     4                           // Size of the audio header (in bytes)

class FamiTrack {
    public:
        bool wasPlaying = false;                // Indicates if the track was just played (stays true the entire time the track plays)
        bool playing = false;                   // Indicates if the track is currently playing
        bool loop = false;                      // If true, track repeats when finished
        int curBufCtr = 0;                      // Represents which note index is currently being played in the buffer
        int numNotes = 0;                       // Total number of notes in the track
        int curNote = 0;                        // Keeps track of which note is currently being played relative to the size of the entire track
        int bufferIndex = 0;                    // Keeps track of where we are in the source file
        const char *fileName;                   // Name of the audio file to load
        
        uint16_t chPulse1[BUF_SZ];              // Buffer for the Pulse channel #1
        uint16_t chPulse2[BUF_SZ];              // Buffer for the Pulse channel #2
        uint16_t chTriangle[BUF_SZ];            // Buffer for the Triangle wave channel
        uint16_t chNoise[BUF_SZ];               // Buffer for the Noise channel
};

class FamiPlayer
{
    private:
        uint16_t get_seed();
        uint8_t get_noise();

    public:
        FamiPlayer();
        ~FamiPlayer();
        void serviceTracks();
        void PlayTrack(int trackNum);
        void StopTrack(int trackNum);
        bool IsPlaying(int trackNum);
        uint16_t AddTrack(const char *filename, uint8_t slot, bool loop=false);
        void FillBuffer(FamiTrack *pF);
        void ProcessWave();
        
        int numNotesTemp = 0;
        uint16_t noteFreq[64] = {0, 65, 69, 73, 78, 82, 87, 92, 98, 104, 110, 117, 123, 131, 139, 147, 156, 165, 175, 185, 196, 208, 220, 233, 247, 262, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494, 523, 554, 587, 622, 659, 698, 740, 784, 831, 880, 932, 988, 1047, 1109, 1175, 1245, 1319, 1397, 1480, 1568, 1661, 1760, 1865, 1976, 2093, 2217, 2349 };
        FamiTrack tracks[NUM_TRACKS];
        uint32_t seed;

        uint32_t triangleFreqCounter = 0;
        uint16_t triangleNotePrev = 0;
        uint16_t triangleNote = 0;
        uint8_t triangleIndex = 0;
        uint8_t triangleMap[100] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
        uint8_t triangleVolume = 50;     

        uint32_t noiseNotePrev = 0;
        uint32_t noiseNote = 0;
        uint32_t noiseFreqCounter = 0;
        uint8_t noiseVolume = 0;
        uint32_t noiseMap[16] = { 447443, 223721, 111860, 55930, 27965, 18643, 13982, 11186, 8860, 7046, 4709, 3523, 2348, 1760, 880, 440 };
};

#endif
