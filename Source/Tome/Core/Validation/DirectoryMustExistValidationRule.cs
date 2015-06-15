// --------------------------------------------------------------------------------------------------------------------
// <copyright file="DirectoryMustExistValidationRule.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Core.Validation
{
    using System.Globalization;
    using System.IO;
    using System.Windows.Controls;

    public class DirectoryMustExistValidationRule : StringNotEmptyValidationRule
    {
        #region Public Methods and Operators

        public override ValidationResult Validate(object value, CultureInfo cultureInfo)
        {
            // https://msdn.microsoft.com/en-us/library/aa969773(v=vs.110).aspx
            var validationResult = base.Validate(value, cultureInfo);

            if (!validationResult.IsValid)
            {
                return validationResult;
            }

            var path = (string)value;
            var directoryInfo = new DirectoryInfo(path);

            if (!directoryInfo.Exists)
            {
                return new ValidationResult(false, "Directory does not exist.");
            }

            return ValidationResult.ValidResult;
        }

        #endregion
    }
}