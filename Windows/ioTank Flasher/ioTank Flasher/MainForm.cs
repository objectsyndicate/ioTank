/*
 * Created by John Spounias (d3cline) @@ OBJECT SYNDICATE LLC
 */
using System;
using System.ComponentModel;
using System.Diagnostics;
using System.IO;
using System.IO.Ports;
using System.Linq;
using System.Reflection;
using System.Text.RegularExpressions;
using System.Windows.Forms;
using System.Threading;

namespace ioTank_Flasher
{
	public partial class MainForm : Form
	{
		public MainForm()
		{
			// The InitializeComponent() call is required for Windows Forms designer support.
			InitializeComponent();
            this.Load += MainForm_Load;
		}

    void MainForm_Load(object sender, EventArgs e)
    {
    	string executableLocation = Path.GetDirectoryName(
        Assembly.GetExecutingAssembly().Location);
        var ports = SerialPort.GetPortNames();
        serial.DataSource = ports;
	    serial.SelectedIndex = ports.Count() -1;
        var settings_f = File.ReadAllLines(executableLocation +"\\data\\f");
        int count = 0;
		foreach (var element in settings_f)
        {	
			count += 1;
			if(count == 1){ssid.Text = element;}
			if(count == 2){password.Text = element;}
			if(count == 3){Cloud.Checked = Boolean.Parse(element);}
			if(count == 4){token.Text = element;}
			if(count == 7){Radio.Checked = Boolean.Parse(element);}


		}
	
	InitTimer();
		
		


		
   }// on create end

    
      

    
		void FlashClick(object sender, EventArgs e)
		{
	
			

    
			string executableLocation = Path.GetDirectoryName(
            Assembly.GetExecutingAssembly().Location);
			using (var objWriter = new StreamWriter(executableLocation +"\\data\\f"))
			{
		    objWriter.Write(ssid.Text+Environment.NewLine);
		    objWriter.Write(password.Text+Environment.NewLine);
		    objWriter.Write(Cloud.Checked+Environment.NewLine);
		    objWriter.Write(token.Text+Environment.NewLine);
		    objWriter.Write("objectsyndicate.com"+Environment.NewLine);
			objWriter.Write("/api/v1/post/"+Environment.NewLine);
			objWriter.Write(Radio.Checked+Environment.NewLine);
			}
					

			string strMkspiffsCmd = executableLocation +"\\mkspiffs.exe";
			string strEsptoolCmd = executableLocation + "\\esptool.exe";

			var mkspiffs = new Process();
			mkspiffs.StartInfo.FileName = strMkspiffsCmd;
			mkspiffs.StartInfo.Arguments = " -c data -p 256 -b 8192 out.bin";
			mkspiffs.Start();
			System.Threading.Thread.Sleep(500);

			var port = new SerialPort(serial.Text, 115200, Parity.None, 8, StopBits.One);
			
			

			port.Close();
			var esptool = new Process();
			esptool.StartInfo.FileName = strEsptoolCmd;
			esptool.StartInfo.Arguments = "  -cd nodemcu -cb 115200 -cp "+serial.Text+" -ca 0x300000 -cf out.bin";
			timer1.Stop();
			esptool.Start();
			port.Close();
			timer1.Start();
								
		}

	
		
		static Regex isGuid = new Regex(@"[0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12}", RegexOptions.Compiled);
		static Regex isIP = new Regex(@"\b\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}\b");

		bool IsGuid(string candidate)
		{
		   if (candidate != null)
		   {
		   	//Debug.Write(candidate);
		      if (isGuid.IsMatch(candidate))
		      {
		         return true;
		      }
		   }
		
		   return false;
		}
		
		bool IsIP(string candidate)
		{
		   if (candidate != null)
		   {
		   	//Debug.Write(candidate);
		      if (isIP.IsMatch(candidate))
		      {
		         return true;
		      }
		   }
		
		   return false;
		}	
	
	
		void TokenTooltip(object sender, EventArgs e){
         // Create the ToolTip and associate with the Form container.
         ToolTip toolTip1 = new ToolTip();

         // Set up the delays for the ToolTip.
         toolTip1.AutoPopDelay = 0;
         toolTip1.InitialDelay = 0;
         toolTip1.ReshowDelay = 500;
         // Force the ToolTip text to be displayed whether or not the form is active.
         toolTip1.ShowAlways = true;

         // Set up the ToolTip text for the Button and Checkbox.
         toolTip1.SetToolTip(token, "Tokens are issued at objectsyndicate.com/devices");
         //toolTip1.SetToolTip(this.checkBox1, "My checkBox1");
			
		}



		void SSIDTooltip(object sender, EventArgs e){
         // Create the ToolTip and associate with the Form container.
         ToolTip toolTip1 = new ToolTip();

         // Set up the delays for the ToolTip.
         toolTip1.AutoPopDelay = 0;
         toolTip1.InitialDelay = 0;
         toolTip1.ReshowDelay = 500;
         // Force the ToolTip text to be displayed whether or not the form is active.
         toolTip1.ShowAlways = true;

         // Set up the ToolTip text for the Button and Checkbox.
         toolTip1.SetToolTip(ssid, "Device SSID is case sensitve");
         //toolTip1.SetToolTip(this.checkBox1, "My checkBox1");
			
		}
		
		
		void PwdTooltip(object sender, EventArgs e){
         // Create the ToolTip and associate with the Form container.
         ToolTip toolTip1 = new ToolTip();

         // Set up the delays for the ToolTip.
         toolTip1.AutoPopDelay = 0;
         toolTip1.InitialDelay = 0;
         toolTip1.ReshowDelay = 500;
         // Force the ToolTip text to be displayed whether or not the form is active.
         toolTip1.ShowAlways = true;

         // Set up the ToolTip text for the Button and Checkbox.
         toolTip1.SetToolTip(password, "WPA security required");
         //toolTip1.SetToolTip(this.checkBox1, "My checkBox1");
			
		}
	
		
		
		 void RadioTooltip(object sender, EventArgs e){
         // Create the ToolTip and associate with the Form container.
         ToolTip toolTip1 = new ToolTip();

         // Set up the delays for the ToolTip.
         toolTip1.AutoPopDelay = 0;
         toolTip1.InitialDelay = 0;
         toolTip1.ReshowDelay = 500;
         // Force the ToolTip text to be displayed whether or not the form is active.
         toolTip1.ShowAlways = true;

         // Set up the ToolTip text for the Button and Checkbox.
         toolTip1.SetToolTip(Radio, "Enable Wifi Radio (USB only if disabled)");
         //toolTip1.SetToolTip(this.checkBox1, "My checkBox1");
		}
		
		 void CloudTooltip(object sender, EventArgs e){
         // Create the ToolTip and associate with the Form container.
         ToolTip toolTip1 = new ToolTip();

         // Set up the delays for the ToolTip.
         toolTip1.AutoPopDelay = 0;
         toolTip1.InitialDelay = 0;
         toolTip1.ReshowDelay = 500;
         // Force the ToolTip text to be displayed whether or not the form is active.
         toolTip1.ShowAlways = true;

         // Set up the ToolTip text for the Button and Checkbox.
         toolTip1.SetToolTip(Cloud, "Send data to the cloud");
         //toolTip1.SetToolTip(this.checkBox1, "My checkBox1");
		
		}

		
		void MainFormLoad(object sender, EventArgs e)
		{
	
		}
		void Button1Click(object sender, EventArgs e)
		{
			Process.Start("https://www.objectsyndicate.com/docs/");

		}
		
		
			

		
		
		
	
System.Windows.Forms.Timer timer1; 
public void InitTimer()
{
    timer1 = new System.Windows.Forms.Timer();
    timer1.Tick += new EventHandler(timer1_Tick);
    timer1.Interval = 2000; // in miliseconds
    timer1.Start();
}



Thread thread;
void timer1_Tick(object sender, EventArgs e){
	thread = new Thread(new ThreadStart(Thread1));
	
	thread.Start();

}
	
void Thread1(){

	string val = "";
	serial.Invoke(new Action(() => {  val=serial.Text; }));
	try{
		var port = new SerialPort(val, 115200, Parity.None, 8, StopBits.One);
	
		port.ReadTimeout = 500;
  		try 
        {	
		port.Open();
		string data = port.ReadLine();
		port.Close();


		if (IsIP(data))
		{
			status.Invoke(new Action(() => { status.Text = data; }));
		}
		else{
			port.Open();
			data = port.ReadLine();
			port.Close();

			if (IsIP(data))
			{
				status.Invoke(new Action(() => { status.Text = data; }));
			}
			else{
				port.Open();
				data = port.ReadLine();
				port.Close();

				if (IsIP(data))
				{
					status.Invoke(new Action(() => { status.Text = data; }));
				}
				else{


		try{
		var dataList = data.Split('_');

		var IP = dataList[0];
		var RSSI = dataList[1];
		var T1 = dataList[2];
		var T2 = dataList[3];
		var H = dataList[4];
		var UV = dataList[5];
		var L = dataList[6];		
		
		
		
		//progressBar1.Invoke(new Action(() => { progressBar1.Value = Convert.ToInt32(Convert.ToDouble(T1)); }));
		
		status.Invoke(new Action(() => { status.Text ="IP Address: "+IP; }));
		rssi.Invoke(new Action(() => { rssi.Text ="RSSI: "+RSSI; }));
		t1.Invoke(new Action(() => { t1.Text ="Temp 1: "+T1+"C"; }));
		t2.Invoke(new Action(() => { t2.Text ="Temp 2: "+T2+"C"; }));
		h.Invoke(new Action(() => { h.Text ="Humidity: "+H+"%"; }));
		uvi.Invoke(new Action(() => { uvi.Text ="UV Index: "+UV; }));
		lux.Invoke(new Action(() => { lux.Text ="Lux: "+L; }));
		
		} catch{
		status.Invoke(new Action(() => { status.Text = data; }));
		rssi.Invoke(new Action(() => { rssi.Text ="RSSI: "; }));
		t1.Invoke(new Action(() => { t1.Text ="Temp 1: "; }));
		t2.Invoke(new Action(() => { t2.Text ="Temp 2: "; }));
		h.Invoke(new Action(() => { h.Text ="Humidity: "; }));
		uvi.Invoke(new Action(() => { uvi.Text ="UV Index: "; }));
		lux.Invoke(new Action(() => { lux.Text ="Lux: "; }));
		
		}
					
					
					
					
					
				}
				
			}
		
		}

		
		

		}

  		catch (TimeoutException ex)
        {
        status.Invoke(new Action(() => { status.Text = "Communication error"; }));
		rssi.Invoke(new Action(() => { rssi.Text ="RSSI: "; }));
		t1.Invoke(new Action(() => { t1.Text ="Temp 1: "; }));
		t2.Invoke(new Action(() => { t2.Text ="Temp 2: "; }));
		h.Invoke(new Action(() => { h.Text ="Humidity: "; }));
		uvi.Invoke(new Action(() => { uvi.Text ="UV Index: "; }));
		lux.Invoke(new Action(() => { lux.Text ="Lux: "; }));
		
        }
        catch (UnauthorizedAccessException ex)
        {
        status.Invoke(new Action(() => { status.Text = "Communication error"; }));
        rssi.Invoke(new Action(() => { rssi.Text ="RSSI: "; }));
		t1.Invoke(new Action(() => { t1.Text ="Temp 1: "; }));
		t2.Invoke(new Action(() => { t2.Text ="Temp 2: "; }));
		h.Invoke(new Action(() => { h.Text ="Humidity: "; }));
		uvi.Invoke(new Action(() => { uvi.Text ="UV Index: "; }));
		lux.Invoke(new Action(() => { lux.Text ="Lux: "; }));
		
        }
    	
			port.Close();

	}catch{
		if (!ending){
		ending = true;
		MessageBox.Show("No Serial port detected install the included drivers.");
		Environment.Exit(Environment.ExitCode);
		} 
	}		
	} // close
		
bool ending;
		
	void MainForm_Closing(object sender, FormClosingEventArgs e){
	ending = true;	
	try{
	thread.Abort();
	}catch{
	}

}
	
		
		void FactoryClick(object sender, EventArgs e)
		{
timer1.Stop();
			string executableLocation = Path.GetDirectoryName(
            Assembly.GetExecutingAssembly().Location);
			
			string strEsptoolCmd = executableLocation + "\\esptool.exe";
			var port = new SerialPort(serial.Text, 115200, Parity.None, 8, StopBits.One);
			port.Close();
			var firmware = new Process();
			firmware.StartInfo.FileName = strEsptoolCmd;
			firmware.StartInfo.Arguments = " -cd nodemcu -cb 115200 -cp "+serial.Text+" -ca 0x000000 -cf ioTank.bin";
			firmware.Start();
			firmware.WaitForExit();
			System.Threading.Thread.Sleep(5000);
			

			port.Close();
			
			var spiff = new Process();
			spiff.StartInfo.FileName = strEsptoolCmd;
			spiff.StartInfo.Arguments = " -cd nodemcu -cb 115200 -cp "+serial.Text+" -ca 0x300000 -cf ioTank.spiffs.bin";
			spiff.Start();
			
timer1.Start();
			port.Close();
		}
	}
}
