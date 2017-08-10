# jy-901 - By: TimFan - 周五 8月 4 2017




#Roll=((RollH<<8)|RollL)/32768*180(°)
from pyb import UART
import time
import sensor

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time = 200)


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
        Xx = (temp[2] | temp[3] << 8)/32768*180
        Yy = (temp[4] | temp[5] << 8)/32768*180
        Zz = (temp[6] | temp[7] << 8)/32768*180
        return (Xx,Yy,Zz)
    else:
        return False

uart = UART(3,19200,bits = 8,parity = None,stop = 1,read_buf_len = 64)
while(1):
    img = sensor.snapshot()
    jy901_raw = uart.read(64)
    
    print(datapharse(jy901_raw))


