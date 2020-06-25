using System;
using System.IO;
using System.Windows;
using System.Threading;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Threading;
using Microsoft.WindowsAPICodePack.Dialogs;
using static SeedEncryption.UserINI;
using static SeedEncryption.UserFunction;
using System.Linq;
using System.Windows.Media.Animation;
using System.Windows.Media;

namespace SeedEncryption
{
    /// <summary>
    /// MainWindow.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class MainWindow : Window
    {
        Data_Option m_data_Option = new Data_Option();
        const string m_strSettingPath = ".\\Setting.ini";
        Thread m_thread;
        public MainWindow()
        {
            InitializeComponent();
            this.DataContext = m_data_Option;
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            // Load Data
            fn_ReadData();
        }

        private void Window_Closing(object sender, EventArgs e)
        {
            // Save Data
            fn_WriteData();
        }
        private void Title_MouseDown(object sender, MouseButtonEventArgs e)
        {
            this.DragMove();
        }

        private void bn_Close_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }

        private void Chk_All_Click(object sender, RoutedEventArgs e)
        {
            CheckBox cb = sender as CheckBox;
            if (cb != null)
            {
                m_data_Option.FileH     = (bool)cb.IsChecked;
                m_data_Option.FileCpp   = (bool)cb.IsChecked;
                m_data_Option.FileCs    = (bool)cb.IsChecked;
                m_data_Option.FileXaml  = (bool)cb.IsChecked;
                m_data_Option.FileTxt   = (bool)cb.IsChecked;
                fn_WriteMsg($"Option All Check : {cb.IsChecked}");
            }
        }

        private void fn_ReadData()
        {
            try
            { 
                m_data_Option.LastPath = fn_Load("Path"   , "Path"   , ""   , m_strSettingPath);
                m_data_Option.FileH    = fn_Load("Option" , ".h"     , false, m_strSettingPath);
                m_data_Option.FileCpp  = fn_Load("Option" , ".cpp"   , false, m_strSettingPath);
                m_data_Option.FileCs   = fn_Load("Option" , ".cs"    , false, m_strSettingPath);
                m_data_Option.FileXaml = fn_Load("Option" , ".xaml"  , false, m_strSettingPath);
                m_data_Option.FileTxt  = fn_Load("Option" , ".txt"   , false, m_strSettingPath);
                m_data_Option.Skipvs   = fn_Load("Option" , ".vsSkip", false, m_strSettingPath);
                fn_CheckDecryption();
                fn_WriteMsg($"Load From {m_strSettingPath}.");
            }
            catch (Exception ex)
            {
                fn_WriteMsg($"Load Exception : {ex.Message}.");
            }
        }

        private void fn_WriteData()
        {
            try
            {
                fn_Save("Path"  , "Path"    , m_data_Option.LastPath, m_strSettingPath);
                fn_Save("Option", ".h"      , m_data_Option.FileH   , m_strSettingPath);
                fn_Save("Option", ".cpp"    , m_data_Option.FileCpp , m_strSettingPath);
                fn_Save("Option", ".cs"     , m_data_Option.FileCs  , m_strSettingPath);
                fn_Save("Option", ".xaml"   , m_data_Option.FileXaml, m_strSettingPath);
                fn_Save("Option", ".vsSkip" , m_data_Option.Skipvs  , m_strSettingPath);
                fn_WriteMsg($"Save From {m_strSettingPath}.");
            }
            catch (Exception ex)
            {
                fn_WriteMsg($"Save Exception : {ex.Message}.");
            }
        }

        private void bn_SelFolder_Click(object sender, RoutedEventArgs e)
        {
            CommonOpenFileDialog dlg = new CommonOpenFileDialog();
            dlg.IsFolderPicker = true;
            
            if(dlg.ShowDialog() == CommonFileDialogResult.Ok)
            {
                m_data_Option.LastPath = dlg.FileName;
                fn_CheckDecryption();
                fn_WriteMsg($"Directory Selected : {m_data_Option.LastPath}");
            }
            else
                fn_WriteMsg($"Directory Select Cancel.");
        }

        private void fn_CheckDecryption(bool bResult = false)
        {
            if (Directory.Exists(m_data_Option.LastPath))
            {
                string[] strDir = Directory.GetFiles(m_data_Option.LastPath, "*.ect", SearchOption.AllDirectories);
                m_data_Option.IsDecryption = (strDir.Length > 0) ? true : false;
                if (!bResult)
                {
                    strDir = Directory.GetFiles(m_data_Option.LastPath, "*.*", SearchOption.AllDirectories);
                    m_data_Option.PercentCount = 0;
                    m_data_Option.PercentText = $"0.00% (0 / {strDir.Length})";
                }
                else
                {
                    m_data_Option.CtrlEnable1 = true;
                    m_data_Option.CtrlEnable2 = true;
                }
            }
            else
            {
                fn_WriteMsg($"Not Exist Directory : {m_data_Option.LastPath}");
                m_data_Option.CtrlEnable1 = true;
                m_data_Option.CtrlEnable2 = true;
            }
        }

        private void fn_WriteMsg(string strMsg)
        {
            Dispatcher.BeginInvoke(DispatcherPriority.Normal, new Action(delegate ()
            {
                if (lv_Message.Items.Count > 200)
                    lv_Message.Items.RemoveAt(0);
                lv_Message.Items.Add(DateTime.Now.ToString("[HH:mm:ss] ") + strMsg);
                lv_Message.SelectedIndex = lv_Message.Items.Count - 1;
                lv_Message.ScrollIntoView(lv_Message.SelectedItem);
            }));
        }

        private void bn_Encryption_Click(object sender, RoutedEventArgs e)
        {
            fn_RunThread();
        }

        private void bn_Decryption_Click(object sender, RoutedEventArgs e)
        {
            fn_ViewChild();
        }

        private void fn_RunThread()
        {
            if (m_thread == null)
            {
                m_thread = new Thread(new ThreadStart(THREAD_ENCRYPTION));
                m_thread.Start();
            }
            else
            {
                if (!m_thread.IsAlive)
                {
                    m_thread = new Thread(new ThreadStart(THREAD_ENCRYPTION));
                    m_thread.Start();
                }
            }
        }

        private void THREAD_ENCRYPTION()
        {
            int nCount = 0, nTotal = 0;
            string[] strDir = Directory.GetFiles(m_data_Option.LastPath, "*.*", SearchOption.AllDirectories);
            string strDst = m_data_Option.LastPath + "\\..\\.EncryptionTemp\\";
            string strName = string.Empty;
            nTotal = strDir.Length;
            
            m_data_Option.CtrlEnable1 = false;
            m_data_Option.CtrlEnable2 = false;
            m_data_Option.PercentMinimum = 0;
            m_data_Option.PercentMaximum = nTotal;
            m_data_Option.PercentCount = nCount;
            m_data_Option.PercentText = $"{((double)nCount / (double)nTotal) * 100:F2}% ({nCount} / {nTotal})";

            while (nCount < nTotal)
            {
                try
                {
                    int nFileNameIndex = strDir[nCount].LastIndexOf('\\');
                    string strFileName = strDir[nCount].Substring(nFileNameIndex + 1, strDir[nCount].Length - nFileNameIndex - 1);
                    string strExtension = strFileName.Substring(strFileName.LastIndexOf('.') + 1, strFileName.Length - strFileName.LastIndexOf('.') - 1);
                    string strDstSubPath = strDir[nCount].Substring(m_data_Option.LastPath.Length + 1);
                    strDstSubPath = strDstSubPath.Substring(0, strDstSubPath.Length - strFileName.Length);
                    string strTemp = strDst + strDstSubPath + strFileName;
                    Directory.CreateDirectory(strDst + strDstSubPath);

                    if(m_data_Option.Skipvs)
                    {
                        if (strDstSubPath.Length > 3)
                        {
                            if (strDstSubPath.Substring(0, 3) == ".vs")
                            {
                                fn_WriteMsg($"Skip : {strDir[nCount]}");
                                nCount++;
                                continue;
                            }
                        }
                    }
                    if (fn_CheckExtension(strExtension))
                    {
                        if (m_data_Option.IsEncryption)
                            Encryption(strDir[nCount], strDir[nCount].Length, strTemp, strTemp.Length);
                        else
                            Decryption(strDir[nCount], strDir[nCount].Length, strTemp, strTemp.Length);
                    }
                    else
                    {
                         if (strExtension == "ect")
                             File.Delete(strDst + strDstSubPath + strFileName);
                         else
                            File.Copy(strDir[nCount], strDst + strDstSubPath + strFileName, true);
                    }
                    fn_WriteMsg(strDir[nCount]);

                    nCount++;
                    m_data_Option.PercentCount = nCount;
                    m_data_Option.PercentText = $"{((double)nCount / (double)nTotal) * 100:F2}% ({nCount} / {nTotal})";
                }
                catch (Exception ex)
                {
                    fn_WriteMsg($"EncryptionThread Error : {ex.Message}");
                }
            }

            try
            {
                // Src Folder Delete.
                Directory.Delete(m_data_Option.LastPath, true);
                // Folder Rename.
                Directory.Move(strDst, m_data_Option.LastPath);
            }
            catch(Exception ex)
            {
                fn_WriteMsg($"Rename Error : {ex.Message}");
            }
            fn_CheckDecryption(true);
            fn_WriteMsg("Done.");
        }

        private bool fn_CheckExtension(string extensions)
        {
            bool bRtn = false;
            if (m_data_Option.FileH    && extensions == "h"   ) bRtn |= true;
            if (m_data_Option.FileCpp  && extensions == "cpp" ) bRtn |= true;
            if (m_data_Option.FileCs   && extensions == "cs"  ) bRtn |= true;
            if (m_data_Option.FileXaml && extensions == "xaml") bRtn |= true;
            if (m_data_Option.FileTxt  && extensions == "txt" ) bRtn |= true;
            return bRtn;
        }

        private void Project_PreviewDrop(object sender, DragEventArgs e)
        {
            if(e.Data.GetDataPresent(DataFormats.FileDrop))
            {
                string[] files = (string[])e.Data.GetData(DataFormats.FileDrop);
                m_data_Option.LastPath = files[0];
                fn_CheckDecryption();
                fn_WriteMsg($"Directory Drag & Drop : {m_data_Option.LastPath}");
            }
        }

        private void bn_pwok_Click(object sender, RoutedEventArgs e)
        {
            if (tb_password.Password == "1324")
            {
                tb_password.Password = string.Empty;
                fn_RunThread();
                fn_ViewChild(false);
                fn_WriteMsg("OK.");
            }
            else
            {
                DoubleAnimation_Start();
            }
        }

        private void bn_pwcancel_Click(object sender, RoutedEventArgs e)
        {
            fn_ViewChild(false);
            fn_WriteMsg("Decryption Cancel.");
        }

        private void fn_ViewChild(bool bView = true)
        {
            if(bView)
            {
                grid_Control.IsEnabled = false;
                grid_Control.Opacity = 0.5;
                bd_Password.Visibility = Visibility.Visible;
                tb_password.Focus();
            }
            else
            {
                grid_Control.IsEnabled = true;
                bd_Password.Visibility = Visibility.Hidden;
                grid_Control.Opacity = 1;
            }
        }

        private void tb_password_PreviewKeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
            {
                bn_Ok.Focus();
                bn_pwok_Click(null, null);
            }
            if(e.Key == Key.Escape) bn_pwcancel_Click(null, null);
        }

        public void DoubleAnimation_Start()
        {
            DoubleAnimation DAni = new DoubleAnimation();
            DAni.From = 29.0;
            DAni.To = 26.0;
            DAni.AccelerationRatio = 1;
            DAni.Duration = new Duration(TimeSpan.FromMilliseconds(100));

            DAni.FillBehavior = FillBehavior.Stop;
            DAni.Completed += new EventHandler(MyDoubleAnimation_Completed);
            tb_password.BorderBrush = Brushes.Red;
            tb_password.BorderThickness = new Thickness(2);
            tb_password.BeginAnimation(TextBox.HeightProperty, DAni);
        }
        int nAniCount = 0;
        void MyDoubleAnimation_Completed(object sender, EventArgs e)
        {
            //bn_Ok.Focus();
            DoubleAnimation DAni = new DoubleAnimation();
            DAni.From = 26.0;
            DAni.To = 29.0;

            //가속도값 설정하기
            DAni.AccelerationRatio = 1;
            DAni.Duration = new Duration(TimeSpan.FromMilliseconds(100));

            //애니메이션 효과를 적용한 후에는 속성 값을 변경하기
            DAni.FillBehavior = FillBehavior.Stop;
            if (nAniCount < 3)
            {
                DAni.Completed += new EventHandler(MyDoubleAnimation_Completed);
            }
            else
            {
                nAniCount = 0;
                tb_password.BorderBrush = Brushes.Gray;
                tb_password.BorderThickness = new Thickness(1);
                tb_password.Focus();
                tb_password.SelectAll();
                
                return;
            }
            tb_password.BeginAnimation(TextBox.HeightProperty, DAni);
            nAniCount++;
        }
    }
}
