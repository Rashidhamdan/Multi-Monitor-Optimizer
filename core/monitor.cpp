#include "monitor.h"

/***************************************************************************/
/*                                                                         */
/*                               Monitor                                   */
/*                                                                         */
/***************************************************************************/

Monitor::Monitor() = default;

/***************************************************************************/
/*                                                                         */
/*                               Monitor                                   */
/*                                                                         */
/***************************************************************************/

Monitor::Monitor(const QString &title,
                 const QString &name,
                 int widthPixels,
                 int heightPixels,
                 double diagonalInches,
                 int scalingPercent,
                 const QString &position,
                 bool referenceMonitor)
    : m_title(title)
    , m_name(name)
    , m_widthPixels(widthPixels)
    , m_heightPixels(heightPixels)
    , m_diagonalInches(diagonalInches)
    , m_scalingPercent(scalingPercent)
    , m_position(position)
    , m_referenceMonitor(referenceMonitor)
{
}

/***************************************************************************/
/*                                                                         */
/*                                title                                    */
/*                                                                         */
/***************************************************************************/

QString Monitor::title() const
{
    return m_title;
}

/***************************************************************************/
/*                                                                         */
/*                                name                                     */
/*                                                                         */
/***************************************************************************/

QString Monitor::name() const
{
    return m_name;
}

/***************************************************************************/
/*                                                                         */
/*                            widthPixels                                  */
/*                                                                         */
/***************************************************************************/

int Monitor::widthPixels() const
{
    return m_widthPixels;
}

/***************************************************************************/
/*                                                                         */
/*                           heightPixels                                  */
/*                                                                         */
/***************************************************************************/

int Monitor::heightPixels() const
{
    return m_heightPixels;
}

/***************************************************************************/
/*                                                                         */
/*                         diagonalInches                                  */
/*                                                                         */
/***************************************************************************/

double Monitor::diagonalInches() const
{
    return m_diagonalInches;
}

/***************************************************************************/
/*                                                                         */
/*                         scalingPercent                                  */
/*                                                                         */
/***************************************************************************/

int Monitor::scalingPercent() const
{
    return m_scalingPercent;
}

/***************************************************************************/
/*                                                                         */
/*                              position                                   */
/*                                                                         */
/***************************************************************************/

QString Monitor::position() const
{
    return m_position;
}

/***************************************************************************/
/*                                                                         */
/*                        isReferenceMonitor                               */
/*                                                                         */
/***************************************************************************/

bool Monitor::isReferenceMonitor() const
{
    return m_referenceMonitor;
}

/***************************************************************************/
/*                                                                         */
/*                              setTitle                                   */
/*                                                                         */
/***************************************************************************/

void Monitor::setTitle(const QString &title)
{
    m_title = title;
}

/***************************************************************************/
/*                                                                         */
/*                              setName                                    */
/*                                                                         */
/***************************************************************************/

void Monitor::setName(const QString &name)
{
    m_name = name;
}

/***************************************************************************/
/*                                                                         */
/*                           setWidthPixels                                */
/*                                                                         */
/***************************************************************************/

void Monitor::setWidthPixels(int widthPixels)
{
    m_widthPixels = widthPixels;
}

/***************************************************************************/
/*                                                                         */
/*                          setHeightPixels                                */
/*                                                                         */
/***************************************************************************/

void Monitor::setHeightPixels(int heightPixels)
{
    m_heightPixels = heightPixels;
}

/***************************************************************************/
/*                                                                         */
/*                        setDiagonalInches                                */
/*                                                                         */
/***************************************************************************/

void Monitor::setDiagonalInches(double diagonalInches)
{
    m_diagonalInches = diagonalInches;
}

/***************************************************************************/
/*                                                                         */
/*                        setScalingPercent                                */
/*                                                                         */
/***************************************************************************/

void Monitor::setScalingPercent(int scalingPercent)
{
    m_scalingPercent = scalingPercent;
}

/***************************************************************************/
/*                                                                         */
/*                            setPosition                                  */
/*                                                                         */
/***************************************************************************/

void Monitor::setPosition(const QString &position)
{
    m_position = position;
}

/***************************************************************************/
/*                                                                         */
/*                      setReferenceMonitor                                */
/*                                                                         */
/***************************************************************************/

void Monitor::setReferenceMonitor(bool referenceMonitor)
{
    m_referenceMonitor = referenceMonitor;
}

/***************************************************************************/
/*                                                                         */
/*                               isValid                                   */
/*                                                                         */
/***************************************************************************/

bool Monitor::isValid() const
{
    return m_widthPixels > 0
           && m_heightPixels > 0
           && m_diagonalInches > 0.0
           && m_scalingPercent > 0;
}
