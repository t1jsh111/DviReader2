#include "resolutionmanager.h"

void ResolutionManager::initialize(double pixelsPerInch) {
    ResolutionManager::pixelsPerInch = pixelsPerInch;
}

double ResolutionManager::tpToSp(double points)
{
    // one texpoint is 65536 scaled points
    return points * 65536.0;
}

double ResolutionManager::spToTp(double scaledPoints)
{
    // one texpoint is 65536 scaled points
    return scaledPoints / 65536.0;
}





double ResolutionManager::tpToPx(double points) {
    double pixelPerPoint = (double) pixelsPerInch  / (double) tpPerInch;
    return pixelPerPoint * points;
}

double ResolutionManager::pxToTp(double pixels){
    double pixelPerPoint = (double) pixelsPerInch  / (double) tpPerInch;
    return pixels / pixelPerPoint;
}

double ResolutionManager::spToPx(double scaledPoints)
{

    return tpToPx(spToTp(scaledPoints));
}

double ResolutionManager::pxToSp(double pixels)
{
    return tpToSp(pxToTp(pixels));
}



double ResolutionManager::pixelsPerInch = 0;
double ResolutionManager::tpPerInch = 72.27;
