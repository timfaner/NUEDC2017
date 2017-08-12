#m7_debug

import sensor, image, math, pyb, ustruct, utime
from pyb import  SPI, Pin,LED
 

task_timer = [5000,5000,5000,5000,5000] #不同任务的返回land的时长  0,1,2,3,4 对应 5个任务
LED_FLAG = 1 #LED flag
DEBUG_FLAG = 1
task_number = 1



thresholds = [(0, 20, -24, 16, -27, 19),
              (0, 80, 20, 99, 2, 45),
              (0, 15, 0, 40, -80, -20)]

GROUND_LOST = 1
CAR_LOST = 2
UNKNOW_TASK_NUMBER  = 4



red_led = LED(2)
green_led = LED(3)


def isaround(x,y,aera_react): #aera_react:(x,y,w,h)
    try:
        if  x > aera_react[0] - aera_react[2]//2 \
        and x < aera_react[0] + aera_react[2]//2 \
        and y > aera_react[1] - aera_react[3]//2 \
        and y < aera_react[1] + aera_react[3]//2:
            return True
        else:
            return False
    except: #以防空的东西传入
        return False



    
sensor.reset() # Initialize the camera
sensor.set_pixformat(sensor.RGB565) # use grayscale.
sensor.set_framesize(sensor.QQVGA) # use QQVGA for speed.

sensor.set_auto_gain(False,value = 20) # must be turned off for color tracking
#sensor.set_auto_exposure(False,value = 500)
sensor.set_auto_whitebal(False,value = (195,175,245)) # must be turned off for color tracking

sensor.skip_frames(time = 400)

screen_middle_x,screen_middle_y = 80,60
if LED_FLAG == 1:
    green_led.on()
    red_led.off()



error_flag = 0
land_flag = ground_x = ground_y = car_x = car_y = 255
output = [error_flag,land_flag,ground_x,ground_y,car_x,car_y]

timer_lock = 0
while(True):

    largest_ground_blob = None
    largest_car_blob = None
    blob_ground = []
    blob_car = []
    error_flag = 0
    land_flag = 0

    img = sensor.snapshot()
    blobs = img.find_blobs(thresholds, pixels_threshold=100, area_threshold=100, merge=True)
    if blobs:
        for blob in blobs:
            if(blob.code() == 1):  #黑色
                blob_ground.append(blob)
            if(blob.code() == 2):  #车的颜色
                blob_car.append(blob)

            
    if blob_ground:
        largest_ground_blob = max(blob_ground, key=lambda b: b.pixels())
        img.draw_rectangle(largest_ground_blob.rect())
        img.draw_cross(largest_ground_blob.cx(), largest_ground_blob.cy())
    else:
        error_flag |= GROUND_LOST #找不到地

    if blob_car:
            largest_car_blob = max(blob_car, key=lambda b: b.pixels())
            img.draw_rectangle(largest_car_blob.rect())
            img.draw_cross(largest_car_blob.cx(), largest_car_blob.cy())
    else:
        error_flag |= CAR_LOST #找不到车


    if largest_ground_blob:
        ground_x ,ground_y =  largest_ground_blob.cx(), largest_ground_blob.cy()
    else: ground_x ,ground_y = 255,255

    if largest_car_blob:
        car_x ,car_y =  largest_car_blob.cx(), largest_car_blob.cy()
    else:
        car_x ,car_y = 255,255 


    

    output = [error_flag,land_flag,ground_x,ground_y,car_x,car_y]


    print(output)