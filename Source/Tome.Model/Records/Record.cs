// --------------------------------------------------------------------------------------------------------------------
// <copyright file="Record.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Model.Records
{
    using System.Collections.Generic;

    public class Record
    {
        #region Properties

        public Dictionary<string, object> FieldValues { get; set; }

        #endregion
    }
}