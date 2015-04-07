// --------------------------------------------------------------------------------------------------------------------
// <copyright file="FieldDefinitionWindowCommands.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Fields.Commands
{
    using System.Windows.Input;

    public class FieldDefinitionWindowCommands
    {
        #region Constants

        public static RoutedCommand Delete = new RoutedCommand("Delete Field", typeof(FieldDefinitionWindowCommands));

        public static RoutedCommand Edit = new RoutedCommand("Edit Field", typeof(FieldDefinitionWindowCommands));

        #endregion
    }
}