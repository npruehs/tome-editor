#ifndef COMMANDLINEOPTIONS_H
#define COMMANDLINEOPTIONS_H

#include <QString>

namespace Tome
{
    class CommandLineOptions
    {
        public:
            CommandLineOptions();

            void parse(int argc, char *argv[]);

            int argc;
            char* argv[];
            QString exportPath;
            QString exportTemplateName;
            bool noGui;
            QString projectPath;
    };
}

#endif // COMMANDLINEOPTIONS_H
