import cv2
import numpy as np
import time
import requests
import socket
import json

GOAL_AREA_START = (290, 180)
GOAL_AREA_END = (390, 300)

class Tracker:

  def __init__(self, unity_IP = None):
    self.cap = cv2.VideoCapture(0)
    # allow the camera to warmup
    time.sleep(0.1)
    self.ball_last_seen = int(round(time.time() * 1000)) + 5000 # in milliseconds
    self.ball_is_seen = False
    self.ball_is_within_goal = False
    self.game_status_updated = False
    if (unity_IP):
      self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
      self.s.connect((unity_IP, 9092))
      self.s.send(MESSAGE)
      self.unity_IP = unity_IP

  def update_ball_is_within_goal(self, ball_x, ball_y):
    is_within_x = GOAL_AREA_START[0] < ball_x < GOAL_AREA_END[0]
    is_within_y = GOAL_AREA_START[1] < ball_y < GOAL_AREA_END[1]
    self.ball_is_within_goal = is_within_x and is_within_y

  def update_ball_last_seen(self):
    current_time_ms = int(round(time.time() * 1000))
    self.ball_last_seen = current_time_ms
    self.ball_is_seen = True
    self.game_status_updated = False

  def is_ball_lost(self):
    if self.ball_is_seen: return False
    else:
      time_now_ms = int(round(time.time() * 1000))
      return self.ball_last_seen + 1000 < time_now_ms

  def check_if_won(self):
    if self.is_ball_lost() and self.ball_is_within_goal:
      if not self.game_status_updated:
        print('WON!')
        try:
          requests.get("http://localhost:8080/gamegoal")
        except:
          print("Unable to request backend")
        self.game_status_updated = True

  def check_if_lost(self):
    if self.is_ball_lost() and not self.ball_is_within_goal:
      if not self.game_status_updated:
        print('LOST!')
        try:
          requests.get("http://localhost:8080/gamelost")
        except:
          print("Unable to request backend")
        self.game_status_updated = True

  def run(self):
    # capture frames from the camera
    while(True):
      _, frame = self.cap.read()

      # Convert BGR to HSV
      hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

      # define range of green color in HSV
      lower_green = np.array([65, 60, 60])
      upper_green = np.array([80, 255, 255])

      # Threshold the HSV image to get only green colors
      mask = cv2.inRange(hsv, lower_green, upper_green)

      kernel = np.ones((5,5), 'int')
      dilated = cv2.dilate(mask, kernel)
      # Bitwise-AND mask and original image
      res = cv2.bitwise_and(frame,frame, mask=mask)
      ret, thrshed = cv2.threshold(cv2.cvtColor(res,cv2.COLOR_BGR2GRAY), 3, 255, cv2.THRESH_BINARY)
      contours, hier = cv2.findContours(thrshed, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)
      cv2.drawContours(frame, contours, -1, (0,255,0), 3)

      # draw a rectangle around the goal area
      cv2.rectangle(frame, GOAL_AREA_START, GOAL_AREA_END, (124, 252, 0), 3)

      sum_of_contours_x = 0
      sum_of_contours_y = 0
      number_of_contours = 0
      for cnt in contours:
        #Contour area is taken
        area = cv2.contourArea(cnt)
        #print(cnt)

        if area > 0:
          M = cv2.moments(cnt)
          sum_of_contours_x += int(M['m10']/M['m00'])
          sum_of_contours_y += int(M['m01']/M['m00'])
          number_of_contours += 1

      if number_of_contours > 0:
        # draw a circle around the ball
        average_contour_x = sum_of_contours_x / number_of_contours
        average_contour_y = sum_of_contours_y / number_of_contours
        cv2.circle(frame, (average_contour_x, average_contour_y), 25 , (124, 252, 0), 3)
        self.update_ball_last_seen()
        self.update_ball_is_within_goal(average_contour_x, average_contour_y)
        if self.unity_IP:
          pos = {
            "Position": {
              "x": average_contour_x / 640,
              "y": 0.0,
              "z": average_contour_y / 480
            }
          }
          self.s.send(json.dumps(pos))
      else:
        self.ball_is_seen = False

      self.check_if_lost()
      self.check_if_won()

      cv2.imshow('frame',frame)

      k = cv2.waitKey(5) & 0xFF
      if k == 27:
        cap.release()
        cv2.destroyAllWindows()
        self.s.close()
        break