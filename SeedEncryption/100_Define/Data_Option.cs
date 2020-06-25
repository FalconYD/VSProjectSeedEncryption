using System.ComponentModel;

namespace SeedEncryption
{
    public class IPropertyChanged : INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;

        protected void OnPropertyChanged(string propertyName)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }
    class Data_Option : IPropertyChanged
    {
        private string strLastPath = string.Empty;
        public string LastPath
        {
            get { return strLastPath; }
            set
            {
                strLastPath = value;
                OnPropertyChanged("LastPath");
            }
        }

        private bool bIsEncryption = false;
        public bool IsEncryption
        {
            get { return bIsEncryption; }
            set
            {
                bIsEncryption = value;
                //IsDecryption = !bIsEncryption;
                OnPropertyChanged("IsEncryption");
            }
        }

        private bool bIsDecryption = false;
        public bool IsDecryption
        {
            get { return bIsDecryption; }
            set
            {
                bIsDecryption = value;
                IsEncryption = !bIsDecryption;
                OnPropertyChanged("IsDecryption");
            }
        }

        private bool bFileCpp = false;
        public bool FileCpp
        {
            get { return bFileCpp; }
            set
            {
                bFileCpp = value;
                OnPropertyChanged("FileCpp");
            }
        }

        private bool bFileH = false;
        public bool FileH
        {
            get { return bFileH; }
            set
            {
                bFileH = value;
                OnPropertyChanged("FileH");
            }
        }

        private bool bFileCs = false;
        public bool FileCs
        {
            get { return bFileCs; }
            set
            {
                bFileCs = value;
                OnPropertyChanged("FileCs");
            }
        }

        private bool bFileXaml = false;
        public bool FileXaml
        {
            get { return bFileXaml; }
            set
            {
                bFileXaml = value;
                OnPropertyChanged("FileXaml");
            }
        }

        private bool bFileTxt = false;
        public bool FileTxt
        {
            get { return bFileTxt; }
            set
            {
                bFileTxt = value;
                OnPropertyChanged("FileTxt");
            }
        }

        private string strPercentText = string.Empty;
        public string PercentText
        {
            get { return strPercentText; }
            set
            {
                strPercentText = value;
                OnPropertyChanged("PercentText");
            }
        }

        private int nPercentCount = 0;
        public int PercentCount
        {
            get { return nPercentCount; }
            set
            {
                nPercentCount = value;
                OnPropertyChanged("PercentCount");
            }
        }

        private double dPercentMinimum = 0.0;
        public double PercentMinimum
        {
            get { return dPercentMinimum; }
            set
            {
                dPercentMinimum = value;
                OnPropertyChanged("PercentMinimum");
            }
        }

        private double dPercentMaximum = 0.0;
        public double PercentMaximum
        {
            get { return dPercentMaximum; }
            set
            {
                dPercentMaximum = value;
                OnPropertyChanged("PercentMaximum");
            }
        }

        private bool bCtrlEnable1 = true;
        public bool CtrlEnable1
        {
            get { return bCtrlEnable1; }
            set
            {
                bCtrlEnable1 = value;
                OnPropertyChanged("CtrlEnable1");
            }
        }

        private bool bCtrlEnable2 = true;
        public bool CtrlEnable2
        {
            get { return bCtrlEnable2; }
            set
            {
                bCtrlEnable2 = value;
                OnPropertyChanged("CtrlEnable2");
            }
        }

        private bool bSkipvs = false;
        public bool Skipvs
        {
            get { return bSkipvs; }
            set
            {
                bSkipvs = value;
                OnPropertyChanged("Skipvs");
            }
        }
    }
}
