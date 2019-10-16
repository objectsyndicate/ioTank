/*
 * Created by SharpDevelop.
 * User: d3cline
 * Date: 10/22/2016
 * Time: 4:59 PM
 * 
 * To change this template use Tools | Options | Coding | Edit Standard Headers.
 */
namespace ioTank_Flasher
{
	partial class MainForm
	{
		/// <summary>
		/// Designer variable used to keep track of non-visual components.
		/// </summary>
		private System.ComponentModel.IContainer components = null;
		private System.Windows.Forms.ComboBox serial;
		private System.Windows.Forms.TextBox password;
		private System.Windows.Forms.TextBox ssid;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label label5;
		private System.Windows.Forms.Button flash;
		private System.Windows.Forms.Button help;
		private System.Windows.Forms.CheckBox Cloud;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.TextBox token;
		private System.Windows.Forms.Label status;
		private System.Windows.Forms.Button factory;

		private System.Windows.Forms.CheckBox Radio;
		private System.Windows.Forms.Label rssi;
		private System.Windows.Forms.Label t1;
		private System.Windows.Forms.Label t2;
		private System.Windows.Forms.Label h;
		private System.Windows.Forms.Label uvi;
		private System.Windows.Forms.Label lux;


		
		/// <summary>
		/// Disposes resources used by the form.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing) {
				if (components != null) {
					components.Dispose();
				}
			}
			base.Dispose(disposing);
		}
		
		/// <summary>
		/// This method is required for Windows Forms designer support.
		/// Do not change the method contents inside the source code editor. The Forms designer might
		/// not be able to load this method if it was changed manually.
		/// </summary>
		private void InitializeComponent()
		{
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
			this.serial = new System.Windows.Forms.ComboBox();
			this.password = new System.Windows.Forms.TextBox();
			this.ssid = new System.Windows.Forms.TextBox();
			this.label1 = new System.Windows.Forms.Label();
			this.label2 = new System.Windows.Forms.Label();
			this.label5 = new System.Windows.Forms.Label();
			this.flash = new System.Windows.Forms.Button();
			this.help = new System.Windows.Forms.Button();
			this.Cloud = new System.Windows.Forms.CheckBox();
			this.label4 = new System.Windows.Forms.Label();
			this.token = new System.Windows.Forms.TextBox();
			this.status = new System.Windows.Forms.Label();
			this.factory = new System.Windows.Forms.Button();
			this.Radio = new System.Windows.Forms.CheckBox();
			this.rssi = new System.Windows.Forms.Label();
			this.t1 = new System.Windows.Forms.Label();
			this.t2 = new System.Windows.Forms.Label();
			this.h = new System.Windows.Forms.Label();
			this.uvi = new System.Windows.Forms.Label();
			this.lux = new System.Windows.Forms.Label();
			this.SuspendLayout();
			// 
			// serial
			// 
			this.serial.DisplayMember = "ports";
			this.serial.FormattingEnabled = true;
			this.serial.Location = new System.Drawing.Point(135, 146);
			this.serial.Name = "serial";
			this.serial.Size = new System.Drawing.Size(139, 21);
			this.serial.TabIndex = 5;
			// 
			// password
			// 
			this.password.Location = new System.Drawing.Point(133, 36);
			this.password.Name = "password";
			this.password.Size = new System.Drawing.Size(141, 20);
			this.password.TabIndex = 1;
			// 
			// ssid
			// 
			this.ssid.Location = new System.Drawing.Point(133, 10);
			this.ssid.Name = "ssid";
			this.ssid.Size = new System.Drawing.Size(141, 20);
			this.ssid.TabIndex = 0;
			// 
			// label1
			// 
			this.label1.Location = new System.Drawing.Point(13, 13);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(100, 23);
			this.label1.TabIndex = 5;
			this.label1.Text = "Wi-Fi SSID";
			// 
			// label2
			// 
			this.label2.Location = new System.Drawing.Point(13, 39);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(100, 23);
			this.label2.TabIndex = 6;
			this.label2.Text = "Wi-Fi Password";
			// 
			// label5
			// 
			this.label5.Location = new System.Drawing.Point(14, 149);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(100, 23);
			this.label5.TabIndex = 9;
			this.label5.Text = "Port";
			// 
			// flash
			// 
			this.flash.Location = new System.Drawing.Point(14, 198);
			this.flash.Name = "flash";
			this.flash.Size = new System.Drawing.Size(260, 64);
			this.flash.TabIndex = 8;
			this.flash.Text = "Flash";
			this.flash.UseVisualStyleBackColor = true;
			this.flash.Click += new System.EventHandler(this.FlashClick);
			// 
			// help
			// 
			this.help.Location = new System.Drawing.Point(279, 233);
			this.help.Name = "help";
			this.help.Size = new System.Drawing.Size(138, 29);
			this.help.TabIndex = 7;
			this.help.Text = "Help";
			this.help.UseVisualStyleBackColor = true;
			this.help.Click += new System.EventHandler(this.Button1Click);
			// 
			// Cloud
			// 
			this.Cloud.CheckAlign = System.Drawing.ContentAlignment.MiddleRight;
			this.Cloud.Location = new System.Drawing.Point(12, 85);
			this.Cloud.Name = "Cloud";
			this.Cloud.Size = new System.Drawing.Size(261, 34);
			this.Cloud.TabIndex = 4;
			this.Cloud.Text = "Send data to the cloud?";
			this.Cloud.UseVisualStyleBackColor = true;
			// 
			// label4
			// 
			this.label4.Location = new System.Drawing.Point(14, 65);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(100, 20);
			this.label4.TabIndex = 17;
			this.label4.Text = "Token";
			// 
			// token
			// 
			this.token.Location = new System.Drawing.Point(133, 62);
			this.token.Name = "token";
			this.token.Size = new System.Drawing.Size(140, 20);
			this.token.TabIndex = 3;
			// 
			// status
			// 
			this.status.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.status.Location = new System.Drawing.Point(280, 10);
			this.status.Name = "status";
			this.status.Size = new System.Drawing.Size(164, 26);
			this.status.TabIndex = 22;
			this.status.Text = "IP";
			// 
			// factory
			// 
			this.factory.Location = new System.Drawing.Point(279, 198);
			this.factory.Name = "factory";
			this.factory.Size = new System.Drawing.Size(137, 29);
			this.factory.TabIndex = 23;
			this.factory.Text = "Update Firmware";
			this.factory.UseVisualStyleBackColor = true;
			this.factory.Click += new System.EventHandler(this.FactoryClick);
			// 
			// Radio
			// 
			this.Radio.CheckAlign = System.Drawing.ContentAlignment.MiddleRight;
			this.Radio.Location = new System.Drawing.Point(12, 116);
			this.Radio.Name = "Radio";
			this.Radio.Size = new System.Drawing.Size(261, 24);
			this.Radio.TabIndex = 24;
			this.Radio.Text = "Enable Wifi Radios";
			this.Radio.UseVisualStyleBackColor = true;
			// 
			// rssi
			// 
			this.rssi.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.rssi.Location = new System.Drawing.Point(280, 36);
			this.rssi.Name = "rssi";
			this.rssi.Size = new System.Drawing.Size(164, 26);
			this.rssi.TabIndex = 25;
			this.rssi.Text = "RSSI";
			// 
			// t1
			// 
			this.t1.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.t1.Location = new System.Drawing.Point(280, 62);
			this.t1.Name = "t1";
			this.t1.Size = new System.Drawing.Size(164, 26);
			this.t1.TabIndex = 26;
			this.t1.Text = "T1";
			// 
			// t2
			// 
			this.t2.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.t2.Location = new System.Drawing.Point(280, 85);
			this.t2.Name = "t2";
			this.t2.Size = new System.Drawing.Size(164, 26);
			this.t2.TabIndex = 27;
			this.t2.Text = "T2";
			// 
			// h
			// 
			this.h.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.h.Location = new System.Drawing.Point(280, 111);
			this.h.Name = "h";
			this.h.Size = new System.Drawing.Size(164, 26);
			this.h.TabIndex = 28;
			this.h.Text = "H";
			// 
			// uvi
			// 
			this.uvi.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.uvi.Location = new System.Drawing.Point(280, 137);
			this.uvi.Name = "uvi";
			this.uvi.Size = new System.Drawing.Size(164, 26);
			this.uvi.TabIndex = 29;
			this.uvi.Text = "UVI";
			// 
			// lux
			// 
			this.lux.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.lux.Location = new System.Drawing.Point(280, 163);
			this.lux.Name = "lux";
			this.lux.Size = new System.Drawing.Size(164, 26);
			this.lux.TabIndex = 30;
			this.lux.Text = "Lux";
			// 
			// MainForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(430, 276);
			this.Controls.Add(this.lux);
			this.Controls.Add(this.uvi);
			this.Controls.Add(this.h);
			this.Controls.Add(this.t2);
			this.Controls.Add(this.t1);
			this.Controls.Add(this.rssi);
			this.Controls.Add(this.Radio);
			this.Controls.Add(this.factory);
			this.Controls.Add(this.status);
			this.Controls.Add(this.label4);
			this.Controls.Add(this.token);
			this.Controls.Add(this.Cloud);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.help);
			this.Controls.Add(this.flash);
			this.Controls.Add(this.label1);
			this.Controls.Add(this.ssid);
			this.Controls.Add(this.password);
			this.Controls.Add(this.serial);
			this.Controls.Add(this.label5);
			this.Cursor = System.Windows.Forms.Cursors.Arrow;
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "MainForm";
			this.Text = "ioTank Toolkit";
			this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.MainForm_Closing);
			this.Load += new System.EventHandler(this.MainFormLoad);
			this.ResumeLayout(false);
			this.PerformLayout();

		}
	}
}
