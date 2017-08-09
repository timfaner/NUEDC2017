def isaround(x,y,aera_react): #aera_react:(x,y,w,h)
    try:
        if x > aera_react[0] and x < aera_react[0] + aera_react[2] \
        and y > aera_react[1] and y < aera_react[1] + aera_react[3]:
            return True
        else:
            return False
    except:
        return False


