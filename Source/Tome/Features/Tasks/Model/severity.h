#ifndef SEVERITY
#define SEVERITY

namespace Tome
{
    namespace Severity
    {
        enum Severity
        {
            Information,
            Warning,
            Error
        };

        inline const QString toString(Severity severity)
        {
            switch (severity)
            {
                case Severity::Information:
                    return "Information";

                case Severity::Warning:
                    return "Warning";

                case Severity::Error:
                    return "Error";
            }
        }
    }
}
#endif // SEVERITY
