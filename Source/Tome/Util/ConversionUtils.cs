// --------------------------------------------------------------------------------------------------------------------
// <copyright file="ConversionUtils.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Util
{
    using System;
    using System.ComponentModel;

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

        public static object Convert(object o, Type type)
        {
            var typeConverter = TypeDescriptor.GetConverter(type);
            return o is string ? typeConverter.ConvertFrom(o) : typeConverter.ConvertTo(o, type);
        }

        public static object Convert<T>(object o)
        {
            return Convert(o, typeof(T));
        }

        public static object ConvertOrDefault<T>(object o)
        {
            return CanConvertTo<T>(o) ? Convert<T>(o) : default(T);
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