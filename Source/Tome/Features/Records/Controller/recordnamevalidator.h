#ifndef RECORDNAMEVALIDATOR_H
#define RECORDNAMEVALIDATOR_H

#include <QValidator>

namespace Tome
{
    /**
     * @brief Validates record names, ensuring they're not empty and don't start or end with spaces.
     */
    class RecordNameValidator : public QValidator
    {
    public:
        /**
         * @brief Constructs a new validator for record names.
         * @param parent Optional owner object.
         */
        RecordNameValidator(QObject *parent = Q_NULLPTR);
        ~RecordNameValidator();

        /**
         * @brief Removes all leading or trailing spaces from the specified string.
         * @param input String to fix up.
         */
        virtual void fixup(QString &input) const Q_DECL_OVERRIDE;

        /**
         * @brief Validates the specified input string.
         * @param input String to validate.
         * @param pos Unused.
         * @return State::Invalid, if the input string is empty or starts or ends with a space, and State::Acceptable otherwise.
         */
        virtual State validate(QString &input, int &pos) const Q_DECL_OVERRIDE;
    };
}

#endif // RECORDNAMEVALIDATOR_H
