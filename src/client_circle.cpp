// client_circle.cpp
#include <iostream>
#include <ros/ros.h>
#include <random>
#include "testpkg/calcCicleArea.h"
#include "testpkg/circle.h"

bool clientCalcCircleArea(testpkg::calcCicleArea::Request &req, testpkg::calcCicleArea::Response &res)
{
    for (int i = 0; i < req.radius.size(); i++) {
        double area = 0.0;
        area = 3.14 * (double)req.radius[i] * (double)req.radius[i];
        res.area.push_back(area);
    }
    return true;
}

int main(int argc, char** argv){
    ros::init(argc, argv, "client_circle_node");
    ros::NodeHandle nh;
    
    ros::ServiceClient client_calc_circle_area;
    client_calc_circle_area = nh.serviceClient<testpkg::calcCicleArea>("/calc_circle_area", clientCalcCircleArea);

    ros::Publisher pub_circle;
    pub_circle = nh.advertise<testpkg::circle>("/circle", 1);

    ros::Rate loop_rate(1);
    unsigned int seq = 0;
    while (ros::ok()) {
        std::random_device rnd;
        // n개 만큼 랜덤한 반지름 생성
        int num_circle = rnd() % 10;
        int radius = 0;
        testpkg::calcCicleArea srv;
        for (int i = 0; i < num_circle; i++) {
            radius = rnd() % 100;
            srv.request.radius.push_back(radius);
        }
        // 서비스 요청 및 원 정보 Publish
        if (client_calc_circle_area.call(srv)) {
            ROS_INFO("Service call success");
            testpkg::circle msg_circle;
            msg_circle.header.stamp = ros::Time::now();
            msg_circle.header.frame_id = "circle";
            msg_circle.header.seq = seq++;
            for (int i = 0; i < srv.response.area.size(); i++) {
                // msg_circle.radius[i].data = srv.request.radius[i].data;
                msg_circle.area.push_back(srv.response.area[i]);
                msg_circle.radius.push_back(srv.request.radius[i]);
                // std::cout << "radius: " << srv.request.radius[i].data << ",
                // area: " << srv.response.area[i].data << std::endl;
            }
            pub_circle.publish(msg_circle);
        } else {
            ROS_ERROR("Failed to call service calc_circle_area");
        }
        ros::spinOnce();
        loop_rate.sleep();
    }
    return 0;
}