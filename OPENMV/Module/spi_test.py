# Untitled - By: TimFan - 周一 7月 31 2017

import sensor,utime,ustruct,time
from pyb import  SPI, Pin,LED
red_led   = LED(1)
green_led = LED(2)
blue_led  = LED(3)
ir_led    = LED(4)

def led_control(x):
    if   (x&1)==0: red_led.off()
    elif (x&1)==1: red_led.on()
    if   (x&2)==0: green_led.off()
    elif (x&2)==2: green_led.on()
    if   (x&4)==0: blue_led.off()
    elif (x&4)==4: blue_led.on()
    if   (x&8)==0: ir_led.off()
    elif (x&8)==8: ir_led.on()


spi = SPI(2, SPI.MASTER, baudrate=int(100000000/66), polarity=0, phase=1,bits=8)




def sendpackage(*arg):
    temp = ''
    Arg = [int(x) for x in arg]
    while(len(Arg)) < 6:
        Arg.append(0)
    check_bit = Arg[0]
    for data in Arg[1:]:
        check_bit = data ^ check_bit
    spi.send(0XD3)
    utime.sleep_us(50)



    for data in Arg:
        spi.send(data)
        utime.sleep_us(50)

        utime.sleep_us(50)

    spi.send(check_bit)
    utime.sleep_us(50)

    utime.sleep_us(50)
    spi.send(0xC8)
    utime.sleep_us(10)



def test(x):
    print(x)
    print(bin(x))
    spi.send(x)


sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames()
x = 0

#sendpackage(0,0,0)

#print(ustruct.pack("<L",373))
i = 0
while(True):
    sendpackage(x,0,7,x+2)
    #spi.send(159)
    #spi.send(x)
    time.sleep(200)
    #time.sleep(300)
    #spi.send(x)
    #
    #time.sleep(100)
    #img = sensor.snapshot()
    led_control(x)
    x = x+1


