Attribute VB_Name = "Module2"
  
  Declare Function OpenComm Lib "mi.dll" _
        Alias "API_OpenComm" (ByRef comm As Byte, ByVal nBoudrate As Long) As Long

    Declare Function CloseComm Lib "mi.dll" _
        Alias "API_CloseComm" (ByVal handle As Long) As Integer


   Declare Function API_PCDRead Lib "mi.dll" _
        (ByVal handle As Integer, ByVal deviceAddr As Integer, _
        ByVal mode As Byte, ByVal blk_Addr As Byte, ByVal Num_blk As Byte, _
        ByRef snr As Byte, _
        ByRef buffer As Byte) As Integer

    Declare Function API_PCDWrite Lib "mi.dll" _
        (ByVal handle As Integer, ByVal deviceAddr As Integer, _
        ByVal mode As Byte, ByVal blk_Addr As Byte, ByVal Num_blk As Byte, _
        ByRef snr As Byte, _
        ByRef buffer As Byte) As Integer
        
  '����ͨѶ��ַ
   Declare Function API_SetDeviceAddress Lib "mi.dll" _
        (ByVal handle As Integer, ByVal deviceAddr As Integer, _
        ByVal new_Addr As Byte, _
        ByRef buffer As Byte) As Integer
        
  '������ı�����
   Declare Function API_SetBandrate Lib "mi.dll" _
        (ByVal handle As Integer, ByVal deviceAddr As Integer, _
        ByVal newBaud As Byte, _
        ByRef buffer As Byte) As Integer



'���õƵĹ���״̬�������������������Լ�ѭ���Ĵ���
Declare Function ControlLED Lib "mi.dll" _
        (ByVal freq As Byte, ByVal duration As Byte, _
        ByRef buffer As Byte) As Integer
        
        
'���õƵĹ���״̬�������������������Լ�ѭ���Ĵ���
Declare Function ControlBuzzer Lib "mi.dll" _
        (ByVal freq As Byte, ByVal duration As Byte, _
        ByRef buffer As Byte) As Integer
        
'��ȡ�ɳ���Ԥ���1���ֽڵĶ�������ַ��8���ֽ����к�
Declare Function GetSerNum Lib "mi.dll" _
        (ByRef buffer As Byte) As Integer
        
        '��ȡ�ɳ���Ԥ���1���ֽڵĶ�������ַ��8���ֽ����к�
Declare Function SetSerNum Lib "mi.dll" _
        (ByRef newvalue As Byte, ByRef buffer As Byte) As Integer
        
Declare Function GetVersionNum Lib "mi.dll" _
        (ByRef buffer As Byte) As Integer
        
        
Declare Function MF_Read Lib "mi.dll" _
        (ByVal mode As Byte, ByVal blk_Addr As Byte, ByVal Num_blk As Byte, _
        ByRef snr As Byte, _
        ByRef buffer As Byte) As Integer
        
Declare Function MF_Write Lib "mi.dll" _
        (ByVal mode As Byte, ByVal blk_Addr As Byte, ByVal Num_blk As Byte, _
        ByRef snr As Byte, _
        ByRef buffer As Byte) As Integer


Declare Function MF_InitValue Lib "mi.dll" _
        (ByVal mode As Byte, ByVal Num_blk As Byte, _
        ByRef snr As Byte, _
        ByRef buffer As Byte) As Integer
        
Declare Function MF_Dec Lib "mi.dll" _
        (ByVal mode As Byte, ByVal Num_blk As Byte, _
        ByRef snr As Byte, _
        ByRef buffer As Byte) As Integer
        
Declare Function MF_Inc Lib "mi.dll" _
        (ByVal mode As Byte, ByVal Num_blk As Byte, _
        ByRef snr As Byte, _
        ByRef buffer As Byte) As Integer
        
Declare Function MF_Getsnr Lib "mi.dll" _
        (ByVal mode As Byte, ByVal halt As Byte, _
        ByRef snr As Byte, _
        ByRef buffer As Byte) As Integer
        
Declare Function API_PCDInitVal Lib "mi.dll" _
        (ByVal handle As Integer, ByVal deviceAddr As Integer, _
        ByVal mode As Byte, ByVal Num_blk As Byte, _
        ByRef snr As Byte, _
        ByRef buffer As Byte) As Integer
        
'�Զ���ȡ��Ultralight�Ŀ���
Declare Function UL_Request Lib "mi.dll" _
        (ByVal mode As Byte, _
        ByRef snr As Byte) As Integer
        
        'ѡ�񿨣�ʹ�����뱻�жϵ�״̬��
Declare Function MF_Halt Lib "mi.dll" _
        () As Integer
'��ȡ����ָ������������
Declare Function UL_HLRead Lib "mi.dll" _
        (ByVal mode As Byte, ByVal blk_Addr As Byte, _
        ByRef snr As Byte, _
        ByRef buffer As Byte) As Integer
        
        'дȡ����ָ������������
Declare Function UL_HLWrite Lib "mi.dll" _
        (ByVal mode As Byte, ByVal blk_Addr As Byte, _
        ByRef snr As Byte, _
        ByRef buffer As Byte) As Integer
        
          'дȡ����ָ������������
Declare Function ISO15693_Inventory Lib "mi.dll" _
        (ByRef snr As Byte, _
        ByRef buffer As Byte) As Integer
        
Declare Function TypeB_Request Lib "mi.dll" _
        (ByRef snr As Byte) As Integer
        
Declare Function TypeB_TransCOS Lib "mi.dll" _
        (ByRef cmd As Byte, ByVal cmdSize As Integer, ByRef data As Byte) As Integer
        
Declare Function TYPEB_SFZSNR Lib "mi.dll" _
        (ByVal mode As Byte, ByVal halt As Byte, ByRef snr As Byte, ByRef buffer As Byte) As Integer
        
          '��ȡ����
Declare Function ISO15693_Read Lib "mi.dll" _
        (ByVal flags As Byte, ByVal blk_Addr As Integer, ByVal Num_blk As Integer, _
        ByRef uid As Byte, _
        ByRef buffer As Byte) As Integer
        
          'д����
Declare Function ISO15693_Write Lib "mi.dll" _
        (ByVal flags As Byte, ByVal blk_Addr As Integer, ByVal Num_blk As Integer, _
        ByRef uid As Byte, _
        ByRef buffer As Byte) As Integer
        
Declare Function ISO15693_Lock Lib "mi.dll" _
        (ByVal flags As Byte, ByVal Num_blk As Integer, _
        ByRef uid As Byte, _
        ByRef buffer As Byte) As Integer
        
Declare Function ISO15693_Select Lib "mi.dll" _
        (ByVal flags As Byte, ByRef uid As Byte, _
        ByRef buffer As Byte) As Integer
        
Declare Function ISO15693_WriteAFI Lib "mi.dll" _
        (ByVal flags As Byte, ByVal Num_blk As Integer, _
        ByRef uid As Byte, _
        ByRef buffer As Byte) As Integer
        
Declare Function ISO15693_LockAFI Lib "mi.dll" _
        (ByVal flags As Byte, ByRef uid As Byte, _
        ByRef buffer As Byte) As Integer
        
Declare Function ISO15693_LockDSFID Lib "mi.dll" _
        (ByVal flags As Byte, ByRef uid As Byte, _
        ByRef buffer As Byte) As Integer
        
Declare Function GET_SNR Lib "mi.dll" _
        (ByVal handle As Integer, ByVal deviceAddr As Integer, _
        ByVal mode As Byte, ByVal half As Byte, _
        ByRef snr As Byte, _
        ByRef buffer As Byte) As Integer






