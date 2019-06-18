import cv2
import numpy as np
import time
import requests

GOAL_AREA_START = (240, 180)
GOAL_AREA_END = (340, 300)

cap = cv2.VideoCapture(0)

# allow the camera to warmup
time.sleep(0.1)

ball_last_seen = int(round(time.time() * 1000)) + 5000 # in milliseconds
ball_is_seen = False
ball_is_within_goal = False


def update_ball_is_within_goal(ball_x, ball_y):
    global ball_is_within_goal
    is_within_x = GOAL_AREA_START[0] < ball_x < GOAL_AREA_END[0]
    is_within_y = GOAL_AREA_START[1] < ball_y < GOAL_AREA_END[1]
    ball_is_within_goal = is_within_x and is_within_y

def update_ball_last_seen():
    global ball_last_seen
    current_time_ms = int(round(time.time() * 1000))
    ball_last_seen = current_time_ms
    ball_is_seen = True

def is_ball_lost():
    if ball_is_seen: return False
    else:
        time_now_ms = int(round(time.time() * 1000))
        return ball_last_seen + 1000 < time_now_ms

def check_if_won():
    if is_ball_lost() and ball_is_within_goal:
        global ball_is_seen
        if not ball_is_seen:
            print('WON!')
            ball_is_seen = True

def check_if_lost():
    if is_ball_lost() and not ball_is_within_goal:
        global ball_is_seen
        if not ball_is_seen:
            print('LOST!')
            ball_is_seen = True

def run():
    # capture frames from the camera
    while(True):
        _, frame = cap.read()

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
            cv2.circle(frame, (average_contour_x, average_contour_y), 25 , (124,252,0), 3)
            update_ball_last_seen()
            update_ball_is_within_goal(average_contour_x, average_contour_y)
        else:
            global ball_is_seen
            ball_is_seen = False

        check_if_lost()
        check_if_won()

        cv2.imshow('frame',frame)

        k = cv2.waitKey(5) & 0xFF
        if k == 27:
            break

if __name__ == '__main__':
    run()
    cap.release()
    cv2.destroyAllWindows()
