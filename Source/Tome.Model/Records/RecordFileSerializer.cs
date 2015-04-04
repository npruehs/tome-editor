// --------------------------------------------------------------------------------------------------------------------
// <copyright file="RecordFileSerializer.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Model.Records
{
    using System.IO;
    using System.Xml;

    public class RecordFileSerializer
    {
        #region Public Methods and Operators

        public void Serialize(RecordFile recordFile, string basePath)
        {
            var recordFilePath = Path.Combine(basePath, recordFile.Path);
            var recordFileInfo = new FileInfo(recordFilePath);

            using (var streamWriter = recordFileInfo.CreateText())
            {
                var xmlWriterSettings = new XmlWriterSettings { Indent = true };

                using (var xmlWriter = XmlWriter.Create(streamWriter, xmlWriterSettings))
                {
                    var records = recordFile.Records;

                    xmlWriter.WriteStartElement("Records");
                    {
                        foreach (var record in records)
                        {
                            xmlWriter.WriteStartElement("Record");
                            xmlWriter.WriteAttributeString("Id", record.Id);
                            {
                                foreach (var recordField in record.FieldValues)
                                {
                                    xmlWriter.WriteStartElement(recordField.Key);
                                    xmlWriter.WriteAttributeString("Value", recordField.Value.ToString());
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