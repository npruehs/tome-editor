#ifndef RECORDNAMEVALIDATOR_H
#define RECORDNAMEVALIDATOR_H

#include <QValidator>

class RecordNameValidator : public QValidator
{
public:

    RecordNameValidator(QObject *parent = Q_NULLPTR);
    ~RecordNameValidator();
    virtual void	fixup(QString &input) const  Q_DECL_OVERRIDE;
    virtual State	validate(QString &input, int &pos) const  Q_DECL_OVERRIDE;

};

#endif // RECORDNAMEVALIDATOR_H
