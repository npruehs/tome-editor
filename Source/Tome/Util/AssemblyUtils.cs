// --------------------------------------------------------------------------------------------------------------------
// <copyright file="AssemblyUtils.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Util
{
    using System.Reflection;

    public static class AssemblyUtils
    {
        #region Public Methods and Operators

        public static string GetName()
        {
            return Assembly.GetExecutingAssembly().GetName().Name;
        }

        public static string GetVersion()
        {
            return Assembly.GetExecutingAssembly().GetName().Version.ToString();
        }

        #endregion
    }
}