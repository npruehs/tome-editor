#include "commandlineoptions.h"

using namespace Tome;

CommandLineOptions::CommandLineOptions()
    : noGui(false)
{
}

void CommandLineOptions::parse(int argc, char* argv[])
{
    for (int i = 1; i < argc; ++i)
    {
        // Parse GUI.
        if (!qstrcmp(argv[i], "-no-gui"))
        {
            this->noGui = true;
            continue;
        }

        // Parse project path.
        if (!qstrcmp(argv[i], "-project") && (i + 1 < argc))
        {
            this->projectPath = QString(argv[i + 1]);
            i = i + 1;
            continue;
        }

        // Parse export.
        if (!qstrcmp(argv[i], "-export") && (i + 2 < argc))
        {
            this->exportTemplateName = QString(argv[i + 1]);
            this->exportPath = QString(argv[i + 2]);
            i = i + 2;
            continue;
        }

        // Parse project path.
        if (argv[i][0] != '-')
        {
            this->projectPath = QString(argv[i]);
            continue;
        }
    }
    this->argc = argc;
    this->argv = argv;
}
