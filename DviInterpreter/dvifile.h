#ifndef DVIFILE_H
#define DVIFILE_H

#include "DviInterpreter_global.h"
#include "character.h"
#include "rectangle.h"
#include "dvipage.h"
#include "fontdefinition.h"


#include <vector>
#include <string>
#include <map>
#include <stdio.h>
#include <iostream>
#include <set>



class DVIINTERPRETER_EXPORT DviFile
{
public:
    struct DVIFilePreamble{
            int versionNumber; //Dvi version number
            int num; //numinator
            int den; //denominator
            int mag; //magnification
            //int dviUnitMultiplier; //multiply by this to get length in 10^{-7} meter (10^{-7}m per scaled point)

            // TeX works with Scaled Points. There are 2^16= 65536 scaled points in a point.
            // There are 7227 TeX points in 254 cm (25400000 m^7).
            // in order to get the dvi unit multiplier, it is clear that
            // We should divide as follows 25400000 / (7227 *  65536) which gives number of ... per scaled point.
            // be more precies mn/100d

            std::string comment;

        }dviFilePre;


        struct DVIFilePostamble{
            int maxv;
            int maxh;
            int maxStackDepth;
            int pages;

        }dviFilePost;


        std::map<int, std::shared_ptr<FontDefinition> > definedFonts;

        std::vector<DVIPage> dviPages;


        void printDvi();



};

#endif // DVIFILE_H
