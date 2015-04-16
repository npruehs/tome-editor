// --------------------------------------------------------------------------------------------------------------------
// <copyright file="FieldDefinitionFileSerializer.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Model.Fields
{
    using System;
    using System.Collections.Generic;
    using System.IO;
    using System.Xml;

    public class FieldDefinitionFileSerializer
    {
        #region Constants

        private const string AttributeDefaultValue = "DefaultValue";

        private const string AttributeDescription = "Description";

        private const string AttributeDisplayName = "DisplayName";

        private const string AttributeId = "Id";

        private const string AttributeType = "Type";

        private const string ElementField = "Field";

        private const string ElementFields = "Fields";

        #endregion

        #region Public Methods and Operators

        public void Deserialize(FieldDefinitionFile fieldDefinitionFile, string basePath)
        {
            // Read field definition file.
            var fieldDefinitionFilePath = Path.Combine(basePath, fieldDefinitionFile.Path);
            var fieldDefinitionFileInfo = new FileInfo(fieldDefinitionFilePath);

            using (var streamReader = fieldDefinitionFileInfo.OpenText())
            {
                var xmlReaderSettings = new XmlReaderSettings { IgnoreWhitespace = true };

                using (var xmlReader = XmlReader.Create(streamReader, xmlReaderSettings))
                {
                    fieldDefinitionFile.FieldDefinitions = new List<FieldDefinition>();
                    var fields = fieldDefinitionFile.FieldDefinitions;

                    xmlReader.ReadStartElement(ElementFields);
                    {
                        // Read fields.
                        while (xmlReader.Name.Equals(ElementField))
                        {
                            var field = new FieldDefinition();
                            field.Id = xmlReader[AttributeId];
                            field.DisplayName = xmlReader[AttributeDisplayName];

                            FieldType fieldType;
                            Enum.TryParse(xmlReader[AttributeType], out fieldType);
                            field.FieldType = fieldType;

                            field.Description = xmlReader[AttributeDescription];
                            field.DefaultValue = xmlReader[AttributeDefaultValue];

                            fields.Add(field);

                            xmlReader.Read();
                        }
                    }
                }
            }
        }

        public void Serialize(FieldDefinitionFile fieldDefinitionFile, string basePath)
        {
            // Write field definition file.
            var fieldDefinitionFilePath = Path.Combine(basePath, fieldDefinitionFile.Path);
            var fieldDefinitionFileInfo = new FileInfo(fieldDefinitionFilePath);

            using (var streamWriter = fieldDefinitionFileInfo.CreateText())
            {
                var xmlWriterSettings = new XmlWriterSettings { Indent = true };

                using (var xmlWriter = XmlWriter.Create(streamWriter, xmlWriterSettings))
                {
                    var fields = fieldDefinitionFile.FieldDefinitions;

                    xmlWriter.WriteStartElement(ElementFields);
                    {
                        // Write fields.
                        foreach (var field in fields)
                        {
                            xmlWriter.WriteStartElement(ElementField);
                            xmlWriter.WriteAttributeString(AttributeId, field.Id);
                            xmlWriter.WriteAttributeString(AttributeDisplayName, field.DisplayName);
                            xmlWriter.WriteAttributeString(AttributeType, field.FieldType.ToString());
                            xmlWriter.WriteAttributeString(AttributeDescription, field.Description);
                            xmlWriter.WriteAttributeString(AttributeDefaultValue, field.DefaultValue.ToString());
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