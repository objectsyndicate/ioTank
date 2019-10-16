/*
 * Created by SharpDevelop.
 * User: d3cline
 * Date: 4/29/2017
 * Time: 2:46 PM
 * 
 * To change this template use Tools | Options | Coding | Edit Standard Headers.
 */
namespace ioFake
{
	partial class MainForm
	{
		/// <summary>
		/// Designer variable used to keep track of non-visual components.
		/// </summary>
		private System.ComponentModel.IContainer components = null;
		private System.Windows.Forms.Button send;
		private System.Windows.Forms.Label response;
		private System.Windows.Forms.TextBox host;
		private System.Windows.Forms.Label hostLabel;
		private System.Windows.Forms.Label endpointLabel;
		private System.Windows.Forms.TextBox endpoint;
		private System.Windows.Forms.Label tokenLabel;
		private System.Windows.Forms.TextBox token;
		private System.Windows.Forms.Label t1Label;
		private System.Windows.Forms.NumericUpDown t1;
		private System.Windows.Forms.Label label5;
		private System.Windows.Forms.NumericUpDown t2;
		private System.Windows.Forms.Label label6;
		private System.Windows.Forms.Label label7;
		private System.Windows.Forms.Label label8;
		private System.Windows.Forms.NumericUpDown h;
		private System.Windows.Forms.NumericUpDown uv;
		private System.Windows.Forms.NumericUpDown l;
		
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
			this.send = new System.Windows.Forms.Button();
			this.response = new System.Windows.Forms.Label();
			this.host = new System.Windows.Forms.TextBox();
			this.hostLabel = new System.Windows.Forms.Label();
			this.endpointLabel = new System.Windows.Forms.Label();
			this.endpoint = new System.Windows.Forms.TextBox();
			this.tokenLabel = new System.Windows.Forms.Label();
			this.token = new System.Windows.Forms.TextBox();
			this.t1Label = new System.Windows.Forms.Label();
			this.label5 = new System.Windows.Forms.Label();
			this.label6 = new System.Windows.Forms.Label();
			this.label7 = new System.Windows.Forms.Label();
			this.label8 = new System.Windows.Forms.Label();
			this.t1 = new System.Windows.Forms.NumericUpDown();
			this.t2 = new System.Windows.Forms.NumericUpDown();
			this.h = new System.Windows.Forms.NumericUpDown();
			this.uv = new System.Windows.Forms.NumericUpDown();
			this.l = new System.Windows.Forms.NumericUpDown();
			((System.ComponentModel.ISupportInitialize)(this.t1)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.t2)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.h)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.uv)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.l)).BeginInit();
			this.SuspendLayout();
			// 
			// send
			// 
			this.send.Location = new System.Drawing.Point(12, 244);
			this.send.Name = "send";
			this.send.Size = new System.Drawing.Size(260, 36);
			this.send.TabIndex = 0;
			this.send.Text = "Send";
			this.send.UseVisualStyleBackColor = true;
			this.send.Click += new System.EventHandler(this.Button1Click);
			// 
			// response
			// 
			this.response.Location = new System.Drawing.Point(12, 218);
			this.response.Name = "response";
			this.response.Size = new System.Drawing.Size(260, 23);
			this.response.TabIndex = 1;
			// 
			// host
			// 
			this.host.Location = new System.Drawing.Point(129, 12);
			this.host.Name = "host";
			this.host.Size = new System.Drawing.Size(143, 20);
			this.host.TabIndex = 2;
			this.host.Text = "objectsyndicate.com";
			this.host.TextChanged += new System.EventHandler(this.HostTextChanged);
			this.host.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.host_KeyPress);
			// 
			// hostLabel
			// 
			this.hostLabel.Location = new System.Drawing.Point(12, 13);
			this.hostLabel.Name = "hostLabel";
			this.hostLabel.Size = new System.Drawing.Size(100, 23);
			this.hostLabel.TabIndex = 3;
			this.hostLabel.Text = "Host";
			// 
			// endpointLabel
			// 
			this.endpointLabel.Location = new System.Drawing.Point(12, 39);
			this.endpointLabel.Name = "endpointLabel";
			this.endpointLabel.Size = new System.Drawing.Size(100, 23);
			this.endpointLabel.TabIndex = 5;
			this.endpointLabel.Text = "Endpoint";
			// 
			// endpoint
			// 
			this.endpoint.Location = new System.Drawing.Point(129, 38);
			this.endpoint.Name = "endpoint";
			this.endpoint.Size = new System.Drawing.Size(143, 20);
			this.endpoint.TabIndex = 4;
			this.endpoint.Text = "/api/v1/post/";
			this.endpoint.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.endpoint_KeyPress);
			// 
			// tokenLabel
			// 
			this.tokenLabel.Location = new System.Drawing.Point(12, 65);
			this.tokenLabel.Name = "tokenLabel";
			this.tokenLabel.Size = new System.Drawing.Size(100, 23);
			this.tokenLabel.TabIndex = 7;
			this.tokenLabel.Text = "Token";
			// 
			// token
			// 
			this.token.Location = new System.Drawing.Point(129, 64);
			this.token.MaxLength = 256;
			this.token.Name = "token";
			this.token.Size = new System.Drawing.Size(143, 20);
			this.token.TabIndex = 6;
			this.token.Text = "TOKEN";
			this.token.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.token_KeyPress);
			// 
			// t1Label
			// 
			this.t1Label.Location = new System.Drawing.Point(12, 91);
			this.t1Label.Name = "t1Label";
			this.t1Label.Size = new System.Drawing.Size(100, 23);
			this.t1Label.TabIndex = 9;
			this.t1Label.Text = "T1";
			// 
			// label5
			// 
			this.label5.Location = new System.Drawing.Point(12, 117);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(100, 23);
			this.label5.TabIndex = 11;
			this.label5.Text = "T2";
			// 
			// label6
			// 
			this.label6.Location = new System.Drawing.Point(12, 143);
			this.label6.Name = "label6";
			this.label6.Size = new System.Drawing.Size(100, 23);
			this.label6.TabIndex = 13;
			this.label6.Text = "Humidity";
			// 
			// label7
			// 
			this.label7.Location = new System.Drawing.Point(12, 169);
			this.label7.Name = "label7";
			this.label7.Size = new System.Drawing.Size(100, 23);
			this.label7.TabIndex = 15;
			this.label7.Text = "UV Index";
			// 
			// label8
			// 
			this.label8.Location = new System.Drawing.Point(12, 195);
			this.label8.Name = "label8";
			this.label8.Size = new System.Drawing.Size(100, 23);
			this.label8.TabIndex = 17;
			this.label8.Text = "Lux";
			// 
			// t1
			// 
			this.t1.DecimalPlaces = 1;
			this.t1.Location = new System.Drawing.Point(129, 91);
			this.t1.Maximum = new decimal(new int[] {
			120,
			0,
			0,
			0});
			this.t1.Name = "t1";
			this.t1.Size = new System.Drawing.Size(143, 20);
			this.t1.TabIndex = 18;
			// 
			// t2
			// 
			this.t2.Location = new System.Drawing.Point(129, 117);
			this.t2.Maximum = new decimal(new int[] {
			120,
			0,
			0,
			0});
			this.t2.Name = "t2";
			this.t2.Size = new System.Drawing.Size(143, 20);
			this.t2.TabIndex = 19;
			// 
			// h
			// 
			this.h.Location = new System.Drawing.Point(129, 143);
			this.h.Name = "h";
			this.h.Size = new System.Drawing.Size(143, 20);
			this.h.TabIndex = 20;
			// 
			// uv
			// 
			this.uv.DecimalPlaces = 2;
			this.uv.Location = new System.Drawing.Point(129, 167);
			this.uv.Maximum = new decimal(new int[] {
			15,
			0,
			0,
			0});
			this.uv.Name = "uv";
			this.uv.Size = new System.Drawing.Size(143, 20);
			this.uv.TabIndex = 21;
			// 
			// l
			// 
			this.l.DecimalPlaces = 2;
			this.l.Location = new System.Drawing.Point(129, 193);
			this.l.Maximum = new decimal(new int[] {
			30000,
			0,
			0,
			0});
			this.l.Name = "l";
			this.l.Size = new System.Drawing.Size(143, 20);
			this.l.TabIndex = 22;
			// 
			// MainForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(284, 292);
			this.Controls.Add(this.l);
			this.Controls.Add(this.uv);
			this.Controls.Add(this.h);
			this.Controls.Add(this.t2);
			this.Controls.Add(this.t1);
			this.Controls.Add(this.label8);
			this.Controls.Add(this.label7);
			this.Controls.Add(this.label6);
			this.Controls.Add(this.label5);
			this.Controls.Add(this.t1Label);
			this.Controls.Add(this.tokenLabel);
			this.Controls.Add(this.token);
			this.Controls.Add(this.endpointLabel);
			this.Controls.Add(this.endpoint);
			this.Controls.Add(this.hostLabel);
			this.Controls.Add(this.host);
			this.Controls.Add(this.response);
			this.Controls.Add(this.send);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.MaximizeBox = false;
			this.Name = "MainForm";
			this.Text = "ioTank Emulator";
			((System.ComponentModel.ISupportInitialize)(this.t1)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.t2)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.h)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.uv)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.l)).EndInit();
			this.ResumeLayout(false);
			this.PerformLayout();

		}
	}
}
