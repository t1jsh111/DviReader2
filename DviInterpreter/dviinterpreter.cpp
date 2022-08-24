#include "dviinterpreter.h"

#include <cstdint>
#include <bitset>
#include <QDebug>


DviInterpreter::DviInterpreter(std::string filePath, double pointsPerInchX, double pointsPerInchY,
                               double pixelsPerInchX, double pixelsPerInchY) :
    pointsPerInchX(pointsPerInchX), pointsPerInchY(pointsPerInchY), pixelsPerInchX(pixelsPerInchX), pixelsPerInchY(pixelsPerInchY)
{
    dviStream.open(filePath, std::ios::binary);

    if(!dviStream){
        throw "File not found!!!";
    }

}

DviFile DviInterpreter::readDvi(){

    bool readingFile = true;
    bool inPage = false;
    int value;
    while(readingFile){
        value = readNextBytes(1);

        executeCommandByValue(value, readingFile, inPage);

    }
    return file;
}


//Function to read a certain number of bytes and return it as a decimal
int DviInterpreter::readNextBytes(int numberOfBytes){
    if(numberOfBytes > 4){
        throw "Can convert up to 4 bytest to decimal number";
    }
    int value = 0;
    for(int i = 0; i < numberOfBytes; i++){
        unsigned char cValue;
        dviStream.read((char* ) &cValue, 1);
        value = value << 8 | cValue;
    }


    return value;
}

//Function to read a certain number of bytes and return it as a decimal
//https://www.youtube.com/watch?v=4qH4unVtJkE
int DviInterpreter::readNextBytesComplement(int numberOfBytes){
    uint32_t ret = uint32_t(dviStream.get());

    if ((ret & 128) == 128){        // negative value?
        ret = ret | 0xffffff00; // bitwise or
    }

    for(int i = 1; i < numberOfBytes; i++){
        ret = (ret << 8) | dviStream.get();
    }
    return int32_t(ret);
}



std::string DviInterpreter::stringOf(int numberOfBytes){
    std::string returnString = "";
    for(int i = 0; i < numberOfBytes; i++){
        returnString = returnString + ((char) readNextBytes(1));
    }
    return returnString;
}

void DviInterpreter::skipNextBytes(int numberOfBytes){
    for(int i = 0; i < numberOfBytes; i++){
        unsigned char cValue;
        dviStream.read((char* ) &cValue, 1);
    }
}





void DviInterpreter::executeCommandByValue(int value, bool& readingFile, bool& inPage){
    if(valueInClosedRange(value, 0, 128)){
        set_char_i(value);
    }
    else if(valueInClosedRange(value, 128, 132)){
        seti(value);
    }

    else if(valueInClosedRange(value, 132, 133)){

        set_rule();
    }
    else if(valueInClosedRange(value, 133, 137)){

        puti(value);
    }
    else if(valueInClosedRange(value, 137, 138)){
        put_rule();
    }
    else if(valueInClosedRange(value, 138, 139)){
        nop();
    }
    else if(valueInClosedRange(value, 139, 140)){
        bop(inPage);
    }
    else if(valueInClosedRange(value, 140, 141)){
        eop(inPage);
    }
    else if(valueInClosedRange(value, 141, 142)){
        push();
    }
    else if(valueInClosedRange(value, 142, 143)){
        pop();
    }
    else if(valueInClosedRange(value, 143, 147)){
        righti(value);
    }
    else if(valueInClosedRange(value, 147, 152)){
        wi(value);
    }
    else if(valueInClosedRange(value, 152, 157)){
        xi(value);
    }
    else if(valueInClosedRange(value, 157, 161)){
        downi(value);
    }
    else if(valueInClosedRange(value, 161, 166)){
        yi(value);
    }
    else if(valueInClosedRange(value, 166, 171)){
        zi(value);
    }
    else if(valueInClosedRange(value, 171, 235)){
        fnt_num_i(value);
    }
    else if(valueInClosedRange(value, 235, 239)){
        fnti(value);
    }
    else if(valueInClosedRange(value, 239, 243)){
        xxxi(value);
    }
    else if(valueInClosedRange(value, 243, 247)){
        fnt_defi(value);
    }
    else if(valueInClosedRange(value, 247, 248)){

        pre();

    }
    else if(valueInClosedRange(value, 248, 249)){
        post();
    }
    else if(valueInClosedRange(value, 249, 250)){
        post_post(readingFile);
    }
    else if(valueInClosedRange(value, 250, 256)){
        undefined();
    }
    else{
        throw "Cannot match value to command";
    }


}



bool DviInterpreter::valueInClosedRange(int value, int a, int b){
    //std::cout << "Value is:" << value;
    return (a <= value && value < b);
}

Character DviInterpreter::valueToCharacter(int value)
{
    return Character(value, writeHead.h, writeHead.v, currentFont);
}



