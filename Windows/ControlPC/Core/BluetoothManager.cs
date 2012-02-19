using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using InTheHand.Net.Sockets;
using System.IO;
using System.Xml.Linq;

namespace ControlPC.Core {
    public static class BluetoothManager {

        private static BluetoothListener _BtListener;
        private static Guid BT_GUID = new Guid("{00001101-0000-1000-8000-00805F9B34FB}");

        public delegate void NewMessageDelegate(string msg);

        private static NewMessageDelegate _MsgDelegate;
        private static bool _Listen;

        public static bool Open(NewMessageDelegate msg) {

            if (_BtListener == null) {
                _BtListener = new BluetoothListener(BT_GUID);
            }
            _BtListener.Stop();
            _BtListener.Start();

            _Listen = true;

            _BtListener.BeginAcceptBluetoothClient(ListenThread, null);

            return true;
        }

        public static void Close() {
            _Listen = false;
        }

        private static void ListenThread(IAsyncResult ar) {

            BluetoothClient conn = _BtListener.EndAcceptBluetoothClient(ar);
            AudioManager am = new AudioManager();

            Thread t = new Thread(new ThreadStart(delegate {
                string data;
                while (_Listen) {
                    Stream peerStream = conn.GetStream();
                    Byte[] buffer = new Byte[255];
                    peerStream.Read(buffer, 0, 255);
                    data = System.Text.ASCIIEncoding.ASCII.GetString(buffer);
                    data = data.Replace('\0', ' ');

                    data = GetLastMessage(data);

                    System.Diagnostics.Debug.WriteLine(data);

                    if (data != string.Empty) {
                        try {
                            XElement msg = XElement.Parse(data);
                            float f = float.Parse(msg.Element("Acc").Element("z").Value) / 10.0F;
                            am.SetVolume(f);
                        }
                        catch {
                        }
                    }
                }
            }));
            t.IsBackground = true;
            t.Start();
        }


        private static string GetLastMessage(string data) {
            string startTag = "<msg>";
            string endTag = "</msg>";

            bool endTagFound = false;
            for (int i = data.Length - 1; i >= 0; i--) {
                bool pass = true;
                for (int j = endTag.Length - 1; j >= 0; j--) {
                    if (data[i - endTag.Length + j] != endTag[j]) {
                        pass = false;
                        break;
                    }
                }
                if (pass) {
                    data = data.Substring(0, i);
                    endTagFound = true;
                    break;
                }
            }

            if (endTagFound) {
                for (int i = data.Length - 1; i >= startTag.Length; i--) {
                    bool pass = true;
                    for (int j = startTag.Length - 1; j >= 0; j--) {
                        if (data[i - startTag.Length + j] != startTag[j]) {
                            pass = false;
                            break;
                        }
                    }
                    if (pass == true) {
                        return data.Substring(i - startTag.Length, data.Length - i + startTag.Length);
                    }
                }
            }

            return string.Empty;
        }
    }
}
