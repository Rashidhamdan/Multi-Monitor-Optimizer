#ifndef MONITORWIDGET_H
#define MONITORWIDGET_H

#include <QFrame>

class QLabel;
class QLineEdit;
class QSpinBox;
class QDoubleSpinBox;
class QComboBox;
class QCheckBox;
class QPushButton;
class QString;

class Monitor;

/***************************************************************************/
/*                                                                         */
/*                           MonitorWidget                                 */
/*                                                                         */
/***************************************************************************/

class MonitorWidget : public QFrame
{
    Q_OBJECT

public:
    explicit MonitorWidget(QWidget *parent = nullptr);
    ~MonitorWidget() override;

    void setMonitorTitle(const QString &title);

    QString monitorTitle() const;
    QString monitorName() const;
    int resolutionWidth() const;
    int resolutionHeight() const;
    double diagonalInches() const;
    int scalingPercent() const;
    QString positionText() const;
    bool isReferenceMonitor() const;

    void setMonitorName(const QString &name);
    void setResolutionWidth(int width);
    void setResolutionHeight(int height);
    void setDiagonalInches(double inches);
    void setScalingPercent(int percent);
    void setPositionText(const QString &position);
    void setReferenceMonitor(bool reference);
    void setReferenceModeActive(bool active);

    Monitor toMonitor() const;

signals:
    void removeRequested();
    void referenceSelected(MonitorWidget* widget);

private slots:
    void emitRemoveRequested();

private:
    void createUi();
    void applyDefaultValues();
    void applyStyles();
    void setupConnections();
    void applyHelpTexts();

    QString resolutionHelpText() const;
    QString sizeHelpText() const;
    QString scalingHelpText() const;
    QString positionHelpText() const;
    QString referenceHelpText() const;
    QString nameHelpText() const;

private:
    QLabel *m_titleLabel = nullptr;

    QLabel *m_nameLabel = nullptr;
    QLabel *m_resolutionLabel = nullptr;
    QLabel *m_sizeLabel = nullptr;
    QLabel *m_scalingLabel = nullptr;
    QLabel *m_positionLabel = nullptr;

    QLineEdit *m_nameLineEdit = nullptr;
    QSpinBox *m_widthSpinBox = nullptr;
    QSpinBox *m_heightSpinBox = nullptr;
    QDoubleSpinBox *m_sizeSpinBox = nullptr;
    QComboBox *m_scalingComboBox = nullptr;
    QComboBox *m_positionComboBox = nullptr;

    QCheckBox *m_referenceCheckBox = nullptr;
    QPushButton *m_removeButton = nullptr;
};

#endif // MONITORWIDGET_H
