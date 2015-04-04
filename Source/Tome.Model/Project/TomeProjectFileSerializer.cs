// --------------------------------------------------------------------------------------------------------------------
// <copyright file="TomeProjectFileSerializer.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Model.Project
{
    using System.IO;
    using System.Xml;

    using Tome.Model.Fields;
    using Tome.Model.Records;

    public class TomeProjectFileSerializer
    {
        #region Public Methods and Operators

        public void Serialize(TomeProjectFile projectFile)
        {
            // Write project file.
            var projectFileInfo = new FileInfo(projectFile.Path);
            var basePath = projectFileInfo.Directory != null ? projectFileInfo.Directory.FullName : string.Empty;

            using (var streamWriter = projectFileInfo.CreateText())
            {
                var xmlWriterSettings = new XmlWriterSettings { Indent = true };

                using (var xmlWriter = XmlWriter.Create(streamWriter, xmlWriterSettings))
                {
                    var project = projectFile.Project;

                    xmlWriter.WriteStartElement("TomeProject");
                    {
                        xmlWriter.WriteElementString("Name", project.Name);
                        xmlWriter.WriteElementString("Description", project.Description);

                        xmlWriter.WriteStartElement("FieldDefinitions");
                        {
                            foreach (var fieldDefinitionFile in project.FieldDefinitionFiles)
                            {
                                xmlWriter.WriteElementString("Path", fieldDefinitionFile.Path);
                            }
                        }
                        xmlWriter.WriteEndElement();

                        xmlWriter.WriteStartElement("Records");
                        {
                            foreach (var recordFile in project.RecordFiles)
                            {
                                xmlWriter.WriteElementString("Path", recordFile.Path);
                            }
                        }
                        xmlWriter.WriteEndElement();
                    }
                    xmlWriter.WriteEndElement();
                }
            }

            // Write field definition files.
            var fieldDefinitionFileSerializer = new FieldDefinitionFileSerializer();

            foreach (var fieldDefinitionFile in projectFile.Project.FieldDefinitionFiles)
            {
                fieldDefinitionFileSerializer.Serialize(fieldDefinitionFile, basePath);
            }

            // Write record files.
            var recordFileSerializer = new RecordFileSerializer();

            foreach (var recordFile in projectFile.Project.RecordFiles)
            {
                recordFileSerializer.Serialize(recordFile, basePath);
            }
        }

        #endregion
    }
}