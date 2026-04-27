#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include "calculationresult.h"
#include "monitor.h"

#include <QVector>

/***************************************************************************/
/*                                                                         */
/*                               Optimizer                                 */
/*                                                                         */
/***************************************************************************/

class Optimizer
{
public:
    CalculationResult analyze(const QVector<Monitor> &monitors,
                              bool automaticReferenceMode) const;

private:
    int determineReferenceMonitorIndex(const QVector<Monitor> &monitors,
                                       bool automaticReferenceMode) const;

    QString determineMatchQuality(double absoluteDeltaPercent) const;

    QString buildRecommendation(const Monitor &monitor,
                                double currentEffectivePpi,
                                double targetEffectivePpi,
                                int recommendedScalingPercent,
                                bool isReferenceMonitor) const;

    QString determineOverallMatchQuality(const QVector<PerMonitorResult> &results) const;
    QString displayNameForMonitor(const Monitor &monitor, int index) const;
};

#endif // OPTIMIZER_H
