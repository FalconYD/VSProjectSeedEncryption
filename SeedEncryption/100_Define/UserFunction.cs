using System;
using System.Runtime.InteropServices;
using System.Text;

namespace SeedEncryption
{
    class UserFunction
    {
        const string STRDLLPATH = "SeedEncryptionCore.dll";
        public delegate int UserCallBack(int func, int seq, IntPtr data1, ref IntPtr data2);

        [DllImport(STRDLLPATH, CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        public extern static void Encryption(string inputpath, int inputlen, string outputpath, int outputlen);
        [DllImport(STRDLLPATH, CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        public extern static void Decryption(string inputpath, int inputlen, string outputpath, int outputlen);
    }

    class UserINI
    {
        [DllImport("kernel32")]
        private static extern long WritePrivateProfileString(string section, string key, string val, string filepath);
        [DllImport("kernel32")]
        private static extern long GetPrivateProfileString(string section, string key, string val, StringBuilder retVal, int size, string filepath);

        const int _MaxPathLength = 256;

        static public string fn_Load(string section, string key, string val, string filepath)
        {
            StringBuilder sbReturnValue = new StringBuilder(_MaxPathLength);
            GetPrivateProfileString(section, key, val, sbReturnValue, sbReturnValue.Capacity, filepath);
            return sbReturnValue.ToString();
        }
        
        static public int fn_Load(string section, string key, int val, string filepath)
        {
            string strValue = fn_GetINIString(section, key, val.ToString(), filepath);
            int rtnValue = 0;
            try
            {
                rtnValue = Convert.ToInt32(strValue);
            }
            catch
            {
                rtnValue = -1;
                return -1;
            }

            return rtnValue;
        }
        
        static public bool fn_Load(string section, string key, bool val, string filepath)
        {
            int nTemp = val ? 1 : 0;
            string strValue = fn_GetINIString(section, key, nTemp.ToString(), filepath);
            int rtnValue = 0;
            try
            {
                rtnValue = Convert.ToInt32(strValue);
            }
            catch
            {
                rtnValue = -1;
                return false;
            }

            return (rtnValue == 1);

        }

        static public double fn_Load(string section, string key, double val, string filepath)
        {
            string strValue = fn_GetINIString(section, key, val.ToString(), filepath);
            double rtnValue = 0;
            try
            {
                rtnValue = Convert.ToDouble(strValue);
            }
            catch
            {
                rtnValue = -1;
                return 0.0;
            }

            return rtnValue;
        }

        static private string fn_GetINIString(string section, string key, string val, string filepath)
        {
            StringBuilder sbReturnValue = new StringBuilder(_MaxPathLength);
            GetPrivateProfileString(section, key, val, sbReturnValue, sbReturnValue.Capacity, filepath);
            return sbReturnValue.ToString();
        }

        static public long fn_Save(string section, string key, bool val, string filepath)
        {
            string strSetValue;
            try
            {
                strSetValue = val ? "1" : "0";
                return WritePrivateProfileString(section, key, strSetValue, filepath);
            }
            catch
            {
                return -1;
            }

        }

        static public long fn_Save(string section, string key, int val, string filepath)
        {
            string strSetValue;
            try
            {
                strSetValue = Convert.ToString(val);
                return WritePrivateProfileString(section, key, strSetValue, filepath);
            }
            catch
            {
                return -1;
            }

        }
        
        static public long fn_Save(string section, string key, double val, string filepath)
        {
            string strSetValue = string.Empty;

            try
            {
                strSetValue = Convert.ToString(val);
                return WritePrivateProfileString(section, key, strSetValue, filepath);
            }
            catch
            {
                return -1;
            }

        }
        
        static public long fn_Save(string section, string key, string val, string filepath)
        {
            return WritePrivateProfileString(section, key, val, filepath);
        }

    }
}
