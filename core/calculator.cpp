#include "calculator.h"

#include <QtGlobal>

#include <algorithm>
#include <cmath>

/***************************************************************************/
/*                                                                         */
/*                       calculatePixelDiagonal                            */
/*                                                                         */
/***************************************************************************/

double Calculator::calculatePixelDiagonal(int widthPixels, int heightPixels)
{
    if (widthPixels <= 0 || heightPixels <= 0) {
        return 0.0;
    }

    const double width = static_cast<double>(widthPixels);
    const double height = static_cast<double>(heightPixels);

    return std::sqrt((width * width) + (height * height));
}

/***************************************************************************/
/*                                                                         */
/*                            calculatePpi                                 */
/*                                                                         */
/***************************************************************************/

double Calculator::calculatePpi(int widthPixels, int heightPixels, double diagonalInches)
{
    if (widthPixels <= 0 || heightPixels <= 0 || diagonalInches <= 0.0) {
        return 0.0;
    }

    const double pixelDiagonal = calculatePixelDiagonal(widthPixels, heightPixels);
    if (pixelDiagonal <= 0.0) {
        return 0.0;
    }

    return pixelDiagonal / diagonalInches;
}

/***************************************************************************/
/*                                                                         */
/*                            calculatePpi                                 */
/*                                                                         */
/***************************************************************************/

double Calculator::calculatePpi(const Monitor &monitor)
{
    if (!monitor.isValid()) {
        return 0.0;
    }

    return calculatePpi(
        monitor.widthPixels(),
        monitor.heightPixels(),
        monitor.diagonalInches());
}

/***************************************************************************/
/*                                                                         */
/*                       calculateEffectivePpi                             */
/*                                                                         */
/***************************************************************************/

double Calculator::calculateEffectivePpi(double ppi, int scalingPercent)
{
    if (ppi <= 0.0 || scalingPercent <= 0) {
        return 0.0;
    }

    const double factor = static_cast<double>(scalingPercent) / 100.0;
    return ppi / factor;
}

/***************************************************************************/
/*                                                                         */
/*                       calculateEffectivePpi                             */
/*                                                                         */
/***************************************************************************/

double Calculator::calculateEffectivePpi(const Monitor &monitor)
{
    if (!monitor.isValid()) {
        return 0.0;
    }

    return calculateEffectivePpi(calculatePpi(monitor), monitor.scalingPercent());
}

/***************************************************************************/
/*                                                                         */
/*                        calculateDeltaPercent                            */
/*                                                                         */
/***************************************************************************/

double Calculator::calculateDeltaPercent(double referenceValue, double value)
{
    if (referenceValue <= 0.0) {
        return 0.0;
    }

    return ((value - referenceValue) / referenceValue) * 100.0;
}

/***************************************************************************/
/*                                                                         */
/*                         roundToDecimals                                 */
/*                                                                         */
/***************************************************************************/

double Calculator::roundToDecimals(double value, int decimals)
{
    if (decimals < 0) {
        return value;
    }

    const double factor = std::pow(10.0, static_cast<double>(decimals));
    return std::round(value * factor) / factor;
}

/***************************************************************************/
/*                                                                         */
/*                      recommendScalingPercent                            */
/*                                                                         */
/***************************************************************************/

int Calculator::recommendScalingPercent(double monitorPpi, double targetEffectivePpi)
{
    if (monitorPpi <= 0.0 || targetEffectivePpi <= 0.0) {
        return 100;
    }

    const double rawScaling = (monitorPpi / targetEffectivePpi) * 100.0;
    return snapScalingPercent(static_cast<int>(std::round(rawScaling)));
}

/***************************************************************************/
/*                                                                         */
/*                         snapScalingPercent                              */
/*                                                                         */
/***************************************************************************/

int Calculator::snapScalingPercent(int rawPercent)
{
    static const int kAllowedValues[] = {100, 125, 150, 175, 200, 225, 250, 300};

    int bestValue = kAllowedValues[0];
    int smallestDistance = std::abs(rawPercent - bestValue);

    for (int value : kAllowedValues) {
        const int distance = std::abs(rawPercent - value);
        if (distance < smallestDistance) {
            smallestDistance = distance;
            bestValue = value;
        }
    }

    return qBound(100, bestValue, 300);
}

/***************************************************************************/
/*                                                                         */
/*                               median                                    */
/*                                                                         */
/***************************************************************************/

double Calculator::median(QVector<double> values)
{
    if (values.isEmpty()) {
        return 0.0;
    }

    std::sort(values.begin(), values.end());

    const int size = values.size();
    const int middle = size / 2;

    if ((size % 2) == 0) {
        return (values[middle - 1] + values[middle]) / 2.0;
    }

    return values[middle];
}
