using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Runtime.Serialization;
using System.Text;
using System.Threading.Tasks;

namespace AeroEngineEditor.Utilities
{
    public static class Serialiser
    {
        public static void ToFile<T>(T instance, string path)
        {
            try
            {
                using var fs = new FileStream(path, FileMode.Create);
                var serialiser = new DataContractSerializer(typeof(T));
                serialiser.WriteObject(fs, instance);
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
            }
        }
        internal static T FromFile<T>(string path)
        {
            try
            {
                using var fs = new FileStream(path, FileMode.Open);
                var serialiser = new DataContractSerializer(typeof(T));
                T instance = (T)serialiser.ReadObject(fs);
                return instance;
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
                return default;
            }
        }
    }
}
