#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "monitor.h"

#include <QVector>

/***************************************************************************/
/*                                                                         */
/*                              Calculator                                 */
/*                                                                         */
/***************************************************************************/

class Calculator
{
public:
    static double calculatePixelDiagonal(int widthPixels, int heightPixels);
    static double calculatePpi(int widthPixels, int heightPixels, double diagonalInches);
    static double calculatePpi(const Monitor &monitor);

    static double calculateEffectivePpi(double ppi, int scalingPercent);
    static double calculateEffectivePpi(const Monitor &monitor);

    static double calculateDeltaPercent(double referenceValue, double value);
    static double roundToDecimals(double value, int decimals);

    static int recommendScalingPercent(double monitorPpi, double targetEffectivePpi);
    static int snapScalingPercent(int rawPercent);

    static double median(QVector<double> values);
};

#endif // CALCULATOR_H
