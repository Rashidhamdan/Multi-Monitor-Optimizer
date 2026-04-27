#ifndef RESULTITEMWIDGET_H
#define RESULTITEMWIDGET_H

#include <QFrame>

class QLabel;
class QHBoxLayout;
class QVBoxLayout;

/***************************************************************************/
/*                                                                         */
/*                         ResultItemWidget                                */
/*                                                                         */
/***************************************************************************/

class ResultItemWidget : public QFrame
{
    Q_OBJECT

public:
    explicit ResultItemWidget(QWidget *parent = nullptr);
    ~ResultItemWidget() override;

    void setData(const QString &name,
                 double ppi,
                 double deltaPercent,
                 int currentScaling,
                 int recommendedScaling,
                 bool isReference,
                 const QString &quality);

private:
    void createUi();
    void applyStyles();
    QString qualityColor(const QString &quality) const;

private:
    QLabel *m_nameLabel = nullptr;
    QLabel *m_badgeLabel = nullptr;

    QLabel *m_ppiLabel = nullptr;
    QLabel *m_deltaLabel = nullptr;
    QLabel *m_scalingLabel = nullptr;
};

#endif
