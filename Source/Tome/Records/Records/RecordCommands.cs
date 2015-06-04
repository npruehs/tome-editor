// --------------------------------------------------------------------------------------------------------------------
// <copyright file="RecordCommands.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Records.Records
{
    using System.Windows.Input;

    public class RecordCommands
    {
        #region Constants

        public static RoutedCommand Add = new RoutedCommand("Add Record", typeof(RecordCommands));

        public static RoutedCommand Edit = new RoutedCommand("Edit Record", typeof(RecordCommands));

        #endregion
    }
}