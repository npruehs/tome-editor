#ifndef RECORDNAMEVALIDATOR_H
#define RECORDNAMEVALIDATOR_H

#include <QValidator>

class RecordNameValidator : public QValidator
{
public:

    RecordNameValidator(QObject *parent = Q_NULLPTR);
    ~RecordNameValidator();
    virtual void	fixup(QString &input) const override;
    virtual State	validate(QString &input, int &pos) const override;

};

#endif // RECORDNAMEVALIDATOR_H
