// --------------------------------------------------------------------------------------------------------------------
// <copyright file="RecordExportTemplateFile.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Model.Export
{
    public class RecordExportTemplateFile
    {
        #region Properties

        public string RecordFileTemplatePath { get; set; }

        public string RecordTemplatePath { get; set; }

        public RecordExportTemplate Template { get; set; }

        #endregion
    }
}