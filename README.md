# WAV-Reader

This project originated as a college assignment but was expanded with additional functions, structured into a library, and made available for anyone who needs to manipulate WAV files or study how this process works.

The folder contains the library `readWAV.c`, the header file `readWAV.h`, the code `main.c`, and example audio files. 

The `main` file is a program that demonstrates the usage of all the functions present in the `readWAV` library and can be used for educational purposes. Those who are only interested in using the library's functions are advised to ignore the `main` file.

---
# WAV File Manipulation Library (`readWAV`)

## 📖 Overview

This library was developed for the Procedural Programming course (2024-1).  
It provides functions to read, write, and manipulate WAV audio files.  

Key features include:
- Reading WAV file metadata and audio data.
- Writing modified WAV files.
- Audio processing operations, such as:
  - Converting stereo audio to mono.
  - Reversing audio playback.
  - Compressing audio by reducing sample frequency.
  - Swapping stereo channels.
  - Retrieving the smallest and largest audio samples.

## 🛠️ Features and Functions

### Core Functions
- **`readWAV()`**: Reads a WAV file and loads its metadata and audio data.
- **`writeWAV()`**: Writes modified WAV data to a new file.
- **`printWAVFileInfo()`**: Prints detailed information about the WAV file, including metadata and duration.

### Audio Manipulation
- **`invertSamples()`**: Swaps left and right channels in stereo WAV files.
- **`convertToMono()`**: Converts stereo audio to mono by averaging left and right channel samples.
- **`reverseAudio()`**: Reverses the audio playback.
- **`compression()`**: Reduces the sample frequency to compress the audio file.

### Metadata Analysis
- **`getSmallestSample()`**: Finds and returns the smallest sample value in the audio data.
- **`getBiggestSample()`**: Finds and returns the largest sample value in the audio data.

## 📝 File Structure

### `readWAV.h`
Defines the data structures and function prototypes used in the library:
- **`WAVHeader`**: Stores WAV file metadata.
- **`WAVFile`**: Stores the WAV file's metadata, audio data, and file path.

### `readWAV.c`
Implements the functionality declared in `readWAV.h`.

## ⚙️ Example Usage

```c
#include "readWAV.h"

int main() {
    WAVFile wavFile;

    // Read WAV file
    if (!readWAV("example.wav", &wavFile)) {
        printf("Failed to read the WAV file.\n");
        return 1;
    }

    // Print WAV file info
    printWAVFileInfo(wavFile);

    // Convert to mono
    convertToMono(&wavFile);

    // Write changes to a new file
    if (!writeWAV(&wavFile)) {
        printf("Failed to write the WAV file.\n");
        return 1;
    }

    printf("Operation completed successfully.\n");
    return 0;
}

## 🚀 How to Compile

To compile the project, use a C compiler like `gcc`:

```bash
gcc -o wav_tool readWAV.c main.c

## 📂 Authors
- Arthur Borges Martins
- Davi Santos Oliveira 
