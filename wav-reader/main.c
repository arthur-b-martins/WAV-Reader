/*
    Arquivo principal do programa, responsável por chamar as funções de leitura e escrita de arquivos WAV.
    O programa lê um arquivo WAV, inverte os canais e escreve o arquivo invertido em um diretório especificado.
    Autores:
        _Davi Santos Oliveira (12411BCC088)
        _Arthur Borges Martins (12411BCC063)
*/

char *inputFilePath;
char *outputfolder;
short VERBOSE = 1;

#include "readWAV.h"

#define INPUT_FILE_TAG 1
#define OUTPUT_FOLDER_TAG 2
#define VERBOSE_TAG 3

short getTags(char* tag){
    if (strcmp(tag, "-i") == 0){
        return 1;
    }
    if (strcmp(tag, "-o") == 0){
        return 2;
    }
    if(strcmp(tag, "-v") == 0){
        return 3;
    }
    return 0;
}

int main(int argc, char *argv[]){

    // verifica se o número de argumentos é suficiente
    if (argc == 1){
        printf("Execucao invalida\n");
        printf("digite main.exe -h para receber ajuda\n");
        return 1;
    }
    
    // verifica se o usuário quer ajuda
    if (!strcmp(argv[1], "-h"))
    {
        if(argc == 2){
            printf("Uso: -i <inputFilePath> -o <outputFolder> -v <(0 or 1)>\n");
            printf("caso precise de ajuda em uma tag especifica digite: -h <tag>\n");
            printf("para ver as tags, digite: -h tags\n");
        }

        if (strcmp(argv[2], "tags") == 0)
        {
            printf("Tags:\n");
            printf("-i: input file path\n");
            printf("-o: output folder\n");
            printf("-v: verbose\n");
        }else if (strcmp(argv[2], "-i") == 0){
            printf("Tag -i: input file path\n");
            printf("Essa tag e responsavel por definir o caminho do arquivo de entrada\n");
        }else if (strcmp(argv[2], "-o") == 0){
            printf("Tag -o: output folder\n");
            printf("Essa tag e responsavel por definir o caminho da pasta de saida\n");
        }else if (strcmp(argv[2], "-v") == 0){
            printf("Tag -v: verbose\n");
            printf("Essa tag e responsavel por definir se o programa ira imprimir informacoes (as funcoes print nao sao afetadas)\n");
            printf("0: nao imprime\n");
            printf("1: imprime\n");
        }
        return 1;
    }
    
    short iTagFound = 0;
    short oTagFound = 0;

    for (int i = 1; i < argc; i+=2)
    {
        short tag = getTags(argv[i]);
        switch (tag)
        {
        case INPUT_FILE_TAG:
            inputFilePath = argv[i+1];
            iTagFound = 1;
            break;
        
        case OUTPUT_FOLDER_TAG:
            outputfolder = argv[i+1];
            oTagFound = 1;
            break;

        case VERBOSE_TAG:
            VERBOSE = atoi(argv[i+1]);
            break;
        default:
            printf("Invalid tag\n");
            return 0;
        }   
    }


    if (!iTagFound){
        printf("\033[31mError\033[0m: caminho para o arquivo de leitura nao especificado\n");
        return 0;
    }
    if (!oTagFound){
        printf("\033[31mError\033[0m: caminho para a pasta de output nao especificado\n");
        return 0;
    }

    WAVFile wavFile, invertedWavFile, reverseWavFile;

    if (!readWAV(inputFilePath, &wavFile)){
        printf("\033[31mError\033[0m: could not read WAV file\n");
        return 0;
    }
    
    printWAVFileInfo(wavFile);
    
    // copia o arquivo de entrada para um novo arquivo Tarefa 2
    free(wavFile.path);

    char *tempPath = (char *)malloc(strlen(outputfolder) + strlen("\\audioCopiado.wav") + 1);
    if (tempPath == NULL) {
        printf("\033[31mError\033[0m: memory allocation failed\n");
        return 0;
    }

    strcpy(tempPath,outputfolder);
    strcat(tempPath, "\\audioCopiado.wav");
    
    wavFile.path = (char *)malloc(strlen(tempPath) + 1);
    strcpy(wavFile.path, tempPath);
    writeWAV(&wavFile);



    // create an inverted audio
    copyWAVFile(wavFile, &invertedWavFile);
    invertSamples(&invertedWavFile);

    // create a reverse audio 
    copyWAVFile(wavFile, &reverseWavFile);
    reverseAudio(&reverseWavFile);

    // define the inverted audio path 
    tempPath = (char *)realloc(tempPath, (strlen(outputfolder) + strlen("\\invertedAudio.wav") + 1));
    if (tempPath == NULL) {
        printf("\033[31mError\033[0m: memory allocation failed\n");
        return 0;
    }

    free(invertedWavFile.path);
    invertedWavFile.path = (char *)malloc(strlen(tempPath) + 1);
    if(invertedWavFile.path == NULL){
        printf("\033[31mError\033[0m: path not allocated\n");
        return 0;
    }else{
        strcpy(tempPath, outputfolder);
        strcat(tempPath, "\\invertedAudio.wav");
        strcpy(invertedWavFile.path, tempPath);
    }

    writeWAV(&invertedWavFile);

    // define the reverse audio path 
    tempPath = (char *)realloc(tempPath, (strlen(outputfolder) + strlen("\\reverseAudio.wav") + 1));
    if (tempPath == NULL) {
        printf("\033[31mError\033[0m: memory allocation failed\n");
        return 0;
    }

    reverseWavFile.path = (char *)malloc(strlen(tempPath) + 1);
    if(reverseWavFile.path == NULL){
        printf("\033[31mError\033[0m: path not allocated\n");
        return 0;
    }else{
        strcpy(tempPath, outputfolder);
        strcat(tempPath, "\\reverseAudio.wav");
        strcpy(reverseWavFile.path, tempPath);
    }

    writeWAV(&reverseWavFile);
    free(tempPath);
    

    // imprime o menor e o maior valor de amostra do arquivo de entrada (caso VERBOSE seja 1, o que é o padrão, imprime também qual canal é o menor e qual é o maior) Tarefa 3
    int smallest = getSmallestSample(wavFile);
    int biggest = getBiggestSample(wavFile);
    printf("\033[1;32mSmallest sample: %d\n", smallest);
    printf("Biggest sample: %d\n\033[0m", biggest);
    smallest = getSmallestSample(invertedWavFile);
    biggest = getBiggestSample(invertedWavFile);
    printf("\033[1;32mSmallest sample: %d\n", smallest);
    printf("Biggest sample: %d\n\033[0m", biggest);
    // libera a memória alocada

    free(invertedWavFile.path);
    free(invertedWavFile.header);
    free(invertedWavFile.data);
    free(wavFile.path);
    free(wavFile.header);
    free(wavFile.data);
    return 1;
}