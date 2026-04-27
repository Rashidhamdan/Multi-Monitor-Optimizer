#ifndef MONITOR_H
#define MONITOR_H

#include <QString>

/***************************************************************************/
/*                                                                         */
/*                               Monitor                                   */
/*                                                                         */
/***************************************************************************/

class Monitor
{
public:
    Monitor();
    Monitor(const QString &title,
            const QString &name,
            int widthPixels,
            int heightPixels,
            double diagonalInches,
            int scalingPercent,
            const QString &position,
            bool referenceMonitor);

    QString title() const;
    QString name() const;
    int widthPixels() const;
    int heightPixels() const;
    double diagonalInches() const;
    int scalingPercent() const;
    QString position() const;
    bool isReferenceMonitor() const;

    void setTitle(const QString &title);
    void setName(const QString &name);
    void setWidthPixels(int widthPixels);
    void setHeightPixels(int heightPixels);
    void setDiagonalInches(double diagonalInches);
    void setScalingPercent(int scalingPercent);
    void setPosition(const QString &position);
    void setReferenceMonitor(bool referenceMonitor);

    bool isValid() const;

private:
    QString m_title;
    QString m_name;
    int m_widthPixels = 0;
    int m_heightPixels = 0;
    double m_diagonalInches = 0.0;
    int m_scalingPercent = 100;
    QString m_position;
    bool m_referenceMonitor = false;
};

#endif // MONITOR_H
