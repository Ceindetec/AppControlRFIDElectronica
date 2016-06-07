package com.syc.function;

import org.xvolks.jnative.JNative;
import org.xvolks.jnative.Type;
import org.xvolks.jnative.pointers.Pointer;
import org.xvolks.jnative.pointers.memory.MemoryBlockFactory;

/**
 * 
 * @author Snail
 * @version v1.0
 *
 */

/**
 *RFID������USB�ӿڣ�Linux�µ�Java�ӿ��ࡣʹ�øð�ʱ���뽫Function.jar,jNative.jar,libfunction.so,libJNativeCpp.so���Ƶ�����Ŀ¼�¡����� Function.jar,jNative.jar�ӿڰ���<br/> <br/>
 *<b>��������ֵ�����</b><br/>
 *0x00 ����ִ�гɹ�  <br/>
 *0x01 �������ʧ�ܣ�ʧ��ԭ����ο��ײ㵥Ƭ���ϴ������<br/>
 *0x02 ��ַУ�����<br/>
 *0x03 û��ѡ��COM��<br/>
 *0x04 ��д�����س�ʱ<br/>
 *0x05 ���ݰ���ˮ�Ų���ȷ <br/>
 *0x07 �����쳣<br/>
 *0x0A  ����ֵ������Χ <br/> <br/>
 *<b>�ײ㵥Ƭ���ϴ������</b><br/>
 *0x80 �������óɹ�<br/>
 *0x81 ��������ʧ��<br/>
 *0x82 ͨѶ��ʱ <br/>
 *0x83 ��������<br/>
 *0x84 ���տ����ݳ���<br/>
 *0x87 δ֪�Ĵ���<br/>
 *0x85 ��������������������ʽ����<br/>
 *0x8f �����ָ����벻����<br/>
 *0x8A �ڶ��ڿ����ʼ�������г��ִ��󣨽�����14443���<br/>
 *0x8B �ڷ���ͻ�����еõ���������кţ�������14443���<br/>
 *0x8C ������֤ûͨ����������14443���<br/>
 *0x90 ����֧��������������15693���<br/>
 *0x91 �����ʽ�д��󣨽�����15693���<br/>
 *0x92 �������FLAG�����У���֧��OPTION ģʽ��������15693���<br/>
 *0x93 Ҫ������BLOCK�����ڣ�������15693���<br/>
 *0x94 Ҫ�����Ķ����Ѿ������������ܽ����޸ģ�������15693���<br/>
 *0x95 �����������ɹ���������15693���<br/>
 *0x96 д�������ɹ���������15693���<br/>
 */

public class Function {
	private static String functionLib = "libfunction.so";

	private static Pointer getPointer(byte[] s) {
		try {
			Pointer pointer = new Pointer(
					MemoryBlockFactory.createMemoryBlock(s.length));
			for (int i = 0; i < s.length; i++) {
				pointer.setByteAt(i, s[i]);
			}
			return pointer;
		} catch (Exception e) {
			System.out.print("Failed to get Pointer Data!");
			return null;
		}
	}

	private static void getPointerData(Pointer p, byte[] s) {
		try {
			for (int i = 0; i < p.getSize(); i++) {
				s[i] = p.getAsByte(i);
			}
		} catch (Exception e) {
			System.out.print("Failed to get Pointer Data!");
		}
	}

	private static int loadCommand(String functionName, int... parameter) {
		JNative n = null;
		try {
			n = new JNative(functionLib, functionName);
			n.setRetVal(Type.INT);
			int i = 0;
			for (int p : parameter) {
				n.setParameter(i++, p);
			}
			n.invoke();
			return Integer.parseInt(n.getRetVal());
		} catch (Exception e) {
			System.out.print("Failed to load libfunction.so");
			return 1;
		}
	}

	/**
	 * ϵͳ������� 8���ֽڵĲ�Ʒ���кš�
	 * @param newValue 8���ֽڵĶ�д�����к�.
	 * @param buffer �������ؽ��յ�������.
	 * @return 0x00	���óɹ�.<br/>
	 * 0x01	����ʧ��.<br/><br/>
	 * ���óɹ���<br/>
	 * buffer=0x80<br/>
	 * ����ʧ�ܣ�<br/>
	 * ��bufferΪ�ײ㣨��������Ƭ�����ϴ��Ĵ�����롣
	 */
	public static int API_SetSerNum(byte[] newValue, byte[] buffer) {
		Pointer newValuePtr = getPointer(newValue);
		Pointer bufferPtr = getPointer(buffer);
		int result = loadCommand("API_SetSerNum", newValuePtr.getPointer(),
				bufferPtr.getPointer());
		getPointerData(bufferPtr, buffer);
		return result;
	}

	/**
	 * ϵͳ�����ȡ�ɳ���Ԥ���1���ֽڵĶ�������ַ��8���ֽ����кš�
	 * @param buffer �������ؽ��յ������ݡ�
	 * @return 0x00	���óɹ�.<br/>
	 * 0x01	����ʧ��.<br/><br/>
	 * buffer[0] ��д����ַ��<br/>
	 * buffer[1...8] 8���ֽڵĶ�д�����кš�
	 */
	public static int API_GetSerNum(byte[] buffer) {
		Pointer bufferPtr = getPointer(buffer);
		int result = loadCommand("API_GetSerNum", bufferPtr.getPointer());
		getPointerData(bufferPtr, buffer);
		return result;
	}

	/**
	 * ϵͳ����������ṩ4���飨ÿ���鲻�ܴ���120���ֽڣ�����480���ֽڿռ���û����������û����Ը�����Ҫ��������Ӧ���û���Ϣ����д���С�
	 * @param num_blk ����š�
	 * @param num_length ���ݳ��ȡ�
	 * @param user_info �û����ݡ�
	 * @return 0x00	���óɹ�.<br/>
	 * 0x01	����ʧ��.
	 */
	public static int WriteUserInfo(int num_blk, int num_length,
			byte[] user_info) {
		Pointer user_infoPtr = getPointer(user_info);
		int result = loadCommand("WriteUserInfo", num_blk, num_length,
				user_infoPtr.getPointer());
		getPointerData(user_infoPtr, user_info);
		return result;
	}

	/**
	 * ϵͳ�����ȡ���������ṩ4���飨ÿ���鲻�ܴ���120���ֽڣ������ݡ�
	 * @param num_blk ����š�
	 * @param num_length ��ȡ�����ݳ��ȡ�
	 * @param user_info ��������û����� ��
	 * @return 0x00	���óɹ�.<br/>
	 * 0x01	����ʧ��.
	 */
	public static int ReadUserInfo(int num_blk, int num_length, byte[] user_info) {
		Pointer user_infoPtr = getPointer(user_info);
		int result = loadCommand("ReadUserInfo", num_blk, num_length,
				user_infoPtr.getPointer());
		getPointerData(user_infoPtr, user_info);
		return result;
	}

	/**
	 * ϵͳ�����ȡ��д���İ汾��.
	 * @param VersionNum ������İ汾�š�
	 * @return 0x00	���óɹ�.<br/>
	 * 0x01	����ʧ��.
	 */
	public static int GetVersionNum(byte[] VersionNum) {
		Pointer VersionNumPtr = getPointer(VersionNum);
		int result = loadCommand("GetVersionNum", VersionNumPtr.getPointer());
		getPointerData(VersionNumPtr, VersionNum);
		return result;
	}

	/**
	 * ϵͳ������õƵĹ���״̬�������������������Լ�ѭ���Ĵ�����
	 * @param freq ��������
	 * @param duration ������
	 * @param buffer �����صĲ�����
	 * @return 0x00	���óɹ�.<br/>
	 * 0x01	����ʧ��.<br/><br/>
	 * ����ʧ�ܣ�  ��buffer[0]Ϊ ������롣<br/>
	 * �����ɹ���  ��buffer[0]Ϊ�ɹ���־ ����Ϊ0x80��
	 */
	public static int API_ControlLED(byte freq, byte duration, byte[] buffer) {
		Pointer bufferPtr = getPointer(buffer);
		int result = loadCommand("API_ControlLED", freq, duration,
				bufferPtr.getPointer());
		getPointerData(bufferPtr, buffer);
		return result;
	}

	/**
	 * ϵͳ������÷������Ĺ���״̬���������������Ĺ��������Լ�ѭ���Ĵ�����
	 * @param freq ��������
	 * @param duration ������
	 * @param buffer �����صĲ�����
	 * @return 0x00	���óɹ�.<br/>
	 * 0x01	����ʧ��.<br/><br/>
	 * ����ʧ�ܣ�  ��buffer[0]Ϊ ������롣<br/>
	 * �����ɹ���  ��buffer[0]Ϊ�ɹ���־ ����Ϊ0x80��
	 */
	public static int API_ControlBuzzer(byte freq, byte duration, byte[] buffer) {
		Pointer bufferPtr = getPointer(buffer);
		int result = loadCommand("API_ControlBuzzer", freq, duration,
				bufferPtr.getPointer());
		getPointerData(bufferPtr, buffer);
		return result;
	}

	/**
	 * ISO14443A�������ISO14443 A Ѱ��ָ�
	 * @param inf_mode Ѱ��ģʽ��<br/>
	 * 0x52�CIdleģʽ��һ��ֻ��һ�ſ���������<br/>
	 * 0x26�CAllģʽ��һ�οɶԶ��ſ���������
	 * @param buffer
	 * @return 0x00	���óɹ�.<br/>
	 * 0x01	����ʧ��.<br/><br/>
	 * ����ʧ�ܣ�  ��snr[0]Ϊ ������롣<br/>
	 * �����ɹ���  ��buffer[0..1]����2���ֽڵ����ݴ���
	 */
	public static int MF_Request(byte inf_mode, byte[] buffer) {
		Pointer bufferPtr = getPointer(buffer);
		int result = loadCommand("MF_Request", inf_mode, bufferPtr.getPointer());
		getPointerData(bufferPtr, buffer);
		return result;
	}

	/**
	 * ISO14443A�����⿨Ƭ������������࿨��������4���ֽڵĿ��š�������ж��ſ����ͷ�������һ�ſ��Ŀ��ţ���
	 * @param snr ����4���ֽڵĿ��š�
	 * @param status ���ؿ�Ƭ��������
	 * @return 0x00	���óɹ�.<br/>
	 * 0x01	����ʧ��.<br/><br/>
	 * �����ɹ���<br/>
	 * status[0] ��⵽�Ŀ�Ƭ������(0x00��ʾ��⵽����,0x01��ʾ��⵽�࿨)��<br/>
	 * snr 4���ֽڵĿ��ţ�snr[0..3]���� <br/>
	 * ����ʧ�ܣ�<br/>
	 * snr[0] ������롣
	 */
	public static int MF_Anticoll(byte[] snr, byte[] status) {
		Pointer snrPtr = getPointer(snr);
		Pointer statusPtr = getPointer(status);
		int result = loadCommand("MF_Anticoll", snrPtr.getPointer(),
				statusPtr.getPointer());
		getPointerData(snrPtr, snr);
		getPointerData(statusPtr, status);
		return result;
	}

	/**
	 * ISO14443A���ʹ�����뱻ѡ���״̬.
	 * @param snr ����4���ֽڿ��ţ����ҷ���4���ֽڵĿ���.
	 * @return 0x00	���óɹ�.<br/>
	 * 0x01	����ʧ��.<br/><br/>
	 * snr 4���ֽڵĿ��ţ�snr[0..3]�� 
	 */
	public static int MF_Select(byte[] snr) {
		Pointer snrPtr = getPointer(snr);
		int result = loadCommand("MF_Select", snrPtr.getPointer());
		getPointerData(snrPtr, snr);
		return result;
	}

	/**
	 * ISO14443A���ѡ�񿨣�ʹ�����뱻�жϵ�״̬.
	 * @return 0x00	���óɹ�.<br/>
	 * 0x01	����ʧ��.
	 */
	public static int MF_Halt() {
		int result = loadCommand("MF_Halt");
		return result;
	}

	/**
	 * ISO14443A�����ָ��λ�ö�ȡָ�����ȵ�����.
	 * @param mode ��ȡģʽ<br/>
	 * Idle+KeyA mode=00<br/>
	 * Idle+KeyB mode=02<br/>
	 * All+KeyA mode=01<br/>
	 * All+KeyB mode=03<br/>
	 * @param blk_add ��ȡ���ַ.
	 * @param num_blk ��ȡ����Ŀ.
	 * @param snr �����ֽڵ���Կ.
	 * @param buffer �ȴ������������.
	 * @return 0x00	���óɹ�.<br/>
	 * 0x01	����ʧ��.<br/><br/>
	 * �����ɹ�:<br/>
	 * snr 4���ֽڵĿ���.<br/>
	 * buffer ��ȡ��������(��������Ϊ��num_blk*16).<br/>
	 * ����ʧ��:<br/>
	 * snr[0] �������.
	 */
	public static int API_PCDRead(byte mode, byte blk_add, byte num_blk,
			byte[] snr, byte[] buffer) {
		Pointer snrPtr = getPointer(snr);
		Pointer bufferPtr = getPointer(buffer);
		int result = loadCommand("API_PCDRead", mode, blk_add, num_blk,
				snrPtr.getPointer(), bufferPtr.getPointer());
		getPointerData(snrPtr, snr);
		getPointerData(bufferPtr, buffer);
		return result;
	}

	/**
	 * ISO14443A�����ָ��λ��д������.
	 * @param mode ��ȡģʽ<br/>
	 * Idle+KeyA mode=00<br/>
	 * Idle+KeyB mode=02<br/>
	 * All+KeyA mode=01<br/>
	 * All+KeyB mode=03<br/>
	 * @param blk_add Ҫд���ַ.
	 * @param num_blk Ҫд����Ŀ.
	 * @param snr 6�ֽ�����.
	 * @param buffer ��д�������.
	 * @return 0x00	���óɹ�.<br/>
	 * 0x01	����ʧ��.<br/><br/>
	 * �����ɹ�:<br/>
	 * snr[0..3] 4���ֽڵĿ���.<br/>
	 * ����ʧ��:<br/>
	 * snr[0] �������.
	 */
	public static int API_PCDWrite(byte mode, byte blk_add, byte num_blk,
			byte[] snr, byte[] buffer) {
		Pointer snrPtr = getPointer(snr);
		Pointer bufferPtr = getPointer(buffer);
		int result = loadCommand("API_PCDWrite", mode, blk_add, num_blk,
				snrPtr.getPointer(), bufferPtr.getPointer());
		getPointerData(snrPtr, snr);
		return result;
	}

	/**
	 * ISO14443A�����ʼ����.
	 * @param mode ��ȡģʽ<br/>
	 * Idle+KeyA mode=00<br/>
	 * Idle+KeyB mode=02<br/>
	 * All+KeyA mode=01<br/>
	 * All+KeyB mode=03<br/>
	 * @param SectNum Ҫ��ʼ����������00-0F.
	 * @param snr 6�ֽ���Կ.
	 * @param value  4 �ֽڵ�Ҫ��ʼ��������.
	 * @return 0x00	���óɹ�.<br/>
	 * 0x01	����ʧ��.<br/><br/>
	 * �����ɹ�:<br/>
	 * snr[0..3] 4���ֽڵĿ���.<br/>
	 * ����ʧ��:<br/>
	 * snr[0] �������.
	 */
	public static int API_PCDInitVal(byte mode, byte SectNum, byte[] snr,
			byte[] value) {
		Pointer snrPtr = getPointer(snr);
		Pointer valuePtr = getPointer(value);
		int result = loadCommand("API_PCDInitVal", mode, SectNum,
				snrPtr.getPointer(), valuePtr.getPointer());
		getPointerData(snrPtr, snr);
		return result;
	}

	/**
	 * ISO14443A����Կ���ָ���������м�ֵ������
	 * @param mode ��ȡģʽ<br/>
	 * Idle+KeyA mode=00<br/>
	 * Idle+KeyB mode=02<br/>
	 * All+KeyA mode=01<br/>
	 * All+KeyB mode=03<br/>
	 * @param SectNum Ҫдֵ��������00-0F.
	 * @param snr 6�ֽ���Կ.
	 * @param value Ҫ����ֵ��4���ֽڳ���.
	 * @return 0x00	���óɹ�.<br/>
	 * 0x01	����ʧ��.<br/><br/>
	 * �����ɹ�:<br/>
	 * snr[0..3] 4���ֽڵĿ���.<br/>
	 * value[0..3] 4���ֽڲ���������ݴ�.<br/>
	 * ����ʧ��:<br/>
	 * snr[0] �������.
	 */
	public static int API_PCDDec(byte mode, byte SectNum, byte[] snr,
			byte[] value) {
		Pointer snrPtr = getPointer(snr);
		Pointer valuePtr = getPointer(value);
		int result = loadCommand("API_PCDDec", mode, SectNum,
				snrPtr.getPointer(), valuePtr.getPointer());
		getPointerData(snrPtr, snr);
		return result;
	}

	/**
	 * ISO14443A����Կ���ָ���������м�ֵ������
	 * @param mode ��ȡģʽ<br/>
	 * Idle+KeyA mode=00<br/>
	 * Idle+KeyB mode=02<br/>
	 * All+KeyA mode=01<br/>
	 * All+KeyB mode=03<br/>
	 * @param SectNum Ҫ��ֵ��������00-0F.
	 * @param snr 6�ֽ���Կ.
	 * @param value Ҫ�ӵ�ֵ��4���ֽڳ���.
	 * @return 0x00	���óɹ�.<br/>
	 * 0x01	����ʧ��.<br/><br/>
	 * �����ɹ�:<br/>
	 * snr[0..3] 4���ֽڵĿ���.<br/>
	 * value[0..3] 4���ֽڲ���������ݴ�.<br/>
	 * ����ʧ��:<br/>
	 * snr[0] �������.
	 */
	public static int API_PCDInc(byte mode, byte SectNum, byte[] snr,
			byte[] value) {
		Pointer snrPtr = getPointer(snr);
		Pointer valuePtr = getPointer(value);
		int result = loadCommand("API_PCDInc", mode, SectNum,
				snrPtr.getPointer(), valuePtr.getPointer());
		getPointerData(snrPtr, snr);
		return result;
	}

	/**
	 * ISO14443A�������1���ֽڵĵ�����࿨��ʶ��4���ֽڵĿ��š�
	 * @param mode ģʽ����.<br/>
	 * 0x26�CIdleģʽ��һ��ֻ��һ�ſ�������.<br/>
	 * 0x52�CAllģʽ��һ�οɶԶ��ſ�����).
	 * @param API_halt
	 * @param snr
	 * @param value
	 * @return 0x00	���óɹ�.<br/>
	 * 0x01	����ʧ��.<br/><br/>
	 * �����ɹ�:<br/>
	 * snr[0] 1���ֽڵĵ�����࿨��ʶ.<br/>
	 * value[0..3] ���ص�4���ֽڵĿ���.<br/>
	 * ����ʧ��:<br/>
	 * snr[0] �������.
	 */
	public static int GET_SNR(byte mode, byte API_halt, byte[] snr, byte[] value) {
		Pointer snrPtr = getPointer(snr);
		Pointer valuePtr = getPointer(value);
		int result = loadCommand("GET_SNR", mode, API_halt,
				snrPtr.getPointer(), valuePtr.getPointer());
		getPointerData(snrPtr, snr);
		getPointerData(valuePtr, value);
		return result;
	}

	/**
	 * ISO14443A�������ѡ���ģʽ���������ݵķ���.
	 * @param mode ģʽ����.<br/>
	 * 0x00������Ҫ����CRCУ��.<br/>
	 * 0x01����Ҫ����CRCУ��.
	 * @param cardlength
	 * @param carddata
	 * @return 0x00	���óɹ�.<br/>
	 * 0x01	����ʧ��.<br/><br/>
	 * �����ɹ���<br/>
	 * carddata[0..N] ���շ������ݡ�<br/>
	 * ����ʧ�ܣ�<br/>
	 * carddata[0] �������.
	 */
	public static int MF_Restore(byte mode, int cardlength, byte[] carddata) {
		Pointer carddataPtr = getPointer(carddata);
		int result = loadCommand("MF_Restore", mode, cardlength,
				carddataPtr.getPointer());
		getPointerData(carddataPtr, carddata);
		return result;
	}

	/**
	 * ISO14443B���������ִ��ISO14443B�е�REQB�����ȡ��Ƭ��PUPI���롣
	 * @param buffer ��Ƭ��λ������ݴ� ��ATQB����
	 * @return 0x00	���óɹ�.<br/>
	 * 0x01	����ʧ��.<br/><br/>
	 * �����ɹ���<br/>
	 * buffer ��Ƭ��λ������ݴ���ATQB��.<br/>
	 * buffer[0] ��Ƭ��λ���ݵĳ���.<br/>
	 * buffer[1..N] ����������ݴ���ATQB��.<br/>
	 * ����ʧ�ܣ�<br/>
	 * buffer[0] �������.
	 */
	public static int RequestType_B(byte[] buffer) {
		Pointer bufferPtr = getPointer(buffer);
		int result = loadCommand("RequestType_B", bufferPtr.getPointer());
		getPointerData(bufferPtr, buffer);
		return result;
	}

	/**
	 * ISO14443B���������ִ��ISO14443B�е�AnticollB����.
	 * @param buffer ��Ƭ���ص����ݴ���ATQB��.
	 * @return 0x00	���óɹ�.<br/>
	 * 0x01	����ʧ��.<br/><br/>
	 * ��������ɹ���<br/>
	 * buffer[0..N] ��Ƭ���ص����ݴ���ATQB��.<br/>
	 * �������ʧ�ܣ�<br/>
	 * buffer[0] �������.
	 */
	public static int AntiType_B(byte[] buffer) {
		Pointer bufferPtr = getPointer(buffer);
		int result = loadCommand("AntiType_B", bufferPtr.getPointer());
		getPointerData(bufferPtr, buffer);
		return result;
	}

	/**
	 * ISO14443B���������ִ��ISO14443B�е�ATTRIB�������֪PUPI�Ŀ�Ƭ����һ��ʶ���CID.
	 * @param SerialNum �������к�.
	 * @return 0x00	���óɹ�.<br/>
	 * 0x01	����ʧ��.
	 */
	public static int SelectType_B(byte[] SerialNum) {
		Pointer SerialNumPtr = getPointer(SerialNum);
		int result = loadCommand("SelectType_B", SerialNumPtr.getPointer());
		getPointerData(SerialNumPtr, SerialNum);
		return result;
	}

	/**
	 * ISO14443B���������ִ�м�������ISO14443B�еģ�REQUEST��ATTRIB���ͨ��һ������ʹ����λ��
	 * @param buffer ���ز�����Ŀ������к�4���ֽ�.
	 * @return 0x00	���óɹ�.<br/>
	 * 0x01	����ʧ��.<br/><br/>
	 * ��������ɹ���<br/>
	 * buffer[0..3] ���ز�����Ŀ������к�4���ֽ�.<br/>
	 * �������ʧ�ܣ�<br/>
	 * buffer[0] �������.
	 */
	public static int Request_AB(byte[] buffer) {
		Pointer bufferPtr = getPointer(buffer);
		int result = loadCommand("Request_AB", bufferPtr.getPointer());
		getPointerData(bufferPtr, buffer);
		return result;
	}

	/**
	 * ISO14443B���ISO14443�����������ͨ���������򿨷�������Ч���������.
	 * @param cmd �����͵�����.
	 * @param cmdSize ���ݳ���.
	 * @param buffer ���յ�����.
	 * @return 0x00	���óɹ�.<br/>
	 * 0x01	����ʧ��.<br/><br/>
	 * �����ɹ�����buffer[0..N] Ϊ�ӿ����ص�����.<br/>
	 * ����ʧ�ܣ���buffer[0] Ϊ�������.
	 */
	public static int API_ISO14443TypeBTransCOSCmd(byte[] cmd, int cmdSize,
			byte[] buffer) {
		Pointer cmdPtr = getPointer(cmd);
		Pointer bufferPtr = getPointer(buffer);
		int result = loadCommand("API_ISO14443TypeBTransCOSCmd",
				cmdPtr.getPointer(), cmdSize, bufferPtr.getPointer());
		getPointerData(bufferPtr, buffer);
		return result;
	}

	/**
	 * ISO15693���������ͨ������ͻ���ڵõ��������������п�Ƭ�����кţ��ܵõ��Ŀ�Ƭ������ģ�����ߵ���������йأ�һ���ܶ�2~6�����з���ͻ��.
	 * @param flag ��ʶ�ֽ�1���ֽ�.
	 * @param afi AFI�������ݳ���.
	 * @param pData ���͵�����.
	 * @param nrOfCard ���صĿ�������.
	 * @param pBuffer ���ص����ݣ�����FLAG��DSFID��8[]n���ֽڵĿ��ţ�.
	 * @return 0x00	���óɹ�.<br/>
	 * 0x01	����ʧ��.<br/><br/>
	 * �����ɹ�:<br/>
	 * nrOfCard ���صĿ���������һ���ֽڣ�.<br/>
	 * pBuffer ���ص����ݣ�����FLAG��DSFID��8*n���ֽڵĿ��ţ�.<br/>
	 * ����ʧ��:<br/>
	 * nrOfCard �������.
	 */
	public static int API_ISO15693_Inventory(byte flag, byte afi, byte[] pData,
			byte[] nrOfCard, byte[] pBuffer) {
		Pointer pDataPtr = getPointer(pData);
		Pointer nrOfCardPtr = getPointer(nrOfCard);
		Pointer pBufferPtr = getPointer(pBuffer);
		int result = loadCommand("API_ISO15693_Inventory", flag, afi,
				pDataPtr.getPointer(), nrOfCardPtr.getPointer(),
				pBufferPtr.getPointer());
		getPointerData(pDataPtr, pData);
		getPointerData(nrOfCardPtr, nrOfCard);
		getPointerData(pBufferPtr, pBuffer);
		return result;
	}

	/**
	 * ISO15693���������ȡ1������������ֵ�����Ҫ��ÿ����İ�ȫλ����FLAGS��Option_flag��Ϊ1����FLAG = 0X42��ÿ������������5���ֽڣ�����1����ʾ��ȫ״̬�ֽں�4���ֽڵĿ����ݣ���ʱ��ÿ������ܶ�12���顣���FLAG = 02����ֻ����4�ֽڵĿ����ݣ���ʱ��ÿ������ܶ�63���顣
	 * @param flags ��־λ��<br/>
	 * 0x02  ����uid��<br/>
	 * 0x22    ��uid��<br/>
	 * 0x42  ����uid����Ҫ����ȫλ��
	 * @param blk_add Ҫ������ʼ��š�
	 * @param num_blk ���������
	 * @param uid UID��Ϣ��
	 * @param buffer ����ֵ��
	 * @return 0x00	���óɹ�.<br/>
	 * 0x01	����ʧ��.<br/><br/>
	 * �����ɹ���<br/>
	 * buffer[0] ���ص�flag��<br/>
	 * buffer[1..N] Data��<br/>
	 * ����ʧ�ܣ�<br/>
	 * buffer[0]Ϊ������롣
	 */
	public static int API_ISO15693Read(byte flags, byte blk_add, byte num_blk,
			byte[] uid, byte[] buffer) {
		Pointer uidPtr = getPointer(uid);
		Pointer bufferPtr = getPointer(buffer);
		int result = loadCommand("API_ISO15693Read", flags, blk_add, num_blk,
				uidPtr.getPointer(), bufferPtr.getPointer());
		getPointerData(uidPtr, uid);
		getPointerData(bufferPtr, buffer);
		return result;
	}

	/**
	 * ISO15693�����һ�������д������ÿ��ֻ��дһ���飩��
	 * @param flags ��־λ��<br/>
	 * 0x02  ����uid��<br/>
	 * 0x22    ��uid��<br/>
	 * 0x42  ����uid����Ҫ����ȫλ��
	 * @param blk_add Ҫд����ʼ��š�
	 * @param num_blk д�Ŀ��������
	 * @param uid UID��Ϣ��
	 * @param data ����ֵ��
	 * @return 0x00	���óɹ�.<br/>
	 * 0x01	����ʧ��.<br/><br/>
	 * ����ʧ�ܣ���data[0]Ϊ�������
	 */
	public static int API_ISO15693Write(byte flags, byte blk_add, byte num_blk,
			byte[] uid, byte[] data) {
		Pointer uidPtr = getPointer(uid);
		Pointer dataPtr = getPointer(data);
		int result = loadCommand("API_ISO15693Write", flags, blk_add, num_blk,
				uidPtr.getPointer(), dataPtr.getPointer());
		getPointerData(uidPtr, uid);
		getPointerData(dataPtr, data);
		return result;
	}

	/**
	 * ISO15693����������������ݡ�ע�⣺�˹��̲����棨���ܽ����������������ݲ������޸ġ�
	 * @param flags ��־λ��<br/>
	 * 0x02  ����uid��<br/>
	 * 0x22    ��uid��<br/>
	 * 0x42  ����uid����Ҫ����ȫλ��
	 * @param num_blk Ҫ���Ŀ�š�
	 * @param uid UID��Ϣ��
	 * @param buffer ����ֵ��
	 * @return 0x00	���óɹ�.<br/>
	 * 0x01	����ʧ��.<br/><br/>
	 * �����ɹ�����buffer[0]ֵΪ0x80�� <br/>
	 * ����ʧ�ܣ���buffer[0]Ϊ������롣
	 */
	public static int API_ISO15693Lock(byte flags, byte num_blk, byte[] uid,
			byte[] buffer) {
		Pointer uidPtr = getPointer(uid);
		Pointer bufferPtr = getPointer(buffer);
		int result = loadCommand("API_ISO15693Lock", flags, num_blk,
				uidPtr.getPointer(), bufferPtr.getPointer());
		getPointerData(uidPtr, uid);
		getPointerData(bufferPtr, buffer);
		return result;
	}

	/**
	 * ISO15693������������ڽ������ھ�ֹ��״̬, �����õ�ַģʽ��������͵������뱻�����Ŀ������к���ͬ�������ɹ��󣬿������뾲ֹ״̬������״̬���䡣
	 * @param flags ��־λ��<br/>
	 * 0x02  ����uid��<br/>
	 * 0x22    ��uid��<br/>
	 * 0x42  ����uid����Ҫ����ȫλ��
	 * @param uid UID��Ϣ��
	 * @param buffer ����ֵ��
	 * @return 0x00	���óɹ�.<br/>
	 * 0x01	����ʧ��.<br/><br/>
	 * �����ɹ�����buffer[0]ֵΪ0x80�� <br/>
	 * ����ʧ�ܣ���buffer[0]Ϊ������롣
	 */
	public static int API_ISO15693StayQuiet(byte flags, byte[] uid,
			byte[] buffer) {
		Pointer uidPtr = getPointer(uid);
		Pointer bufferPtr = getPointer(buffer);
		int result = loadCommand("API_ISO15693StayQuiet", flags,
				uidPtr.getPointer(), bufferPtr.getPointer());
		getPointerData(uidPtr, uid);
		getPointerData(bufferPtr, buffer);
		return result;
	}

	/**
	 * ISO15693�������������õ�ַģʽ��������͵������뱻�����Ŀ������к���ͬ�������ɹ��󣬿������뱻ѡ��״̬������״̬���䡣
	 * @param flags ��־λ��<br/>
	 * 0x02  ����uid��<br/>
	 * 0x22    ��uid��<br/>
	 * 0x42  ����uid����Ҫ����ȫλ��
	 * @param uid UID��Ϣ��
	 * @param buffer ����ֵ��
	 * @return 0x00	���óɹ�.<br/>
	 * 0x01	����ʧ��.<br/><br/>
	 * �����ɹ�����buffer[0]ֵΪ0x80�� <br/>
	 * ����ʧ�ܣ���buffer[0]Ϊ������롣
	 */
	public static int API_ISO15693Select(byte flags, byte[] uid, byte[] buffer) {
		Pointer uidPtr = getPointer(uid);
		Pointer bufferPtr = getPointer(buffer);
		int result = loadCommand("API_ISO15693Select", flags,
				uidPtr.getPointer(), bufferPtr.getPointer());
		getPointerData(uidPtr, uid);
		getPointerData(bufferPtr, buffer);
		return result;
	}

	/**
	 * ISO15693��������ɹ��󣬿��ص�Ready״̬��
	 * @param flags ��־λ��<br/>
	 * 0x02  ����uid��<br/>
	 * 0x22    ��uid��<br/>
	 * 0x42  ����uid����Ҫ����ȫλ��
	 * @param uid UID��Ϣ��
	 * @param buffer ����ֵ��
	 * @return 0x00	���óɹ�.<br/>
	 * 0x01	����ʧ��.<br/><br/>
	 * �����ɹ�����buffer[0]ֵΪ0x80�� <br/>
	 * ����ʧ�ܣ���buffer[0]Ϊ�������
	 */
	public static int API_ResetToReady(byte flags, byte[] uid, byte[] buffer) {
		Pointer uidPtr = getPointer(uid);
		Pointer bufferPtr = getPointer(buffer);
		int result = loadCommand("API_ResetToReady", flags,
				uidPtr.getPointer(), bufferPtr.getPointer());
		getPointerData(uidPtr, uid);
		getPointerData(bufferPtr, buffer);
		return result;
	}

	/**
	 * ISO15693����Կ�����дAFI������
	 * @param flags ��־λ��<br/>
	 * 0x02  ����uid��<br/>
	 * 0x22    ��uid��<br/>
	 * 0x42  ����uid����Ҫ����ȫλ��
	 * @param afi ��д��AFI.
	 * @param uid UID��Ϣ��
	 * @param buffer ����ֵ��
	 * @return 0x00	���óɹ�.<br/>
	 * 0x01	����ʧ��.<br/><br/>
	 * �����ɹ�����buffer[0]ֵΪ0x80�� <br/>
	 * ����ʧ�ܣ���buffer[0]Ϊ�������
	 */
	public static int API_WriteAFI(byte flags, byte afi, byte[] uid,
			byte[] buffer) {
		Pointer uidPtr = getPointer(uid);
		Pointer bufferPtr = getPointer(buffer);
		int result = loadCommand("API_WriteAFI", flags, afi,
				uidPtr.getPointer(), bufferPtr.getPointer());
		getPointerData(uidPtr, uid);
		getPointerData(bufferPtr, buffer);
		return result;
	}

	/**
	 * ISO15693���������������AFI��������AFI�����Ը��ġ�
	 * @param flags ��־λ��<br/>
	 * 0x02  ����uid��<br/>
	 * 0x22    ��uid��<br/>
	 * 0x42  ����uid����Ҫ����ȫλ��
	 * @param uid UID��Ϣ��
	 * @param buffer ����ֵ��
	 * @return 0x00	���óɹ�.<br/>
	 * 0x01	����ʧ��.<br/><br/>
	 * �����ɹ�����buffer[0]ֵΪ0x80�� <br/>
	 * ����ʧ�ܣ���buffer[0]Ϊ�������
	 */
	public static int API_LockAFI(byte flags, byte[] uid, byte[] buffer) {
		Pointer uidPtr = getPointer(uid);
		Pointer bufferPtr = getPointer(buffer);
		int result = loadCommand("API_LockAFI", flags, uidPtr.getPointer(),
				bufferPtr.getPointer());
		getPointerData(uidPtr, uid);
		getPointerData(bufferPtr, buffer);
		return result;
	}

	/**
	 * ISO15693����Կ��Ľ���дDSFID����.
	 * @param flags ��־λ��<br/>
	 * 0x02  ����uid��<br/>
	 * 0x22    ��uid��<br/>
	 * 0x42  ����uid����Ҫ����ȫλ��
	 * @param DSFID Ҫд��DSFID�ֽڣ�����Ϊ1���ֽ�.
	 * @param uid UID��Ϣ��
	 * @param buffer ����ֵ��
	 * @return 0x00	���óɹ�.<br/>
	 * 0x01	����ʧ��.<br/><br/>
	 * �����ɹ�����buffer[0]ֵΪ0x80�� <br/>
	 * ����ʧ�ܣ���buffer[0]Ϊ�������
	 */
	public static int API_WriteDSFID(byte flags, byte DSFID, byte[] uid,
			byte[] buffer) {
		Pointer uidPtr = getPointer(uid);
		Pointer bufferPtr = getPointer(buffer);
		int result = loadCommand("API_WriteDSFID", flags, DSFID,
				uidPtr.getPointer(), bufferPtr.getPointer());
		getPointerData(uidPtr, uid);
		getPointerData(bufferPtr, buffer);
		return result;
	}

	/**
	 * ISO15693���������������DSFID��������DSFID�����Ը���.
	 * @param flags ��־λ��<br/>
	 * 0x02  ����uid��<br/>
	 * 0x22    ��uid��<br/>
	 * 0x42  ����uid����Ҫ����ȫλ��
	 * @param uid UID��Ϣ��
	 * @param buffer ����ֵ��
	 * @return 0x00	���óɹ�.<br/>
	 * 0x01	����ʧ��.<br/><br/>
	 * �����ɹ�����buffer[0]ֵΪ0x80�� <br/>
	 * ����ʧ�ܣ���buffer[0]Ϊ�������
	 */
	public static int API_LockDSFID(byte flags, byte[] uid, byte[] buffer) {
		Pointer uidPtr = getPointer(uid);
		Pointer bufferPtr = getPointer(buffer);
		int result = loadCommand("API_LockDSFID", flags, uidPtr.getPointer(),
				bufferPtr.getPointer());
		getPointerData(uidPtr, uid);
		getPointerData(bufferPtr, buffer);
		return result;
	}

	/**
	 * ISO15693������ڵõ�������ϸ��Ϣ������������ο���ISO15693Э�����ϡ�
	 * @param flags ��־λ��<br/>
	 * 0x02  ����uid��<br/>
	 * 0x22    ��uid��<br/>
	 * 0x42  ����uid����Ҫ����ȫλ��
	 * @param uid UID��Ϣ��
	 * @param buffer ����ֵ.
	 * @return 0x00	���óɹ�.<br/>
	 * 0x01	����ʧ��.<br/><br/>
	 * �����ɹ���<br/>
	 * Buffer [0]:	Flags<br/>
	 * Buffer [1]:	INFO Flags<br/>
	 * Buffer [2..9]:	UID<br/>
	 * Buffer [10]:	DSFID<br/>
	 * Buffer [11]:	AFI<br/>
	 * Buffer [12..N]: Other fields<br/>
	 * ����ʧ�ܣ���Buffer[0] ���� Ϊ������롣
	 */
	public static int API_ISO15693_GetSysInfo(byte flags, byte[] uid,
			byte[] buffer) {
		Pointer uidPtr = getPointer(uid);
		Pointer bufferPtr = getPointer(buffer);
		int result = loadCommand("API_ISO15693_GetSysInfo", flags,
				uidPtr.getPointer(), bufferPtr.getPointer());
		getPointerData(uidPtr, uid);
		getPointerData(bufferPtr, buffer);
		return result;
	}

	/**
	 * ISO15693������ڻ�ȡ���ĸ�����İ�ȫ״̬λ������
	 * @param flag ��־λ��<br/>
	 * 0x02  ����uid��<br/>
	 * 0x22    ��uid��<br/>
	 * 0x42  ����uid����Ҫ����ȫλ��
	 * @param blkAddr Ҫ������ʼ��š�
	 * @param blkNum ���Ŀ��������
	 * @param uid UID��Ϣ��
	 * @param pBuffer ����ֵ��
	 * @return 0x00	���óɹ�.<br/>
	 * 0x01	����ʧ��.<br/><br/>
	 * �����ɹ���<br/>
	 * pBuffer ���ص����ݡ�<br/>
	 * pBuffer[0]  ���ص�flags��<br/>
	 * pBuffer [1..N]  Block security status (��İ�ȫ״̬)�� <br/>
	 * ����ʧ�ܣ���pBuffer[0]Ϊ������롣
	 */
	public static int API_ISO15693_GetMulSecurity(byte flags, byte blkAddr,
			byte blkNum, byte[] uid, byte[] pBuffer) {
		Pointer uidPtr = getPointer(uid);
		Pointer pBufferPtr = getPointer(pBuffer);
		int result = loadCommand("API_ISO15693_GetMulSecurity", flags, blkAddr,
				blkNum, uidPtr.getPointer(), pBufferPtr.getPointer());
		getPointerData(uidPtr, uid);
		getPointerData(pBufferPtr, pBuffer);
		return result;
	}

	/**
	 * ISO15693���һ��ͨ������û�����ͨ��������Կ����и��ֲ�����
	 * @param cmd ��Ҫ���͵����ݡ�
	 * @param cmdSize ���ݳ��ȡ�
	 * @param buffer ����ֵ��
	 * @return 0x00	���óɹ�.<br/>
	 * 0x01	����ʧ��.<br/><br/>
	 * �����ɹ���<br/>
	 * buffer ���ص����ݡ�<br/>
	 * buffer[0..N] �ӿ����ص����ݡ�<br/>
	 * ����ʧ�ܣ���buffer[0]Ϊ������롣
	 */
	public static int API_ISO15693TransCOSCmd(byte[] cmd, int cmdSize,
			byte[] buffer) {
		Pointer cmdPtr = getPointer(cmd);
		Pointer bufferPtr = getPointer(buffer);
		int result = loadCommand("API_ISO15693TransCOSCmd",
				cmdPtr.getPointer(), cmdSize, bufferPtr.getPointer());
		getPointerData(cmdPtr, cmd);
		getPointerData(bufferPtr, buffer);
		return result;
	}

	/**
	 * Ultralight�������Ultralight�������
	 * @param mode Ѱ��ģʽ ��<br/>
	 * 0x00�CIdleģʽ��<br/>
	 * 0x01�CAllģʽ��
	 * @param blk_add д�������š�
	 * @param snr ����7���ֽڵĿ��š�
	 * @param buffer ����16���ֽڵĿ����ݡ�
	 * @return 0x00	���óɹ�.<br/>
	 * 0x01	����ʧ��.<br/><br/>
	 * ��������ɹ���<br/>
	 * snr 7���ֽڵĿ���.<br/>
	 * buffer 16���ֽڵĿ�����.<br/>
	 * �������ʧ�ܣ�<br/>
	 * buffer[0] ������롣
	 */
	public static int UL_HLRead(byte mode, byte blk_add, byte[] snr,
			byte[] buffer) {
		Pointer snrPtr = getPointer(snr);
		Pointer bufferPtr = getPointer(buffer);
		int result = loadCommand("UL_HLRead", mode, blk_add,
				snrPtr.getPointer(), bufferPtr.getPointer());
		getPointerData(snrPtr, snr);
		getPointerData(bufferPtr, buffer);
		return result;
	}

	/**
	 * Ultralight�������Ultralightд�����
	 * @param mode Ѱ��ģʽ ��<br/>
	 * 0x00�CIdleģʽ��<br/>
	 * 0x01�CAllģʽ��
	 * @param blk_add д�������š�
	 * @param snr ����7���ֽڵĿ��š�
	 * @param buffer �ĸ��ֽڵ�д�뿨���������ݡ�
	 * @return 0x00	���óɹ�.<br/>
	 * 0x01	����ʧ��.<br/><br/>
	 * ��������ɹ���<br/>
	 * snr 7���ֽڵĿ���.<br/>
	 * buffer 16���ֽڵĿ�����.<br/>
	 * �������ʧ�ܣ�<br/>
	 * buffer[0] ������롣
	 */
	public static int UL_HLWrite(byte mode, byte blk_add, byte[] snr,
			byte[] buffer) {
		Pointer snrPtr = getPointer(snr);
		Pointer bufferPtr = getPointer(buffer);
		int result = loadCommand("UL_HLWrite", mode, blk_add,
				snrPtr.getPointer(), bufferPtr.getPointer());
		getPointerData(snrPtr, snr);
		getPointerData(bufferPtr, buffer);
		return result;
	}

	/**
	 * Ultralight�������UltralightѰ��ָ�
	 * @param mode Ѱ��ģʽ ��<br/>
	 * 0x00�CIdleģʽ��<br/>
	 * 0x01�CAllģʽ��
	 * @param snr �����صĲ��� 
	 * @return 0x00	���óɹ�.<br/>
	 * 0x01	����ʧ��.<br/><br/>
	 * �����ɹ���  ��snr [0..6]������7���ֽڵ����ݴ���<br/>
	 * ����ʧ�ܣ�  ��snr [0] Ϊ ������롣
	 */
	public static int UL_Request(byte mode, byte[] snr) {
		Pointer snrPtr = getPointer(snr);
		int result = loadCommand("UL_Request", mode, snrPtr.getPointer());
		getPointerData(snrPtr, snr);
		return result;
	}

}