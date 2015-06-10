// --------------------------------------------------------------------------------------------------------------------
// <copyright file="RecordExportTemplateFile.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Model.Export
{
    public class RecordExportTemplateFile
    {
        #region Constants

        public const string FieldValueTemplateExtension = ".texportv";

        public const string RecordFileTemplateExtension = ".texportf";

        public const string RecordTemplateExtension = ".texportr";

        #endregion

        #region Properties

        public string FieldValueTemplatePath { get; set; }

        public string RecordFileTemplatePath { get; set; }

        public string RecordTemplatePath { get; set; }

        public RecordExportTemplate Template { get; set; }

        #endregion
    }
}