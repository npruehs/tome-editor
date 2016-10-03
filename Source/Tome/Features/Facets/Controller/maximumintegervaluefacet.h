#ifndef MAXIMUMINTEGERVALUEFACET_H
#define MAXIMUMINTEGERVALUEFACET_H

#include <QObject>

#include "facet.h"


namespace Tome
{
    class MaximumIntegerValueFacet : public QObject, public Facet
    {
            Q_OBJECT

        public:
            MaximumIntegerValueFacet();

            QWidget* createWidget() const;
            const QString getDescriptionForValue(const QVariant facetValue) const;
            const QString getDisplayName() const;
            const QString getKey() const;
            const QString getTargetType() const;
            const QVariant getWidgetValue(QWidget* widget) const;
            void setWidgetValue(QWidget* widget, const QVariant value) const;
            QString validateValue(const QVariant value, const QVariant facetValue) const;
    };
}

#endif // MAXIMUMINTEGERVALUEFACET_H
