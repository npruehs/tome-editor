// --------------------------------------------------------------------------------------------------------------------
// <copyright file="TomeProjectFile.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Model.Project
{
    public class TomeProjectFile
    {
        #region Constants

        public const string FileExtension = ".tproj";

        #endregion

        #region Properties

        public string Path { get; set; }

        public TomeProject Project { get; set; }

        #endregion
    }
}