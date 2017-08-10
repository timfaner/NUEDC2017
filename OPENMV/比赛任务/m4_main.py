#m4_main
import sensor, image, math, pyb, ustruct, utime
from pyb import  SPI, Pin,LED
 

task_timer = [5000,5000,5000,5000,5000] #不同任务的返回land的时长  0,1,2,3,4 对应 5个任务
LED_FLAG = 1 
DEBUG_FLAG = 1
task_number = 1


thresholds = [(30, 100, 15, 127, 15, 127), # red
              (30, 100, -64, -8, -32, 32), # green
              (0, 15, 0, 40, -80, -20)]    # blue

GROUND_LOST = 1
CAR_LOST = 2
UNKNOW_TASK_NUMBER  = 4

pin_start = Pin('P6', Pin.IN, Pin.PULL_DOWN)
pin_task1 = Pin('P7', Pin.IN, Pin.PULL_DOWN)
pin_task2 = Pin('P8', Pin.IN, Pin.PULL_DOWN)
pin_task3 = Pin('P3', Pin.IN, Pin.PULL_DOWN)


spi = SPI(2, SPI.MASTER, baudrate=int(1000000000/66), polarity=0, phase=0,bits=32)


red_led   = LED(1)
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

def sendpackage(*arg):
    
    Arg = [int(x) for x in arg]
    for i in Arg:
        if i < 0:
            raise BaseException('arg less than 0:',i)

        elif i > 255:
            raise BaseException('arg more than 0:',i)
            
        

    while(len(Arg)) < 6:
        Arg.append(0)
    check_bit = Arg[0]
    for data in Arg[1:]:
        check_bit = data ^ check_bit
    spi.send(0XD3)
    utime.sleep_us(50)
    for data in Arg:
        spi.send(ustruct.pack("<B",data))
        utime.sleep_us(50)
    spi.send(check_bit)
    utime.sleep_us(50)
    spi.send(0xC8)
    utime.sleep_us(10)



#任务选择加等待使能

if DEBUG_FLAG == 0:
    while not pin_start.value():
        task_number = pin_task1.value()*4+pin_task2.value()*2+pin_task3.value()
        utime.sleep_ms(1)
        if LED_FLAG == 1:
            if task_number == 0 or task_number in range(6,8):
                while(1):
                    red_led.toggle()
                    green_led.toggle()
                    utime.sleep(200)
            elif task_number in range (1,6):
                red_led.off()
                green_led.on()
            else:
                while(1):
                    red_led.toggle()
                    utime.sleep(200)
    
else:
    pass
    
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
                    if pyb.millis() - time_begin > task_timer[0]:
                        land_flag = 1
                    else:
                        print('In timer,remain : ',task_timer[0] - (pyb.millis() - time_begin))
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
    try:
        sendpackage(*output)
    except Exception as e:
        print('send error',e)

    print(output)