// --------------------------------------------------------------------------------------------------------------------
// <copyright file="TomeProjectFileSerializer.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Model.Project
{
    using System.IO;
    using System.Xml;

    using Tome.Model.Export;
    using Tome.Model.Fields;
    using Tome.Model.Records;

    public class TomeProjectFileSerializer
    {
        #region Constants

        private const string ElementDescription = "Description";

        private const string ElementFieldDefinitions = "FieldDefinitions";

        private const string ElementFieldValueDelimiter = "FieldValueDelimiter";

        private const string ElementFieldValueTemplate = "FieldValueTemplate";

        private const string ElementName = "Name";

        private const string ElementPath = "Path";

        private const string ElementRecordDelimiter = "RecordDelimiter";

        private const string ElementRecordExportTemplates = "RecordExportTemplates";

        private const string ElementRecordFileTemplate = "RecordFileTemplate";

        private const string ElementRecords = "Records";

        private const string ElementRecordTemplate = "RecordTemplate";

        private const string ElementRecordTemplateFileExtension = "FileExtension";

        private const string ElementRecordTemplateName = "Name";

        private const string ElementTemplate = "Template";

        private const string ElementTomeProject = "TomeProject";

        #endregion

        #region Public Methods and Operators

        public TomeProjectFile Deserialize(string path)
        {
            // Create new project file object.
            var projectFile = new TomeProjectFile { Path = path, Project = new TomeProject() };

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

                        // Read record export template file paths.
                        if (!xmlReader.IsEmptyElement)
                        {
                            xmlReader.ReadStartElement(ElementRecordExportTemplates);
                            {
                                while (xmlReader.Name.Equals(ElementTemplate))
                                {
                                    xmlReader.ReadStartElement(ElementTemplate);
                                    {
                                        var name = xmlReader.ReadElementString(ElementRecordTemplateName);
                                        var fileExtension =
                                            xmlReader.ReadElementString(ElementRecordTemplateFileExtension);
                                        var recordFileTemplatePath =
                                            xmlReader.ReadElementString(ElementRecordFileTemplate);
                                        var recordTemplatePath = xmlReader.ReadElementString(ElementRecordTemplate);
                                        var recordDelimiterPath = xmlReader.ReadElementString(ElementRecordDelimiter);
                                        var fieldValueTemplatePath =
                                            xmlReader.ReadElementString(ElementFieldValueTemplate);
                                        var fieldValueDelimiterPath =
                                            xmlReader.ReadElementString(ElementFieldValueDelimiter);

                                        var recordExportTemplateFile = new RecordExportTemplateFile
                                        {
                                            FieldValueDelimiterPath = fieldValueDelimiterPath,
                                            FieldValueTemplatePath = fieldValueTemplatePath,
                                            RecordDelimiterPath = recordDelimiterPath,
                                            RecordFileTemplatePath = recordFileTemplatePath,
                                            RecordTemplatePath = recordTemplatePath,
                                            Template =
                                                new RecordExportTemplate { Name = name, FileExtension = fileExtension }
                                        };

                                        project.RecordExportTemplateFiles.Add(recordExportTemplateFile);
                                    }
                                    xmlReader.ReadEndElement();
                                }
                            }
                            xmlReader.ReadEndElement();
                        }
                        else
                        {
                            xmlReader.ReadStartElement(ElementRecordExportTemplates);
                        }
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

            // Read record export template files.
            var recordExportTemplateFileSerializer = new RecordExportTemplateFileSerializer();

            foreach (var recordExportTemplateFile in projectFile.Project.RecordExportTemplateFiles)
            {
                recordExportTemplateFileSerializer.Deserialize(recordExportTemplateFile, basePath);
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
                                xmlWriter.WriteElementString(ElementPath, recordFile.Path);
                            }
                        }
                        xmlWriter.WriteEndElement();

                        // Write record export template file paths.
                        xmlWriter.WriteStartElement(ElementRecordExportTemplates);
                        {
                            foreach (var recordExportTemplateFile in project.RecordExportTemplateFiles)
                            {
                                xmlWriter.WriteStartElement(ElementTemplate);
                                {
                                    xmlWriter.WriteElementString(
                                        ElementRecordTemplateName,
                                        recordExportTemplateFile.Template.Name);
                                    xmlWriter.WriteElementString(
                                        ElementRecordTemplateFileExtension,
                                        recordExportTemplateFile.Template.FileExtension);
                                    xmlWriter.WriteElementString(
                                        ElementRecordFileTemplate,
                                        recordExportTemplateFile.RecordFileTemplatePath);
                                    xmlWriter.WriteElementString(
                                        ElementRecordTemplate,
                                        recordExportTemplateFile.RecordTemplatePath);
                                    xmlWriter.WriteElementString(
                                       ElementRecordDelimiter,
                                       recordExportTemplateFile.RecordDelimiterPath);
                                    xmlWriter.WriteElementString(
                                        ElementFieldValueTemplate,
                                        recordExportTemplateFile.FieldValueTemplatePath);
                                    xmlWriter.WriteElementString(
                                        ElementFieldValueDelimiter,
                                        recordExportTemplateFile.FieldValueDelimiterPath);
                                }
                                xmlWriter.WriteEndElement();
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

            // Write record export template files.
            var recordExportTemplateFileSerializer = new RecordExportTemplateFileSerializer();

            foreach (var recordExportTemplateFile in projectFile.Project.RecordExportTemplateFiles)
            {
                recordExportTemplateFileSerializer.Serialize(recordExportTemplateFile, basePath);
            }
        }

        #endregion
    }
}