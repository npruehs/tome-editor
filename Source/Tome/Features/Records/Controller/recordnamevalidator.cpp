#include "recordnamevalidator.h"

using namespace Tome;


RecordNameValidator::RecordNameValidator(QObject *parent)
    : QValidator(parent)
{
}

RecordNameValidator::~RecordNameValidator()
{
}

void RecordNameValidator::fixup(QString &input) const
{
    while (input.startsWith(' '))
    {
        input.remove(0, 1);
    }
    while (input.endsWith(' '))
    {
        input.remove(input.size() - 1, 1);
    }
}

QValidator::State RecordNameValidator::validate(QString &input, int &pos) const
{
    Q_UNUSED(pos)
    return
        ( !input.isEmpty() && !input.startsWith(' ') && !input.endsWith(' ') ) ?
        State::Acceptable :
        State::Invalid;
}

