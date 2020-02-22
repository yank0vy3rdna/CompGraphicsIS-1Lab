//
// Created by yank0vy3rdna on 23.02.2020.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <cstdio>
#include <string>
#include <cstring>
#include <sstream>
using namespace std;

using byte = unsigned char;

class Picture{
private:
    int Width, Height, ColourDepth;
    int Type;
    byte* PictureData;
public:

    //считывание метаданных
    /*static vector<byte> ReadBinary(char* path, uint_fast64_t length){
        ifstream infile(path, ios::binary);
        //проверка на открытие файла
        if (!infile){
            cout << "error input file" << endl;
            exit(1);
        }
        vector<byte> data(length);
        infile.read(reinterpret_cast<char*>(&data), length);
        return data;
    }*/

    //запись метаданных
    static void WriteBinary(char* path, char* vector, int countbytes){
        FILE *outfile = fopen(path, "w");

        if(outfile)
        {
            fwrite(vector,1,countbytes,outfile);
            fclose(outfile);
        }else{
            cout << "error output file" << endl;
            exit(1);
        }
    }

    //нахождение размера файла
    /*uint_fast64_t FileSize(char *path, error_code errorCode) {
        return 0;
    }*/

    bool Read(char* path){
        FILE *infile = fopen(path, "r");
        if (!infile){
            cout << "error input file" << endl;
            return -1;
        }
        fscanf(infile, "P%d\n%d %d\n%d\n", &this -> Type, &this -> Width, &this -> Height, &this -> ColourDepth);
        if (Type == 5) {
            PictureData = static_cast<byte *>(malloc(sizeof(byte) * Width * Height));
            fread(PictureData,1,Width*Height,infile);
        }else{
            PictureData = static_cast<byte *>(malloc(sizeof(byte) * Width * Height*3));
            fread(PictureData,1,Width*Height*3,infile);
        }
        return true;
    }


    explicit Picture(char *path){
        error_code ec{};
        if (ec != error_code{}) {
            cout << ec.message() << endl;
            exit(1);
        }
        Read(path);
        if (Type != 5 && Type != 6) {
            cout << "wrong type" << endl;
            exit(1);
        }
    }
    //запись метаданных в файл
    void Output(const char* path){
        //запись хэдера
        //buff.clear();
        //buff.reserve(1);
//        buff.push_back('P');
//        buff.push_back(char(Type + '0'));
//        buff.push_back('\n');
        stringstream ss;
        ss << Width;
        string WidthOut;
        ss >> WidthOut;
        ss.clear();
        ss << Height;
        string HeightOut;
        ss >> HeightOut;
        ss.clear();
        ss << ColourDepth;
        string ColourDepthOut;
        ss >> ColourDepthOut;
        ss.clear();
        int buff_size;
        if(Type==5){
            buff_size = WidthOut.size() + HeightOut.size() + ColourDepthOut.size() + Width * Height + 6;
        }else {
            buff_size = WidthOut.size() + HeightOut.size() + ColourDepthOut.size() + Width * Height * 3 + 6;
        }
        char buff[buff_size];
        buff[0] = 'P';
        buff[1] = char(Type + '0');
        buff[2] = '\n';
        int i = 3;
        for (auto c : WidthOut) {
            buff[i] = c;
            i += 1;
        }
        buff[i] = ' ';
        i += 1;
        for (auto c : HeightOut) {
            buff[i] = c;
            i += 1;
        }
        buff[i] = '\n';
        i += 1;
        for (auto c : ColourDepthOut) {
            buff[i] = c;
            i += 1;
        }
        buff[i] = ' ';
        i += 1;
        if(Type == 5) {
            for (int j = 0; j < Width * Height; j++) {
                buff[i] = PictureData[j];
                i += 1;
            }
        }else{
            for (int j = 0; j < Width * Height * 3; j++) {
                buff[i] = PictureData[j];
                i += 1;
            }
        }
        i = 0;
        //запись данных
        WriteBinary(const_cast<char *>(path), buff, sizeof(buff));
    }
    //реверс цвета
    void Inverse(){
        int countbytes;
        if (Type == 5){
            countbytes = Width*Height;
        }else{
            countbytes=Width*Height*3;
        }
        for (int i = 0; i < countbytes; i++) {
            PictureData[i] = ~PictureData[i];
        }
    }
    //отражение по гор/вер
    void Mirror(const char *direction){
        //по горизонтали
        if (direction[0] == 'H'){
            //для моно
            if (Type == 5){
                uint_fast64_t i,j;
                for (i = 0; i < Height; i++) {
                    for (j = 0; j < Width/2; j++) {
                        swap(PictureData[i * Width + j], PictureData[i * Width + Width - 1 - j]);
                    }
                }
            } else{//для ргб
                uint_fast64_t i,j;
                for (i = 0; i < Height; i++){
                    for (j = 0; j < Width*3/2; j+=3){
                        swap(PictureData[i * Width * 3 + j], PictureData[i * Width * 3 + Width * 3 - j - 3]);
                        swap(PictureData[i * Width * 3 + j + 1], PictureData[i * Width * 3 + Width * 3 - j - 2]);
                        swap(PictureData[i * Width * 3 + j + 2], PictureData[i * Width * 3 + Width * 3 - j - 1]);
                    }
                }
            }
        }
        //по вертикали
        if (direction[0] == 'V'){
            //для моно
            if (Type == 5){
                uint_fast64_t i,j;
                for (i = 0; i < Width; i++){
                    for (j = 0; j < Height/2; j++){
                        swap(PictureData[j * Width + i], PictureData[(Height - 1 - j) * Width + i]);
                    }
                }
            } else{//для ргб
                uint_fast64_t i,j;
                for (i = 0; i < Width*3; i++){
                    for (j = 0; j < Height/2; j++){
                        swap(PictureData[j * Width * 3 + i], PictureData[(Height - 1 - j) * Width * 3 + i]);
                    }
                }
            }
        }
    }
    //разворот
    void Rotation(const char *direction){
        //по часовой
        if (direction[0] == 'R'){
            //для моно
            if (Type == 5){
                byte* TurnedMetaData;
                uint_fast64_t TurnedWidth, TurnedHeight;
                TurnedHeight = Width;
                TurnedWidth = Height;
                TurnedMetaData = static_cast<byte *>(malloc(Width * Height));
                for (uint_fast64_t i = 0; i < Height; i++){
                    for (uint_fast64_t j = 0; j < Width; j++) {
                        TurnedMetaData[(TurnedWidth - i - 1) + j * TurnedWidth] = PictureData[i * Width + j];
                    }
                }
                Width = TurnedWidth;
                Height = TurnedHeight;
                PictureData = TurnedMetaData;
            } else{//для ргб
                byte* TurnedMetaData;
                uint64_t TurnedWidth, TurnedHeight;
                TurnedHeight = Width;
                TurnedWidth = Height;
                TurnedMetaData = static_cast<byte *>(malloc(Width * Height * 9));
                for (uint64_t i = 0; i < Height; i++) {
                    for (uint64_t j = 0; j < Width; j++) {
                        TurnedMetaData[j * 3 + i * TurnedWidth * 3] = PictureData[(Height - 1 - j) * Width * 3 + i * 3];
                        TurnedMetaData[j * 3 + 1 + i * TurnedWidth * 3] = PictureData[(Height - 1 - j) * Width * 3 + i * 3 + 1];
                        TurnedMetaData[j * 3 + 2 + i * TurnedWidth * 3] = PictureData[(Height - 1 - j) * Width * 3 + i * 3 + 2];
                    }
                }
                Width = TurnedWidth;
                Height = TurnedHeight;
                PictureData = TurnedMetaData;
            }
        }
        //против часовой
        if (direction[0] == 'L'){
            //для моно
            if (Type == 5){
                byte* TurnedMetaData;
                uint64_t TurnedWidth, TurnedHeight;
                TurnedHeight = Width;
                TurnedWidth = Height;
                TurnedMetaData = static_cast<byte *>(malloc(Width * Height));
                for (uint64_t i = 0; i < Height; i++){
                    for (uint64_t j = 0; j < Width; j++){
                        TurnedMetaData[j * TurnedWidth + i] = PictureData[(i + 1) * Width - 1 - j];
                    }
                }
                Width = TurnedWidth;
                Height = TurnedHeight;
                PictureData = TurnedMetaData;
            } else{//для ргб
                byte* TurnedMetaData;
                uint64_t TurnedWidth, TurnedHeight;
                TurnedHeight = Width;
                TurnedWidth = Height;
                TurnedMetaData = static_cast<byte *>(malloc(Width * Height * 9));
                for (uint64_t i = 0; i < Height; i++) {
                    for (uint64_t j = 0; j < Width; j++) {
                        TurnedMetaData[(TurnedHeight - 1 - j) * TurnedWidth * 3 + i * 3] = PictureData[i * Width * 3 + j * 3];
                        TurnedMetaData[(TurnedHeight - 1 - j) * TurnedWidth * 3 + i * 3 + 1] = PictureData[i * Width * 3 + j * 3 + 1];
                        TurnedMetaData[(TurnedHeight - 1 - j) * TurnedWidth * 3 + i * 3 + 2] = PictureData[i * Width * 3 + j * 3 + 2];
                    }
                }
                Width = TurnedWidth;
                Height = TurnedHeight;
                PictureData = TurnedMetaData;
            }
        }
    }
};