// --------------------------------------------------------------------------------------------------------------------
// <copyright file="FieldDefinitionImporter.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Model.Fields
{
    using System;
    using System.Collections.Generic;
    using System.IO;
    using System.Linq;

    public class FieldDefinitionImporter
    {
        #region Constants

        private const string FieldDefaultTag = "@tomedefault";

        private const string FieldDescriptionTag = "@tomedesc";

        private const string FieldDisplayNameTag = "@tomedn";

        private const string FieldIdTag = "@tomeid";

        private const string FieldTypeTag = "@tometype";

        private static readonly string[] Extensions = { ".c", ".cpp", ".cs", ".java", ".php", ".py" };

        #endregion

        #region Public Methods and Operators

        public List<FieldDefinition> Import(string path)
        {
            // Setup result list.
            var fields = new List<FieldDefinition>();

            // Enumerate source code files.
            var directoryInfo = new DirectoryInfo(path);

            var files =
                directoryInfo.EnumerateFiles("*", SearchOption.AllDirectories)
                    .Where(file => Extensions.Contains(file.Extension))
                    .ToList();

            foreach (var file in files)
            {
                // Open source file.
                using (var streamReader = file.OpenText())
                {
                    // Look for first field.
                    string line;
                    FieldDefinition currentField = null;

                    while ((line = streamReader.ReadLine()) != null)
                    {
                        // Check for field id tag.
                        var fieldIdIndex = line.IndexOf(FieldIdTag, StringComparison.Ordinal);

                        if (fieldIdIndex >= 0)
                        {
                            // If there has already been a field id tag before, write that field to the result set now.
                            if (currentField != null)
                            {
                                fields.Add(currentField);
                            }

                            // Start collecting data about that new field.
                            var fieldId = line.Substring(fieldIdIndex + FieldIdTag.Length + 1);
                            currentField = new FieldDefinition { Id = fieldId };
                            continue;
                        }

                        if (currentField == null)
                        {
                            continue;
                        }

                        // Check for display name tag.
                        var fieldDisplayNameIndex = line.IndexOf(FieldDisplayNameTag, StringComparison.Ordinal);

                        if (fieldDisplayNameIndex >= 0)
                        {
                            currentField.DisplayName =
                                line.Substring(fieldDisplayNameIndex + FieldDisplayNameTag.Length + 1);
                            continue;
                        }

                        // Check for type tag.
                        var fieldTypeIndex = line.IndexOf(FieldTypeTag, StringComparison.Ordinal);

                        if (fieldTypeIndex >= 0)
                        {
                            var typeString = line.Substring(fieldTypeIndex + FieldTypeTag.Length + 1);
                            FieldType fieldType;
                            if (Enum.TryParse(typeString, true, out fieldType))
                            {
                                currentField.FieldType = fieldType;
                            }
                            continue;
                        }

                        // Check for default value tag.
                        var fieldDefaultIndex = line.IndexOf(FieldDefaultTag, StringComparison.Ordinal);

                        if (fieldDefaultIndex >= 0)
                        {
                            var fieldDefaultValueIndex = fieldDefaultIndex + FieldDefaultTag.Length + 1;

                            if (line.Length > fieldDefaultValueIndex)
                            {
                                currentField.DefaultValue = line.Substring(fieldDefaultValueIndex);
                                continue;
                            }
                        }

                        // Check for description tag.
                        var fieldDescriptionIndex = line.IndexOf(FieldDescriptionTag, StringComparison.Ordinal);

                        if (fieldDescriptionIndex >= 0)
                        {
                            currentField.Description =
                                line.Substring(fieldDescriptionIndex + FieldDescriptionTag.Length + 1);
                        }
                    }

                    // Write last field to result set, if any.
                    if (currentField != null)
                    {
                        fields.Add(currentField);
                    }
                }
            }

            return fields;
        }

        #endregion
    }
}