#include "resultitemwidget.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

/***************************************************************************/
/*                                                                         */
/*                         ResultItemWidget                                */
/*                                                                         */
/***************************************************************************/

ResultItemWidget::ResultItemWidget(QWidget *parent)
    : QFrame(parent)
{
    createUi();
    applyStyles();
}

ResultItemWidget::~ResultItemWidget() = default;

/***************************************************************************/
/*                                                                         */
/*                             createUi                                    */
/*                                                                         */
/***************************************************************************/

void ResultItemWidget::createUi()
{
    setObjectName("resultItem");

    QVBoxLayout *root = new QVBoxLayout(this);
    root->setContentsMargins(12,12,12,12);
    root->setSpacing(6);

    /************************ Top Row ************************/

    QHBoxLayout *top = new QHBoxLayout();

    m_nameLabel = new QLabel(this);
    m_nameLabel->setStyleSheet("font-weight:600;");

    m_badgeLabel = new QLabel(this);

    top->addWidget(m_nameLabel);
    top->addStretch();
    top->addWidget(m_badgeLabel);

    /************************ Bottom Row ************************/

    QHBoxLayout *bottom = new QHBoxLayout();

    m_ppiLabel = new QLabel(this);
    m_deltaLabel = new QLabel(this);
    m_scalingLabel = new QLabel(this);

    bottom->addWidget(m_ppiLabel);
    bottom->addWidget(m_deltaLabel);
    bottom->addWidget(m_scalingLabel);

    root->addLayout(top);
    root->addLayout(bottom);
}

/***************************************************************************/
/*                                                                         */
/*                             applyStyles                                 */
/*                                                                         */
/***************************************************************************/

void ResultItemWidget::applyStyles()
{
    setStyleSheet(
        "#resultItem {"
        "border:1px solid rgba(255,255,255,0.1);"
        "border-radius:8px;"
        "background:rgba(255,255,255,0.02);"
        "}"
        );
}

/***************************************************************************/
/*                                                                         */
/*                             setData                                     */
/*                                                                         */
/***************************************************************************/

void ResultItemWidget::setData(const QString &name,
                               double ppi,
                               double deltaPercent,
                               int currentScaling,
                               int recommendedScaling,
                               bool isReference,
                               const QString &quality)
{
    m_nameLabel->setText(name);

    /************************ Badge ************************/

    if (isReference) {
        m_badgeLabel->setText("REFERENCE");
        m_badgeLabel->setStyleSheet(
            "color:#4CAF50; font-weight:600;"
            );
    } else {
        m_badgeLabel->setText(quality.toUpper());
        m_badgeLabel->setStyleSheet(
            QString("color:%1; font-weight:600;")
                .arg(qualityColor(quality))
            );
    }

    /************************ Values ************************/

    m_ppiLabel->setText(
        QString("PPI: %1").arg(ppi,0,'f',1)
        );

    if (!isReference) {
        m_deltaLabel->setText(
            QString("Δ %1%").arg(deltaPercent,0,'f',1)
            );

        m_scalingLabel->setText(
            QString("Scaling: %1% → %2%")
                .arg(currentScaling)
                .arg(recommendedScaling)
            );
    } else {
        m_deltaLabel->setText("Baseline");
        m_scalingLabel->setText(
            QString("Scaling: %1%").arg(currentScaling)
            );
    }
}

/***************************************************************************/
/*                                                                         */
/*                         qualityColor                                    */
/*                                                                         */
/***************************************************************************/

QString ResultItemWidget::qualityColor(const QString &quality) const
{
    if (quality.contains("Excellent", Qt::CaseInsensitive))
        return "#4CAF50"; // green

    if (quality.contains("Good", Qt::CaseInsensitive))
        return "#8BC34A";

    if (quality.contains("Moderate", Qt::CaseInsensitive))
        return "#FFC107";

    return "#F44336"; // poor
}
