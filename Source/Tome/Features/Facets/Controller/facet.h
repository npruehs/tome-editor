#ifndef FACET_H
#define FACET_H

#include <QString>
#include <QWidget>

namespace Tome
{
    class FacetContext;

    /**
     * @brief Restricts the domain of a custom type.
     */
    class Facet
    {
        public:
            /**
             * @brief Constructs a new facet for restricting the domain of a custom type.
             */
            Facet();
            virtual ~Facet();

            /**
             * @brief Creates a widget for specifying the restriction of the domain of the custom type (e.g. QSpinBox).
             * @param context Additional information for creating the widget.
             * @return Widget for specifying the restriction of the domain of the custom type.
             */
            virtual QWidget* createWidget(const FacetContext& context) const = 0;

            /**
             * @brief Gets the default restriction of the domain of the custom type (e.g. default minimum value).
             * @return Default restriction of the domain of the custom type (e.g. default minimum value).
             */
            virtual const QVariant getDefaultValue() const = 0;

            /**
             * @brief Gets the text to add to the description of a field, explaining the restriction of the domain of the custom type (e.g. "Minimum value is 0.").
             * @param facetValue Restriction of the domain of the custom type.
             * @return Text to add to the description of a field, explaining the restriction of the domain of the custom type.
             */
            virtual const QString getDescriptionForValue(const QVariant facetValue) const = 0;

            /**
             * @brief Gets the name of the restriction of the domain of the custom type (e.g. "Minimum Value").
             * @return Name of the restriction of the domain of the custom type.
             */
            virtual const QString getDisplayName() const = 0;

            /**
             * @brief Gets the key of the restriction of the domain of the custom type used for serialization.
             * @return Key of the restriction of the domain of the custom type used for serialization.
             */
            virtual const QString getKey() const = 0;

            /**
             * @brief Gets the custom type whose domain can be restricted by this facet.
             * @return Custom type whose domain can be restricted by this facet.
             */
            virtual const QString getTargetType() const = 0;

            /**
             * @brief Gets the current value of the widget for specifying the restriction of the domain of the custom type (e.g. value of a QSpinBox).
             * @param widget Widget to get the current value of.
             * @return Current value of the widget for specifying the restriction of the domain of the custom type.
             */
            virtual const QVariant getWidgetValue(QWidget* widget) const = 0;

            /**
             * @brief Sets the current value of the widget for specifying the restriction of the domain of the custom type (e.g. value of a QSpinBox).
             * @param widget Widget to set the current value of.
             * @param value Value to set.
             */
            virtual void setWidgetValue(QWidget* widget, const QVariant value) const = 0;

            /**
             * @brief Validates the specified field value against the domain restrictions imposed by this facet.
             * @param context Context to validate the field value in.
             * @param value Field value to validate.
             * @param facetValue Restriction of the domain of the custom type (e.g. minimum value).
             * @return Translated error message, if any validation errors occur, and an empty string otherwise.
             */
            virtual QString validateValue(const FacetContext& context, const QVariant value, const QVariant facetValue) const = 0;
    };
}

#endif // FACET_H
