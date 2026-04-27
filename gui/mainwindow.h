#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>

class QAction;
class QLabel;
class QMenuBar;
class QScrollArea;
class QVBoxLayout;
class QWidget;
class QComboBox;
class QPushButton;
class QFrame;
class QSplitter;

class MonitorWidget;
class ResultsWidget;

/***************************************************************************/
/*                                                                         */
/*                             MainWindow                                  */
/*                                                                         */
/***************************************************************************/

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void addMonitor();
    void resetMonitors();
    void analyzeConfiguration();
    void removeMonitor(MonitorWidget *monitorWidget);
    void updateReferenceMode();
    void showAboutDialog();
    void showHelpDialog();

private:
    void createCentralUi();
    void createMenuBar(QMenuBar *menuBar);
    void createToolBar();
    void createStatusBar();
    void createWindowsAndWidgets();
    void setupActions();
    void setupConnections();
    void applyActionToolTip(QAction *action);
    void applyWindowProperties();
    void applySectionTitles();
    void addInitialMonitors();
    void addMonitorCard(MonitorWidget *monitorWidget);
    void rebuildMonitorTitles();
    void clearMonitorCards();
    void updateUiState();
    void updateStatusMessage(const QString &message, int timeoutMs = 0);
    QString currentReferenceModeText() const;

private:
    /************************ Actions ************************/

    QAction *m_actionAddMonitor = nullptr;
    QAction *m_actionAnalyze = nullptr;
    QAction *m_actionReset = nullptr;
    QAction *m_actionExit = nullptr;
    QAction *m_actionAbout = nullptr;
    QAction *m_actionHelp = nullptr;
    QAction* m_actionDetect = nullptr;

    /************************ Central UI ************************/

    QWidget *m_centralWidget = nullptr;
    QWidget *m_headerWidget = nullptr;
    QWidget *m_controlBarWidget = nullptr;
    QWidget *m_contentWidget = nullptr;

    QLabel *m_titleLabel = nullptr;
    QLabel *m_subtitleLabel = nullptr;

    QPushButton *m_addMonitorButton = nullptr;
    QPushButton *m_analyzeButton = nullptr;
    QPushButton *m_resetButton = nullptr;
    QLabel *m_referenceModeLabel = nullptr;
    QComboBox *m_referenceModeComboBox = nullptr;

    QSplitter *m_mainSplitter = nullptr;

    /************************ Left ************************/

    QFrame *m_monitorPanel = nullptr;
    QLabel *m_monitorPanelTitleLabel = nullptr;
    QLabel *m_monitorPanelSubtitleLabel = nullptr;

    QScrollArea *m_monitorScrollArea = nullptr;
    QWidget *m_monitorListContainer = nullptr;
    QVBoxLayout *m_monitorListLayout = nullptr;

    /************************ Right ************************/

    QFrame *m_analysisPanel = nullptr;
    QLabel *m_analysisPanelTitleLabel = nullptr;
    QLabel *m_analysisPanelSubtitleLabel = nullptr;

    ResultsWidget *m_resultsWidget = nullptr;

    /************************ Status ************************/

    QLabel *m_statusInfoLabel = nullptr;

    /************************ Data ************************/

    QList<MonitorWidget*> m_monitorWidgets;
    int m_nextMonitorNumber = 1;

    /************************ Detect Monitors ************************/

    void detectMonitors();
};

#endif // MAINWINDOW_H
