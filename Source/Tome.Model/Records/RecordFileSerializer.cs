// --------------------------------------------------------------------------------------------------------------------
// <copyright file="RecordFileSerializer.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Model.Records
{
    using System.Collections.Generic;
    using System.IO;
    using System.Xml;

    public class RecordFileSerializer
    {
        #region Constants

        private const string AttributeDisplayName = "DisplayName";

        private const string AttributeId = "Id";

        private const string AttributeValue = "Value";

        private const string ElementRecord = "Record";

        private const string ElementRecords = "Records";

        #endregion

        #region Public Methods and Operators

        public void Deserialize(RecordFile recordFile, string basePath)
        {
            // Read record file.
            var recordFilePath = Path.Combine(basePath, recordFile.Path);
            var recordFileInfo = new FileInfo(recordFilePath);

            using (var streamReader = recordFileInfo.OpenText())
            {
                var xmlReaderSettings = new XmlReaderSettings { IgnoreWhitespace = true };

                using (var xmlReader = XmlReader.Create(streamReader, xmlReaderSettings))
                {
                    recordFile.Records = new List<Record>();
                    var records = recordFile.Records;

                    xmlReader.ReadStartElement(ElementRecords);
                    {
                        // Read records.
                        while (xmlReader.Name.Equals(ElementRecord))
                        {
                            var record = new Record();
                            record.Id = xmlReader[AttributeId];
                            record.DisplayName = xmlReader[AttributeDisplayName];
                            record.FieldValues = new Dictionary<string, object>();

                            xmlReader.ReadStartElement(ElementRecord);
                            {
                                // Read record fields.
                                while (!xmlReader.Name.Equals(ElementRecord))
                                {
                                    var fieldKey = xmlReader.Name;
                                    var fieldValue = xmlReader[AttributeValue];

                                    record.FieldValues.Add(fieldKey, fieldValue);

                                    xmlReader.Read();
                                }
                            }
                            xmlReader.ReadEndElement();

                            records.Add(record);
                        }
                    }
                }
            }
        }

        public void Serialize(RecordFile recordFile, string basePath)
        {
            // Write record file.
            var recordFilePath = Path.Combine(basePath, recordFile.Path);
            var recordFileInfo = new FileInfo(recordFilePath);

            using (var streamWriter = recordFileInfo.CreateText())
            {
                var xmlWriterSettings = new XmlWriterSettings { Indent = true };

                using (var xmlWriter = XmlWriter.Create(streamWriter, xmlWriterSettings))
                {
                    var records = recordFile.Records;

                    xmlWriter.WriteStartElement(ElementRecords);
                    {
                        // Write recrods.
                        foreach (var record in records)
                        {
                            xmlWriter.WriteStartElement(ElementRecord);
                            xmlWriter.WriteAttributeString(AttributeId, record.Id);
                            xmlWriter.WriteAttributeString(AttributeDisplayName, record.DisplayName);
                            {
                                // Write record fields.
                                foreach (var recordField in record.FieldValues)
                                {
                                    xmlWriter.WriteStartElement(recordField.Key);
                                    xmlWriter.WriteAttributeString(AttributeValue, recordField.Value.ToString());
                                    xmlWriter.WriteEndElement();
                                }
                            }
                            xmlWriter.WriteEndElement();
                        }
                    }
                    xmlWriter.WriteEndElement();
                }
            }
        }

        #endregion
    }
}