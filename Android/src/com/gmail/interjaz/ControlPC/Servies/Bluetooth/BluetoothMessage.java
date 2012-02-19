package com.gmail.interjaz.ControlPC.Servies.Bluetooth;

public class BluetoothMessage {

	private StringBuilder msg;
	
	public void SetAcc(long timestamp, float x, float y, float z) {
		msg.append("<Acc>");
		msg.append("<time>"); msg.append(timestamp); msg.append("</time>");
		msg.append("<x>"); msg.append(x); msg.append("</x>");
		msg.append("<y>"); msg.append(y); msg.append("</y>");
		msg.append("<z>"); msg.append(z); msg.append("</z>");
		msg.append("</Acc>");
	}
	
	public void create() {
		msg = new StringBuilder();	
		msg.append("<msg>");
	}
	
	public void finish() {
		msg.append("</msg>\n");
	}
	
	@Override
	public String toString() {
		return msg.toString();
	}
	
	public byte[] getBytes() {
		return msg.toString().getBytes();
	}
}
