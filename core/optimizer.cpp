#include "optimizer.h"

#include "calculator.h"

#include <QtMath>

/***************************************************************************/
/*                                                                         */
/*                               analyze                                   */
/*                                                                         */
/***************************************************************************/

CalculationResult Optimizer::analyze(const QVector<Monitor> &monitors,
                                     bool automaticReferenceMode) const
{
    CalculationResult result;

    if (monitors.isEmpty()) {
        result.success = false;
        result.errorMessage = QStringLiteral("No monitors available for analysis.");
        return result;
    }

    for (const Monitor &monitor : monitors) {
        if (!monitor.isValid()) {
            result.success = false;
            result.errorMessage = QStringLiteral(
                "One or more monitor configurations are invalid.");
            return result;
        }
    }

    const int referenceIndex = determineReferenceMonitorIndex(monitors, automaticReferenceMode);
    if (referenceIndex < 0 || referenceIndex >= monitors.size()) {
        result.success = false;
        result.errorMessage = QStringLiteral(
            "Unable to determine a valid reference monitor.");
        return result;
    }

    const Monitor &referenceMonitor = monitors[referenceIndex];

    const double referencePpi = Calculator::calculatePpi(referenceMonitor);
    const double referenceEffectivePpi = Calculator::calculateEffectivePpi(referenceMonitor);

    result.success = true;
    result.referenceMonitorIndex = referenceIndex;
    result.referenceMonitorTitle = displayNameForMonitor(referenceMonitor, referenceIndex);
    result.targetPpi = Calculator::roundToDecimals(referencePpi, 1);
    result.targetEffectivePpi = Calculator::roundToDecimals(referenceEffectivePpi, 1);

    for (int i = 0; i < monitors.size(); ++i) {
        const Monitor &monitor = monitors[i];

        PerMonitorResult monitorResult;
        monitorResult.title = monitor.title();
        monitorResult.name = displayNameForMonitor(monitor, i);
        monitorResult.isReferenceMonitor = (i == referenceIndex);
        monitorResult.currentScalingPercent = monitor.scalingPercent();

        const double ppi = Calculator::calculatePpi(monitor);
        const double effectivePpi = Calculator::calculateEffectivePpi(monitor);
        const double deltaPercent =
            Calculator::calculateDeltaPercent(referenceEffectivePpi, effectivePpi);

        const int recommendedScaling = monitorResult.isReferenceMonitor
                                           ? monitor.scalingPercent()
                                           : Calculator::recommendScalingPercent(
                                                 ppi,
                                                 referenceEffectivePpi);

        monitorResult.ppi = Calculator::roundToDecimals(ppi, 1);
        monitorResult.effectivePpi = Calculator::roundToDecimals(effectivePpi, 1);
        monitorResult.deltaPercent = Calculator::roundToDecimals(deltaPercent, 1);
        monitorResult.recommendedScalingPercent = recommendedScaling;
        monitorResult.matchQuality = determineMatchQuality(qAbs(deltaPercent));
        monitorResult.recommendation = buildRecommendation(
            monitor,
            effectivePpi,
            referenceEffectivePpi,
            recommendedScaling,
            monitorResult.isReferenceMonitor);
        monitorResult.valid = true;

        result.perMonitorResults.append(monitorResult);
    }

    result.overallMatchQuality = determineOverallMatchQuality(result.perMonitorResults);

    result.recommendations.append(
        QStringLiteral("Use %1 as the visual baseline for cross-display consistency.")
            .arg(result.referenceMonitorTitle));

    for (const PerMonitorResult &monitorResult : std::as_const(result.perMonitorResults)) {
        if (!monitorResult.recommendation.trimmed().isEmpty()) {
            result.recommendations.append(monitorResult.recommendation);
        }
    }

    return result;
}

/***************************************************************************/
/*                                                                         */
/*                    determineReferenceMonitorIndex                       */
/*                                                                         */
/***************************************************************************/

int Optimizer::determineReferenceMonitorIndex(const QVector<Monitor> &monitors,
                                              bool automaticReferenceMode) const
{
    if (monitors.isEmpty()) {
        return -1;
    }

    if (!automaticReferenceMode) {
        for (int i = 0; i < monitors.size(); ++i) {
            if (monitors[i].isReferenceMonitor()) {
                return i;
            }
        }
    }

    QVector<double> effectivePpis;
    effectivePpis.reserve(monitors.size());

    for (const Monitor &monitor : monitors) {
        effectivePpis.append(Calculator::calculateEffectivePpi(monitor));
    }

    const double medianEffectivePpi = Calculator::median(effectivePpis);

    int bestIndex = 0;
    double bestDistance = qAbs(effectivePpis[0] - medianEffectivePpi);

    for (int i = 1; i < effectivePpis.size(); ++i) {
        const double distance = qAbs(effectivePpis[i] - medianEffectivePpi);
        if (distance < bestDistance) {
            bestDistance = distance;
            bestIndex = i;
        }
    }

    return bestIndex;
}

/***************************************************************************/
/*                                                                         */
/*                       determineMatchQuality                             */
/*                                                                         */
/***************************************************************************/

QString Optimizer::determineMatchQuality(double absoluteDeltaPercent) const
{
    if (absoluteDeltaPercent <= 5.0) {
        return QStringLiteral("Excellent");
    }

    if (absoluteDeltaPercent <= 10.0) {
        return QStringLiteral("Good");
    }

    if (absoluteDeltaPercent <= 20.0) {
        return QStringLiteral("Moderate");
    }

    return QStringLiteral("Poor");
}

/***************************************************************************/
/*                                                                         */
/*                        buildRecommendation                              */
/*                                                                         */
/***************************************************************************/

QString Optimizer::buildRecommendation(const Monitor &monitor,
                                       double currentEffectivePpi,
                                       double targetEffectivePpi,
                                       int recommendedScalingPercent,
                                       bool isReferenceMonitor) const
{
    const QString monitorName = monitor.name().trimmed().isEmpty()
    ? monitor.title()
    : monitor.name().trimmed();

    if (isReferenceMonitor) {
        return QStringLiteral(
                   "%1 is the selected reference monitor. Keep its current scaling at %2%%.")
            .arg(monitorName)
            .arg(monitor.scalingPercent());
    }

    if (currentEffectivePpi <= 0.0 || targetEffectivePpi <= 0.0) {
        return QStringLiteral("%1 has invalid data and could not be analyzed.")
        .arg(monitorName);
    }

    const double deltaPercent =
        Calculator::calculateDeltaPercent(targetEffectivePpi, currentEffectivePpi);

    if (qAbs(deltaPercent) <= 5.0) {
        return QStringLiteral(
                   "%1 is already visually well aligned with the reference monitor.")
            .arg(monitorName);
    }

    if (recommendedScalingPercent == monitor.scalingPercent()) {
        return QStringLiteral(
                   "%1 is close to the target already. Keeping %2%% scaling is acceptable.")
            .arg(monitorName)
            .arg(monitor.scalingPercent());
    }

    return QStringLiteral(
               "%1 would align better visually with %2%% scaling instead of %3%%.")
        .arg(monitorName)
        .arg(recommendedScalingPercent)
        .arg(monitor.scalingPercent());
}

/***************************************************************************/
/*                                                                         */
/*                    determineOverallMatchQuality                         */
/*                                                                         */
/***************************************************************************/

QString Optimizer::determineOverallMatchQuality(const QVector<PerMonitorResult> &results) const
{
    if (results.isEmpty()) {
        return QStringLiteral("Unknown");
    }

    double maxAbsoluteDelta = 0.0;

    for (const PerMonitorResult &result : results) {
        const double absoluteDelta = qAbs(result.deltaPercent);
        if (absoluteDelta > maxAbsoluteDelta) {
            maxAbsoluteDelta = absoluteDelta;
        }
    }

    return determineMatchQuality(maxAbsoluteDelta);
}

/***************************************************************************/
/*                                                                         */
/*                      displayNameForMonitor                              */
/*                                                                         */
/***************************************************************************/

QString Optimizer::displayNameForMonitor(const Monitor &monitor, int index) const
{
    if (!monitor.name().trimmed().isEmpty()) {
        return monitor.name().trimmed();
    }

    if (!monitor.title().trimmed().isEmpty()) {
        return monitor.title().trimmed();
    }

    return QStringLiteral("Monitor %1").arg(index + 1);
}
