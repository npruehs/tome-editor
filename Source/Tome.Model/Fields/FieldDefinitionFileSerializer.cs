// --------------------------------------------------------------------------------------------------------------------
// <copyright file="FieldDefinitionFileSerializer.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Model.Fields
{
    using System.IO;
    using System.Xml;

    public class FieldDefinitionFileSerializer
    {
        #region Public Methods and Operators

        public void Serialize(FieldDefinitionFile fieldDefinitionFile, string basePath)
        {
            var fieldDefinitionFilePath = Path.Combine(basePath, fieldDefinitionFile.Path);
            var fieldDefinitionFileInfo = new FileInfo(fieldDefinitionFilePath);

            using (var streamWriter = fieldDefinitionFileInfo.CreateText())
            {
                var xmlWriterSettings = new XmlWriterSettings { Indent = true };

                using (var xmlWriter = XmlWriter.Create(streamWriter, xmlWriterSettings))
                {
                    var fields = fieldDefinitionFile.FieldDefinitions;

                    xmlWriter.WriteStartElement("Fields");
                    {
                        foreach (var field in fields)
                        {
                            xmlWriter.WriteStartElement("Field");
                            xmlWriter.WriteAttributeString("Id", field.Id);
                            xmlWriter.WriteAttributeString("DisplayName", field.DisplayName);
                            xmlWriter.WriteAttributeString("Type", field.FieldType.ToString());
                            xmlWriter.WriteAttributeString("Description", field.Description);
                            xmlWriter.WriteAttributeString("DefaultValue", field.DefaultValue.ToString());
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