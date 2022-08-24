#include "dvifile.h"


void DviFile::printDvi(){
    std::cout << "----DVI PRE---" << std::endl;
    std::cout << "id: " << dviFilePre.versionNumber << std::endl;
    std::cout << "numerator: " << dviFilePre.num << std::endl;
    std::cout << "denominator: " << dviFilePre.den << std::endl;
    std::cout << "magnification: " << dviFilePre.mag << std::endl;
    std::cout << "Comment: " << dviFilePre.comment << std::endl;


    std::cout << "----DVI POST---" << std::endl;
    std::cout << "maxv: " << dviFilePost.maxv << std::endl;
    std::cout << "maxh: " << dviFilePost.maxh << std::endl;
    std::cout << "maxs: " << dviFilePost.maxStackDepth << std::endl;
    std::cout << "pages: " << dviFilePost.pages << std::endl;

    std::cout << "----Defined Fonts----" << std::endl;
    for(const auto& font : definedFonts){

        std::cout << "----" << font.second->fontName << "----" << std::endl;
        std::cout << "scale: " << font.second->scaleFactor << "design: " << font.second->designSize << std::endl;
        std::cout << "represented" << (double) (dviFilePre.mag * font.second->scaleFactor) / (1000 * font.second->designSize) << std::endl;
        std::cout << "Font directory: " << font.second->directory << std::endl;
        double fontPoints = (font.second->designSize / 65536.0);
        double printSize =  (font.second->scaleFactor / 65536.0);
        std::cout << "Font name: " << font.second->fontName << "(" << fontPoints << ") at " <<  printSize << std::endl;

    }

    std::cout << "----Pages----" << std::endl;
    for(auto page : dviPages){
        std::cout << "--Page--" << std::endl;
        const auto& characters = page.getCharacters();
        for(auto character : characters){
            double h_points = (double) character.h/ (double) 65536;
            double v_points = (double) character.v/ (double) 65536;
            std::cout << "char: " << character.getChar() << " h: " << h_points << "pt v: " << v_points << "pt " <<  "font" << character.font->fontName << std::endl;
        }
    }

}

