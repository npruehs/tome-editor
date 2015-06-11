// --------------------------------------------------------------------------------------------------------------------
// <copyright file="VersionUtils.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Util
{
    using System.Reflection;

    public static class VersionUtils
    {
        #region Public Methods and Operators

        public static string GetVersion()
        {
            return Assembly.GetExecutingAssembly().GetName().Version.ToString();
        }

        #endregion
    }
}