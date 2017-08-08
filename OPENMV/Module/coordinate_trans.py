import math
def coordinate_trans(h0 = 1.0,attitude = [0,0,0],pix = [80,60]):
    alpha = math.atan(pix[0]/162.71)
    y_transed = -h0 * math.tan(attitude[2])/math.cos(attitude[0])+ \
    h0 / (math.cos(attitude[0])*math.cos(attitude[2])) *math.sin(alpha) * math.sin(alpha + math.pi/2 - attitude[2])

    gamma = math.atan(pix[1]/162.71)
    x_transed = h0 * math.tan(attitude[0]) - \
    h0/math.cos(attitude[0])* math.sin(gamma) / math.sin(math.pi/2 - attitude[0]+gamma)

    return x_transed,y_transed

