using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WindowsFormsApp3
{
    public partial class Form1 : Form
    {
        Thread t;
        BitArray b;
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            using (OpenFileDialog d = new OpenFileDialog())
            {
                d.Filter = "Бинарный файл|*.bin";
                if (d.ShowDialog() == DialogResult.OK)
                {
                    b = new BitArray(File.ReadAllBytes(d.FileName));
                    progressBar1.Maximum = b.Length;
                    progressBar1.Value = 0;
                    button2.Enabled = true;
                    button3.Enabled = true;
                    textBox1.Text = "";
                    t = new Thread(new ThreadStart(delegate
                    {
                        for (int i = 0; i < b.Length; ++i)
                        {
                            this.Invoke(new ThreadStart(delegate
                            {

                                textBox1.Text += b.Get(i) ? '█' : ' ';
                                ++progressBar1.Value;
                            }));
                        }
                    }
                    ));
                    t.Start();
                }
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (t != null)
            {
                t.Abort();
                button2.Enabled = false;
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
            int[] P = new int[1000];
            for (int i = 0; i < 1000; ++i)
            {
                P[i] = 0;
            }
            int serial = 0;
            for (int i = 0; i < b.Length; ++i)
            {
                if (b.Get(i))
                {
                    ++serial;
                }
                else
                {
                    ++P[serial];
                    serial = 0;
                }
            }
            StreamWriter f = new StreamWriter("out.csv");
            for (int i = 1; i < 1000; ++i)
            {
                f.WriteLine($"{i}; {P[i]}");
            }
        }
    }
}
