// --------------------------------------------------------------------------------------------------------------------
// <copyright file="TomeProjectFileSerializer.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Model.Project
{
    using System.Collections.Generic;
    using System.IO;
    using System.Xml;

    using Tome.Model.Fields;
    using Tome.Model.Records;

    public class TomeProjectFileSerializer
    {
        #region Constants

        private const string ElementDescription = "Description";

        private const string ElementFieldDefinitions = "FieldDefinitions";

        private const string ElementName = "Name";

        private const string ElementPath = "Path";

        private const string ElementRecords = "Records";

        private const string ElementTomeProject = "TomeProject";

        #endregion

        #region Public Methods and Operators

        public TomeProjectFile Deserialize(string path)
        {
            // Create new project file object.
            var projectFile = new TomeProjectFile
            {
                Path = path,
                Project =
                    new TomeProject
                    {
                        FieldDefinitionFiles = new List<FieldDefinitionFile>(),
                        RecordFiles = new List<RecordFile>()
                    }
            };

            // Read project file.
            var projectFileInfo = new FileInfo(path);
            var basePath = projectFileInfo.Directory != null ? projectFileInfo.Directory.FullName : string.Empty;

            using (var streamReader = projectFileInfo.OpenText())
            {
                var xmlReaderSettings = new XmlReaderSettings { IgnoreWhitespace = true };

                using (var xmlReader = XmlReader.Create(streamReader, xmlReaderSettings))
                {
                    var project = projectFile.Project;

                    xmlReader.ReadStartElement(ElementTomeProject);
                    {
                        // Read name and description.
                        project.Name = xmlReader.ReadElementString(ElementName);
                        project.Description = xmlReader.ReadElementString(ElementDescription);

                        // Read field definition file paths.
                        xmlReader.ReadStartElement(ElementFieldDefinitions);
                        {
                            while (xmlReader.Name.Equals(ElementPath))
                            {
                                var fieldDefinitionFilePath = xmlReader.ReadElementString(ElementPath);
                                project.FieldDefinitionFiles.Add(
                                    new FieldDefinitionFile { Path = fieldDefinitionFilePath });
                            }
                        }
                        xmlReader.ReadEndElement();

                        // Read record file paths.
                        xmlReader.ReadStartElement(ElementRecords);
                        {
                            while (xmlReader.Name.Equals(ElementPath))
                            {
                                var recordFilePath = xmlReader.ReadElementString(ElementPath);
                                project.RecordFiles.Add(new RecordFile { Path = recordFilePath });
                            }
                        }
                        xmlReader.ReadEndElement();
                    }
                    xmlReader.ReadEndElement();
                }
            }

            // Read field definition files.
            var fieldDefinitionFileSerializer = new FieldDefinitionFileSerializer();

            foreach (var fieldDefinitionFile in projectFile.Project.FieldDefinitionFiles)
            {
                fieldDefinitionFileSerializer.Deserialize(fieldDefinitionFile, basePath);
            }

            // Read record files.
            var recordFileSerializer = new RecordFileSerializer();

            foreach (var recordFile in projectFile.Project.RecordFiles)
            {
                recordFileSerializer.Deserialize(recordFile, basePath);
            }

            return projectFile;
        }

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

                    xmlWriter.WriteStartElement(ElementTomeProject);
                    {
                        // Write name and description.
                        xmlWriter.WriteElementString(ElementName, project.Name);
                        xmlWriter.WriteElementString(ElementDescription, project.Description);

                        // Write field definition file paths.
                        xmlWriter.WriteStartElement(ElementFieldDefinitions);
                        {
                            foreach (var fieldDefinitionFile in project.FieldDefinitionFiles)
                            {
                                xmlWriter.WriteElementString(ElementPath, fieldDefinitionFile.Path);
                            }
                        }
                        xmlWriter.WriteEndElement();

                        // Write record file paths.
                        xmlWriter.WriteStartElement(ElementRecords);
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