
#include <iostream>
#include <math.h>

#include "robot.hh"

using std::cout;
using std::endl;

const double goal_x = 20.0;
const double goal_y = 0.0;
bool done = false;

void handleBothTurnRequired(Robot* robot, double widestLeftObstacle, double widestRightObstacle);
void handleNoTurnRequired(Robot* robot, bool goStraight);


void
callback(Robot* robot)
{
    /*
    cout << endl;
    cout << "robot x =" << robot->pos_x << endl;
    cout << "robot y =" << robot->pos_y << endl;
    */
    double dx = goal_x - robot->pos_x;
    double dy = goal_y - robot->pos_y;
    if (abs(dx) < 0.75 && abs(dy) < 0.75) {
        cout << "we win!" << endl;
        robot->set_vel(0.0);
        robot->set_turn(0.0);
        robot->done();
        return;
    }

    //Set the velocity of the robot
    robot->set_vel(50.0);

    //Initialize Boolean Values for turn decision
    bool rightTurn = false;
    bool leftTurn = false;
    bool goStraight = false;

    //Initialize double values for important left and right obstacles
    double widestRightObstacle = 0.0;
    double widestLeftObstacle = 0.0;

    //Loop sensor hits to determine right turn, left turn, or straight
    //Also keep track of the widest angle hit on the left and right
    for (LaserHit hit : robot->hits) {
        if (hit.range < 5) {
            if (hit.angle < 0.3 && hit.angle > 0) {
              //Right turn necessary if obstacle is in path on left side
              rightTurn = true;
              if(hit.angle > widestLeftObstacle){
                widestLeftObstacle = hit.angle;
              }
            } else if(hit.angle > -0.3 && hit.angle < 0){
              //Left turn necessary if obstacle is in path on right side
              leftTurn = true;
              if(hit.angle < widestRightObstacle){
                widestRightObstacle = hit.angle;
              }
            } else if (hit.angle < 0.7 && hit.angle > -0.7) {
              //go straight until passing an obstacle ahead that is not in path
              goStraight = true;
            }
        }

    }

    //Go left, right, straight or to the goal
    //If both left and right turn required, choose easiest side of obstacle
    //Turn angle is standardized
    if(leftTurn && rightTurn){

      handleBothTurnRequired(robot, widestLeftObstacle, widestRightObstacle);

    } else if(leftTurn){
      //There is an obstacle on the right side, turn left
      robot->set_turn(-0.1);
    } else if (rightTurn) {
      //There is an obstacle on the left side, turn right
      robot->set_turn(0.1);
    } else {

      handleNoTurnRequired(robot, goStraight);
    }
}

void handleBothTurnRequired(Robot* robot, double widestLeftObstacle, double widestRightObstacle){
  if(widestLeftObstacle > -1*widestRightObstacle){
    //if there is more room on the right, turn right
    robot->set_turn(0.1);
  } else if(widestLeftObstacle < -1*widestRightObstacle){
    //if there is more room on the left, turn left
    robot->set_turn(-0.1);
  } else {
    //If there is equal room on the left and right, use current yaw to choose

    //If yaw points to the right of the obstacle turn right, else turn left
    if(robot->pos_t < 0){
      robot->set_turn(0.1);
    } else {
        robot->set_turn(-0.1);
    }
  }
}

void handleNoTurnRequired(Robot* robot, bool goStraight){
  //If no turn required, either go straight or to the goal
  //Go straight if obstacles ahead but not in the robots path
  if(goStraight){
    robot->set_turn(0);
  } else {
    //Go to the goal if there are no obstacles that could affect the path

    //Determine the angle from the robot's position to the goal
    double angle = atan2(goal_y - robot->pos_y, goal_x - robot->pos_x);

     //Turn value is the difference between robot's yaw angle and goal angle
    double turn = (double) robot->pos_t - angle;

    //If turn angle to goal is large, reduce it to avoid sharp turns
    if(turn > 0.2 || turn < -0.2){
      turn = turn / 4;
    }

    robot->set_turn(turn);
  }

}

int
main(int argc, char* argv[])
{
    cout << "making robot" << endl;
    Robot robot(argc, argv, callback);
    robot.do_stuff();

    return 0;
}
