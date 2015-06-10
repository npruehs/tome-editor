// --------------------------------------------------------------------------------------------------------------------
// <copyright file="RecordExportTemplate.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Model.Export
{
    public class RecordExportTemplate
    {
        #region Properties

        public string FieldValueTemplate { get; set; }

        public string FileExtension { get; set; }

        public string Name { get; set; }

        public string RecordFileTemplate { get; set; }

        public string RecordTemplate { get; set; }

        #endregion
    }
}