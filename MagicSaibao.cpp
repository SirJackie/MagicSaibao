#include <stdio.h>    // 包含标准输入输出库，用于打印信息
#include <math.h>     // 包含数学库，用于数学计算
#include <stdlib.h>   // 包含标准库，用于内存分配和随机数生成
#include <time.h>     // 包含时间库，用于获取当前时间
#include "Magic.h"    // 包含用户自定义的魔法库，可能包含图形和绘制功能

// randFloat函数生成一个范围在-1到1之间的浮动随机数
float randFloat() {
    // rand() 返回一个介于 0 和 RAND_MAX 之间的整数
    float ZeroToOne = (float)rand() * (1.0f / RAND_MAX);
    // 将 0 到 1 的范围转换到 -1 到 1
    return (ZeroToOne - 0.5f) * 2.0f;
}

// sparse函数会返回原始值加上一个随机的浮动值
float sparse(float x, float sparser) {
    return x + randFloat() * sparser;  // 随机扰动原始值 x
}

int main() {
    // 使用当前时间作为随机数种子，确保每次运行的随机数不同
    srand((unsigned int)time(NULL));

    // 调用 Magic 函数，可能是初始化或绘制背景
    Magic();

    // 定义不同的图像对象，表示游戏的不同元素
    Picture saibao, gameOver, paddle, background;
    
    // 加载游戏所需的图片文件
    saibao.Load(".\\BMP\\01-140px-PinkBottom.bmp");
    gameOver.Load(".\\BMP\\GameOver.bmp");
    paddle.Load(".\\BMP\\Paddle.bmp");
    background.Load(".\\BMP\\Background.bmp");

    // 设置球的移动速度和初始位置
    int speed = 7;
    int saiX = 300, saiY = 100;
    int dtX = speed, dtY = -speed;  // 球在 X 和 Y 轴的移动增量
    int acceY = 1;  // Y轴的加速度（模拟重力）

    bool isGameOver = false;  // 游戏是否结束
    int paddleHeight = 600 - paddle.height - 20;  // 挡板的高度
    int paddleCenter;  // 挡板的中心位置

	// 游戏主循环
    while (true) {
        // 绘制背景
        background.Draw(0, 0);

        // 绘制赛宝
        saibao.Draw(saiX, saiY);

        // 移动赛宝
        saiX += dtX;  // 更新X轴位置
        saiY += dtY;  // 更新Y轴位置
        dtY += acceY;  // 更新Y轴速度（模拟加速度）

        // 边界反弹检测
        if (saiX <= 0 || saiX >= 800 - saibao.width) {
            dtX = -dtX;  // X轴反向
        }
        if (saiY <= 0) {
            dtY = -dtY;  // Y轴反向
        }

        // 挡板控制（根据鼠标位置）
        if (!isGameOver) {
            // 只有在游戏未结束时才更新挡板位置
            paddleCenter = mouseX;  // 假设 mouseX 是当前鼠标的X坐标
        }
        paddle.Draw(paddleCenter - paddle.width / 2, paddleHeight);  // 绘制挡板

        // 检测球与挡板碰撞，进行反弹处理
        if ((saiY + saibao.height) >= paddleHeight) {
            int saibaoCenter = saiX + saibao.width / 2;

            // 检查赛宝的中心是否与挡板相交
            if (
                (saibaoCenter >= paddleCenter - paddle.width / 2)
                &&
                (saibaoCenter <= paddleCenter + paddle.width / 2)
            ) {
                // 随机扰动球的速度，使反弹角度更自然
                dtY = int(-speed * sparse(3.5, 0.5));  // 随机值范围：3到4
                saiY -= speed * 2;  // 防止反弹时球穿透挡板
            }
        }

        // 游戏结束条件：球碰到底部
        if (saiY >= 600 - saibao.height) {
            dtX = 0;  // 停止球的水平移动
            dtY = 0;  // 停止球的垂直移动
            isGameOver = true;  // 设置游戏结束标志
        }

        // 如果游戏结束，显示“Game Over”画面
        if (isGameOver == true) {
            gameOver.Draw(
                (800 - gameOver.width) / 2,  // 居中显示
                (600 - gameOver.height) / 2
            );
        }

        // 更新屏幕显示
        Show();
    }

    // 游戏结束后退出
    Quit();
}
