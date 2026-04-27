#include "resultswidget.h"
#include "resultitemwidget.h"

#include <QAbstractItemView>
#include <QFrame>
#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>
#include <QTextEdit>
#include <QVBoxLayout>

/***************************************************************************/
/*                                                                         */
/*                           ResultsWidget                                 */
/*                                                                         */
/***************************************************************************/

ResultsWidget::ResultsWidget(QWidget *parent)
    : QWidget(parent)
{
    createUi();
    applyStyles();
    resetView();
}

/***************************************************************************/
/*                                                                         */
/*                          ~ResultsWidget                                 */
/*                                                                         */
/***************************************************************************/

ResultsWidget::~ResultsWidget() = default;

/***************************************************************************/
/*                                                                         */
/*                             createUi                                    */
/*                                                                         */
/***************************************************************************/

void ResultsWidget::createUi()
{
    QVBoxLayout *rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(12);

    /************************ Summary Card ************************/

    m_summaryCard = new QFrame(this);
    m_summaryCard->setObjectName(QStringLiteral("resultsSummaryCard"));
    m_summaryCard->setFrameShape(QFrame::StyledPanel);

    QVBoxLayout *summaryLayout = new QVBoxLayout(m_summaryCard);
    summaryLayout->setContentsMargins(16, 16, 16, 16);
    summaryLayout->setSpacing(8);

    m_summaryTitleLabel = new QLabel(tr("Summary"), m_summaryCard);
    m_summaryTitleLabel->setObjectName(QStringLiteral("resultsCardTitleLabel"));

    m_referenceMonitorLabel = new QLabel(m_summaryCard);
    m_targetPpiLabel = new QLabel(m_summaryCard);
    m_targetEffectivePpiLabel = new QLabel(m_summaryCard);
    m_overallMatchLabel = new QLabel(m_summaryCard);
    m_displayCountLabel = new QLabel(m_summaryCard);

    summaryLayout->addWidget(m_summaryTitleLabel);
    summaryLayout->addWidget(m_referenceMonitorLabel);
    summaryLayout->addWidget(m_targetPpiLabel);
    summaryLayout->addWidget(m_targetEffectivePpiLabel);
    summaryLayout->addWidget(m_overallMatchLabel);
    summaryLayout->addWidget(m_displayCountLabel);

    /************************ Per-Monitor Analysis Card ************************/

    m_perMonitorCard = new QFrame(this);
    m_perMonitorCard->setObjectName(QStringLiteral("resultsPerMonitorCard"));
    m_perMonitorCard->setFrameShape(QFrame::StyledPanel);

    QVBoxLayout *perMonitorLayout = new QVBoxLayout(m_perMonitorCard);
    perMonitorLayout->setContentsMargins(16, 16, 16, 16);
    perMonitorLayout->setSpacing(8);

    m_perMonitorTitleLabel = new QLabel(tr("Per-Monitor Analysis"), m_perMonitorCard);
    m_perMonitorTitleLabel->setObjectName(QStringLiteral("resultsCardTitleLabel"));

    m_perMonitorListWidget = new QListWidget(m_perMonitorCard);
    m_perMonitorListWidget->setObjectName(QStringLiteral("perMonitorListWidget"));
    m_perMonitorListWidget->setSelectionMode(QAbstractItemView::NoSelection);
    m_perMonitorListWidget->setFocusPolicy(Qt::NoFocus);

    perMonitorLayout->addWidget(m_perMonitorTitleLabel);
    perMonitorLayout->addWidget(m_perMonitorListWidget);

    /************************ Recommendations Card ************************/

    m_recommendationsCard = new QFrame(this);
    m_recommendationsCard->setObjectName(QStringLiteral("resultsRecommendationsCard"));
    m_recommendationsCard->setFrameShape(QFrame::StyledPanel);

    QVBoxLayout *recommendationsLayout = new QVBoxLayout(m_recommendationsCard);
    recommendationsLayout->setContentsMargins(16, 16, 16, 16);
    recommendationsLayout->setSpacing(8);

    m_recommendationsTitleLabel = new QLabel(tr("Recommendations"), m_recommendationsCard);
    m_recommendationsTitleLabel->setObjectName(QStringLiteral("resultsCardTitleLabel"));

    m_recommendationsTextEdit = new QTextEdit(m_recommendationsCard);
    m_recommendationsTextEdit->setObjectName(QStringLiteral("recommendationsTextEdit"));
    m_recommendationsTextEdit->setReadOnly(true);

    recommendationsLayout->addWidget(m_recommendationsTitleLabel);
    recommendationsLayout->addWidget(m_recommendationsTextEdit);

    /************************ Chart Card ************************/

    m_chartCard = new QFrame(this);
    m_chartCard->setObjectName(QStringLiteral("resultsChartCard"));
    m_chartCard->setFrameShape(QFrame::StyledPanel);

    QVBoxLayout *chartLayout = new QVBoxLayout(m_chartCard);
    chartLayout->setContentsMargins(16, 16, 16, 16);
    chartLayout->setSpacing(8);

    m_chartTitleLabel = new QLabel(tr("PPI / Effective PPI Chart"), m_chartCard);
    m_chartTitleLabel->setObjectName(QStringLiteral("resultsCardTitleLabel"));

    m_chartView = new QChartView(m_chartCard);
    m_chartView->setRenderHint(QPainter::Antialiasing);
    m_chartView->setMinimumHeight(240);

    chartLayout->addWidget(m_chartTitleLabel);
    chartLayout->addWidget(m_chartView);


    /************************ Root ************************/

    rootLayout->addWidget(m_summaryCard);
    rootLayout->addWidget(m_chartCard, 1);
    rootLayout->addWidget(m_perMonitorCard, 1);
    rootLayout->addWidget(m_recommendationsCard, 1);
}

/***************************************************************************/
/*                                                                         */
/*                           applyStyles                                   */
/*                                                                         */
/***************************************************************************/

void ResultsWidget::applyStyles()
{
    setStyleSheet(QStringLiteral(
        "QFrame#resultsSummaryCard, "
        "QFrame#resultsChartCard, "
        "QFrame#resultsPerMonitorCard, "
        "QFrame#resultsRecommendationsCard {"
        "    border: 1px solid rgba(255, 255, 255, 0.12);"
        "    border-radius: 10px;"
        "    background-color: rgba(255, 255, 255, 0.03);"
        "}"
        "QLabel#resultsCardTitleLabel {"
        "    font-size: 15px;"
        "    font-weight: 600;"
        "}"
        "QListWidget#perMonitorListWidget {"
        "    border: none;"
        "    background: transparent;"
        "}"
        "QTextEdit#recommendationsTextEdit {"
        "    border: none;"
        "    background: transparent;"
        "}"
        "QFrame#resultsChartCard {"
        "    border: 1px solid rgba(255,255,255,0.12);"
        "    border-radius: 10px;"
        "    background-color: rgba(255,255,255,0.03);"
        "}"
        ));
}

/***************************************************************************/
/*                                                                         */
/*                             resetView                                   */
/*                                                                         */
/***************************************************************************/

void ResultsWidget::resetView()
{
    updateSummary(
        tr("Reference Monitor: Auto"),
        tr("Target PPI: -"),
        tr("Target Effective PPI: -"),
        tr("Overall Match: Not analyzed"),
        tr("Displays Analyzed: 0"));

    updatePerMonitorAnalysis(QStringList{
        tr("No monitor analysis available yet."),
        tr("Add one or more monitors and click Analyze.")
    });

    updateRecommendations(QStringList{
        tr("• Add your monitor configurations on the left side."),
        tr("• Choose Auto or Manual reference mode."),
        tr("• Click Analyze to generate results.")
    });

    resetChart();
}

/***************************************************************************/
/*                                                                         */
/*                      showPlaceholderAnalysis                            */
/*                                                                         */
/***************************************************************************/

void ResultsWidget::showPlaceholderAnalysis(int monitorCount, const QString &referenceMode)
{
    const QString referenceMonitorText =
        (referenceMode.compare(tr("Manual"), Qt::CaseInsensitive) == 0)
            ? tr("Reference Monitor: Manual selection")
            : tr("Reference Monitor: Auto-selected");

    QString overallMatchText;
    if (monitorCount <= 1) {
        overallMatchText = tr("Overall Match: Baseline only");
    } else if (monitorCount == 2) {
        overallMatchText = tr("Overall Match: Moderate");
    } else {
        overallMatchText = tr("Overall Match: Good");
    }

    updateSummary(
        referenceMonitorText,
        tr("Target PPI: 109.7"),
        tr("Target Effective PPI: 96.0"),
        overallMatchText,
        tr("Displays Analyzed: %1").arg(monitorCount));

    QStringList perMonitorLines;
    for (int i = 0; i < monitorCount; ++i) {
        if (i == 0) {
            perMonitorLines.append(
                tr("Monitor %1: 109.7 PPI | Effective: 109.7 | Baseline | Scaling: 100%%")
                    .arg(i + 1));
        } else if (i == 1) {
            perMonitorLines.append(
                tr("Monitor %1: 91.8 PPI | Effective: 91.8 | Δ -16.3%% | Suggested: 125%%")
                    .arg(i + 1));
        } else {
            perMonitorLines.append(
                tr("Monitor %1: 108.8 PPI | Effective: 87.0 | Δ -8.2%% | Suggested: 100%%")
                    .arg(i + 1));
        }
    }

    updatePerMonitorAnalysis(perMonitorLines);

    QStringList recommendationLines;
    recommendationLines.append(
        tr("• Use the monitor with the most balanced effective density as reference."));
    recommendationLines.append(
        tr("• Prefer scaling adjustments before forcing non-native resolutions."));

    if (referenceMode.compare(tr("Manual"), Qt::CaseInsensitive) == 0) {
        recommendationLines.append(
            tr("• Manual reference mode is enabled. Verify the selected monitor carefully."));
    } else {
        recommendationLines.append(
            tr("• Auto reference mode is enabled. The application selects the best baseline."));
    }

    updateRecommendations(recommendationLines);
}

/***************************************************************************/
/*                                                                         */
/*                           updateSummary                                 */
/*                                                                         */
/***************************************************************************/

void ResultsWidget::updateSummary(const QString &referenceMonitor,
                                  const QString &targetPpi,
                                  const QString &targetEffectivePpi,
                                  const QString &overallMatch,
                                  const QString &displayCount)
{
    if (m_referenceMonitorLabel != nullptr) {
        m_referenceMonitorLabel->setText(referenceMonitor);
    }

    if (m_targetPpiLabel != nullptr) {
        m_targetPpiLabel->setText(targetPpi);
    }

    if (m_targetEffectivePpiLabel != nullptr) {
        m_targetEffectivePpiLabel->setText(targetEffectivePpi);
    }

    if (m_overallMatchLabel != nullptr) {
        m_overallMatchLabel->setText(overallMatch);
    }

    if (m_displayCountLabel != nullptr) {
        m_displayCountLabel->setText(displayCount);
    }
}

/***************************************************************************/
/*                                                                         */
/*                     updatePerMonitorAnalysis                            */
/*                                                                         */
/***************************************************************************/

void ResultsWidget::updatePerMonitorAnalysis(const QStringList &lines)
{
    if (!m_perMonitorListWidget) return;

    m_perMonitorListWidget->clear();

    for (const QString &line : lines) {
        QListWidgetItem *item = new QListWidgetItem(line);

        /************************ Color Coding ************************/

        if (line.contains("Excellent")) {
            item->setForeground(QColor(0x4CAF50)); // green
        } else if (line.contains("Good")) {
            item->setForeground(QColor(0xFFC107)); // yellow
        } else if (line.contains("Poor")) {
            item->setForeground(QColor(0xF44336)); // red
        }

        item->setFlags(item->flags() & ~Qt::ItemIsSelectable);

        m_perMonitorListWidget->addItem(item);
    }
}

/***************************************************************************/
/*                                                                         */
/*                     updateRecommendations                               */
/*                                                                         */
/***************************************************************************/

void ResultsWidget::updateRecommendations(const QStringList &lines)
{
    if (m_recommendationsTextEdit == nullptr) {
        return;
    }

    m_recommendationsTextEdit->setPlainText(lines.join('\n'));
}

/***************************************************************************/
/*                                                                         */
/*                   showCalculationResult                                 */
/*                                                                         */
/***************************************************************************/

void ResultsWidget::showCalculationResult(const CalculationResult &result)
{
    if (!result.success) {
        updateSummary(
            tr("Error"),
            tr("-"),
            tr("-"),
            result.errorMessage,
            tr("Displays Analyzed: 0")
            );

        updatePerMonitorAnalysis(QStringList{
            tr("Analysis failed.")
        });

        updateRecommendations(QStringList{
            result.errorMessage
        });

        buildChart(result);

        return;
    }

    /************************ Summary ************************/

    updateSummary(
        tr("Reference Monitor: %1").arg(result.referenceMonitorTitle),
        tr("Target PPI: %1").arg(result.targetPpi, 0, 'f', 1),
        tr("Target Effective PPI: %1").arg(result.targetEffectivePpi, 0, 'f', 1), // ✅ FIX
        tr("Overall Match: %1").arg(result.overallMatchQuality),
        tr("Displays Analyzed: %1").arg(result.perMonitorResults.size())
        );

    /************************ Clear List ************************/

    m_perMonitorListWidget->clear();

    /************************ Items ************************/

    for (const PerMonitorResult &m : result.perMonitorResults)
    {
        QListWidgetItem *item = new QListWidgetItem();
        m_perMonitorListWidget->addItem(item);

        ResultItemWidget *widget = new ResultItemWidget(m_perMonitorListWidget);

        widget->setData(
            m.name,
            m.ppi,
            m.deltaPercent,
            m.currentScalingPercent,
            m.recommendedScalingPercent,
            m.isReferenceMonitor,
            m.matchQuality
            );

        item->setSizeHint(widget->sizeHint());
        m_perMonitorListWidget->setItemWidget(item, widget);
    }

    /************************ Recommendations ************************/

    QStringList recommendationLines;

    for (const QString &rec : result.recommendations) {
        recommendationLines.append("• " + rec);
    }

    updateRecommendations(recommendationLines);
}

/***************************************************************************/
/*                                                                         */
/*                            resetChart                                   */
/*                                                                         */
/***************************************************************************/

void ResultsWidget::resetChart()
{
    if (m_chartView == nullptr) {
        return;
    }

    QChart *chart = new QChart();
    chart->setTitle(tr("No chart data available"));
    chart->legend()->hide();

    m_chartView->setChart(chart);
}

/***************************************************************************/
/*                                                                         */
/*                   buildChart (Heatmap + Overlay)                        */
/*                                                                         */
/***************************************************************************/

void ResultsWidget::buildChart(const CalculationResult &result)
{
    if (m_chartView == nullptr) {
        return;
    }

    QChart *chart = new QChart();
    chart->setTitle(tr("Effective PPI Analysis"));

    /************************ Series ************************/

    QLineSeries *effectiveLine = new QLineSeries();
    effectiveLine->setName(tr("Effective PPI"));

    QLineSeries *referenceLine = new QLineSeries();
    referenceLine->setName(tr("Reference Effective PPI"));

    QScatterSeries *excellentSeries = new QScatterSeries();
    QScatterSeries *goodSeries = new QScatterSeries();
    QScatterSeries *moderateSeries = new QScatterSeries();
    QScatterSeries *poorSeries = new QScatterSeries();

    excellentSeries->setName(tr("Excellent"));
    goodSeries->setName(tr("Good"));
    moderateSeries->setName(tr("Moderate"));
    poorSeries->setName(tr("Poor"));

    excellentSeries->setColor(QColor(QStringLiteral("#4CAF50")));
    goodSeries->setColor(QColor(QStringLiteral("#8BC34A")));
    moderateSeries->setColor(QColor(QStringLiteral("#FFC107")));
    poorSeries->setColor(QColor(QStringLiteral("#F44336")));

    excellentSeries->setMarkerSize(12.0);
    goodSeries->setMarkerSize(12.0);
    moderateSeries->setMarkerSize(12.0);
    poorSeries->setMarkerSize(12.0);

    QStringList categories;

    double minY = result.targetEffectivePpi;
    double maxY = result.targetEffectivePpi;

    int index = 0;

    for (const PerMonitorResult &monitor : result.perMonitorResults) {
        effectiveLine->append(index, monitor.effectivePpi);
        referenceLine->append(index, result.targetEffectivePpi);

        if (monitor.matchQuality.contains(QStringLiteral("Excellent"), Qt::CaseInsensitive)) {
            excellentSeries->append(index, monitor.effectivePpi);
        } else if (monitor.matchQuality.contains(QStringLiteral("Good"), Qt::CaseInsensitive)) {
            goodSeries->append(index, monitor.effectivePpi);
        } else if (monitor.matchQuality.contains(QStringLiteral("Moderate"), Qt::CaseInsensitive)) {
            moderateSeries->append(index, monitor.effectivePpi);
        } else {
            poorSeries->append(index, monitor.effectivePpi);
        }

        categories.append(monitor.name);

        minY = qMin(minY, monitor.effectivePpi);
        maxY = qMax(maxY, monitor.effectivePpi);

        ++index;
    }

    /************************ Add Series ************************/

    chart->addSeries(effectiveLine);
    chart->addSeries(referenceLine);
    chart->addSeries(excellentSeries);
    chart->addSeries(goodSeries);
    chart->addSeries(moderateSeries);
    chart->addSeries(poorSeries);

    /************************ Axes ************************/

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    axisX->setTitleText(tr("Monitors"));

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText(tr("Effective PPI"));
    axisY->setLabelFormat(QStringLiteral("%.0f"));

    const double padding = qMax(5.0, (maxY - minY) * 0.15);
    axisY->setRange(qMax(0.0, minY - padding), maxY + padding);

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);

    for (QAbstractSeries *series : chart->series()) {
        series->attachAxis(axisX);
        series->attachAxis(axisY);
    }

    /************************ Styling ************************/

    QPen effectivePen(QColor(QStringLiteral("#00BCD4")));
    effectivePen.setWidth(3);
    effectiveLine->setPen(effectivePen);

    QPen referencePen(QColor(QStringLiteral("#F44336")));
    referencePen.setStyle(Qt::DashLine);
    referencePen.setWidth(2);
    referenceLine->setPen(referencePen);

    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    m_chartView->setChart(chart);
}
