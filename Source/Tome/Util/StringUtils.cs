// --------------------------------------------------------------------------------------------------------------------
// <copyright file="StringUtils.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Util
{
    using System.Linq;
    using System.Text;

    public static class StringUtils
    {
        #region Public Methods and Operators

        /// <summary>
        ///   Converts the specified string to Pascal Case, removing all
        ///   whitespaces and capitalizing the respective following letter.
        /// </summary>
        /// <param name="s">String to convert to Pascal Case.</param>
        /// <returns><paramref name="s" /> in Pascal Case.</returns>
        public static string ToPascalCase(string s)
        {
            var words = s.Split(' ');
            var stringBuilder = new StringBuilder();
            foreach (var word in words.Where(w => !string.IsNullOrWhiteSpace(w)))
            {
                stringBuilder.Append(word[0].ToString().ToUpper());
                stringBuilder.Append(word.Substring(1));
            }
            return stringBuilder.ToString();
        }

        #endregion
    }
}