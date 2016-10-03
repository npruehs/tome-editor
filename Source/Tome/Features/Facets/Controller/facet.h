#ifndef FACET_H
#define FACET_H

#include <QString>
#include <QWidget>

namespace Tome
{
    class FacetContext;

    class Facet
    {
        public:
            Facet();
            virtual ~Facet();

            virtual QWidget* createWidget(const FacetContext& context) const = 0;
            virtual const QVariant getDefaultValue() const = 0;
            virtual const QString getDescriptionForValue(const QVariant facetValue) const = 0;
            virtual const QString getDisplayName() const = 0;
            virtual const QString getKey() const = 0;
            virtual const QString getTargetType() const = 0;
            virtual const QVariant getWidgetValue(QWidget* widget) const = 0;
            virtual void setWidgetValue(QWidget* widget, const QVariant value) const = 0;
            virtual QString validateValue(const FacetContext& context, const QVariant value, const QVariant facetValue) const = 0;
    };
}

#endif // FACET_H
