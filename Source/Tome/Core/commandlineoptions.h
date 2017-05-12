#ifndef COMMANDLINEOPTIONS_H
#define COMMANDLINEOPTIONS_H

#include <QString>

namespace Tome
{
    /**
     * @brief Utility class for parsing command-line options.
     */
    class CommandLineOptions
    {
        public:
            /**
             * @brief Parses the specified arguments passed from the command-line.
             * @param argc Provided argument count.
             * @param argv Provided arguments.
             */
            void parse(int argc, char *argv[]);

            /**
             * @brief Provided argument count.
             */
            int argc = 0;

            /**
             * @brief Provided arguments.
             */
            char** argv = nullptr;

            /**
             * @brief Path to export all data to.
             */
            QString exportPath;

            /**
             * @brief Name of the export template to use for the export.
             */
            QString exportTemplateName;

            /**
             * @brief Whether to prevent Tome from opening a window.
             */
            bool noGui = false;

            /**
             * @brief Project to open.
             */
            QString projectPath;
    };
}

#endif // COMMANDLINEOPTIONS_H
