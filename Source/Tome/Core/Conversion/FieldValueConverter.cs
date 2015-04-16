// --------------------------------------------------------------------------------------------------------------------
// <copyright file="FieldValueConverter.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Core.Conversion
{
    using System;
    using System.ComponentModel;
    using System.Globalization;
    using System.Windows.Data;

    public class FieldValueConverter : IValueConverter
    {
        #region Constructors and Destructors

        public FieldValueConverter()
        {
        }

        public FieldValueConverter(Type type)
        {
            this.Type = type;
        }

        #endregion

        #region Properties

        public Type Type { get; set; }

        #endregion

        #region Public Methods and Operators

        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return value.ToString();
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            var typeConverter = TypeDescriptor.GetConverter(this.Type);
            return typeConverter.ConvertTo(value, this.Type);
        }

        #endregion
    }
}