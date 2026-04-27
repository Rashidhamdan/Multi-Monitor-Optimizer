#ifndef CALCULATIONRESULT_H
#define CALCULATIONRESULT_H

#include <QString>
#include <QStringList>
#include <QVector>

/***************************************************************************/
/*                                                                         */
/*                         PerMonitorResult                                */
/*                                                                         */
/***************************************************************************/

struct PerMonitorResult
{
    QString title;
    QString name;

    double ppi = 0.0;
    double effectivePpi = 0.0;
    double deltaPercent = 0.0;

    int currentScalingPercent = 100;
    int recommendedScalingPercent = 100;

    bool isReferenceMonitor = false;
    bool valid = false;

    QString matchQuality;
    QString recommendation;
};

/***************************************************************************/
/*                                                                         */
/*                        CalculationResult                                */
/*                                                                         */
/***************************************************************************/

struct CalculationResult
{
    bool success = false;
    QString errorMessage;

    int referenceMonitorIndex = -1;
    QString referenceMonitorTitle;

    double targetPpi = 0.0;
    double targetEffectivePpi = 0.0;

    QString overallMatchQuality;

    QVector<PerMonitorResult> perMonitorResults;
    QStringList recommendations;
};

#endif // CALCULATIONRESULT_H
