   M S B A M L   `   `   `  ���� I  ESeedEncryption, Version=1.0.0.0, Culture=neutral, PublicKeyToken=null/clr-namespace:SeedEncryptionSeedEncryption      SeedEncryption.MainWindow   W SPresentationCore, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35R NWindowsBase, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35\ XPresentationFramework, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35D 9http://schemas.microsoft.com/winfx/2006/xaml/presentation    R NSystem.Xaml, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089:x,http://schemas.microsoft.com/winfx/2006/xaml     @d2http://schemas.microsoft.com/expression/blend/2008     Jmc;http://schemas.openxmlformats.org/markup-compatibility/2006     .localclr-namespace:SeedEncryption     -     � Title$  
MainWindow��$	��450��$	��500�� � WindowStyle$
 None=�$��0q�.��� $��0q� � RowDefinitions �� $��30 ��� .+�� $��0��
������� � ColumnDefinitions �� �� $��30 �.+��� -   $��10,0,0,0q�$��Center=�$��0q�.���� $��Project Encryption��$��18(�$
��Bold"��� -   $��1��$��X��$��5q����	#FFC82828
�������$
��Bold"�� $��1��$��5q�.+�� -   $��grid_Control�� J� 	AllowDrop . 
������� �� $��50 ��� $��60 ��� $��50 ��� .+��� $��0��$��대상 프로젝트��{���  �� Path$ CtrlEnable1� �� Mode$ TwoWay=� �� UpdateSourceTrigger$ PropertyChanged=�.���  �� �� $��80 �.+��� $��0��$��Center=�.���� ���� $ LastPath�$ TwoWay=�$ PropertyChanged=��� -   $��1��$��Select Project���� $��1��$��옵션��{��� $ CtrlEnable2�$ TwoWay=�$ PropertyChanged=�.���  �� �� $	��160 � �� �� .+�A� -   $��	ug_Option�� A� Columns$ 6��$��Center=�.+��� -   $	��ALL��$
��Bold"��� -   $
��cb_h��$��.h��	 d� 	IsChecked	 �� $ FileH�$ TwoWay=�$ PropertyChanged=��� -	   $��cb_cpp��$
��.cpp��	 �� $ FileCpp�$ TwoWay=�$ PropertyChanged=��� -
   $��cb_cs��$	��.cs��	 �� $ FileCs�$ TwoWay=�$ PropertyChanged=��� -   $��cb_xaml��$��.xaml��	 �� $ FileXaml�$ TwoWay=�$ PropertyChanged=��� -   $��cb_txt��$
��.txt��	 �� $ FileTxt�$ TwoWay=�$ PropertyChanged=�� $��1��.+�A� $ 3��.+��� -   $��	cb_Skipvs��$��.vs Skip��	 �� $ Skipvs�$ TwoWay=�$ PropertyChanged=�A� $��1��$ 2��$��2��.+��� -   $��bn_Encryption��$��
Encryption��$	��2,0q�{��� $ IsEncryption��� -   $��bn_Decryption��$��
Decryption��$	��2,0q�{��� $ IsDecryption��� $��2��$��	진행율��.��� .+�� -   $��pgbar_Percent��
 � Value
 �� $ PercentCount�$ TwoWay=�$ PropertyChanged=� � Minimum �� $ PercentMinimum�$ TwoWay=�$ PropertyChanged=� � Maximum �� $ PercentMaximum�$ TwoWay=�$ PropertyChanged=��� $��Center=�$��Center=�.���� -   $��
tb_Percent������ $ PercentText�$ TwoWay=�$ PropertyChanged=��� $��3��$��Message��.���� -   $��
lv_Message���� -   $��bd_Password��$	��200��$	��100��
�������$��1q�
�������$y�Hidden=�._��  �� $��20 ��� .+�� $��0��
�������.+��� $��0q�$��Center=�.���� $��10,0,0,0q�$��Type Password��$
��bold"�� $��1�� �� $��10 ��� �� $��10 ��� .+�� $��1�� �� $��75 ��� �� $��5 �.+��� $��0��$��Center=�.���� $��
Password :��$
��Bold"�G� -   $��tb_password��$��1��
���   �$��1q� G� PasswordChar$ *��$��Center=�$��Center=�� $��3�� �� �� $��10 ��� .+��� -   $��bn_Ok��$��0��$��4q�$��OK���� -   $��	bn_Cancel��$��2��$��4q�$��Cancel��