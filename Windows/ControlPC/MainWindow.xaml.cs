using System;
using System.Windows;
using ControlPC.Core;

namespace com.gmail.interjaz.ControlPC
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {



        public MainWindow() {
            InitializeComponent();

            Loaded += new RoutedEventHandler(MainWindow_Loaded);
        }


        void MainWindow_Loaded(object sender, RoutedEventArgs e) {
            BluetoothManager.Open(null);
         
        }


    }
}
