#include "mainwindow.h"

#include "monitorwidget.h"
#include "resultswidget.h"
#include "core/monitor.h"
#include "core/optimizer.h"

#include <QAction>
#include <QComboBox>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollArea>
#include <QSplitter>
#include <QStatusBar>
#include <QStyle>
#include <QToolBar>
#include <QVBoxLayout>
#include <QWidget>
#include <QDialog>
#include <QTabWidget>
#include <QGuiApplication>
#include <QScreen>

/***************************************************************************/
/*                                                                         */
/*                             MainWindow                                  */
/*                                                                         */
/***************************************************************************/

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    createCentralUi();
    createWindowsAndWidgets();
    createMenuBar(new QMenuBar(this));
    setupActions();
    createToolBar();
    createStatusBar();
    setupConnections();
    applySectionTitles();
    applyWindowProperties();
    addInitialMonitors();
    updateUiState();

    detectMonitors();
}

/***************************************************************************/
/*                                                                         */
/*                             ~MainWindow                                 */
/*                                                                         */
/***************************************************************************/

MainWindow::~MainWindow() = default;

/***************************************************************************/
/*                                                                         */
/*                          createCentralUi                                */
/*                                                                         */
/***************************************************************************/

void MainWindow::createCentralUi()
{
    m_centralWidget = new QWidget(this);
    setCentralWidget(m_centralWidget);

    QVBoxLayout *rootLayout = new QVBoxLayout(m_centralWidget);
    rootLayout->setContentsMargins(16, 16, 16, 16);
    rootLayout->setSpacing(12);

    /************************ Header ************************/

    m_headerWidget = new QWidget(m_centralWidget);
    QVBoxLayout *headerLayout = new QVBoxLayout(m_headerWidget);
    headerLayout->setContentsMargins(18, 18, 18, 18);
    headerLayout->setSpacing(4);

    m_titleLabel = new QLabel(tr("Multi Monitor Optimizer"), m_headerWidget);
    m_titleLabel->setObjectName(QStringLiteral("titleLabel"));

    m_subtitleLabel = new QLabel(
        tr("Normalize DPI, scaling, and cross-display window consistency."),
        m_headerWidget);
    m_subtitleLabel->setObjectName(QStringLiteral("subtitleLabel"));

    headerLayout->addWidget(m_titleLabel);
    headerLayout->addWidget(m_subtitleLabel);

    /************************ Control Bar ************************/

    m_controlBarWidget = new QWidget(m_centralWidget);
    QHBoxLayout *controlBarLayout = new QHBoxLayout(m_controlBarWidget);
    controlBarLayout->setContentsMargins(18, 12, 18, 12);
    controlBarLayout->setSpacing(10);

    m_addMonitorButton = new QPushButton(
        style()->standardIcon(QStyle::SP_FileDialogNewFolder),
        tr("Add Monitor"),
        m_controlBarWidget);

    m_analyzeButton = new QPushButton(
        style()->standardIcon(QStyle::SP_MediaPlay),
        tr("Analyze"),
        m_controlBarWidget);

    m_resetButton = new QPushButton(
        style()->standardIcon(QStyle::SP_DialogResetButton),
        tr("Reset"),
        m_controlBarWidget);

    m_referenceModeLabel = new QLabel(tr("Reference Mode:"), m_controlBarWidget);

    m_referenceModeComboBox = new QComboBox(m_controlBarWidget);
    m_referenceModeComboBox->addItem(tr("Auto"));
    m_referenceModeComboBox->addItem(tr("Manual"));

    controlBarLayout->addWidget(m_addMonitorButton);
    controlBarLayout->addWidget(m_analyzeButton);
    controlBarLayout->addWidget(m_resetButton);
    controlBarLayout->addStretch(1);
    controlBarLayout->addWidget(m_referenceModeLabel);
    controlBarLayout->addWidget(m_referenceModeComboBox);

    /************************ Content ************************/

    m_contentWidget = new QWidget(m_centralWidget);
    QHBoxLayout *contentLayout = new QHBoxLayout(m_contentWidget);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->setSpacing(12);

    m_mainSplitter = new QSplitter(Qt::Horizontal, m_contentWidget);
    m_mainSplitter->setChildrenCollapsible(false);
    m_mainSplitter->setHandleWidth(6);

    /************************ Left Panel ************************/

    m_monitorPanel = new QFrame(m_mainSplitter);
    m_monitorPanel->setObjectName(QStringLiteral("monitorPanel"));
    m_monitorPanel->setFrameShape(QFrame::StyledPanel);

    QVBoxLayout *monitorPanelLayout = new QVBoxLayout(m_monitorPanel);
    monitorPanelLayout->setContentsMargins(16, 16, 16, 16);
    monitorPanelLayout->setSpacing(12);

    m_monitorPanelTitleLabel = new QLabel(m_monitorPanel);
    m_monitorPanelTitleLabel->setObjectName(QStringLiteral("sectionTitleLabel"));

    m_monitorPanelSubtitleLabel = new QLabel(m_monitorPanel);
    m_monitorPanelSubtitleLabel->setObjectName(QStringLiteral("sectionSubtitleLabel"));
    m_monitorPanelSubtitleLabel->setWordWrap(true);

    m_monitorScrollArea = new QScrollArea(m_monitorPanel);
    m_monitorScrollArea->setWidgetResizable(true);
    m_monitorScrollArea->setFrameShape(QFrame::NoFrame);

    m_monitorListContainer = new QWidget(m_monitorScrollArea);
    m_monitorListLayout = new QVBoxLayout(m_monitorListContainer);
    m_monitorListLayout->setContentsMargins(0, 0, 0, 0);
    m_monitorListLayout->setSpacing(12);
    m_monitorListLayout->addStretch(1);

    m_monitorScrollArea->setWidget(m_monitorListContainer);

    monitorPanelLayout->addWidget(m_monitorPanelTitleLabel);
    monitorPanelLayout->addWidget(m_monitorPanelSubtitleLabel);
    monitorPanelLayout->addWidget(m_monitorScrollArea, 1);

    /************************ Right Panel ************************/

    m_analysisPanel = new QFrame(m_mainSplitter);
    m_analysisPanel->setObjectName(QStringLiteral("analysisPanel"));
    m_analysisPanel->setFrameShape(QFrame::StyledPanel);

    QVBoxLayout *analysisPanelLayout = new QVBoxLayout(m_analysisPanel);
    analysisPanelLayout->setContentsMargins(16, 16, 16, 16);
    analysisPanelLayout->setSpacing(12);

    m_analysisPanelTitleLabel = new QLabel(m_analysisPanel);
    m_analysisPanelTitleLabel->setObjectName(QStringLiteral("sectionTitleLabel"));

    m_analysisPanelSubtitleLabel = new QLabel(m_analysisPanel);
    m_analysisPanelSubtitleLabel->setObjectName(QStringLiteral("sectionSubtitleLabel"));
    m_analysisPanelSubtitleLabel->setWordWrap(true);

    m_resultsWidget = new ResultsWidget(m_analysisPanel);

    analysisPanelLayout->addWidget(m_analysisPanelTitleLabel);
    analysisPanelLayout->addWidget(m_analysisPanelSubtitleLabel);
    analysisPanelLayout->addWidget(m_resultsWidget, 1);

    m_mainSplitter->addWidget(m_monitorPanel);
    m_mainSplitter->addWidget(m_analysisPanel);
    m_mainSplitter->setStretchFactor(0, 3);
    m_mainSplitter->setStretchFactor(1, 2);
    m_mainSplitter->setSizes({700, 420});

    contentLayout->addWidget(m_mainSplitter);

    /************************ Root ************************/

    rootLayout->addWidget(m_headerWidget);
    rootLayout->addWidget(m_controlBarWidget);
    rootLayout->addWidget(m_contentWidget, 1);
}

/***************************************************************************/
/*                                                                         */
/*                           createMenuBar                                 */
/*                                                                         */
/***************************************************************************/

void MainWindow::createMenuBar(QMenuBar *menuBar)
{
    setMenuBar(menuBar);

    /************************ File ************************/

    QMenu *fileMenu = menuBar->addMenu(tr("File"));

    m_actionAddMonitor = new QAction(
        style()->standardIcon(QStyle::SP_FileDialogNewFolder),
        tr("Add Monitor"), this);

    m_actionAnalyze = new QAction(
        style()->standardIcon(QStyle::SP_MediaPlay),
        tr("Analyze"), this);

    m_actionReset = new QAction(
        style()->standardIcon(QStyle::SP_DialogResetButton),
        tr("Reset"), this);

    m_actionExit = new QAction(
        style()->standardIcon(QStyle::SP_DialogCloseButton),
        tr("Exit"), this);

    m_actionDetect = new QAction(
        style()->standardIcon(QStyle::SP_ComputerIcon),
        tr("Detect Monitors"), this);

    fileMenu->addAction(m_actionAddMonitor);
    fileMenu->addAction(m_actionAnalyze);
    fileMenu->addAction(m_actionReset);
    fileMenu->addSeparator();
    fileMenu->addAction(m_actionExit);

    /************************ Help ************************/

    QMenu *helpMenu = menuBar->addMenu(tr("Help"));

    m_actionHelp = new QAction(
        style()->standardIcon(QStyle::SP_DialogHelpButton),
        tr("Documentation"), this);

    m_actionAbout = new QAction(
        style()->standardIcon(QStyle::SP_MessageBoxInformation),
        tr("About"), this);

    helpMenu->addAction(m_actionHelp);
    helpMenu->addSeparator();
    helpMenu->addAction(m_actionAbout);
}

/***************************************************************************/
/*                                                                         */
/*                             createToolBar                               */
/*                                                                         */
/***************************************************************************/

void MainWindow::createToolBar()
{
    QToolBar *mainToolBar = addToolBar(tr("Main Toolbar"));
    mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
    mainToolBar->setMovable(false);

    mainToolBar->addAction(m_actionAddMonitor);
    mainToolBar->addAction(m_actionAnalyze);
    mainToolBar->addAction(m_actionReset);
}

/***************************************************************************/
/*                                                                         */
/*                           createStatusBar                               */
/*                                                                         */
/***************************************************************************/

void MainWindow::createStatusBar()
{
    m_statusInfoLabel = new QLabel(this);
    m_statusInfoLabel->setText(tr("Reference Mode: %1").arg(currentReferenceModeText()));

    statusBar()->showMessage(tr("Ready"));
    statusBar()->addPermanentWidget(m_statusInfoLabel, 1);
}

/***************************************************************************/
/*                                                                         */
/*                    createWindowsAndWidgets                              */
/*                                                                         */
/***************************************************************************/

void MainWindow::createWindowsAndWidgets()
{
    if (m_resultsWidget != nullptr) {
        m_resultsWidget->setObjectName(QStringLiteral("resultsWidget"));
    }
}

/***************************************************************************/
/*                                                                         */
/*                             setupActions                                */
/*                                                                         */
/***************************************************************************/

void MainWindow::setupActions()
{
    m_actionAddMonitor->setShortcut(QKeySequence(tr("Ctrl+N")));
    m_actionAnalyze->setShortcut(QKeySequence(tr("Ctrl+Return")));
    m_actionReset->setShortcut(QKeySequence(tr("Ctrl+R")));
    m_actionExit->setShortcut(QKeySequence::Quit);
    m_actionAbout->setShortcut(QKeySequence(tr("F1")));
    m_actionHelp->setShortcut(QKeySequence(tr("F2")));

    m_actionAddMonitor->setStatusTip(tr("Add a new monitor configuration card"));
    m_actionAnalyze->setStatusTip(tr("Analyze DPI, scaling, and monitor consistency"));
    m_actionReset->setStatusTip(tr("Reset all monitor configurations"));
    m_actionExit->setStatusTip(tr("Exit the application"));
    m_actionAbout->setStatusTip(tr("Show application information"));
    m_actionHelp->setStatusTip(tr("Open documentation"));

    applyActionToolTip(m_actionAddMonitor);
    applyActionToolTip(m_actionAnalyze);
    applyActionToolTip(m_actionReset);
    applyActionToolTip(m_actionExit);
    applyActionToolTip(m_actionAbout);
    applyActionToolTip(m_actionHelp);

    if (m_addMonitorButton != nullptr) {
        m_addMonitorButton->setToolTip(tr("Add a new monitor configuration card"));
    }

    if (m_analyzeButton != nullptr) {
        m_analyzeButton->setToolTip(tr("Analyze the current monitor setup"));
    }

    if (m_resetButton != nullptr) {
        m_resetButton->setToolTip(tr("Reset all monitor entries"));
    }
}

/***************************************************************************/
/*                                                                         */
/*                         applyActionToolTip                              */
/*                                                                         */
/***************************************************************************/

void MainWindow::applyActionToolTip(QAction *action)
{
    if (action == nullptr) {
        return;
    }

    const QString shortcutText = action->shortcut().toString(QKeySequence::NativeText);

    if (shortcutText.isEmpty()) {
        action->setToolTip(action->text());
    } else {
        action->setToolTip(tr("%1 (%2)").arg(action->text(), shortcutText));
    }
}

/***************************************************************************/
/*                                                                         */
/*                          setupConnections                               */
/*                                                                         */
/***************************************************************************/

void MainWindow::setupConnections()
{
    connect(m_actionAddMonitor, &QAction::triggered, this, &MainWindow::addMonitor);
    connect(m_actionAnalyze, &QAction::triggered, this, &MainWindow::analyzeConfiguration);
    connect(m_actionReset, &QAction::triggered, this, &MainWindow::resetMonitors);
    connect(m_actionExit, &QAction::triggered, this, &MainWindow::close);
    connect(m_actionAbout, &QAction::triggered, this, &MainWindow::showAboutDialog);

    connect(m_addMonitorButton, &QPushButton::clicked, this, &MainWindow::addMonitor);
    connect(m_analyzeButton, &QPushButton::clicked, this, &MainWindow::analyzeConfiguration);
    connect(m_resetButton, &QPushButton::clicked, this, &MainWindow::resetMonitors);

    connect(m_referenceModeComboBox, &QComboBox::currentTextChanged, this,
            [this]() {
                updateReferenceMode();
                updateUiState();
            });
    connect(m_actionHelp, &QAction::triggered, this, &MainWindow::showHelpDialog);

    connect(m_actionDetect, &QAction::triggered, this, &MainWindow::detectMonitors);
}

/***************************************************************************/
/*                                                                         */
/*                        applyWindowProperties                            */
/*                                                                         */
/***************************************************************************/

void MainWindow::applyWindowProperties()
{
    setWindowTitle(tr("Multi Monitor Optimizer"));

    setMinimumSize(1400, 850);
    resize(1650, 950);

    if (m_mainSplitter != nullptr) {
        m_mainSplitter->setSizes({950, 650});
    }
}

/***************************************************************************/
/*                                                                         */
/*                         applySectionTitles                              */
/*                                                                         */
/***************************************************************************/

void MainWindow::applySectionTitles()
{
    if (m_monitorPanelTitleLabel != nullptr) {
        m_monitorPanelTitleLabel->setText(tr("Monitor Configuration"));
    }

    if (m_monitorPanelSubtitleLabel != nullptr) {
        m_monitorPanelSubtitleLabel->setText(
            tr("Add and configure all displays that are part of your current setup."));
    }

    if (m_analysisPanelTitleLabel != nullptr) {
        m_analysisPanelTitleLabel->setText(tr("Analysis & Recommendations"));
    }

    if (m_analysisPanelSubtitleLabel != nullptr) {
        m_analysisPanelSubtitleLabel->setText(
            tr("Review calculated DPI/PPI consistency, scaling alignment, and optimization guidance."));
    }
}

/***************************************************************************/
/*                                                                         */
/*                          addInitialMonitors                             */
/*                                                                         */
/***************************************************************************/

void MainWindow::addInitialMonitors()
{
    addMonitor();
    addMonitor();
}

/***************************************************************************/
/*                                                                         */
/*                            addMonitorCard                               */
/*                                                                         */
/***************************************************************************/

void MainWindow::addMonitorCard(MonitorWidget *monitorWidget)
{
    if (monitorWidget == nullptr || m_monitorListLayout == nullptr) {
        return;
    }

    const int stretchIndex = m_monitorListLayout->count() - 1;
    m_monitorListLayout->insertWidget(stretchIndex, monitorWidget);

    m_monitorWidgets.append(monitorWidget);

    connect(monitorWidget, &MonitorWidget::removeRequested, this,
            [this, monitorWidget]() {
                removeMonitor(monitorWidget);
            });
    connect(monitorWidget, &MonitorWidget::referenceSelected, this,
            [this](MonitorWidget* selectedWidget) {

                for (MonitorWidget* w : std::as_const(m_monitorWidgets)) {
                    if (w != nullptr && w != selectedWidget) {
                        w->setReferenceMonitor(false);
                    }
                }
            });

    rebuildMonitorTitles();
    updateUiState();
}

/***************************************************************************/
/*                                                                         */
/*                             addMonitor                                  */
/*                                                                         */
/***************************************************************************/

void MainWindow::addMonitor()
{
    MonitorWidget *monitorWidget = new MonitorWidget(m_monitorListContainer);
    monitorWidget->setMonitorTitle(tr("Monitor %1").arg(m_nextMonitorNumber));
    ++m_nextMonitorNumber;

    addMonitorCard(monitorWidget);

    updateStatusMessage(tr("Monitor added."), 2500);
}

/***************************************************************************/
/*                                                                         */
/*                           removeMonitor                                 */
/*                                                                         */
/***************************************************************************/

void MainWindow::removeMonitor(MonitorWidget *monitorWidget)
{
    if (monitorWidget == nullptr) {
        return;
    }

    const int index = m_monitorWidgets.indexOf(monitorWidget);
    if (index < 0) {
        return;
    }

    m_monitorWidgets.removeAt(index);
    m_monitorListLayout->removeWidget(monitorWidget);
    monitorWidget->deleteLater();

    rebuildMonitorTitles();
    updateUiState();
    updateStatusMessage(tr("Monitor removed."), 2500);
}

/***************************************************************************/
/*                                                                         */
/*                          rebuildMonitorTitles                           */
/*                                                                         */
/***************************************************************************/

void MainWindow::rebuildMonitorTitles()
{
    for (int i = 0; i < m_monitorWidgets.size(); ++i) {
        if (m_monitorWidgets[i] != nullptr) {
            m_monitorWidgets[i]->setMonitorTitle(tr("Monitor %1").arg(i + 1));
        }
    }

    m_nextMonitorNumber = m_monitorWidgets.size() + 1;
}

/***************************************************************************/
/*                                                                         */
/*                          clearMonitorCards                              */
/*                                                                         */
/***************************************************************************/

void MainWindow::clearMonitorCards()
{
    while (!m_monitorWidgets.isEmpty()) {
        MonitorWidget *monitorWidget = m_monitorWidgets.takeLast();
        if (monitorWidget != nullptr) {
            m_monitorListLayout->removeWidget(monitorWidget);
            monitorWidget->deleteLater();
        }
    }

    m_nextMonitorNumber = 1;
}

/***************************************************************************/
/*                                                                         */
/*                           resetMonitors                                 */
/*                                                                         */
/***************************************************************************/

void MainWindow::resetMonitors()
{
    clearMonitorCards();
    addInitialMonitors();

    if (m_referenceModeComboBox != nullptr) {
        m_referenceModeComboBox->setCurrentIndex(0);
    }

    if (m_resultsWidget != nullptr) {
        m_resultsWidget->resetView();
    }

    updateUiState();
    updateStatusMessage(tr("Monitor configuration reset."), 3000);
}

/***************************************************************************/
/*                                                                         */
/*                       analyzeConfiguration                              */
/*                                                                         */
/***************************************************************************/

void MainWindow::analyzeConfiguration()
{
    if (m_monitorWidgets.isEmpty()) {
        QMessageBox::information(
            this,
            tr("No Monitors"),
            tr("Please add at least one monitor before starting the analysis."));
        return;
    }

    QVector<Monitor> monitors;

    for (MonitorWidget *widget : m_monitorWidgets) {
        if (widget != nullptr) {
            monitors.append(widget->toMonitor());
        }
    }

    const bool automaticReference =
        (currentReferenceModeText().compare("Auto", Qt::CaseInsensitive) == 0);

    Optimizer optimizer;
    CalculationResult result = optimizer.analyze(monitors, automaticReference);

    if (m_resultsWidget != nullptr) {
        m_resultsWidget->showCalculationResult(result);
    }

    updateStatusMessage(
        result.success
            ? tr("Analysis completed successfully.")
            : tr("Analysis failed."),
        4000);
}

/***************************************************************************/
/*                                                                         */
/*                        updateReferenceMode                              */
/*                                                                         */
/***************************************************************************/

void MainWindow::updateReferenceMode()
{
    const QString mode = currentReferenceModeText();
    const bool isManual = (mode.compare("Manual", Qt::CaseInsensitive) == 0);

    /************************ Status Bar ************************/

    if (m_statusInfoLabel != nullptr) {
        m_statusInfoLabel->setText(tr("Reference Mode: %1").arg(mode));
    }

    /************************ Monitor Widgets ************************/

    for (MonitorWidget *widget : m_monitorWidgets) {
        if (widget != nullptr) {
            widget->setReferenceModeActive(isManual);
        }
    }

    /************************ UX Feedback ************************/

    if (isManual) {
        updateStatusMessage(
            tr("Manual mode: Please select a reference monitor."),
            4000);
    } else {
        updateStatusMessage(
            tr("Auto mode: Reference monitor will be selected automatically."),
            3000);
    }
}

/***************************************************************************/
/*                                                                         */
/*                          currentReferenceModeText                       */
/*                                                                         */
/***************************************************************************/

QString MainWindow::currentReferenceModeText() const
{
    if (m_referenceModeComboBox == nullptr) {
        return tr("Auto");
    }

    return m_referenceModeComboBox->currentText();
}

/***************************************************************************/
/*                                                                         */
/*                             updateUiState                               */
/*                                                                         */
/***************************************************************************/

void MainWindow::updateUiState()
{
    const bool hasMonitors = !m_monitorWidgets.isEmpty();

    if (m_actionAnalyze != nullptr) {
        m_actionAnalyze->setEnabled(hasMonitors);
    }

    if (m_actionReset != nullptr) {
        m_actionReset->setEnabled(hasMonitors);
    }

    if (m_analyzeButton != nullptr) {
        m_analyzeButton->setEnabled(hasMonitors);
    }

    if (m_resetButton != nullptr) {
        m_resetButton->setEnabled(hasMonitors);
    }
}

/***************************************************************************/
/*                                                                         */
/*                         updateStatusMessage                             */
/*                                                                         */
/***************************************************************************/

void MainWindow::updateStatusMessage(const QString &message, int timeoutMs)
{
    if (statusBar() == nullptr) {
        return;
    }

    if (timeoutMs > 0) {
        statusBar()->showMessage(message, timeoutMs);
    } else {
        statusBar()->showMessage(message);
    }
}

/***************************************************************************/
/*                                                                         */
/*                             showAboutDialog                             */
/*                                                                         */
/***************************************************************************/

void MainWindow::showAboutDialog()
{
    QMessageBox aboutBox(this);
    aboutBox.setWindowTitle(tr("About Multi Monitor Optimizer"));
    aboutBox.setTextFormat(Qt::RichText);
    aboutBox.setStandardButtons(QMessageBox::Ok);

    const QString htmlText =
        QStringLiteral(
            "<div style='min-width: 420px;'>"
            "<h2 style='margin-bottom: 4px;'>Multi Monitor Optimizer</h2>"
            "<p style='margin-top: 0;'>Professional desktop tool for analyzing DPI, PPI, and scaling consistency across multiple displays.</p>"
            "<p><b>Core capabilities:</b><br>"
            "• Multi-monitor configuration<br>"
            "• DPI/PPI analysis<br>"
            "• Scaling consistency recommendations<br>"
            "• Reference monitor strategies</p>"
            "<p>This version currently provides the professional UI foundation and analysis workflow scaffold.</p>"
            "</div>");

    aboutBox.setText(htmlText);
    aboutBox.setIconPixmap(
        style()->standardIcon(QStyle::SP_MessageBoxInformation).pixmap(72, 72));

    aboutBox.exec();
}

/***************************************************************************/
/*                                                                         */
/*                          showHelpDialog                                 */
/*                                                                         */
/***************************************************************************/

void MainWindow::showHelpDialog()
{
    QDialog dialog(this);
    dialog.setWindowTitle(tr("Documentation - Multi Monitor Optimizer"));
    dialog.resize(800, 600);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    /************************ Tabs ************************/

    QTabWidget *tabWidget = new QTabWidget(&dialog);

    /******************** Overview Tab ********************/

    QWidget *overviewTab = new QWidget();
    QVBoxLayout *overviewLayout = new QVBoxLayout(overviewTab);

    QLabel *overviewLabel = new QLabel(
        tr("<h2>Overview</h2>"
           "<p>This tool helps normalize DPI, PPI, and scaling across multiple monitors.</p>"
           "<p>It ensures that windows appear consistent when moving between displays.</p>"
           "<ul>"
           "<li>Analyze monitor differences</li>"
           "<li>Find scaling mismatches</li>"
           "<li>Get optimization suggestions</li>"
           "</ul>"));

    overviewLabel->setWordWrap(true);
    overviewLayout->addWidget(overviewLabel);

    /******************** Concepts Tab ********************/

    QWidget *conceptsTab = new QWidget();
    QVBoxLayout *conceptsLayout = new QVBoxLayout(conceptsTab);

    QLabel *conceptsLabel = new QLabel(
        tr("<h2>Key Concepts</h2>"

           "<h3>PPI (Pixels Per Inch)</h3>"
           "<p>Defines how many pixels exist per inch of screen.</p>"
           "<p>Higher PPI = sharper image, smaller UI elements.</p>"

           "<h3>DPI</h3>"
           "<p>Often used interchangeably with PPI, but in OS context refers to scaling behavior.</p>"

           "<h3>Scaling</h3>"
           "<p>Operating systems scale UI elements to compensate for high PPI displays.</p>"
           "<p>Example: 150% scaling makes everything 1.5x larger.</p>"

           "<h3>Problem</h3>"
           "<p>Different monitors → different PPI → windows change size when moved.</p>"));

    conceptsLabel->setWordWrap(true);
    conceptsLayout->addWidget(conceptsLabel);

    /******************** Usage Tab ********************/

    QWidget *usageTab = new QWidget();
    QVBoxLayout *usageLayout = new QVBoxLayout(usageTab);

    QLabel *usageLabel = new QLabel(
        tr("<h2>How to Use</h2>"
           "<ol>"
           "<li>Add your monitors</li>"
           "<li>Enter resolution and size</li>"
           "<li>Select scaling</li>"
           "<li>Choose reference mode</li>"
           "<li>Click Analyze</li>"
           "</ol>"

           "<h3>Reference Mode</h3>"
           "<p><b>Auto:</b> Tool selects best monitor</p>"
           "<p><b>Manual:</b> You choose reference monitor</p>"));

    usageLabel->setWordWrap(true);
    usageLayout->addWidget(usageLabel);

    /******************** Results Tab ********************/

    QWidget *resultsTab = new QWidget();
    QVBoxLayout *resultsLayout = new QVBoxLayout(resultsTab);

    QLabel *resultsLabel = new QLabel(
        tr("<h2>Understanding Results</h2>"

           "<h3>PPI Overview</h3>"
           "<p>Shows pixel density per monitor.</p>"

           "<h3>Delta %</h3>"
           "<p>Difference from reference monitor.</p>"

           "<h3>Suggested Scaling</h3>"
           "<p>Recommended OS scaling to match visual size.</p>"

           "<h3>Match Quality</h3>"
           "<ul>"
           "<li>Excellent (0–5%)</li>"
           "<li>Good (5–10%)</li>"
           "<li>Moderate (10–20%)</li>"
           "<li>Poor (>20%)</li>"
           "</ul>"));

    resultsLabel->setWordWrap(true);
    resultsLayout->addWidget(resultsLabel);

    /******************** Add Tabs ********************/

    tabWidget->addTab(overviewTab, tr("Overview"));
    tabWidget->addTab(conceptsTab, tr("Concepts"));
    tabWidget->addTab(usageTab, tr("Usage"));
    tabWidget->addTab(resultsTab, tr("Results"));

    layout->addWidget(tabWidget);

    /************************ Close Button ************************/

    QPushButton *closeButton = new QPushButton(tr("Close"));
    layout->addWidget(closeButton);

    connect(closeButton, &QPushButton::clicked, &dialog, &QDialog::accept);

    dialog.exec();
}


/***************************************************************************/
/*                                                                         */
/*                       detectMonitors                                    */
/*                                                                         */
/***************************************************************************/

void MainWindow::detectMonitors()
{
    clearMonitorCards();

    const QList<QScreen*> screens = QGuiApplication::screens();

    int index = 1;

    for (QScreen* screen : screens) {
        if (!screen) continue;

        MonitorWidget* widget = new MonitorWidget(m_monitorListContainer);

        /************************ Name ************************/

        widget->setMonitorTitle(tr("Monitor %1").arg(index));
        widget->setMonitorName(screen->name());

        /************************ Resolution ************************/

        const QRect geometry = screen->geometry();
        widget->setResolutionWidth(geometry.width());
        widget->setResolutionHeight(geometry.height());

        /************************ Physical Size ************************/

        QSizeF physicalSizeMM = screen->physicalSize();

        double diagonalInches = 24.0; // fallback

        if (physicalSizeMM.width() > 0 && physicalSizeMM.height() > 0) {
            const double widthInches  = physicalSizeMM.width() / 25.4;
            const double heightInches = physicalSizeMM.height() / 25.4;

            diagonalInches = std::sqrt(
                widthInches * widthInches +
                heightInches * heightInches
                );
        }

        widget->setDiagonalInches(diagonalInches);

        /************************ Scaling ************************/

        const double dpi = screen->logicalDotsPerInch();
        const int scaling = static_cast<int>((dpi / 96.0) * 100.0);

        widget->setScalingPercent(qBound(100, scaling, 300));

        /************************ Position ************************/

        if (screen == QGuiApplication::primaryScreen()) {
            widget->setPositionText("Primary");
            widget->setReferenceMonitor(true);
        } else {
            widget->setPositionText("Secondary");
        }

        addMonitorCard(widget);
        ++index;
    }

    updateStatusMessage(
        tr("%1 monitor(s) detected automatically.").arg(screens.size()),
        4000
        );
}
