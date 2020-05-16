#include "PlotHelpers.h"

#include "../AppSettings.h"

#include "qcpl_plot.h"
#include "qcpl_cursor.h"

namespace PlotHelpers {

void rescaleLimits(QCPL::Plot* plot, PlotAxis axis, Z::Unit unitFrom, Z::Unit unitTo)
{
    auto limits = axis == PlotAxis::X
            ? plot->limitsX()
            : plot->limitsY();
    limits.min = unitTo->fromSi(unitFrom->toSi(limits.min));
    limits.max = unitTo->fromSi(unitFrom->toSi(limits.max));
    if (axis == PlotAxis::X)
        plot->setLimitsX(limits, false);
    else
        plot->setLimitsY(limits, false);
}

void rescaleCursor(QCPL::Cursor* cursor, PlotAxis axis, Z::Unit unitFrom, Z::Unit unitTo)
{
    auto pos = axis == PlotAxis::X
            ? cursor->position().x()
            : cursor->position().y();
    pos = unitTo->fromSi(unitFrom->toSi(pos));
    if (axis == PlotAxis::X)
        cursor->setPositionX(pos, false);
    else
        cursor->setPositionY(pos, false);
}

QCPL::GraphDataExportSettings makeExportSettings()
{
    QCPL::GraphDataExportSettings es;
    auto as = AppSettings::instance();
    es.csv = as.exportAsCsv;
    es.systemLocale = as.exportSystemLocale;
    es.numberPrecision = as.exportNumberPrecision;
    es.transposed = as.exportTransposed;
    return es;
}

} // namespace PlotHelpers
