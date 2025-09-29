#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <termios.h>
#include <unistd.h>
#include <iostream>

using namespace std;

// 获取键盘输入函数
char getKey()
{
    struct termios oldt, newt;
    char ch;
    tcgetattr(STDIN_FILENO, &oldt);  // 保存原始终端设置
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO); // 关闭缓冲和回显
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // 恢复设置
    return ch;
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "teleop_car");
    ros::NodeHandle nh;

    // 创建发布者，发布到 /cmd_vel
    ros::Publisher cmd_pub = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 10);

    geometry_msgs::Twist cmd_vel;
    double linear_speed = 0.1;   // 线速度 (m/s)
    double angular_speed = 0.5;  // 角速度 (rad/s)

    cout << "键盘控制小车: W=前进, S=后退, A=左转, D=右转, Q=停止, E=退出" << endl;

    ros::Rate rate(10);  // 循环频率 10Hz
    while (ros::ok())
    {
        char key = getKey();
        cmd_vel.linear.x = 0.0;
        cmd_vel.angular.z = 0.0;

        if (key == 'w' || key == 'W')
            cmd_vel.linear.x = linear_speed;
        else if (key == 's' || key == 'S')
            cmd_vel.linear.x = -linear_speed;
        else if (key == 'a' || key == 'A')
            cmd_vel.angular.z = angular_speed;
        else if (key == 'd' || key == 'D')
            cmd_vel.angular.z = -angular_speed;
        else if (key == 'q' || key == 'Q')
            cmd_vel.linear.x = 0.0;
        else if (key == 'e' || key == 'E')
            break; // 退出循环

        // 发布控制消息
        cmd_pub.publish(cmd_vel);

        ros::spinOnce();
        rate.sleep();
    }

    return 0;
}
