// calc_circle_area.cpp
#include <iostream>
#include <ros/ros.h>
#include "testpkg/calcCicleArea.h"

bool srvCalcCircleArea(testpkg::calcCicleArea::Request &req, testpkg::calcCicleArea::Response &res)
{
    for (int i = 0; i < req.radius.size(); i++) {
        double area = 0.0;
        area = 3.14 * (double)req.radius[i] * (double)req.radius[i];
        res.area.push_back(area);
    }
    ROS_INFO("Service call success");
    return true;
}

int main(int argc, char** argv){
    ros::init(argc, argv, "calc_circle_area_node");
    ros::NodeHandle nh;
    double radius;
    ros::ServiceServer calc_circle_area;
    calc_circle_area = nh.advertiseService("/calc_circle_area", srvCalcCircleArea);
    ros::spin();
    return 0;
}