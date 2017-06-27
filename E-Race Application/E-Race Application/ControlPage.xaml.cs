using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Net.Sockets;

namespace E_Race_Application
{
    /// <summary>
    /// Interaction logic for ControlPage.xaml
    /// </summary>
    public partial class ControlPage : Page
    {
        /*Page constructor*/
        public ControlPage()
        {
            InitializeComponent();
        }

        /*Grid and button constructor*/
        private void ButtonGrid_Loaded(object sender, RoutedEventArgs e)
        {
            for (int row = 0; row < 3; row++)
            {
                ButtonGrid.RowDefinitions.Add(new RowDefinition());
            }

            for (int col = 0; col < 3; col++)
            {
                ButtonGrid.ColumnDefinitions.Add(new ColumnDefinition());
            }

            int count = 1;
            for (int row = 0; row < ButtonGrid.RowDefinitions.Count; row++)
            {
                for (int col = 0; col < ButtonGrid.ColumnDefinitions.Count; col++)
                {
                    Button btn = new Button();
                    btn.Content = count.ToString(); count++;
                    btn.Height = 150;

                    btn.HorizontalAlignment = HorizontalAlignment.Stretch;
                    btn.VerticalAlignment = VerticalAlignment.Stretch;

                    btn.SetValue(Grid.ColumnProperty, col);
                    btn.SetValue(Grid.RowProperty, row);
                    btn.Click += new RoutedEventHandler(Grid_Btn);

                    //btnContainer.Add(btn);
                    ButtonGrid.Children.Add(btn);
                }
            }
        }

        /*This method tells the client to close the connection*/
        public void Dis_con_btn(object sender, RoutedEventArgs e)
        {
            if (ISCONNECTED)
            {
                byte[] byteBuffer = Encoding.ASCII.GetBytes("end");
                myClientConnection.Client.Send(byteBuffer);

                myClientConnection.Close();
                IsConnectedBox.Text = "Disconnected";
                ISCONNECTED = false;
            }
        }

        public void Terminate_con(object sender, RoutedEventArgs e)
        {
            if (ISCONNECTED)
            {
                byte[] byteBuffer = Encoding.ASCII.GetBytes("Terminate");
                myClientConnection.Client.Send(byteBuffer);

                myClientConnection.Close();
                IsConnectedBox.Text = "CAN NO LONGER CONNECT TO BOARD";
                IsConnectedBox.IsReadOnly = true;
                ISCONNECTED = false;
            }
        }

        /*This method tells the eraser to move to origin*/
        public void Origin_Button(object sender,RoutedEventArgs e)
        {
            if (ISCONNECTED)
            {
                byte[] bytebuffer = Encoding.UTF8.GetBytes("Orig=/");
                myClientConnection.Client.Send(bytebuffer);

                int RbufSize = 100;
                byte[] RecieveBuffer = new byte[RbufSize];
                myClientConnection.Client.Receive(RecieveBuffer);

                String server_message = Encoding.UTF8.GetString(RecieveBuffer);
                Server_message_handler(server_message);
            }
        }

        /*Defines the Erase all button*/
        public void Erase_Button(object sender, RoutedEventArgs e)
        {
            if (ISCONNECTED)
            {
                byte[] bytebuffer = Encoding.UTF8.GetBytes("Cord=/1/2/3/4/5/6/7/8/9");
                myClientConnection.Client.Send(bytebuffer);

                int RbufSize = 100;
                byte[] RecieveBuffer = new byte[RbufSize];
                myClientConnection.Client.Receive(RecieveBuffer);

                String server_message = Encoding.UTF8.GetString(RecieveBuffer);
                Server_message_handler(server_message);
            }
        }

        /* Defines the send position input button */
        public void Send_Position(object sender, RoutedEventArgs e)
        {
            if (ISCONNECTED)
            {
                string pos = "Posi=/";
                pos = pos + X_pos_box.Text + "." + Y_pos_box.Text;

                byte[] bytebuffer = Encoding.UTF8.GetBytes(pos);
                myClientConnection.Client.Send(bytebuffer);

                int RbufSize = 100;
                byte[] RecieveBuffer = new byte[RbufSize];
                myClientConnection.Client.Receive(RecieveBuffer);

                String server_message = Encoding.UTF8.GetString(RecieveBuffer);
                Server_message_handler(server_message);
            }
        }

        public void Write_To_Board(object sender, RoutedEventArgs e)
        {
            WriteLetters = "Writ=/" + Write_Box.Text;
            if (ISCONNECTED)
            {
                byte[] byteBuffer = Encoding.UTF8.GetBytes(WriteLetters);

                myClientConnection.Client.Send(byteBuffer);
                int RbufSize = myClientConnection.Client.ReceiveBufferSize;
                byte[] RecieveBuffer = new byte[RbufSize];
                myClientConnection.Client.Receive(RecieveBuffer);
                String server_message = Encoding.UTF8.GetString(RecieveBuffer);
                Server_message_handler(server_message);
            }
        }

        public void Clear_Write_Box(object sender, RoutedEventArgs e)
        {
            Write_Box.Text = "Enter Statement Here";
        }

        /*This method tells the client to connect to the specified address*/
        public void Connect_to(object sender, RoutedEventArgs e)
        {
            Address = AddressBox.Text;
            Port = Convert.ToInt32(PortBox.Text);
            //myClientConnection = new TcpClient();
            myClientConnection.Connect(Address, Port);
            
            if (myClientConnection.Connected)
                   IsConnectedBox.Text = "Connected";
            ISCONNECTED = true;
        }


        /*This method clears the coordinates text box*/
        public void Clear_Cord(object sender, RoutedEventArgs e)
        {
            SCord_Box.Text = "Coordinates:";
            Coordinate = "";
        }

        /*This Method sends the contents of the coordinates textbox to Eraser */
        public void Send_Cord(object sender, RoutedEventArgs e)
        {
            if (ISCONNECTED)
            {
                byte[] byteBuffer = Encoding.UTF8.GetBytes(Coordinate);
                
                myClientConnection.Client.Send(byteBuffer);

                int RbufSize = 1000;
                byte[] RecieveBuffer = new byte[RbufSize];
                myClientConnection.Client.Receive(RecieveBuffer);

                String server_message = Encoding.UTF8.GetString(RecieveBuffer);
                Server_message_handler(server_message);
            }
        }

        /* Constructs the coordinates Text box */
        private void Grid_Btn(object sender, RoutedEventArgs e)
        {
            if (Coordinate == "")
            {
                Coordinate = Coordinate + "Cord=/";
            }
            string buttonText = ((Button)sender).Content.ToString();

            switch (buttonText)
            {
                case "1":
                    Coordinate = Coordinate + "1/";
                    break;
                case "2":
                    Coordinate = Coordinate + "2/";
                    break;
                case "3":
                    Coordinate = Coordinate + "3/";
                    break;
                case "4":
                    Coordinate = Coordinate + "4/";
                    break;
                case "5":
                    Coordinate = Coordinate + "5/";
                    break;
                case "6":
                    Coordinate = Coordinate + "6/";
                    break;
                case "7":
                    Coordinate = Coordinate + "7/";
                    break;
                case "8":
                    Coordinate = Coordinate + "8/";
                    break;
                case "9":
                    Coordinate = Coordinate + "9/";
                    break;
            }

            SCord_Box.Text = "Coordinates:" + Coordinate;
        }



        /* Out puts the recieved message into the server messages textbox */
        private void Server_message_handler(String Message)
        {
            Server_Messages.Text += "\r\n";
            Server_Messages.Text += Message;
        }

        private bool ISCONNECTED = false;
        private string Coordinate="";
        private string WriteLetters = "";
        private string Address;
        private Int32 Port;
        private TcpClient myClientConnection = new TcpClient(); //= new TcpClient();
    }
}
