#ifndef INTERPRETERRESOLUTIONMANAGER_H
#define INTERPRETERRESOLUTIONMANAGER_H

#include "DviInterpreter_global.h"

class DVIINTERPRETER_EXPORT InterpreterResolutionManager {
public:
    static double pixelsPerInch;
    static double tpPerInch;


    static void initialize(double pixelsPerInch);


    static double tpToSp(double points);

    static double spToTp(double scaledPoints);

    static double tpToPx(double points);

    static double pxToTp(double pixels);

    static double spToPx(double scaledPoints);

    static double pxToSp(double pixels);

private:
    InterpreterResolutionManager() = delete;





};

#endif // INTERPRETERRESOLUTIONMANAGER_H
