// --------------------------------------------------------------------------------------------------------------------
// <copyright file="RecordExporter.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Model.Export
{
    using System.IO;
    using System.Linq;
    using System.Text;

    using Tome.Model.Project;

    public class RecordExporter
    {
        #region Constants

        private const string FieldIdPlaceholder = "$FIELD_ID$";

        private const string FieldTypePlaceholder = "$FIELD_TYPE$";

        private const string FieldValuePlaceholder = "$FIELD_VALUE$";

        private const string RecordFieldsPlaceholder = "$RECORD_FIELDS$";

        private const string RecordIdPlaceholder = "$RECORD_ID$";

        private const string RecordsPlaceholder = "$RECORDS$";

        #endregion

        #region Public Methods and Operators

        public void Export(TomeProject project, RecordExportTemplate template, string fileName)
        {
            // Build record file string.
            var recordStringBuilder = new StringBuilder();

            foreach (var recordFile in project.RecordFiles)
            {
                for (var recordIndex = 0; recordIndex < recordFile.Records.Count; ++recordIndex)
                {
                    var record = recordFile.Records[recordIndex];

                    // Build field values string.
                    var fieldValuesStringBuilder = new StringBuilder();
                    var fieldValues = record.FieldValues.ToList();

                    for (var fieldIndex = 0; fieldIndex < fieldValues.Count; ++fieldIndex)
                    {
                        var field = fieldValues[fieldIndex];

                        // Apply field value template.
                        var fieldDefinition =
                            project.FieldDefinitionFiles.SelectMany(file => file.FieldDefinitions)
                                .FirstOrDefault(definition => Equals(definition.Id, field.Key));

                        var fieldValueString = template.FieldValueTemplate;
                        fieldValueString = fieldValueString.Replace(FieldIdPlaceholder, field.Key);
                        fieldValueString = fieldValueString.Replace(
                            FieldTypePlaceholder,
                            fieldDefinition.FieldType.ToString());
                        fieldValueString = fieldValueString.Replace(FieldValuePlaceholder, field.Value.ToString());

                        fieldValuesStringBuilder.Append(fieldValueString);

                        // Add delimiter, if necessary.
                        if (fieldIndex < fieldValues.Count - 1)
                        {
                            fieldValuesStringBuilder.Append(template.FieldValueDelimiter);
                        }
                    }

                    // Apply record template.
                    var recordString = template.RecordTemplate;
                    recordString = recordString.Replace(RecordIdPlaceholder, record.Id);
                    recordString = recordString.Replace(RecordFieldsPlaceholder, fieldValuesStringBuilder.ToString());

                    recordStringBuilder.Append(recordString);

                    // Add delimiter, if necessary.
                    if (recordIndex < recordFile.Records.Count - 1)
                    {
                        recordStringBuilder.Append(template.RecordDelimiter);
                    }
                }
            }

            // Apply record file template.
            var recordFileString = template.RecordFileTemplate;
            recordFileString = recordFileString.Replace(RecordsPlaceholder, recordStringBuilder.ToString());

            // Write record file.
            var recordFileInfo = new FileInfo(fileName);

            using (var streamWriter = recordFileInfo.CreateText())
            {
                streamWriter.Write(recordFileString);
            }
        }

        #endregion
    }
}