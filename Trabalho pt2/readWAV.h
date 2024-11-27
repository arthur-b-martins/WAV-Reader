#ifndef READWAV_H
#define READWAV_H

/*
    arquivo: readWAV.h
    Autores:
        Arthur Borges Martins (12411BCC063)
        Davi Santos Oliveira (12411BCC088)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern short VERBOSE;

//Tarefa 2
typedef struct WAVHeader{
    char RIFFID[5];
    int fileSize; // int bytes
    char fmtChunkType[5]; // "WAVE"
    char formatChunkID[5]; // "fmt "
    int formatChunkSize; // 16
    short formatType; // 1 (PCM)
    short numChannels; // 1 (mono) or 2 (stereo)
    int sampleFrequence; // frequencia de amostragem
    int byteRate; // sampleFrequence * numChannels * bitsPerSample/8
    short bytesPerBlock; // numChannels * bitsPerSample/8
    short bitsPerSample; // 8, 16, 24
    char dataChunkID[5]; // "data"
    int dataSize; // int bytes
}WAVHeader;

typedef struct WAVFile{
    WAVHeader *header; // header
    short *data; // data
    char *path; // file path
    int duration; // in seconds
}WAVFile;


int initHeader(WAVHeader *header, FILE *file);
void printHeader(WAVHeader header);
int copyHeader(WAVHeader header, WAVHeader *newHeader);
int readWAV(char *filePath, WAVFile *wavFile);
int writeWAV(WAVFile *wavFile);
void invertSamples(WAVFile *wavFile);
int copyWAVFile(WAVFile wavFile, WAVFile *newWavFile);
void printWAVFileInfo(WAVFile wavFile);
int getSmallestSample(WAVFile wavFile);
int getBiggestSample(WAVFile wavFile);
#endif // READWAV_H