/*
 * Created by SharpDevelop.
 * User: d3cline
 * Date: 4/29/2017
 * Time: 2:46 PM
 * 
 * To change this template use Tools | Options | Coding | Edit Standard Headers.
 */
using System;
using System.Windows.Forms;
using System.Net;
using System.IO;

namespace ioFake
{
	/// <summary>
	/// Description of MainForm.
	/// </summary>
	public partial class MainForm : Form
	{
		public MainForm()
		{
			//
			// The InitializeComponent() call is required for Windows Forms designer support.
			//
			InitializeComponent();
			
			//
			// TODO: Add constructor code after the InitializeComponent() call.
			//
		}
        
		void token_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == ' ') e.Handled = true;
        }
     	
		void host_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == ' ') e.Handled = true;
		}
        void endpoint_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == ' ') e.Handled = true;
        }   
        
		void Button1Click(object sender, EventArgs e)
		{

			
			
        WebClient client = new WebClient ();
        client.Headers.Add ("Authorization", "Token "+token.Text);
        client.Headers.Add ("Content-Type", "application/json");
        string data = "{\"t1\":"+t1.Value+" , \"t2\":"+t2.Value+" , \"h\":"+h.Value+", \"uv\":"+uv.Value+", \"l\":"+l.Value+"}";
        // Upload the data.
        
        try {
    		string reply = client.UploadString ("https://"+host.Text + endpoint.Text,"POST", data);
    		// Disply the server's response.
        	response.Text = reply;
		}
		catch (WebException ex) {
        	MessageBox.Show(ex.Message);
		}
        
        
        
        

		
		
		}
		
		
		
		
		void HostTextChanged(object sender, EventArgs e)
		{
	
		}
		void EndpointKeyPress(object sender, KeyPressEventArgs e)
		{
	
		}
		void HostKeyPress(object sender, KeyPressEventArgs e)
		{
	
		}
	}
}
