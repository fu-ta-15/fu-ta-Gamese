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
        
        Vector ballPos;                 // ballの位置
        Vector ballSpeed;               // ballのスピード
        int ballRadius;                 // ballの半径
        Rectangle paddlePos;            // パドルの位置
        List<Rectangle> blockPos;       // ブロックの位置

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

            // パドルの位置設定
            this.paddlePos = new Rectangle(100, this.Height - 50, 100, 5);

            this.blockPos = new List<Rectangle>();

            for(int x = 0; x <= this.Height; x += 100)
            {
                for(int y = 0; y <= 150; y += 40)
                {
                    this.blockPos.Add(new Rectangle(25 + x, y, 80, 25));
                }
            }

            Timer timer = new Timer();
            timer.Interval = 33;                    // 33秒ごと
            timer.Tick += new EventHandler(Update); // 更新処理の呼び出し
            timer.Start();
        }


        double DotProduct(Vector a, Vector b)
        {
            return a.X * b.X + a.Y * b.Y;   // 内積計算
        }


        bool LineVsCircle(Vector p1, Vector p2, Vector center, float radius)
        {
            Vector lineDir = (p2 - p1);                     // パドルの方向ベクトル
            Vector n = new Vector(lineDir.Y, -lineDir.X);   // パドルの法線
            n.Normalize();

            Vector dir1 = center - p1;
            Vector dir2 = center - p2;

            double dist = Math.Abs(DotProduct(dir1, n));
            double a1 = DotProduct(dir1, lineDir);
            double a2 = DotProduct(dir2, lineDir);

            return (a1 * a2 < 0 && dist < radius) ? true : false;
        }

        int BlockVsCrircle(Rectangle block,Vector ball)
        {
            if (LineVsCircle(new Vector(block.Left, block.Top),
                new Vector(block.Right, block.Top), ball, ballRadius)) 
                return 1;

            if (LineVsCircle(new Vector(block.Left, block.Bottom),
                new Vector(block.Right, block.Bottom), ball, ballRadius))
                return 2;

            if (LineVsCircle(new Vector(block.Right, block.Top),
                new Vector(block.Right, block.Bottom), ball, ballRadius))
                return 3;

            if (LineVsCircle(new Vector(block.Left, block.Top),
                new Vector(block.Left, block.Bottom), ball, ballRadius))
                return 4;

            return -1;
        }

        private void Update(object sender,EventArgs e)
        {
            // ballの移動
            ballPos += ballSpeed;

            // 左右の壁でのBound
            if(ballPos.X + ballRadius > this.Bounds.Width 
                || ballPos.X -  ballRadius*2 < 0)
            {
                ballSpeed.X *= -1;
            }

            // 上の壁でBound
            if(ballPos.Y - ballRadius < 0)
            {
                ballSpeed.Y *= -1;
            }

            // パドルの当たり判定
            if(LineVsCircle(new Vector(this.paddlePos.Left,this.paddlePos.Top),
                new Vector(this.paddlePos.Right,this.paddlePos.Top),
                ballPos,ballRadius))
            {
                ballSpeed.Y *= -1;
            }

            // ブロックとの当たり判定

            for (int nCntBlock = 0; nCntBlock < this.blockPos.Count; nCntBlock++)
            {
                int nCollision = BlockVsCrircle(blockPos[nCntBlock], ballPos);

                if (nCollision == 1 || nCollision == 2)
                {
                    ballSpeed.Y *= -1;
                    this.blockPos.Remove(blockPos[nCntBlock]);
                }
                else if (nCollision == 3 || nCollision == 4)
                {
                    ballSpeed.X *= -1;
                    this.blockPos.Remove(blockPos[nCntBlock]);
                }
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
            SolidBrush blueBrush = new SolidBrush(Color.LightBlue);

            // ballのサイズ
            float px = (float)this.ballPos.X - ballRadius;
            float py = (float)this.ballPos.Y - ballRadius;

            // 円の描画・左上の座標・幅と高さの計算
            e.Graphics.FillEllipse(pinkBrush, px, py, this.ballRadius * 2, this.ballRadius * 2);
            e.Graphics.FillRectangle(grayBrush, paddlePos);

            for (int nCntBlock = 0; nCntBlock < this.blockPos.Count; nCntBlock++)
            {
                e.Graphics.FillRectangle(blueBrush, blockPos[nCntBlock]);
            }
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
