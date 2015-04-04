// --------------------------------------------------------------------------------------------------------------------
// <copyright file="FieldDefinitionFile.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Model.Fields
{
    using System.Collections.Generic;

    public class FieldDefinitionFile
    {
        #region Constants

        public const string FileExtension = ".tproj";

        #endregion

        #region Properties

        public List<FieldDefinition> FieldDefinitions { get; set; }

        public string Path { get; set; }

        #endregion
    }
}