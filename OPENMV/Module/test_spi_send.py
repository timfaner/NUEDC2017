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

def pro_send(x):
    if(x<128):
        if(x == 65):
            x = 64
        spi.send(ustruct.pack("<B",0XCA))
        utime.sleep_us(30)
        spi.send(ustruct.pack("<B",x))
    elif ( x>=128):
        if(x == 255):
            x = 254

        spi.send(ustruct.pack("<B",255))
        utime.sleep_us(30)
        spi.send(ustruct.pack("<B",x&127))

def formatt(strr):
    strr = strr[2:]
    qianzhui = ''
    for i in range(8 - len(strr)):
        qianzhui+= '0'
    return(qianzhui+ strr + ' ')

def sendpackage(*arg):
    temp = ''
    Arg = [int(x) for x in arg]
    while(len(Arg)) < 6:
        Arg.append(0)
    check_bit = Arg[0]
    for data in Arg[1:]:
        check_bit = data ^ check_bit
    pro_send(0XD3)
    utime.sleep_us(50)
    temp = formatt(bin(0XD3))


    for data in Arg:
        pro_send(data)
        utime.sleep_us(50)
        temp += formatt(bin(data))
        utime.sleep_us(50)

    pro_send(check_bit)
    utime.sleep_us(50)
    temp += formatt(bin(check_bit))
    utime.sleep_us(50)
    pro_send(0xC8)
    utime.sleep_us(10)
    temp += formatt(bin(0XC8))
    print(temp)


def test(x):
    print(x)
    print(bin(x))
    pro_send(x)


sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames()
x = 0

#sendpackage(0,0,0)

#print(ustruct.pack("<L",373))
i = 0
while(True):
    #pro_send(x)
    #print(x)
#test(x)
    #test(x)
    print(x)
   # spi.send(160)
    #spi.send(110)

    #spi.send(159)
    spi.send(200)
    time.sleep(200)
    #time.sleep(300)
    #spi.send(x)
    #
    #time.sleep(100)
    #img = sensor.snapshot()
    #led_control(x)
    x = x+1


