#include <iostream>
#include <string>
#include <cmath>
#include <array>
#include <sstream>
#include <iomanip>
#include "raylib.h"

const double g = 9.816;
const double pi = 3.1415926535;
const int total_points = 500;
const int screenHeight = 1080;
const int screenWidth = 1920;
const double maxvel = sqrt(screenWidth*0.9*g);
const int FPS = 60;

double vel_map(double v)
{
    return ((maxvel/2) + (maxvel/2)/(1 + exp(-v)));
}
float scX(double physicsX)
{
    return (static_cast<float>(screenWidth) * 0.01f) + static_cast<float>(physicsX);
}

float scY(double physicsY)
{
    return (static_cast<float>(screenHeight) * 0.893055f) - static_cast<float>(physicsY);
}

std::string fmt(double val, int decimals = 2) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(decimals) << val;
    return oss.str();
}

class projectile
{
    public:
        std::array <double, 5> ini_state;
        double D_TOF, TOF, D_Hmax, Hmax, D_range, range;
        projectile(double raw_vel, double angle)
        {
            D_range = (angle == 90) ? 0: (raw_vel*raw_vel * sin(2*angle*pi/180))/g;
            angle *= pi/180 ;
            D_TOF = 2 * raw_vel * sin(angle)/g;
            D_Hmax = (raw_vel*raw_vel * sin(angle)*sin(angle)*0.5/g);
            double ini_vel = vel_map(raw_vel);
            TOF = 2 * ini_vel * sin(angle)/g;
            range = (ini_vel*ini_vel * sin(2*angle))/g;
            Hmax = (ini_vel*ini_vel * sin(angle)*sin(angle)*0.5/g);
            ini_state[0] = 0;
            ini_state[1] = 0;
            ini_state[2] = (ini_vel) * cos(angle);
            ini_state[3] = (ini_vel) * sin(angle);
            ini_state[4] = 1;
        }

        std::array<double, 5> path(double t) const
        {
        std::array<std::array<double, 5>, 5> current_change = {{
            { 1.0, 0.0, 1.0 * t,  0.0,       0.0 },
            { 0.0, 1.0, 0.0,      1.0 * t,  -0.5 * g * t * t },
            { 0.0, 0.0, 1.0,      0.0,       0.0 },
            { 0.0, 0.0, 0.0,      1.0,      -g * t },
            { 0.0, 0.0, 0.0,      0.0,       1.0 }
        }};

        std::array<double, 5> newstate = {0.0, 0.0, 0.0, 0.0, 0.0};

        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                newstate[i] += current_change[i][j] * ini_state[j];
            }
        }
        return newstate;
        }

        std::array <double, 2> Cord(int point) const
        {
            double step = (double) TOF/total_points;
            auto state = path(point * step);
            return {state[0], state[1]};
        }
};

int main()
{
    int point = 0;
    double u = 10;
    double theta = 45;


    std::cout<<"Simple Projectile motion simulator";
    std::cout<<"\nEnter inital velocity (+ve): ";
    std::cin>>u;
    std::cout<<"Enter angle (+ve degrees): ";
    std::cin>>theta;
    projectile p1(u, theta);


    InitWindow(screenWidth, screenHeight, "Projectile Motion");
    SetTargetFPS(FPS);

    Vector2 sun = {screenWidth * 0.94, screenHeight * 0.1};
    Vector2 ball = {scX(0), scY(0)};

    std::string peak_msg = "Peak height: " + fmt(p1.D_Hmax) + "m";
    Vector2 peakLabelPos = {scX(p1.range/2.0) + 50 , scY(p1.Hmax)};

    std::string range_msg = "Range: " + fmt(p1.D_range) + "m";
    Vector2 rangeLabelPos = {scX(p1.range) + 30, screenHeight*0.893055 + screenHeight/50};

    while (!WindowShouldClose())
    {
        ball.x = scX(p1.Cord(point)[0]);
        ball.y = scY(p1.Cord(point)[1]);
        std::string time_msg = "Time = " + fmt(p1.D_TOF*point/total_points) +"s";
        BeginDrawing();

            DrawFPS(10,10);
            ClearBackground(SKYBLUE);
            DrawCircleGradient(sun, screenHeight/18, WHITE, YELLOW);
            DrawCircleV(ball, screenHeight/144, RED);
            DrawRectangle(0, screenHeight * 0.9, screenWidth, screenHeight * 0.2, GREEN);
            DrawText(time_msg.c_str(), 10, 40, screenHeight/18, BLACK);
            if(point >= total_points/2) DrawText(peak_msg.c_str(), peakLabelPos.x - 50, peakLabelPos.y - 25, 20, BLACK);
            if(point >= total_points) DrawText(range_msg.c_str(), rangeLabelPos.x, rangeLabelPos.y, 20, BLACK);

        EndDrawing();
        if(point == 0) WaitTime(0.75);
        if(point < total_points) point++;
    }

    CloseWindow();
    return 0;
}
