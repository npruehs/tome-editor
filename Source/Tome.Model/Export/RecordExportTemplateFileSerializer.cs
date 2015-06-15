// --------------------------------------------------------------------------------------------------------------------
// <copyright file="RecordExportTemplateFileSerializer.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Model.Export
{
    using System.IO;

    public class RecordExportTemplateFileSerializer
    {
        #region Public Methods and Operators

        public void Deserialize(RecordExportTemplateFile recordExportTemplateFile, string basePath)
        {
            // Read record file template.
            var recordFileTemplatePath = Path.Combine(basePath, recordExportTemplateFile.RecordFileTemplatePath);
            var recordFileTemplateFileInfo = new FileInfo(recordFileTemplatePath);

            using (var streamReader = recordFileTemplateFileInfo.OpenText())
            {
                recordExportTemplateFile.Template.RecordFileTemplate = streamReader.ReadToEnd();
            }

            // Read record template.
            var recordTemplatePath = Path.Combine(basePath, recordExportTemplateFile.RecordTemplatePath);
            var recordTemplateFileInfo = new FileInfo(recordTemplatePath);

            using (var streamReader = recordTemplateFileInfo.OpenText())
            {
                recordExportTemplateFile.Template.RecordTemplate = streamReader.ReadToEnd();
            }

            // Read field value template.
            var fieldValueTemplatePath = Path.Combine(basePath, recordExportTemplateFile.FieldValueTemplatePath);
            var fieldValueTemplateFileInfo = new FileInfo(fieldValueTemplatePath);

            using (var streamReader = fieldValueTemplateFileInfo.OpenText())
            {
                recordExportTemplateFile.Template.FieldValueTemplate = streamReader.ReadToEnd();
            }

            // Read record delimiter.
            var recordDelimiterPath = Path.Combine(basePath, recordExportTemplateFile.RecordDelimiterPath);
            var recordDelimiterFileInfo = new FileInfo(recordDelimiterPath);

            using (var streamReader = recordDelimiterFileInfo.OpenText())
            {
                recordExportTemplateFile.Template.RecordDelimiter = streamReader.ReadToEnd();
            }

            // Read field value template.
            var fieldValueDelimiterPath = Path.Combine(basePath, recordExportTemplateFile.FieldValueDelimiterPath);
            var fieldValueDelimiterFileInfo = new FileInfo(fieldValueDelimiterPath);

            using (var streamReader = fieldValueDelimiterFileInfo.OpenText())
            {
                recordExportTemplateFile.Template.FieldValueDelimiter = streamReader.ReadToEnd();
            }
        }

        public void Serialize(RecordExportTemplateFile recordExportTemplateFile, string basePath)
        {
            // Write record file template.
            var recordFileTemplatePath = Path.Combine(basePath, recordExportTemplateFile.RecordFileTemplatePath);
            var recordFileTemplateFileInfo = new FileInfo(recordFileTemplatePath);

            using (var streamWriter = recordFileTemplateFileInfo.CreateText())
            {
                streamWriter.Write(recordExportTemplateFile.Template.RecordFileTemplate);
            }

            // Write record template.
            var recordTemplatePath = Path.Combine(basePath, recordExportTemplateFile.RecordTemplatePath);
            var recordTemplateFileInfo = new FileInfo(recordTemplatePath);

            using (var streamWriter = recordTemplateFileInfo.CreateText())
            {
                streamWriter.Write(recordExportTemplateFile.Template.RecordTemplate);
            }

            // Write field value template.
            var fieldValueTemplatePath = Path.Combine(basePath, recordExportTemplateFile.FieldValueTemplatePath);
            var fieldValueTemplateFileInfo = new FileInfo(fieldValueTemplatePath);

            using (var streamWriter = fieldValueTemplateFileInfo.CreateText())
            {
                streamWriter.Write(recordExportTemplateFile.Template.FieldValueTemplate);
            }

            // Write record delimiter.
            var recordDelimiterPath = Path.Combine(basePath, recordExportTemplateFile.RecordDelimiterPath);
            var recordDelimiterFileInfo = new FileInfo(recordDelimiterPath);

            using (var streamWriter = recordDelimiterFileInfo.CreateText())
            {
                streamWriter.Write(recordExportTemplateFile.Template.RecordDelimiter);
            }

            // Write field value delimiter.
            var fieldValueDelimiterPath = Path.Combine(basePath, recordExportTemplateFile.FieldValueDelimiterPath);
            var fieldValueDelimiterFileInfo = new FileInfo(fieldValueDelimiterPath);

            using (var streamWriter = fieldValueDelimiterFileInfo.CreateText())
            {
                streamWriter.Write(recordExportTemplateFile.Template.FieldValueDelimiter);
            }
        }

        #endregion
    }
}