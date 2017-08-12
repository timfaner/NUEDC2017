#m4_debug

import sensor, image, math, pyb, ustruct, utime
from pyb import  SPI, Pin,LED
 

task_timer = [5000,5000,5000,5000,5000] #不同任务的返回land的时长  0,1,2,3,4 对应 5个任务
LED_FLAG = 1 #LED flag
DEBUG_FLAG = 1
task_number = 1



thresholds = [(0, 31, -24, 7, -48, 58), # generic_red_thresholds -> index is 0 so code == (1 << 0)
              (0, 80, 20, 99, 2, 45), # generic_green_thresholds -> index is 1 so code == (1 << 1)
              (0, 15, 0, 40, -80, -20)]
GROUND_LOST = 1
CAR_LOST = 2
UNKNOW_TASK_NUMBER  = 4



red_led = LED(1)
green_led = LED(2)


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
sensor.skip_frames(time = 400) # Let new settings take affect.
sensor.set_auto_gain(False,value = 910) # must be turned off for color tracking
sensor.set_auto_whitebal(False) # must be turned off for color tracking
sensor.set_auto_exposure(False)

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


    if task_number == 1:
        if largest_ground_blob:
            ground_x ,ground_y =  largest_ground_blob.cx(), largest_ground_blob.cy()
            land_area = (screen_middle_x,screen_middle_y,largest_ground_blob.w(),largest_ground_blob.h())
            if isaround(ground_x,ground_y,land_area):
                if timer_lock == 0:
                    time_begin = pyb.millis()
                    timer_lock = 1
                    print('begin timer')
                else:
                    if pyb.millis() - time_begin > task_timer[0]:
                        land_flag = 1
                    else:
                        print('In timer,remain : ' ,task_timer[0] - (pyb.millis() - time_begin))
                        
            else:
                timer_lock = 0



    elif task_number == 3:
        if largest_ground_blob:
            ground_x ,ground_y =  largest_ground_blob.cx(), largest_ground_blob.cy()
        else: ground_x ,ground_y = 255,255

        if largest_car_blob:
            car_x ,car_y =  largest_car_blob.cx(), largest_car_blob.cy()
            land_area = (screen_middle_x,screen_middle_y,largest_car_blob.w(),largest_car_blob.h())

            if isaround(car_x,car_y,land_area):
                if timer_lock == 0:
                    time_begin = pyb.millis()
                    timer_lock = 1
                    print('begin timer')
                else:
                    if pyb.millis() - time_begin > task_timer[3]:
                        land_flag = 1
                    else:
                        print('In timer,remain : ',task_timer[3] - (pyb.millis() - time_begin))
            else:
                timer_lock = 0


        else:
            car_x ,car_y = 255,255 

    elif task_number == 4:
        if largest_ground_blob:
            ground_x ,ground_y =  largest_ground_blob.cx(), largest_ground_blob.cy()
        else: ground_x ,ground_y = 255,255

        if largest_car_blob:
            car_x ,car_y =  largest_car_blob.cx(), largest_car_blob.cy()
        else:
            car_x ,car_y = 255,255 

    elif task_number == 5:
        if largest_ground_blob:
            ground_x ,ground_y =  largest_ground_blob.cx(), largest_ground_blob.cy()
        else: ground_x ,ground_y = 255,255

        if largest_car_blob:
            car_x ,car_y =  largest_car_blob.cx(), largest_car_blob.cy()
        else:
            car_x ,car_y = 255,255 


    elif task_number == 2:
        if largest_car_blob:
            car_x ,car_y =  largest_car_blob.cx(), largest_car_blob.cy()
        else:
            car_x ,car_y = 255,255 
    else:
        error_flag |= UNKNOW_TASK_NUMBER 

    
    

    output = [error_flag,land_flag,ground_x,ground_y,car_x,car_y]


    print(output)