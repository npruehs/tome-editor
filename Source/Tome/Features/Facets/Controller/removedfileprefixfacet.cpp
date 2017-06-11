#include "removedfileprefixfacet.h"

#include <QLineEdit>

#include "../Model/facetcontext.h"
#include "../../Types/Model/builtintype.h"


using namespace Tome;


const QString RemovedFilePrefixFacet::FacetKey = "RemovedFilePrefix";


RemovedFilePrefixFacet::RemovedFilePrefixFacet()
{
}

QWidget*RemovedFilePrefixFacet::createWidget(const FacetContext& context) const
{
    Q_UNUSED(context);
    return new QLineEdit();
}

const QVariant RemovedFilePrefixFacet::getDefaultValue() const
{
    return QString();
}

const QString RemovedFilePrefixFacet::getDescriptionForValue(const QVariant facetValue) const
{
    QString removedFilePrefix = facetValue.toString();
    return removedFilePrefix.length() > 0
            ? tr("The prefix %1 will automatically be removed.").arg(removedFilePrefix)
            : QString();
}

const QString RemovedFilePrefixFacet::getDisplayName() const
{
    return tr("Removed File Prefix");
}

const QString RemovedFilePrefixFacet::getKey() const
{
    return FacetKey;
}

const QString RemovedFilePrefixFacet::getTargetType() const
{
    return BuiltInType::File;
}

const QVariant RemovedFilePrefixFacet::getWidgetValue(QWidget* widget) const
{
    QLineEdit* lineEdit = static_cast<QLineEdit*>(widget);
    return lineEdit->text();
}

void RemovedFilePrefixFacet::setWidgetValue(QWidget* widget, const QVariant value) const
{
    QLineEdit* lineEdit = static_cast<QLineEdit*>(widget);
    lineEdit->setText(value.toString());
}

QString RemovedFilePrefixFacet::validateValue(const FacetContext& context, const QVariant value, const QVariant facetValue) const
{
    Q_UNUSED(context)
    Q_UNUSED(value)
    Q_UNUSED(facetValue)

    // Always succeeds.
    return QString();
}
