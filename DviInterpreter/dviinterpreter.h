#ifndef DVIINTERPRETER_H
#define DVIINTERPRETER_H

#include "DviInterpreter_global.h"

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <stack>
#include "dvifile.h"
#include "ftfontmanager.h"
#include "fontdefinition.h"


class DVIINTERPRETER_EXPORT DviInterpreter{
public:
    DviInterpreter(std::string filePath, double pointsPerInchX, double pointsPerInchY,
                   double pixelsPerInchX, double pixelsPerInchY);
    DviFile readDvi();
private:
    std::ifstream dviStream;
    DviFile file;
    std::unique_ptr<DVIPage> page;


    //
    struct WriteHead{
        // Values are all in scaled points
        int h; // horizontal position of write head
        int v; // vertical position of write head
        int w; // variable sometimes used to move in horizontal direction
        int x; // variable sometimes used to move in horizontal direction
        int y; // varialbe sometimes used to move in vertical direction
        int z; // variable sometimes used to move in vertical direction
        WriteHead(int h, int v, int w, int x, int y, int z) : h(h), v(v), w(w), x(x), y(y), z(z){}
        void set(int h, int v, int w, int x, int y, int z) {
            this->h = h; this->v = v; this->w = w; this->x = x; this->y = y; this->z = z;
        }
    };
    std::stack<WriteHead> writeHeadStack;
    WriteHead writeHead{0,0,0,0,0,0};


    // foint that the interpreter is currently uses
    std::shared_ptr<FontDefinition> currentFont = nullptr;

    //Read data from file
    int readNextBytes(int numberOfBytes);

    // sometimes bytes need to be ignored
    void skipNextBytes(int numberOfBytes);

    // sometimes bytes need to be interpreted as complement values
    int readNextBytesComplement(int numberOfBytes);

    // executes a command on the basis of the value read (this involves moving write head etc. accordingly).
    void executeCommandByValue(int value, bool& readingFile, bool& inPage);

    // reads a string from dvi file of numberOfBytes size.
    std::string stringOf(int numberOfBytes);

    //Helper functions
    void set_char_i(int value);
    void seti(int value);
    void set_rule();
    void puti(int value);
    void put_rule();
    void nop();
    void bop(bool& inPage);
    void eop(bool& inPage);

    void push();
    void pop();

    void righti(int value);
    void wi(int value);
    void xi(int value);

    void downi(int value);
    void yi(int value);
    void zi(int value);

    void fnt_num_i(int value);
    void fnti(int value);
    void xxxi(int value);
    void fnt_defi(int value);
    void pre();
    void post();
    void post_post(bool& readingFile);
    void undefined();


private:


    double pointsPerInchX;
    double pointsPerInchY;

    double pixelsPerInchX;
    double pixelsPerInchY;

    bool valueInClosedRange(int value, int a, int b);

    // Creates from value read, a character (using position of writehead)
    Character valueToCharacter(int value);

    FTFontManager& fontManager = FTFontManager::getInstance();


};

#endif // DVIINTERPRETER_H
