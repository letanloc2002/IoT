using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports;

namespace COM_PORT
{
    public partial class Form1 : Form
    {
        string dataOUT;
        string sendwith;
        string dataIn;
        char led1 = '1', led2 = '1', led3 = '1', led4 = '1', led5 = '1', led6 = '1', led7 = '1', led8 = '1';
        string led_data;
        public Form1()
        {
            InitializeComponent();
        }

        private void prepare_data()
        {
            led_data = String.Concat('~', led1, led2, led3, led4, led5, led6, led7, led8, (char)0x0D);
        }
        private void send_data()
        {
            if (serialPort1.IsOpen)
            {
                prepare_data();
                dataOUT = led_data;
                serialPort1.Write(dataOUT);
            }
                
        }
        private void groupBox1_Enter(object sender, EventArgs e)
        {

        }

        private void bClose_Click(object sender, EventArgs e)
        {
            if (serialPort1.IsOpen)
            {
                serialPort1.Close();
                bOpen.Enabled = true;
                bClose.Enabled = false;
                progressBar1.Value = 0;

            }

        }

        private void Form1_Load(object sender, EventArgs e)
        {
            string[] ports = SerialPort.GetPortNames(); //Gan gia tri cac port co trong may
            cBoxPort.Items.AddRange(ports); //Hien thi len cBoxPort

            cBoxDTR.Checked = false;
            serialPort1.DtrEnable = false;

            cBoxRTS.Checked = false;
            serialPort1.RtsEnable = false;

            checkBox2.Checked = false;
            checkBox1.Checked = false;

            sendwith = "Write";

            checkBoxOldUpdate.Checked = true;
            checkBoxUpdate.Checked = false;
        }

        private void bOpen_Click(object sender, EventArgs e)
        {
            try
            {   //Set up parameter for COM port
                serialPort1.PortName = cBoxPort.Text;
                serialPort1.BaudRate = Convert.ToInt32(cBoxBaudRate.Text);
                serialPort1.DataBits = Convert.ToInt32(cBoxDataBits.Text);
                serialPort1.StopBits = (StopBits)Enum.Parse(typeof(StopBits), cBoxStopBits.Text);
                serialPort1.Parity = (Parity)Enum.Parse(typeof(Parity), cBoxParityBits.Text);

                serialPort1.Open();
                progressBar1.Value = 100;
                bOpen.Enabled = false;
                bClose.Enabled = true;


            }

            catch (Exception err)
            {
                MessageBox.Show(err.Message, "Lỗi rồi bạn :))", MessageBoxButtons.OK, MessageBoxIcon.Error);
                bOpen.Enabled = true;
                bClose.Enabled = false;

            }

        }

        private void bSend_Click(object sender, EventArgs e)
        {          
            if (serialPort1.IsOpen)
            {
                dataOUT = tBoxDataOut.Text;
                if (sendwith == "Write")
                {
                    serialPort1.Write(dataOUT);
                }
                else if (sendwith == "WriteLine")
                {
                    serialPort1.WriteLine(dataOUT);
                }

            }

        }


        private void cBoxDTR_CheckedChanged(object sender, EventArgs e)
        {
            if (cBoxDTR.Checked)
            {
                serialPort1.DtrEnable = true;
            }
            else { serialPort1.DtrEnable = false; }

        }

        private void cBoxRTS_CheckedChanged(object sender, EventArgs e)
        {
            if (cBoxRTS.Checked)
            {
                serialPort1.RtsEnable = true;
            }
            else { serialPort1.RtsEnable = false; }
        }

        private void bClear_Click(object sender, EventArgs e)
        {
            if (tBoxDataOut.Text != "")
            {
                tBoxDataOut.Text = "";
            }
        }

        private void tBoxDataOut_TextChanged(object sender, EventArgs e)
        {
            int dataLength = tBoxDataOut.TextLength;
            lbLengthText.Text = string.Format("{0:00}", dataLength);

        }

        private void LED_0_Click(object sender, EventArgs e)
        {
            led1 = '0';
            LED_0_ON.Enabled = false;
            LED_0_OFF.Enabled = true;
            send_data();
        }

        private void LED_0_OFF_Click(object sender, EventArgs e)
        {
            led1 = '1';
            LED_0_ON.Enabled = true;
            LED_0_OFF.Enabled = false;
            send_data();
        }

        private void LED_1_ON_Click(object sender, EventArgs e)
        {
            led2 = '0';
            LED_1_ON.Enabled = false;
            LED_1_OFF.Enabled = true;
            send_data();
        }

        private void LED_1_OFF_Click(object sender, EventArgs e)
        {
            led2 = '1';
            LED_1_ON.Enabled = true;
            LED_1_OFF.Enabled = false;
            send_data();
        }

        private void LED_2_ON_Click(object sender, EventArgs e)
        {
            led3 = '0';
            LED_2_ON.Enabled = false;
            LED_2_OFF.Enabled = true;
            send_data();
        }

        private void LED_2_OFF_Click(object sender, EventArgs e)
        {
            led3 = '1';
            LED_2_ON.Enabled = true;
            LED_2_OFF.Enabled = false;
            send_data();
        }

        private void LED_3_ON_Click(object sender, EventArgs e)
        {
            led4 = '0';
            LED_3_ON.Enabled = false;
            LED_3_OFF.Enabled = true;
            send_data();
        }

        private void LED_3_OFF_Click(object sender, EventArgs e)
        {
            led4 = '1';
            LED_3_ON.Enabled = true;
            LED_3_OFF.Enabled = false;
            send_data();
        }

        private void LED_4_ON_Click(object sender, EventArgs e)
        {
            led5 = '0';
            LED_4_ON.Enabled = false;
            LED_4_OFF.Enabled = true;
            send_data();
        }

        private void LED_4_OFF_Click(object sender, EventArgs e)
        {
            led5 = '1';
            LED_4_ON.Enabled = true;
            LED_4_OFF.Enabled = false;
            send_data();
        }

        private void LED_5_ON_Click(object sender, EventArgs e)
        {
            led6 = '0';
            LED_5_ON.Enabled = false;
            LED_5_OFF.Enabled = true;
            send_data();
        }

        private void LED_5_OFF_Click(object sender, EventArgs e)
        {
            led6 = '1';
            LED_5_ON.Enabled = true;
            LED_5_OFF.Enabled = false;
            send_data();
        }

        private void LED_6_ON_Click(object sender, EventArgs e)
        {
            led7 = '0';
            LED_6_ON.Enabled = false;
            LED_6_OFF.Enabled = true;
            send_data();
        }

        private void LED_6_OFF_Click(object sender, EventArgs e)
        {
            led7 = '1';
            LED_6_ON.Enabled = true;
            LED_6_OFF.Enabled = false;
            send_data();
        }

        private void LED_7_ON_Click(object sender, EventArgs e)
        {
            led8 = '0';
            LED_7_ON.Enabled = false;
            LED_7_OFF.Enabled = true;
            send_data();
        }

        private void LED_7_OFF_Click(object sender, EventArgs e)
        {
            led8 = '1';
            LED_7_ON.Enabled = true;
            LED_7_OFF.Enabled = false;
            send_data();
        }

        private void checkBox2_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox2.Checked)
            {
                sendwith = "WriteLine";
                checkBox1.Checked = false;
                checkBox2.Checked = true;
            }
        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox1.Checked)
            {
                sendwith = "Write";
                checkBox2.Checked = false;
                checkBox1.Checked = true;
            }
        }

        private void serialPort1_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            dataIn = serialPort1.ReadExisting();
            this.Invoke(new EventHandler(Showdata));
        }

        private void Showdata(object sender, EventArgs e)
        {
            int dataInLengh = dataIn.Length;
            lbLenghIN.Text = string.Format("{0:00}", dataInLengh);
            if (checkBoxUpdate.Checked)
            {
                tBoxDataIn.Text = dataIn;
            }
            else if (checkBoxOldUpdate.Checked)
            {
                tBoxDataIn.Text += dataIn;
            }
        }

        private void checkBoxUpdate_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBoxUpdate.Checked)
            {
                checkBoxUpdate.Checked = true;
                checkBoxOldUpdate.Checked = false;
            }
            
        }

        private void checkBoxOldUpdate_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBoxOldUpdate.Checked)
            {
                checkBoxUpdate.Checked = false;
                checkBoxOldUpdate.Checked = true;
            }
        }

        private void bClearIN_Click(object sender, EventArgs e)
        {
            if (tBoxDataIn.Text != "")
            {
                tBoxDataIn.Text = "";
            }
        }


    }
}
