#ifndef RESULTSWIDGET_H
#define RESULTSWIDGET_H

#include <QWidget>
#include "core/calculationresult.h"

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QCategoryAxis>

class QLabel;
class QListWidget;
class QTextEdit;
class QFrame;

/***************************************************************************/
/*                                                                         */
/*                           ResultsWidget                                 */
/*                                                                         */
/***************************************************************************/

class ResultsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ResultsWidget(QWidget *parent = nullptr);
    ~ResultsWidget() override;

    void resetView();
    void showPlaceholderAnalysis(int monitorCount, const QString &referenceMode);
    void showCalculationResult(const CalculationResult &result);


private:
    /************************ Summary ************************/

    QFrame *m_summaryCard = nullptr;
    QLabel *m_summaryTitleLabel = nullptr;
    QLabel *m_referenceMonitorLabel = nullptr;
    QLabel *m_targetPpiLabel = nullptr;
    QLabel *m_targetEffectivePpiLabel = nullptr;
    QLabel *m_overallMatchLabel = nullptr;
    QLabel *m_displayCountLabel = nullptr;

    /************************ Per-Monitor Analysis ************************/

    QFrame *m_perMonitorCard = nullptr;
    QLabel *m_perMonitorTitleLabel = nullptr;
    QListWidget *m_perMonitorListWidget = nullptr;

    /************************ Recommendations ************************/

    QFrame *m_recommendationsCard = nullptr;
    QLabel *m_recommendationsTitleLabel = nullptr;
    QTextEdit *m_recommendationsTextEdit = nullptr;

    QFrame *m_chartCard = nullptr;
    QLabel *m_chartTitleLabel = nullptr;
    QChartView *m_chartView = nullptr;

    void createUi();
    void applyStyles();

    void updateSummary(const QString &referenceMonitor,
                       const QString &targetPpi,
                       const QString &targetEffectivePpi,
                       const QString &overallMatch,
                       const QString &displayCount);

    void updatePerMonitorAnalysis(const QStringList &lines);
    void updateRecommendations(const QStringList &lines);

    void resetChart();
    void buildChart(const CalculationResult &result);
};

#endif // RESULTSWIDGET_H
