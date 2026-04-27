#include "monitorwidget.h"

#include "core/monitor.h"

#include <QCheckBox>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QStyle>
#include <QVBoxLayout>
#include <QWidget>

/***************************************************************************/
/*                                                                         */
/*                           MonitorWidget                                 */
/*                                                                         */
/***************************************************************************/

MonitorWidget::MonitorWidget(QWidget *parent)
    : QFrame(parent)
{
    createUi();
    applyDefaultValues();
    applyStyles();
    applyHelpTexts();
    setupConnections();
}

/***************************************************************************/
/*                                                                         */
/*                          ~MonitorWidget                                 */
/*                                                                         */
/***************************************************************************/

MonitorWidget::~MonitorWidget() = default;

/***************************************************************************/
/*                                                                         */
/*                             createUi                                    */
/*                                                                         */
/***************************************************************************/

void MonitorWidget::createUi()
{
    setObjectName(QStringLiteral("monitorWidget"));
    setFrameShape(QFrame::StyledPanel);
    setFrameShadow(QFrame::Plain);

    QVBoxLayout *rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(16, 16, 16, 16);
    rootLayout->setSpacing(14);

    /************************ Header ************************/

    QWidget *headerWidget = new QWidget(this);
    QHBoxLayout *headerLayout = new QHBoxLayout(headerWidget);
    headerLayout->setContentsMargins(0, 0, 0, 0);
    headerLayout->setSpacing(12);

    m_titleLabel = new QLabel(tr("Monitor"), headerWidget);
    m_titleLabel->setObjectName(QStringLiteral("monitorTitleLabel"));

    headerLayout->addWidget(m_titleLabel);
    headerLayout->addStretch(1);

    /************************ Form ************************/

    QWidget *formWidget = new QWidget(this);
    QGridLayout *formLayout = new QGridLayout(formWidget);
    formLayout->setContentsMargins(0, 0, 0, 0);
    formLayout->setHorizontalSpacing(12);
    formLayout->setVerticalSpacing(10);

    m_nameLabel = new QLabel(tr("Name:"), formWidget);
    m_resolutionLabel = new QLabel(tr("Resolution:"), formWidget);
    m_sizeLabel = new QLabel(tr("Size (inches):"), formWidget);
    m_scalingLabel = new QLabel(tr("Scaling:"), formWidget);
    m_positionLabel = new QLabel(tr("Position:"), formWidget);

    m_nameLineEdit = new QLineEdit(formWidget);
    m_nameLineEdit->setPlaceholderText(tr("e.g. Primary Ultrawide"));

    m_widthSpinBox = new QSpinBox(formWidget);
    m_widthSpinBox->setRange(320, 16384);
    m_widthSpinBox->setSingleStep(80);
    m_widthSpinBox->setSuffix(tr(" px"));

    m_heightSpinBox = new QSpinBox(formWidget);
    m_heightSpinBox->setRange(240, 8640);
    m_heightSpinBox->setSingleStep(60);
    m_heightSpinBox->setSuffix(tr(" px"));

    QWidget *resolutionWidget = new QWidget(formWidget);
    QHBoxLayout *resolutionLayout = new QHBoxLayout(resolutionWidget);
    resolutionLayout->setContentsMargins(0, 0, 0, 0);
    resolutionLayout->setSpacing(8);

    QLabel *timesLabel = new QLabel(QStringLiteral("×"), resolutionWidget);
    timesLabel->setAlignment(Qt::AlignCenter);

    resolutionLayout->addWidget(m_widthSpinBox);
    resolutionLayout->addWidget(timesLabel);
    resolutionLayout->addWidget(m_heightSpinBox);

    m_sizeSpinBox = new QDoubleSpinBox(formWidget);
    m_sizeSpinBox->setRange(5.0, 100.0);
    m_sizeSpinBox->setDecimals(1);
    m_sizeSpinBox->setSingleStep(0.5);
    m_sizeSpinBox->setSuffix(tr(" in"));

    m_scalingComboBox = new QComboBox(formWidget);
    m_scalingComboBox->addItem(tr("100%"), 100);
    m_scalingComboBox->addItem(tr("125%"), 125);
    m_scalingComboBox->addItem(tr("150%"), 150);
    m_scalingComboBox->addItem(tr("175%"), 175);
    m_scalingComboBox->addItem(tr("200%"), 200);
    m_scalingComboBox->addItem(tr("225%"), 225);
    m_scalingComboBox->addItem(tr("250%"), 250);

    m_positionComboBox = new QComboBox(formWidget);
    m_positionComboBox->addItem(tr("Primary"));
    m_positionComboBox->addItem(tr("Left of primary"));
    m_positionComboBox->addItem(tr("Right of primary"));
    m_positionComboBox->addItem(tr("Above primary"));
    m_positionComboBox->addItem(tr("Below primary"));
    m_positionComboBox->addItem(tr("Custom"));

    formLayout->addWidget(m_nameLabel, 0, 0);
    formLayout->addWidget(m_nameLineEdit, 0, 1);

    formLayout->addWidget(m_resolutionLabel, 1, 0);
    formLayout->addWidget(resolutionWidget, 1, 1);

    formLayout->addWidget(m_sizeLabel, 2, 0);
    formLayout->addWidget(m_sizeSpinBox, 2, 1);

    formLayout->addWidget(m_scalingLabel, 3, 0);
    formLayout->addWidget(m_scalingComboBox, 3, 1);

    formLayout->addWidget(m_positionLabel, 4, 0);
    formLayout->addWidget(m_positionComboBox, 4, 1);

    formLayout->setColumnStretch(1, 1);

    /************************ Footer ************************/

    QWidget *footerWidget = new QWidget(this);
    QHBoxLayout *footerLayout = new QHBoxLayout(footerWidget);
    footerLayout->setContentsMargins(0, 0, 0, 0);
    footerLayout->setSpacing(10);

    m_referenceCheckBox = new QCheckBox(tr("Set as Reference"), footerWidget);

    m_removeButton = new QPushButton(
        style()->standardIcon(QStyle::SP_DialogDiscardButton),
        tr("Remove"),
        footerWidget);

    footerLayout->addWidget(m_referenceCheckBox);
    footerLayout->addStretch(1);
    footerLayout->addWidget(m_removeButton);

    /************************ Root ************************/

    rootLayout->addWidget(headerWidget);
    rootLayout->addWidget(formWidget);
    rootLayout->addWidget(footerWidget);
}

/***************************************************************************/
/*                                                                         */
/*                        applyDefaultValues                               */
/*                                                                         */
/***************************************************************************/

void MonitorWidget::applyDefaultValues()
{
    m_nameLineEdit->setText(tr("Display"));
    m_widthSpinBox->setValue(1920);
    m_heightSpinBox->setValue(1080);
    m_sizeSpinBox->setValue(24.0);
    m_scalingComboBox->setCurrentIndex(0);
    m_positionComboBox->setCurrentIndex(0);
    m_referenceCheckBox->setChecked(false);
}

/***************************************************************************/
/*                                                                         */
/*                           applyStyles                                   */
/*                                                                         */
/***************************************************************************/

void MonitorWidget::applyStyles()
{
    m_titleLabel->setStyleSheet(
        QStringLiteral("font-size: 16px; font-weight: 600;"));

    setStyleSheet(QStringLiteral(
        "QFrame#monitorWidget {"
        "    border: 1px solid rgba(255, 255, 255, 0.12);"
        "    border-radius: 10px;"
        "    background-color: rgba(255, 255, 255, 0.03);"
        "}"
        ));
}

/***************************************************************************/
/*                                                                         */
/*                         applyHelpTexts                                  */
/*                                                                         */
/***************************************************************************/

void MonitorWidget::applyHelpTexts()
{
    const QString nameText = nameHelpText();
    const QString resolutionText = resolutionHelpText();
    const QString sizeText = sizeHelpText();
    const QString scalingText = scalingHelpText();
    const QString positionTextValue = positionHelpText();
    const QString referenceText = referenceHelpText();

    if (m_titleLabel != nullptr) {
        m_titleLabel->setToolTip(tr("Configuration card for one physical display."));
        m_titleLabel->setStatusTip(tr("Monitor card title"));
        m_titleLabel->setWhatsThis(tr("Each card represents one monitor in your setup. Enter its technical properties here."));
    }

    if (m_nameLabel != nullptr) {
        m_nameLabel->setToolTip(nameText);
        m_nameLabel->setStatusTip(tr("Display name"));
        m_nameLabel->setWhatsThis(nameText);
    }

    if (m_nameLineEdit != nullptr) {
        m_nameLineEdit->setToolTip(nameText);
        m_nameLineEdit->setStatusTip(tr("Enter a custom name for this monitor"));
        m_nameLineEdit->setWhatsThis(nameText);
    }

    if (m_resolutionLabel != nullptr) {
        m_resolutionLabel->setToolTip(resolutionText);
        m_resolutionLabel->setStatusTip(tr("Display resolution in pixels"));
        m_resolutionLabel->setWhatsThis(resolutionText);
    }

    if (m_widthSpinBox != nullptr) {
        m_widthSpinBox->setToolTip(tr("Horizontal resolution in pixels.\n\nExample: 1920, 2560, 3440, 3840."));
        m_widthSpinBox->setStatusTip(tr("Monitor width in pixels"));
        m_widthSpinBox->setWhatsThis(resolutionText);
    }

    if (m_heightSpinBox != nullptr) {
        m_heightSpinBox->setToolTip(tr("Vertical resolution in pixels.\n\nExample: 1080, 1440, 2160."));
        m_heightSpinBox->setStatusTip(tr("Monitor height in pixels"));
        m_heightSpinBox->setWhatsThis(resolutionText);
    }

    if (m_sizeLabel != nullptr) {
        m_sizeLabel->setToolTip(sizeText);
        m_sizeLabel->setStatusTip(tr("Physical screen size in inches"));
        m_sizeLabel->setWhatsThis(sizeText);
    }

    if (m_sizeSpinBox != nullptr) {
        m_sizeSpinBox->setToolTip(sizeText);
        m_sizeSpinBox->setStatusTip(tr("Enter the diagonal screen size in inches"));
        m_sizeSpinBox->setWhatsThis(sizeText);
    }

    if (m_scalingLabel != nullptr) {
        m_scalingLabel->setToolTip(scalingText);
        m_scalingLabel->setStatusTip(tr("Operating system scaling factor"));
        m_scalingLabel->setWhatsThis(scalingText);
    }

    if (m_scalingComboBox != nullptr) {
        m_scalingComboBox->setToolTip(scalingText);
        m_scalingComboBox->setStatusTip(tr("Choose the OS scaling used on this display"));
        m_scalingComboBox->setWhatsThis(scalingText);
    }

    if (m_positionLabel != nullptr) {
        m_positionLabel->setToolTip(positionTextValue);
        m_positionLabel->setStatusTip(tr("Relative screen position"));
        m_positionLabel->setWhatsThis(positionTextValue);
    }

    if (m_positionComboBox != nullptr) {
        m_positionComboBox->setToolTip(positionTextValue);
        m_positionComboBox->setStatusTip(tr("Choose where this monitor is placed relative to the primary display"));
        m_positionComboBox->setWhatsThis(positionTextValue);
    }

    if (m_referenceCheckBox != nullptr) {
        m_referenceCheckBox->setToolTip(referenceText);
        m_referenceCheckBox->setStatusTip(tr("Use this display as the reference monitor"));
        m_referenceCheckBox->setWhatsThis(referenceText);
    }

    if (m_removeButton != nullptr) {
        m_removeButton->setToolTip(tr("Remove this monitor from the current configuration."));
        m_removeButton->setStatusTip(tr("Remove this monitor card"));
        m_removeButton->setWhatsThis(tr("Deletes this monitor entry from the current setup. This does not change any system settings."));
    }
}

/***************************************************************************/
/*                                                                         */
/*                         setupConnections                                */
/*                                                                         */
/***************************************************************************/

void MonitorWidget::setupConnections()
{
    connect(m_removeButton, &QPushButton::clicked,
            this, &MonitorWidget::emitRemoveRequested);

    connect(m_referenceCheckBox, &QCheckBox::toggled, this,
            [this](bool checked) {
                if (checked) {
                    emit referenceSelected(this);
                }
            });
}

/***************************************************************************/
/*                                                                         */
/*                           nameHelpText                                  */
/*                                                                         */
/***************************************************************************/

QString MonitorWidget::nameHelpText() const
{
    return tr(
        "This is a user-defined name for the monitor.\n\n"
        "It is used to identify the display more easily in the results and recommendations.\n\n"
        "Examples:\n"
        "• Primary Ultrawide\n"
        "• Office Monitor\n"
        "• Laptop Display");
}

/***************************************************************************/
/*                                                                         */
/*                        resolutionHelpText                               */
/*                                                                         */
/***************************************************************************/

QString MonitorWidget::resolutionHelpText() const
{
    return tr(
        "Resolution is the number of pixels displayed horizontally and vertically.\n\n"
        "Examples:\n"
        "• 1920 × 1080\n"
        "• 2560 × 1440\n"
        "• 3440 × 1440\n"
        "• 3840 × 2160\n\n"
        "The resolution strongly affects pixel density (PPI). Two monitors with the same "
        "physical size but different resolutions will have different sharpness and visual scale.");
}

/***************************************************************************/
/*                                                                         */
/*                           sizeHelpText                                  */
/*                                                                         */
/***************************************************************************/

QString MonitorWidget::sizeHelpText() const
{
    return tr(
        "Enter the physical diagonal size of the monitor in inches.\n\n"
        "Examples:\n"
        "• 24.0 in\n"
        "• 27.0 in\n"
        "• 34.0 in\n\n"
        "This value is required to calculate PPI (Pixels Per Inch).\n\n"
        "If the size is wrong, the calculated monitor density and scaling recommendations "
        "will also be wrong.");
}

/***************************************************************************/
/*                                                                         */
/*                         scalingHelpText                                 */
/*                                                                         */
/***************************************************************************/

QString MonitorWidget::scalingHelpText() const
{
    return tr(
        "Scaling is the operating system's UI enlargement factor for this monitor.\n\n"
        "Examples:\n"
        "• 100%% = original size\n"
        "• 125%% = UI appears 1.25× larger\n"
        "• 150%% = UI appears 1.5× larger\n\n"
        "Scaling is important because two monitors with different pixel densities may still "
        "look visually similar if the OS scaling compensates for the difference.\n\n"
        "This tool uses scaling together with PPI to estimate how consistently windows will appear.");
}

/***************************************************************************/
/*                                                                         */
/*                         positionHelpText                                */
/*                                                                         */
/***************************************************************************/

QString MonitorWidget::positionHelpText() const
{
    return tr(
        "This describes where the monitor is placed relative to the main screen.\n\n"
        "Examples:\n"
        "• Primary\n"
        "• Left of primary\n"
        "• Right of primary\n"
        "• Above primary\n"
        "• Below primary\n\n"
        "The position does not change the PPI calculation, but it helps document the real layout "
        "of your multi-monitor setup and can be useful for future visualization features.");
}

/***************************************************************************/
/*                                                                         */
/*                        referenceHelpText                                */
/*                                                                         */
/***************************************************************************/

QString MonitorWidget::referenceHelpText() const
{
    return tr(
        "The reference monitor is the visual baseline used for comparison.\n\n"
        "Other monitors are evaluated against it to calculate differences in PPI, scaling, "
        "and suggested adjustments.\n\n"
        "Use this option when Reference Mode in the main window is set to Manual.");
}

/***************************************************************************/
/*                                                                         */
/*                       emitRemoveRequested                               */
/*                                                                         */
/***************************************************************************/

void MonitorWidget::emitRemoveRequested()
{
    emit removeRequested();
}

/***************************************************************************/
/*                                                                         */
/*                        setMonitorTitle                                  */
/*                                                                         */
/***************************************************************************/

void MonitorWidget::setMonitorTitle(const QString &title)
{
    if (m_titleLabel != nullptr) {
        m_titleLabel->setText(title);
    }
}

/***************************************************************************/
/*                                                                         */
/*                         monitorTitle                                    */
/*                                                                         */
/***************************************************************************/

QString MonitorWidget::monitorTitle() const
{
    return (m_titleLabel != nullptr) ? m_titleLabel->text() : QString();
}

/***************************************************************************/
/*                                                                         */
/*                         monitorName                                     */
/*                                                                         */
/***************************************************************************/

QString MonitorWidget::monitorName() const
{
    return (m_nameLineEdit != nullptr) ? m_nameLineEdit->text().trimmed() : QString();
}

/***************************************************************************/
/*                                                                         */
/*                       resolutionWidth                                   */
/*                                                                         */
/***************************************************************************/

int MonitorWidget::resolutionWidth() const
{
    return (m_widthSpinBox != nullptr) ? m_widthSpinBox->value() : 0;
}

/***************************************************************************/
/*                                                                         */
/*                      resolutionHeight                                   */
/*                                                                         */
/***************************************************************************/

int MonitorWidget::resolutionHeight() const
{
    return (m_heightSpinBox != nullptr) ? m_heightSpinBox->value() : 0;
}

/***************************************************************************/
/*                                                                         */
/*                       diagonalInches                                    */
/*                                                                         */
/***************************************************************************/

double MonitorWidget::diagonalInches() const
{
    return (m_sizeSpinBox != nullptr) ? m_sizeSpinBox->value() : 0.0;
}

/***************************************************************************/
/*                                                                         */
/*                       scalingPercent                                    */
/*                                                                         */
/***************************************************************************/

int MonitorWidget::scalingPercent() const
{
    if (m_scalingComboBox == nullptr) {
        return 100;
    }

    const QVariant userData = m_scalingComboBox->currentData();
    if (userData.isValid()) {
        return userData.toInt();
    }

    return 100;
}

/***************************************************************************/
/*                                                                         */
/*                        positionText                                     */
/*                                                                         */
/***************************************************************************/

QString MonitorWidget::positionText() const
{
    return (m_positionComboBox != nullptr)
    ? m_positionComboBox->currentText()
    : QString();
}

/***************************************************************************/
/*                                                                         */
/*                      isReferenceMonitor                                 */
/*                                                                         */
/***************************************************************************/

bool MonitorWidget::isReferenceMonitor() const
{
    return (m_referenceCheckBox != nullptr) && m_referenceCheckBox->isChecked();
}

/***************************************************************************/
/*                                                                         */
/*                        setMonitorName                                   */
/*                                                                         */
/***************************************************************************/

void MonitorWidget::setMonitorName(const QString &name)
{
    if (m_nameLineEdit != nullptr) {
        m_nameLineEdit->setText(name);
    }
}

/***************************************************************************/
/*                                                                         */
/*                      setResolutionWidth                                 */
/*                                                                         */
/***************************************************************************/

void MonitorWidget::setResolutionWidth(int width)
{
    if (m_widthSpinBox != nullptr) {
        m_widthSpinBox->setValue(width);
    }
}

/***************************************************************************/
/*                                                                         */
/*                     setResolutionHeight                                 */
/*                                                                         */
/***************************************************************************/

void MonitorWidget::setResolutionHeight(int height)
{
    if (m_heightSpinBox != nullptr) {
        m_heightSpinBox->setValue(height);
    }
}

/***************************************************************************/
/*                                                                         */
/*                      setDiagonalInches                                  */
/*                                                                         */
/***************************************************************************/

void MonitorWidget::setDiagonalInches(double inches)
{
    if (m_sizeSpinBox != nullptr) {
        m_sizeSpinBox->setValue(inches);
    }
}

/***************************************************************************/
/*                                                                         */
/*                      setScalingPercent                                  */
/*                                                                         */
/***************************************************************************/

void MonitorWidget::setScalingPercent(int percent)
{
    if (m_scalingComboBox == nullptr) {
        return;
    }

    const int index = m_scalingComboBox->findData(percent);
    if (index >= 0) {
        m_scalingComboBox->setCurrentIndex(index);
    }
}

/***************************************************************************/
/*                                                                         */
/*                       setPositionText                                   */
/*                                                                         */
/***************************************************************************/

void MonitorWidget::setPositionText(const QString &position)
{
    if (m_positionComboBox == nullptr) {
        return;
    }

    const int index = m_positionComboBox->findText(position);
    if (index >= 0) {
        m_positionComboBox->setCurrentIndex(index);
    }
}

/***************************************************************************/
/*                                                                         */
/*                    setReferenceMonitor                                  */
/*                                                                         */
/***************************************************************************/

void MonitorWidget::setReferenceMonitor(bool reference)
{
    if (m_referenceCheckBox != nullptr) {
        m_referenceCheckBox->setChecked(reference);
    }
}

/***************************************************************************/
/*                                                                         */
/*                  setReferenceModeActive                                 */
/*                                                                         */
/***************************************************************************/

void MonitorWidget::setReferenceModeActive(bool active)
{
    if (m_referenceCheckBox == nullptr) {
        return;
    }

    if (active) {
        // Visuelles Highlight
        m_referenceCheckBox->setStyleSheet(
            "QCheckBox {"
            "   font-weight: bold;"
            "   color: #4FC3F7;"  // schönes Blau
            "}"
            );

        m_referenceCheckBox->setToolTip(
            tr("Select exactly one monitor as the reference.\n"
               "All other monitors will be compared against it.")
            );
    } else {
        // Reset
        m_referenceCheckBox->setStyleSheet("");
        m_referenceCheckBox->setToolTip(referenceHelpText());
    }
}

/***************************************************************************/
/*                                                                         */
/*                             toMonitor                                   */
/*                                                                         */
/***************************************************************************/

Monitor MonitorWidget::toMonitor() const
{
    Monitor monitor;

    monitor.setTitle(monitorTitle());
    monitor.setName(monitorName());
    monitor.setWidthPixels(resolutionWidth());
    monitor.setHeightPixels(resolutionHeight());
    monitor.setDiagonalInches(diagonalInches());
    monitor.setScalingPercent(scalingPercent());
    monitor.setPosition(positionText());
    monitor.setReferenceMonitor(isReferenceMonitor());

    return monitor;
}
