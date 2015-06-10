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

        public const string FieldValueDelimiterExtension = ".texportvd";

        public const string FieldValueTemplateExtension = ".texportv";

        public const string RecordDelimiterExtension = ".texportrd";

        public const string RecordFileTemplateExtension = ".texportf";

        public const string RecordTemplateExtension = ".texportr";

        #endregion

        #region Properties

        public string FieldValueDelimiterPath { get; set; }

        public string FieldValueTemplatePath { get; set; }

        public string RecordDelimiterPath { get; set; }

        public string RecordFileTemplatePath { get; set; }

        public string RecordTemplatePath { get; set; }

        public RecordExportTemplate Template { get; set; }

        #endregion
    }
}