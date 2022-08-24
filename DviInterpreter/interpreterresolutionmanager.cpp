#include "interpreterresolutionmanager.h"

void InterpreterResolutionManager::initialize(double pixelsPerInch) {
    InterpreterResolutionManager::pixelsPerInch = pixelsPerInch;
}


double InterpreterResolutionManager::tpToSp(double points)
{
    // one texpoint is 65536 scaled points
    return points * 65536.0;
}

double InterpreterResolutionManager::spToTp(double scaledPoints)
{
    // one texpoint is 65536 scaled points
    return scaledPoints / 65536.0;
}





double InterpreterResolutionManager::tpToPx(double points) {
    double pixelPerPoint = (double) pixelsPerInch  / (double) tpPerInch;
    return pixelPerPoint * points;
}

double InterpreterResolutionManager::pxToTp(double pixels){
    double pixelPerPoint = (double) pixelsPerInch  / (double) tpPerInch;
    return pixels / pixelPerPoint;
}

double InterpreterResolutionManager::spToPx(double scaledPoints)
{

    return tpToPx(spToTp(scaledPoints));
}

double InterpreterResolutionManager::pxToSp(double pixels)
{
    return tpToSp(pxToTp(pixels));
}



double InterpreterResolutionManager::pixelsPerInch = 0;
double InterpreterResolutionManager::tpPerInch = 72.27;

