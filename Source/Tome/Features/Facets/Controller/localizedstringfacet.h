#ifndef LOCALIZEDSTRINGFACET_H
#define LOCALIZEDSTRINGFACET_H

#include <QObject>

#include "facet.h"

namespace Tome
{
    class FacetContext;

    /**
     * @brief Marks a custom String type for localization.
     */
    class LocalizedStringFacet : public QObject, public Facet
    {
            Q_OBJECT

        public:
            LocalizedStringFacet();

            QWidget* createWidget(const FacetContext& context) const Q_DECL_OVERRIDE;
            const QVariant getDefaultValue() const Q_DECL_OVERRIDE;
            const QString getDescriptionForValue(const QVariant facetValue) const Q_DECL_OVERRIDE;
            const QString getDisplayName() const Q_DECL_OVERRIDE;
            const QString getKey() const Q_DECL_OVERRIDE;
            const QString getTargetType() const Q_DECL_OVERRIDE;
            const QVariant getWidgetValue(QWidget* widget) const Q_DECL_OVERRIDE;
            void setWidgetValue(QWidget* widget, const QVariant value) const Q_DECL_OVERRIDE;
            QString validateValue(const FacetContext& context, const QVariant value, const QVariant facetValue) const Q_DECL_OVERRIDE;

            /**
             * @brief Key of the restriction of the domain of the custom type used for serialization.
             */
            static const QString FacetKey;
    };
}

#endif // LOCALIZEDSTRINGFACET_H
