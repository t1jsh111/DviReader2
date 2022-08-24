#ifndef RESOLUTIONMANAGER_H
#define RESOLUTIONMANAGER_H

#include <QtCore>


class ResolutionManager {
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
    ResolutionManager() = delete;


};



#endif // RESOLUTIONMANAGER_H
