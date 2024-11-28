/*  
lib feita para o trabalho de Programação Procedimental 2024-1.
A lib contém funções para leitura e escrita de arquivos WAV, além de funções para manipulação de arquivos WAV.
A manipulação feita foi a de inverter os canais de um arquivo estéreo.

Autores:
    _Davi Santos Oliveira (12411BCC088)
    _Arthur Borges Martins (12411BCC063)
*/

#include "readWAV.h"

int initHeader(WAVHeader *header, FILE *file){
    if(VERBOSE) printf("Initializing header...\n");

    header->RIFFID[4] = '\0';
    header->fmtChunkType[4] = '\0';
    header->formatChunkID[4] = '\0';
    header->dataChunkID[4] = '\0';
    header->dataSize = 0;

    fread(header->RIFFID, 1, 4, file);
    if (strcmp(header->RIFFID, "RIFF") != 0) {
        printf("\033[31mError\033[0m: RIFF ID not found\n");
        return 0;
    }
    header->RIFFID[4] = '\0';
    fread(&header->fileSize, 4, 1, file);
    if (VERBOSE) printf("fileSize: %d\n", header->fileSize);
    fread(header->fmtChunkType, 1, 4, file);
    if (VERBOSE) printf("fmtChunkType: %s\n", header->fmtChunkType);
    header->fmtChunkType[4] = '\0';
    if (strcmp(header->fmtChunkType, "WAVE") != 0) {
        printf("\033[31mError\033[0m: lib only supports WAVE type.\nfile type: %s\n", header->fmtChunkType);
        return 0;
    }

    fread(header->formatChunkID, 1, 4, file);
    header->formatChunkID[4] = '\0';
    fread(&header->formatChunkSize, 4, 1, file);
    fread(&header->formatType, 2, 1, file);
    fread(&header->numChannels, 2, 1, file);
    fread(&header->sampleFrequence, 4, 1, file);
    fread(&header->byteRate, 4, 1, file);
    fread(&header->bytesPerBlock, 2, 1, file);
    fread(&header->bitsPerSample, 2, 1, file);
    if (header->byteRate != header->sampleFrequence * (header->numChannels * header->bitsPerSample / 8)) {
        printf("\033[31mError\033[0m: byteRate is incorrect\n");
        return 0;
    }
    fread(header->dataChunkID, 1, 4, file);
    fread(&header->dataSize, 4, 1, file);
    header->dataChunkID[4] = '\0';
    if (!strcmp(header->dataChunkID, "data")) {
        return 1;
    }else{
        fseek(file, header->dataSize, SEEK_CUR);
        fread(header->dataChunkID, 1, 4, file);
        fread(&header->dataSize, 4, 1, file);
        return 1;
        }
    return 0;
}


void printHeader(WAVHeader header){
    printf("\033[1;34mheader:\n");
    printf("\033[1;32m");
    printf("RIFFID: %s\n", header.RIFFID);
    printf("fileSize: %d\n", header.fileSize);
    printf("fmtChunkType: %s\n", header.fmtChunkType);
    printf("formatChunkID: \"%s\"\n", header.formatChunkID);
    printf("formatChunkSize: %d\n", header.formatChunkSize);
    printf("formatType: %d\n", header.formatType);
    printf("numChannels: %d\n", header.numChannels);
    printf("sampleFrequence: %d\n", header.sampleFrequence);
    printf("byteRate: %d\n", header.byteRate);
    printf("bytesPerBlock: %d\n", header.bytesPerBlock);
    printf("bitsPerSample: %d\n", header.bitsPerSample);
    printf("dataChunkID: \"%s\"\n", header.dataChunkID);
    printf("dataSize: %d\n", header.dataSize);
    printf("\033[0m");
}

int copyHeader(WAVHeader header, WAVHeader *newHeader){
    /*
    do not copy metadata
    */
    
    if (strcmp(header.RIFFID, "RIFF")) {
        printf("\033[31mError\033[0m: header does not exist\n");
        return 0;
    }

    if (VERBOSE) printf("Copying header\n");
    newHeader->fileSize = header.fileSize;
    newHeader->formatChunkSize = header.formatChunkSize;
    newHeader->formatType = header.formatType;
    newHeader->numChannels = header.numChannels;
    newHeader->sampleFrequence = header.sampleFrequence;
    newHeader->byteRate = header.byteRate;
    newHeader->bytesPerBlock = header.bytesPerBlock;
    newHeader->bitsPerSample = header.bitsPerSample;
    newHeader->dataSize = header.dataSize;
    strcpy(newHeader->RIFFID, header.RIFFID);
    strcpy(newHeader->fmtChunkType, header.fmtChunkType);
    strcpy(newHeader->formatChunkID, header.formatChunkID);
    strcpy(newHeader->dataChunkID, header.dataChunkID);
}


int readWAV(char *filePath, WAVFile *wavFile){
    printf("Reading WAV file...\n");
    FILE *file = fopen(filePath, "rb");
    if(file == NULL){
        printf("\033[31mError\033[0m: file not found\n");
        fclose(file);
        return 0;
    }

    wavFile->path = (char *)malloc(strlen(filePath) + 1);
    if (wavFile->path == NULL){
        printf("\033[31mError\033[0m: path not allocated\n");
        fclose(file);
        return 0;
    }

    strcpy(wavFile->path, filePath);
    printf("File path: %s\n", wavFile->path);
    WAVHeader *header = (WAVHeader *)malloc(sizeof(WAVHeader));
    if (header == NULL){
        printf("\033[31mError\033[0m: header not allocated\n");
        fclose(file);
        free(header);
        return 0;
    }
    wavFile->header = header;
    if(!initHeader(header, file)){
        fclose(file);
        return 0;
    }
    wavFile->data = (short *)calloc(wavFile->header->dataSize/sizeof(short), sizeof(short));
    if (wavFile->data == NULL){
        printf("\033[31mError\033[0m: data not allocated\n");
        fclose(file);
        free(header);
        return 0;
    }
    fread(wavFile->data, sizeof(short), wavFile->header->dataSize/sizeof(short), file);
    wavFile->duration = header->dataSize / header->byteRate;
    fclose(file);
    return 1;
}

int writeWAV(WAVFile *wavFile) {
    FILE *file = fopen(wavFile->path, "wb");
    if (file == NULL) {
        printf("\033[31mError\033[0m: cannot open file for writing\n");
        return 0;
    }

    fwrite(wavFile->header->RIFFID, 1, 4, file);
    fwrite(&wavFile->header->fileSize, 4, 1, file);
    fwrite(wavFile->header->fmtChunkType, 1, 4, file);
    fwrite(wavFile->header->formatChunkID, 1, 4, file);
    fwrite(&wavFile->header->formatChunkSize, 4, 1, file);
    fwrite(&wavFile->header->formatType, 2, 1, file);
    fwrite(&wavFile->header->numChannels, 2, 1, file);
    fwrite(&wavFile->header->sampleFrequence, 4, 1, file);
    fwrite(&wavFile->header->byteRate, 4, 1, file);
    fwrite(&wavFile->header->bytesPerBlock, 2, 1, file);
    fwrite(&wavFile->header->bitsPerSample, 2, 1, file);
    fwrite(wavFile->header->dataChunkID, 1, 4, file);
    fwrite(&wavFile->header->dataSize, 4, 1, file);
    for (int i = 0; i < wavFile->header->dataSize/sizeof(short); i++) {
        fwrite(&wavFile->data[i], sizeof(short), 1, file);
    }
    if(VERBOSE) printf("Data written\n");
    fclose(file);
    return 1;
}

void invertSamples(WAVFile *wavFile){
    if(wavFile->header->numChannels != 2){
        printf("\033[31mError\033[0m: WAV file is not stereo\n");
        return;
    }
    if (VERBOSE) printf("Inverting samples\n");      
    short temp;
    for (int i = 0; i < wavFile->header->dataSize/sizeof(short); i+=2){
        temp = wavFile->data[i];
        wavFile->data[i] = wavFile->data[i+1];
        wavFile->data[i+1] = temp;
    }
}

int copyWAVFile(WAVFile wavFile, WAVFile *newWavFile){
    if (VERBOSE) printf("Copying WAV file\n");
    WAVHeader *header = (WAVHeader *)malloc(sizeof(WAVHeader));
    if (header == NULL){
        printf("\033[31mError\033[0m: header not allocated\n");
        return 0;
    }
    newWavFile->header = header;
    if(!copyHeader(*wavFile.header, newWavFile->header)) return 0;
    newWavFile->duration = wavFile.duration;
    newWavFile->data = (short *)calloc(wavFile.header->dataSize/sizeof(short), sizeof(short));
    if (newWavFile->data == NULL){
        printf("\033[31mError\033[0m: data not allocated\n");
        return 0;
    }
    memcpy(newWavFile->data, wavFile.data, wavFile.header->dataSize);
    return 1;
}

void printWAVFileInfo(WAVFile wavFile){
    printHeader(*wavFile.header);
    printf("\033[1;34mfile Info:\n");
    printf("\033[1;32mDuration: %d seconds\n", wavFile.duration);
    printf("file Path: %s\n", wavFile.path);
    printf("\033[0m\n");
}

int getSmallestSample(WAVFile wavFile){
    int smallest = wavFile.data[0];
    int i = 0;
    for (i; i < wavFile.header->dataSize/sizeof(short); i++){
        if (wavFile.data[i] < smallest){
            smallest = wavFile.data[i];
        }
    }
    printf("%d", i);
    if (VERBOSE && !(i % 2)) printf("Smallest sample on R channel\n");
    else if (VERBOSE) printf("Smallest sample on L channel\n");
    return smallest;
}

int getBiggestSample(WAVFile wavFile){
    int biggest = wavFile.data[0];
    int i = 0;
    for (i; i < wavFile.header->dataSize/sizeof(short); i++){
        if (wavFile.data[i] > biggest){
            biggest = wavFile.data[i];
        }
    }
    printf("%d", i);
    if (VERBOSE && !(i % 2)) printf("Biggest sample on R channel\n");
    else if (VERBOSE) printf("Biggest sample on L channel\n");
    return biggest;
}

void reverseAudio(WAVFile *wavFile){
    
    int c = 0;
    
    tempWavData = (short *)calloc(wavFile->header->dataSize/sizeof(short), sizeof(short));
    
    if (tempWavData == NULL) {
        printf("\033[31mError\033[0m: data not allocated\n");
        return 0;
    }

    for(int i = wavFile->header->dataSize/sizeof(short); i >= 0; i--){
        tempWavData[c] = wavFile->data[i];
        c++; 
    }

    for(int i = 0; i < sizeof(tempWavData); i++){
        wavFile->data[i] = tempWavData[i];
    }
}

