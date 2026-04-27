#include <QtTest/QtTest>

#include "core/calculator.h"
#include "core/monitor.h"
#include "core/optimizer.h"
#include "core/calculationresult.h"

/***************************************************************************/
/*                                                                         */
/*                         TestCalculator                                  */
/*                                                                         */
/***************************************************************************/

class TestCalculator : public QObject
{
    Q_OBJECT

private slots:
    void calculatePixelDiagonal_shouldReturnCorrectValue();
    void calculatePpi_shouldReturnCorrectValue();
    void calculateEffectivePpi_shouldReturnCorrectValue();
    void calculateDeltaPercent_shouldReturnCorrectValue();
    void recommendScalingPercent_shouldSnapToNearestValidScaling();
    void optimizer_shouldAnalyzeValidMonitorSetup();
};

/***************************************************************************/
/*                                                                         */
/*             calculatePixelDiagonal_shouldReturnCorrectValue              */
/*                                                                         */
/***************************************************************************/

void TestCalculator::calculatePixelDiagonal_shouldReturnCorrectValue()
{
    const double diagonal = Calculator::calculatePixelDiagonal(1920, 1080);

    QVERIFY(diagonal > 2202.0);
    QVERIFY(diagonal < 2204.0);
}

/***************************************************************************/
/*                                                                         */
/*                   calculatePpi_shouldReturnCorrectValue                  */
/*                                                                         */
/***************************************************************************/

void TestCalculator::calculatePpi_shouldReturnCorrectValue()
{
    const double ppi = Calculator::calculatePpi(1920, 1080, 24.0);

    QVERIFY(ppi > 91.0);
    QVERIFY(ppi < 92.5);
}

/***************************************************************************/
/*                                                                         */
/*              calculateEffectivePpi_shouldReturnCorrectValue              */
/*                                                                         */
/***************************************************************************/

void TestCalculator::calculateEffectivePpi_shouldReturnCorrectValue()
{
    const double effectivePpi = Calculator::calculateEffectivePpi(144.0, 150);

    QCOMPARE(effectivePpi, 96.0);
}

/***************************************************************************/
/*                                                                         */
/*                calculateDeltaPercent_shouldReturnCorrectValue            */
/*                                                                         */
/***************************************************************************/

void TestCalculator::calculateDeltaPercent_shouldReturnCorrectValue()
{
    const double delta = Calculator::calculateDeltaPercent(100.0, 90.0);

    QCOMPARE(delta, -10.0);
}

/***************************************************************************/
/*                                                                         */
/*         recommendScalingPercent_shouldSnapToNearestValidScaling          */
/*                                                                         */
/***************************************************************************/

void TestCalculator::recommendScalingPercent_shouldSnapToNearestValidScaling()
{
    const int scaling = Calculator::recommendScalingPercent(160.0, 106.0);

    QCOMPARE(scaling, 150);
}

/***************************************************************************/
/*                                                                         */
/*                  optimizer_shouldAnalyzeValidMonitorSetup                */
/*                                                                         */
/***************************************************************************/

void TestCalculator::optimizer_shouldAnalyzeValidMonitorSetup()
{
    Monitor monitor1;
    monitor1.setTitle(QStringLiteral("Monitor 1"));
    monitor1.setName(QStringLiteral("Primary"));
    monitor1.setWidthPixels(3840);
    monitor1.setHeightPixels(2160);
    monitor1.setDiagonalInches(27.0);
    monitor1.setScalingPercent(150);
    monitor1.setReferenceMonitor(true);

    Monitor monitor2;
    monitor2.setTitle(QStringLiteral("Monitor 2"));
    monitor2.setName(QStringLiteral("Secondary"));
    monitor2.setWidthPixels(1920);
    monitor2.setHeightPixels(1080);
    monitor2.setDiagonalInches(24.0);
    monitor2.setScalingPercent(100);
    monitor2.setReferenceMonitor(false);

    QVector<Monitor> monitors;
    monitors.append(monitor1);
    monitors.append(monitor2);

    Optimizer optimizer;
    const CalculationResult result = optimizer.analyze(monitors, false);

    QVERIFY(result.success);
    QCOMPARE(result.perMonitorResults.size(), 2);
    QCOMPARE(result.referenceMonitorIndex, 0);
    QVERIFY(result.targetPpi > 150.0);
    QVERIFY(result.targetEffectivePpi > 100.0);
    QVERIFY(!result.recommendations.isEmpty());
}

QTEST_MAIN(TestCalculator)
#include "test_calculator.moc"
