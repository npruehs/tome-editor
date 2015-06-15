// --------------------------------------------------------------------------------------------------------------------
// <copyright file="ConversionUtils.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Util
{
    using System;
    using System.ComponentModel;
    using System.Windows.Media;

    using Tome.Model.Data;
    using Tome.Model.Fields;

    public static class ConversionUtils
    {
        #region Public Methods and Operators

        public static bool CanConvertTo(object o, Type type)
        {
            var typeConverter = TypeDescriptor.GetConverter(type);
            return typeConverter.IsValid(o);
        }

        public static bool CanConvertTo<T>(object o)
        {
            return CanConvertTo(o, typeof(T));
        }

        public static object Convert(object o, FieldType type)
        {
            // Convert current value to string.
            var currentDefaultValue = ToString(o);

            // Convert default value to new type.
            switch (type)
            {
                case FieldType.Boolean:
                    return ConvertOrDefault<bool>(currentDefaultValue);

                case FieldType.Color:
                    return ConvertOrDefault<Color>(currentDefaultValue);

                case FieldType.Float:
                    return ConvertOrDefault<float>(currentDefaultValue);

                case FieldType.Int:
                    return ConvertOrDefault<int>(currentDefaultValue);

                case FieldType.Reference:
                    return ConvertOrDefault<string>(currentDefaultValue, string.Empty);

                case FieldType.String:
                    return currentDefaultValue;

                case FieldType.Vector3F:
                    return ConvertOrDefault(currentDefaultValue, Vector3F.Zero);
            }

            return null;
        }

        public static object Convert(object o, Type type)
        {
            var typeConverter = TypeDescriptor.GetConverter(type);
            return o is string ? typeConverter.ConvertFrom(o) : typeConverter.ConvertTo(o, type);
        }

        public static T Convert<T>(object o)
        {
            return (T)Convert(o, typeof(T));
        }

        public static T ConvertOrDefault<T>(object o)
        {
            return ConvertOrDefault<T>(o, default(T));
        }

        public static T ConvertOrDefault<T>(object o, T defaultValue)
        {
            return (T)ConvertOrDefault(o, typeof(T), defaultValue);
        }

        public static object ConvertOrDefault(object o, Type type, object defaultValue)
        {
            return CanConvertTo(o, type) ? Convert(o, type) : defaultValue;
        }

        public static string ToString(object o)
        {
            return o != null ? o.ToString() : string.Empty;
        }

        #endregion
    }
}