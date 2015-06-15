// --------------------------------------------------------------------------------------------------------------------
// <copyright file="RecordFile.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Model.Records
{
    using System.Collections.Generic;

    public class RecordFile
    {
        #region Constants

        public const string FileExtension = ".tdata";

        #endregion

        #region Properties

        public string Path { get; set; }

        public List<Record> Records { get; set; }

        #endregion
    }
}