#ifndef MAXIMUMINTEGERVALUEFACET_H
#define MAXIMUMINTEGERVALUEFACET_H

#include <QObject>

#include "facet.h"


namespace Tome
{
    class FacetContext;

    /**
     * @brief Specifies a maximum value for a custom Integer type.
     */
    class MaximumIntegerValueFacet : public QObject, public Facet
    {
            Q_OBJECT

        public:
            MaximumIntegerValueFacet();

            QWidget* createWidget(const FacetContext& context) const Q_DECL_OVERRIDE;
            const QVariant getDefaultValue() const Q_DECL_OVERRIDE;
            const QString getDescriptionForValue(const QVariant facetValue) const Q_DECL_OVERRIDE;
            const QString getDisplayName() const Q_DECL_OVERRIDE;
            const QString getKey() const Q_DECL_OVERRIDE;
            const QString getTargetType() const Q_DECL_OVERRIDE;
            const QVariant getWidgetValue(QWidget* widget) const Q_DECL_OVERRIDE;
            void setWidgetValue(QWidget* widget, const QVariant value) const Q_DECL_OVERRIDE;
            QString validateValue(const FacetContext& context, const QVariant value, const QVariant facetValue) const Q_DECL_OVERRIDE;
    };
}

#endif // MAXIMUMINTEGERVALUEFACET_H
