#include "removedfilesuffixfacet.h"

#include <QLineEdit>

#include "../Model/facetcontext.h"
#include "../../Types/Model/builtintype.h"


using namespace Tome;


const QString RemovedFileSuffixFacet::FacetKey = "RemovedFileSuffix";


RemovedFileSuffixFacet::RemovedFileSuffixFacet()
{
}

QWidget*RemovedFileSuffixFacet::createWidget(const FacetContext& context) const
{
    Q_UNUSED(context);
    return new QLineEdit();
}

const QVariant RemovedFileSuffixFacet::getDefaultValue() const
{
    return QString();
}

const QString RemovedFileSuffixFacet::getDescriptionForValue(const QVariant facetValue) const
{
    QString removedFilePrefix = facetValue.toString();
    return removedFilePrefix.length() > 0
            ? tr("The suffix %1 will automatically be removed.").arg(removedFilePrefix)
            : QString();
}

const QString RemovedFileSuffixFacet::getDisplayName() const
{
    return tr("Removed File Suffix");
}

const QString RemovedFileSuffixFacet::getKey() const
{
    return FacetKey;
}

const QString RemovedFileSuffixFacet::getTargetType() const
{
    return BuiltInType::File;
}

const QVariant RemovedFileSuffixFacet::getWidgetValue(QWidget* widget) const
{
    QLineEdit* lineEdit = static_cast<QLineEdit*>(widget);
    return lineEdit->text();
}

void RemovedFileSuffixFacet::setWidgetValue(QWidget* widget, const QVariant value) const
{
    QLineEdit* lineEdit = static_cast<QLineEdit*>(widget);
    lineEdit->setText(value.toString());
}

QString RemovedFileSuffixFacet::validateValue(const FacetContext& context, const QVariant value, const QVariant facetValue) const
{
    Q_UNUSED(context)
    Q_UNUSED(value)
    Q_UNUSED(facetValue)

    // Always succeeds.
    return QString();
}
