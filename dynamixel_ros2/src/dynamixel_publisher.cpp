#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <iostream>
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include <conio.h>

using namespace std::chrono_literals;

class DynamixelPublisher : public rclcpp::Node
{
public:
    DynamixelPublisher() : Node("dynamixel_publisher"), count_(0)
    {
        auto qos_profile = rclcpp::QoS(rclcpp::KeepLast(10));
        dynamixel_publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("cmd_vel", qos_profile);
        timer_ = this->create_wall_timer(100ms, std::bind(&DynamixelPublisher::publish_velcmd_msg, this));
    }

private:
    void publish_velcmd_msg()
    {
        static auto msg = geometry_msgs::msg::Twist();
        if (_kbhit())
        {
            char c = _getch();
            switch (c)
            {
            case 's':
                msg.linear.x = 0; //linear speed 
                msg.angular.z = 0; //angular speed
                break; //���ǹ� ����
            case 'f':
                msg.linear.x += 0.1; //increase linear speed 0.1 m/s
                break; //���ǹ� ����
            case 'b':
                msg.linear.x -= 0.1; //decrease linear speed 0.1m/s
                break; //���ǹ� ����
            case 'l':
                // msg.linear.x = -200; // left motor speed
                // msg.linear.y = -200; // right motor speed
                msg.angular.z -= 0.1; //turn left(rad/s)  
                break; //���ǹ� ����
            case 'r':
                // msg.linear.x = 200; // left motor speed
                // msg.linear.y = 200; // right motor speed        
                msg.angular.z += 0.1; //turn right(rad/s)
                break; //���ǹ� ����
            default: //����Ʈ ��
                msg.linear.x = msg.linear.x; // linear speed
                msg.angular.z = msg.angular.z; // angular speed        
                break; //���ǹ� ����
            }
            RCLCPP_INFO(this->get_logger(), "Published message: %lf,%lf", msg.linear.x, msg.angular.z);
            dynamixel_publisher_->publish(msg);
        }
    }
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr dynamixel_publisher_;
    size_t count_;
};
int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<DynamixelPublisher>();
    std::cout << "Enter command(s,f,b,l,r):" << std::endl;
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}