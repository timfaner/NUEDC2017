# jy-901 - By: TimFan - 周五 8月 4 2017




#Roll=((RollH<<8)|RollL)/32768*180(°)
from pyb import UART
import time
import sensor
import math


sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time = 200)


def coordinate_trans(h0 = 1.0,attitude = (0,0,0),pix = [80,60]):
    of_set = math.cos(attitude[0])*math.cos(attitude[2])
    alpha = math.atan(pix[0]/162.71)
    y_transed = -h0 * math.tan(attitude[2])/math.cos(attitude[0])+ \
    h0 / (math.cos(attitude[0])*math.cos(attitude[2])) *math.sin(alpha) * math.sin(alpha + math.pi/2 - attitude[2])
    y_transed = y_transed * of_set
    gamma = math.atan(pix[1]/162.71)
    x_transed = h0 * math.tan(attitude[0]) - \
    h0/math.cos(attitude[0])* math.sin(gamma) / math.sin(math.pi/2 - attitude[0]+gamma)
    x_transed = x_transed * of_set
    return x_transed,y_transed

def datapharse(raw):
    temp = None
    try:
        for i in range(len(raw)):
            if (raw[-i-2] == 0x55) and (raw[-i-1] == 0x53 )and (-i+8 < 0):
                temp = raw[-i-2:]
                break
    except:
        pass
    if temp:
        Xx = (temp[2] | temp[3] << 8)/32768*180 #Roll
        Yy = (temp[4] | temp[5] << 8)/32768*180 #Pitch
        Zz = (temp[6] | temp[7] << 8)/32768*180 #Yaw
        return (Yy,Zz,Xx)
    else:
        return False

uart = UART(3,19200,bits = 8,parity = None,stop = 1,read_buf_len = 64)
while(1):
    img = sensor.snapshot()
    jy901_raw = uart.read(64)
    
    attitude = datapharse(jy901_raw)


