// --------------------------------------------------------------------------------------------------------------------
// <copyright file="MainWindowCommands.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Core.Commands
{
    using System.Windows.Input;

    public static class MainWindowCommands
    {
        #region Constants

        public static RoutedCommand FieldDefinitions = new RoutedCommand(
            "Field Definitions",
            typeof(MainWindowCommands));

        #endregion
    }
}