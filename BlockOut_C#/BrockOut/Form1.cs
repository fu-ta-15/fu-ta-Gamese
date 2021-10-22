using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows;   // Vector型を使うため

namespace BrockOut
{
    public partial class Form1 : Form
    {
        
        Vector ballPos;         // ballの位置
        Vector ballSpeed;       // ballのスピード
        int ballRadius;         // ballの半径
        Rectangle paddlePos;    // 

        // ballの設定
        public Form1()
        {
            InitializeComponent();

            // ballの位置
            this.ballPos = new Vector(200, 200);

            // ballのスピード
            this.ballSpeed = new Vector(-2, -4);

            // 半径
            this.ballRadius = 10;

            this.paddlePos = new Rectangle(100, this.Height - 50, 100, 5);

            Timer timer = new Timer();
            timer.Interval = 33;                    // 33秒ごと
            timer.Tick += new EventHandler(Update); // 更新処理の呼び出し
            timer.Start();
        }


        double DotProduct(Vector a, Vector b)
        {
            return a.X * b.X + a.Y * b.Y;   // 内積計算
        }

        private void Update(object sender,EventArgs e)
        {
            // ballの移動
            ballPos += ballSpeed;

            // 左右の壁でのBound
            if(ballPos.X + ballRadius > this.Bounds.Width 
                || ballPos.X -  ballRadius < 0)
            {
                ballSpeed.X *= -1;
            }

            // 上の壁でBound
            if(ballPos.Y - ballRadius < 0)
            {
                ballSpeed.Y *= -1;
            }

            // 再描画
            Invalidate();
        }

        // ballの描画
        private void Draw(Object sender, PaintEventArgs e)
        {
            // ballを描画するためのブラシ
            SolidBrush pinkBrush = new SolidBrush(Color.HotPink);
            SolidBrush grayBrush = new SolidBrush(Color.DimGray);

            // ballのサイズ
            float px = (float)this.ballPos.X - ballRadius;
            float py = (float)this.ballPos.Y - ballRadius;

            // 円の描画・左上の座標・幅と高さの計算
            e.Graphics.FillEllipse(pinkBrush, px, py, this.ballRadius * 2, this.ballRadius * 2);
            e.Graphics.FillRectangle(grayBrush, paddlePos);
        }

        private void KeyPressed(Object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == 'a') // Aキーが押されたとき
            {
                this.paddlePos.X -= 20;
            }
            else if (e.KeyChar == 's') // Sキーが押されたとき
            {
                this.paddlePos.X += 20;
            }
        }
    }
}
