#pragma once

#include<vector>

#include<iostream>

#include<fstream>

#include<string>

#include<cmath>

using namespace std;

struct Color {

    float r, g, b;

};

class Image {

public:

    Image(int width, int height);

    ~Image();

    void CreateBMP(const char* path, unsigned char* numB, int v, int vDob);

private:

    int m_width;

    int m_height;

    std::vector<Color> m_colors;

};

Image::Image(int width, int height)

    :m_width(width), m_height(height), m_colors(std::vector<Color>(width* height))

{

}

Image::~Image()

{

}

void Image::CreateBMP(const char* path, unsigned char* numB, int v, int vDob) {

    std::ofstream f;

    f.open(path, std::ios::out | std::ios::binary);

    if (!f.is_open()) {

        std::cout << "Filr could not be opened\n";

        return;

    }

    unsigned char bmpPad[3] = { 0, 0, 0 };

    const int paddingAmout = ((4 - (m_width * 3) % 4) % 4);

    const int fileHeaderSize = 14;

    const int informationHeaderSize = 40;

    const int fileSize = fileHeaderSize + informationHeaderSize + m_width * m_height * 3 + paddingAmout * m_width;

    unsigned char fileHeader[fileHeaderSize] = { 0 };

    //File type

    fileHeader[0] = 0x42; // "B";

    fileHeader[1] = 0x4D; // "M";

    //File size

    fileHeader[2] = fileSize;

    fileHeader[3] = fileSize >> 8;

    fileHeader[4] = fileSize >> 16;

    fileHeader[5] = fileSize >> 24;

    //Reserved

    fileHeader[6] = vDob;

    fileHeader[7] = vDob >> 8;

    fileHeader[8] = vDob >> 16;

    fileHeader[9] = vDob >> 24;

    //Pixel data offset

    fileHeader[10] = fileHeaderSize + informationHeaderSize;

    fileHeader[11] = 0;

    fileHeader[12] = 0;

    fileHeader[13] = 0;

    unsigned char informationHeader[informationHeaderSize] = { 0 };

    //HeaderSize

    informationHeader[0] = informationHeaderSize;

    informationHeader[1] = 0;

    informationHeader[2] = 0;

    informationHeader[3] = 0;

    //Image width

    informationHeader[4] = m_width;

    informationHeader[5] = m_width >> 8;

    informationHeader[6] = m_width >> 16;

    informationHeader[7] = m_width >> 24;

    //Image height

    informationHeader[8] = m_height;

    informationHeader[9] = m_height >> 8;

    informationHeader[10] = m_height >> 16;

    informationHeader[11] = m_height >> 24;

    //Planes

    informationHeader[12] = 1;

    informationHeader[13] = 0;

    //Bits per pixel (RGB)

    informationHeader[14] = 24;

    informationHeader[15] = 0;

    //Compresssion (No compression)

    informationHeader[16] = 0;

    informationHeader[17] = 0;

    informationHeader[18] = 0;

    informationHeader[19] = 0;

    //image size (No compression)

    informationHeader[20] = 0;

    informationHeader[21] = 0;

    informationHeader[22] = 0;

    informationHeader[23] = 0;

    // X-pixels per meter (not specified)

    informationHeader[24] = 0;

    informationHeader[25] = 0;

    informationHeader[26] = 0;

    informationHeader[27] = 0;

    // Y-pixels per meter (not specified)

    informationHeader[28] = 0;

    informationHeader[29] = 0;

    informationHeader[30] = 0;

    informationHeader[31] = 0;

    // Total colors(color palette not used)

    informationHeader[32] = 0;

    informationHeader[33] = 0;

    informationHeader[34] = 0;

    informationHeader[35] = 0;

    // Important colors(generally ignored)

    informationHeader[36] = 0;

    informationHeader[37] = 0;

    informationHeader[38] = 0;

    informationHeader[39] = 0;

    f.write(reinterpret_cast<char*>(fileHeader), fileHeaderSize);

    f.write(reinterpret_cast<char*>(informationHeader), informationHeaderSize);

    int d = 0x00;

    int n;

    int poz = 0;

    cout << "V== " << v << endl;

    cout << "paddingAmout = " << paddingAmout << endl;

    for (int y = 1; y <= m_height; y++) {

        for (int x = 1; x <= ((m_width)); x++) {

            poz = (y - 1) * m_width + x;

            if (poz <= v) {

                n = numB[poz];

                for (int c = 0; c < 3; c++) {

                    f.write(reinterpret_cast<char*>(&numB[poz]), 1);

                }

            }

        }

        f.write(reinterpret_cast<char*>(bmpPad), paddingAmout);

    }

    f.close();

    std::cout << "File created\n";

}

void Grafika(const char* path) {

    ifstream inf(path, ios::binary);

    if (!inf)

    {

        cerr << "ERROR\n";

        exit(1);

    }

    inf.seekg(0, ios::end); // перемещаемся в конец файла

    int filesize;

    filesize = inf.tellg();

    std::cout << "FILESIZE = " << filesize << endl;

    const int width = 1200; // ширина картнки, пикселей

    const int height = ceil((float)filesize / width);

    inf.seekg(0, ios::beg);

    unsigned char ch;

    unsigned char* numB;

    int v;

    v = (width + ((4 - (width * 3) % 4) % 4)) * height;

    int vDob = int(width * height - filesize);

    cout << "height = " << height << endl;

    numB = new unsigned char[v];

    unsigned int i = v;

    for (i; i > filesize; i--) {

        numB[i] = 0xFF;

    }

    while (!inf.eof())

    {

        ch = inf.get();

        numB[i] = ch;

        if (i > 0)

            i--;

    }

    inf.close();

    Image image(width, height);

    image.CreateBMP("/Users/a1234/CLionProjects/untitled/bmpag/cmake-build-debug/imageBMP.bmp", numB, v, vDob);
}

void RestoreFileOut(const char* pathin)

{
    ifstream infout;

    infout.open(pathin, std::ios::in | ios::binary);

    if (!infout)

    {

        cerr << "ERROR\n";

        exit(1);

    }

    infout.seekg(0, ios::beg);

    const int strTypeSize = 14;

    unsigned char strType[strTypeSize];

    const int strOutSize = 40;

    unsigned char strOut[strOutSize];

    infout.read(reinterpret_cast<char*>(strType), strTypeSize);

    infout.read(reinterpret_cast<char*>(strOut), strOutSize);

    int fileSize1 = strType[2] + (strType[3] << 8) + (strType[4] << 16) + (strType[5] << 24);

    int m_width = strOut[4] + (strOut[5] << 8) + (strOut[6] << 16) + (strOut[7] << 24);

    int m_height = strOut[8] + (strOut[9] << 8) + (strOut[10] << 16) + (strOut[11] << 24);

    cout << "fileSize1 = " << fileSize1 << endl;

    cout << "m_width = " << m_width << endl;

    cout << " m_height = " << m_height << endl;

    int dob1 = strType[6] + (strType[7] << 8) + (strType[8] << 16) + (strType[9] << 24);

    int raz1 = strType[7];

    cout << "dob1 = " << dob1 << endl;

    //cout << raz1 << endl;

    const int paddingAmOut = ((4 - (m_width * 3) % 4) % 4);

    ofstream fileRest;

    fileRest.open("/Users/a1234/CLionProjects/untitled/bmpag/cmake-build-debug/fileRest.jpg", std::ios::out | std::ios::binary); //Восстановленный файл

    int m_widthPad = m_width * 3 - paddingAmOut;

    const int fS = (m_width * m_height) - dob1;

    cout << "fS = " << fS << endl;

    cout << "paddingAmOut = " << paddingAmOut << endl;

    //cout << "fS = " << fS << endl;

    unsigned char* numR;

    numR = new unsigned char[fS];

    // unsigned char numR;

    unsigned char ch1;

    int nc1 = 1;

    cout << "+++" << (m_height) << endl;

    for (int y = 0; y < m_height; y++) {

        for (int x = 1; x <= (m_width * 3 + paddingAmOut); x++)

        {

            ch1 = infout.get();

            if ((x % 3 == 0) & (x <= (m_width * 3)) & (nc1 <= fS))

            {

                nc1 = (y * m_width) + x / 3;

                numR[nc1] = ch1;

                ++nc1;

            }

        }

    }

    for (int n = fS; n > 0; n--)

        fileRest.write(reinterpret_cast<char*>(&numR[n]), 1);

    std::cout << "File created\n";

    infout.close();

    fileRest.close();
}

int main(int argc, char* argv[])

{
    //Grafika("d:/nika.jpg");
    //RestoreFileOut("d:/imageBMP.bmp");

        int nR = argc;

        char* nA1 = argv[1];

        if (nR <  3) {
            cout << "Неверное количество аргументов. Используйте: <имя программы>.exe -e/-encode <имя файла> или <имя программы>.exe -d/-decode <имя картинки>\n";

            return 1;

        }


        if (strcmp(nA1, "-e") == 0 || strcmp(nA1, "-encode") == 0)

        {
            Grafika(argv[2]);

        }

        else if (strcmp(nA1, "-d") == 0 || strcmp(nA1, "-decode") == 0) {
            RestoreFileOut(argv[3]);

        }
        else {
            cout << "Неверная команда. Использование: " << argv[0] << " -e/-encode <входной_файл> или"  << argv[0] << " -d/-decode <сжатый_файл>" << endl;

            return 1;

        }

    return 0;

}