//
//  DVIReaderHelper.cpp
//  DVITool
//
//  Created by Tijs Hoeijmakers on 14/03/2020.
//  Copyright © 2020 Tijs Hoeijmakers. All rights reserved.
//

#include <stdio.h>
#include "dviinterpreter.h"
#include <QDebug>
#include <memory>
#include <cstdio>
#include <utility>

//#define DEBUG_FUNCTIONS

bool debugging = false;

//TODO fix parameters!!
void DviInterpreter::set_char_i(int value){

    auto character = valueToCharacter(value);
    page->addCharacter(character);

    writeHead.h += character.getSpAdvancement();
    #ifdef DEBUG_FUNCTIONS
        std::cout << "set_char_i("<<value <<")" << ":" << character.letter << std::endl;
    #endif
}

/*
 Pre: Value in [128, 131]
 */
void DviInterpreter::seti(int value){
    if(!(128 <= value && value < 132)){
        throw "Precondition harmed";
    }
    if(129 <= value && value < 132){
        throw "Too complex character";
    }

    int c = readNextBytes(1);
    //again fix parameter


    auto character = valueToCharacter(c);
    page->addCharacter(character);
    writeHead.h += character.getSpAdvancement();

    #ifdef DEBUG_FUNCTIONS
        std::cout << "set_i("<<value <<")" << ":" << character.letter << "increase by width" << std::endl;
    #endif
}

void DviInterpreter::set_rule(){
    int height = readNextBytes(4);
    int width = readNextBytes(4);
    Rectangle rectangle(height, width, writeHead.h, writeHead.v);
    page->addRectangle(rectangle);

    //increase pointer head
    writeHead.h += width;
    #ifdef DEBUG_FUNCTIONS
        std::cout << "set_rule()" << ": rectangle of h:" << height << " w: " << width << " increase by width "<< std::endl;
    #endif
}

void DviInterpreter::puti(int value){
    int c = readNextBytes(value % 132);

    auto character = valueToCharacter(c);

    page->addCharacter(character);
    #ifdef DEBUG_FUNCTIONS
        std::cout << "puti("<< value <<")" << ":" << character.letter << "not move" << std::endl;
    #endif
}

void DviInterpreter::put_rule(){
    int height = readNextBytes(4);
    int width = readNextBytes(4);
    Rectangle rectangle(height, width, writeHead.h, writeHead.v);
    page->addRectangle(rectangle);
    #ifdef DEBUG_FUNCTIONS
        std::cout << "put_rule()" << ": rectangle of h:" << height << " w: " << width << std::endl;
    #endif
}

void DviInterpreter::nop(){
    //No Operation
    #ifdef DEBUG_FUNCTIONS
        std::cout << "nop()" << std::endl;
    #endif

}

void DviInterpreter::bop(bool& inPage){
    page.reset(new DVIPage);
    inPage = true;
    //The page identifier array is filled
    for(int i = 0; i < 10; i++){
        page->setPageIdentifier(i, readNextBytes(4));
    }

    //Set head pointer
    writeHead.set(0,0,0,0,0,0);

    //Ignore previous page pointer
    skipNextBytes(4);
    #ifdef DEBUG_FUNCTIONS
        std::cout << "bop()" << std::endl;
    #endif
}

void DviInterpreter::eop(bool& inPage){
    file.dviPages.push_back(*page);
    inPage = false;
    #ifdef DEBUG_FUNCTIONS
        std::cout << "eop()" << std::endl;
    #endif
}

void DviInterpreter::push(){
    writeHeadStack.push(writeHead);
    #ifdef DEBUG_FUNCTIONS
        std::cout << "push()" << std::endl;
    #endif
}
void DviInterpreter::pop(){
    if(writeHeadStack.empty()){throw "incorrect stack maintenance";}
    writeHead = writeHeadStack.top();
    writeHeadStack.pop();
    #ifdef DEBUG_FUNCTIONS
        std::cout << "pop()" << std::endl;
    #endif
}
void DviInterpreter::righti(int value){
    int unitsToRight = readNextBytesComplement(value % 142);
    writeHead.h = writeHead.h + unitsToRight;
    #ifdef DEBUG_FUNCTIONS
        std::cout << "right_i(" << value << ")"<< ":" << "move " <<unitsToRight << "to the right" << std::endl;
    #endif
}

//Pre value in [147, 152)
void DviInterpreter::wi(int value){
    if(!(147 <= value && value < 152)){
        throw "Precondition harmed";
    }
    //Do not change w
    if(value == 147){
        writeHead.h = writeHead.h + writeHead.w;
    }
    else if(148 <= value && value < 152){
        int unitsToRight = readNextBytesComplement(value % 147);
        writeHead.w = unitsToRight;
        writeHead.h = writeHead.h + unitsToRight;
    }
    #ifdef DEBUG_FUNCTIONS
        std::cout << "wi(" << value << ")"<< ":" << std::endl;
    #endif
}

//Pre value in [152, 157)
void DviInterpreter::xi(int value){
    if(!(152 <= value && value < 157)){
        throw "Precondition harmed";
    }
    if(152 == value){
        writeHead.h = writeHead.h + writeHead.x;

    }
    else if(153 <= value && value < 157){
        int unitsToRight = readNextBytesComplement(value % 152);
        writeHead.x = unitsToRight;
        writeHead.h = writeHead.h + writeHead.x;
    }
    #ifdef DEBUG_FUNCTIONS
        std::cout << "xi(" << value << ")"<< ":" << std::endl;
    #endif
}

void DviInterpreter::downi(int value){
    int unitsDown = readNextBytesComplement(value % 156);
    writeHead.v = writeHead.v + unitsDown;
    #ifdef DEBUG_FUNCTIONS
        std::cout << "downi(" << value << ")"<< ":" << "down by " << unitsDown << std::endl;
    #endif
}

void DviInterpreter::yi(int value){
    if(161 == value){
        writeHead.v = writeHead.v + writeHead.y;

    }
    //Set y and move down by y
    else if(162 <= value && value < 166){
        int unitsDown = readNextBytesComplement(value % 161);
        writeHead.y = unitsDown;
        writeHead.v = writeHead.v + writeHead.y;
    }
    #ifdef DEBUG_FUNCTIONS
        std::cout << "yi(" << value << ")"<< ":" << std::endl;
    #endif
}

void DviInterpreter::zi(int value){
    if(166 == value){
        writeHead.v = writeHead.v + writeHead.z;
    }
    //Set z and down by z
    else if(167 <= value && value < 171){
        int unitsDown = readNextBytesComplement(value % 166);
        writeHead.z = unitsDown;
        writeHead.v = writeHead.v + writeHead.z;
    }
    #ifdef DEBUG_FUNCTIONS
        std::cout << "zi(" << value << ")"<< ":" << std::endl;
    #endif
}

void DviInterpreter::fnt_num_i(int value){
    currentFont = file.definedFonts.at(value % 171);
    #ifdef DEBUG_FUNCTIONS
        std::cout << "fnt_num_i(" << value << ")"<< ":" << std::endl;
    #endif
}

void DviInterpreter::fnti(int value){
    currentFont = file.definedFonts.at(readNextBytes(value % 234));
    #ifdef DEBUG_FUNCTIONS
        std::cout << "fnt_i(" << value << ")"<< ":" << std::endl;
    #endif
}

void DviInterpreter::xxxi(int value){
    int k = readNextBytes(value % 238);
    skipNextBytes(k);
    #ifdef DEBUG_FUNCTIONS
        std::cout << "xxxi(" << value << ")"<< ":" << std::endl;
    #endif
}

void DviInterpreter::fnt_defi(int value){
    int fontNr = readNextBytes(value % 242);


    int checksum = readNextBytes(4);
    int scaleFactor = readNextBytes(4);
    int designSize = readNextBytes(4);

    int directoryLength = readNextBytes(1);
    int fontNameLength = readNextBytes(1);

    std::string directory = stringOf(directoryLength);
    std::string fontName = stringOf(fontNameLength);

    int mag = this->file.dviFilePre.mag;

    std::unique_ptr<FontDefinition> fontDef(new FontDefinition(checksum, scaleFactor, designSize, mag, directory, fontName));

    file.definedFonts[fontNr] = std::move(fontDef);
    #ifdef DEBUG_FUNCTIONS
        std::cout << "fnt_defi(" << value << ")"<< ":" << std::endl;
    #endif
}

void DviInterpreter::pre(){
    int versionNumber = readNextBytes(1);

    if(2 <= versionNumber  && versionNumber < 4){

        file.dviFilePre.versionNumber = versionNumber;

    }

    file.dviFilePre.num = readNextBytes(4);
    file.dviFilePre.den = readNextBytes(4);
    file.dviFilePre.mag = readNextBytes(4);

    int k = readNextBytes(1);
    std::string comment = stringOf(k);
    file.dviFilePre.comment = comment;
    #ifdef DEBUG_FUNCTIONS
        std::cout << "pre()" << std::endl;
    #endif
}

void DviInterpreter::post(){
    //ignore pointer
    skipNextBytes(4);
    //ignore repetitive num,den,mag
    skipNextBytes(12);
    //
    file.dviFilePost.maxv = readNextBytes(4);
    file.dviFilePost.maxh = readNextBytes(4);
    file.dviFilePost.maxStackDepth = readNextBytes(2);
    file.dviFilePost.pages = readNextBytes(2);
    #ifdef DEBUG_FUNCTIONS
        std::cout << "post()" << std::endl;
    #endif
}
void DviInterpreter::post_post(bool& readingFile){

    //Ignore pointer of postamble, and identification byte i.


    readingFile = false;
    #ifdef DEBUG_FUNCTIONS
        std::cout << "post_post()" << std::endl;
    #endif
}
void DviInterpreter::undefined(){
    //Undefined command
    #ifdef DEBUG_FUNCTIONS
        std::cout << "undefined()" << std::endl;
    #endif
}
