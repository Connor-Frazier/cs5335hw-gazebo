Name: Connor Frazier

Git URL:https://github.com/Connor-Frazier/cs5335hw-gazebo

Demo Video URL: https://youtu.be/5DNIvDENWiU

Comments:
The demo video audio is low due to equipment restrictions, headphones may be necessary.

Control Program Decision Outline

Turn Decision:
  If objects on both left and right, choose easiest turn choice based on
  which side has the smallest angle. If there is no difference, choose to
  turn in the direction the robot is already headed. Lastly default turn is
  to the left.

  If there is an object on right, turn left.

  If there is an object on the left, turn right.

  If there is no turn needed but there are obstacles ahead within range, go straight.

  Lastly, go to the goal by calculating the angle from the current position to
  the goal, and setting the turn to difference between the robot's yaw and the
  angle to the goal.
