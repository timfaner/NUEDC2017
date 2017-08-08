# jy-901 - By: TimFan - 周五 8月 4 2017

#I2C(2): (SCL, SDA) = (P4, P5) = (PB10, PB11)


#Roll=((RollH<<8)|RollL)/32768*180(°)
from pyb import I2C
import time

i2c = I2C(2, I2C.MASTER,baudrate = 400000) # The i2c bus must always be 2.

while(1):
    x_raw = i2c.mem_read(2, 0x50, addr_size = 16) # The eeprom slave address is 0x50.
    print(x_raw)
    time.sleep(20)
