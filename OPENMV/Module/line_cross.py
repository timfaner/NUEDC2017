#按照x,y排序，取数量最多的x/y相差不多线段组

enable_lens_corr = True
import sensor, image, time
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QQVGA)
sensor.set_auto_gain(False)
sensor.set_auto_whitebal(False)
clock = time.clock()

#threshold=[(92,255)]
GRAYSCALE_THRESHOLD = [(0,30,-10,10,-10,10)]
threshold=[(30,100,-40,40,-40,40)]
flag=0  #flag=1--corner flag=2--circle

ROIS=[
      (10,17,7,87),
      (142,17,7,87),
      (17,10,125,7),
      (17,102,125,7)
     ]

def not_zero(n):
    return (n!=0)

def not_origin(point):
    return point!=(0,0)

def process_ver(sortedlist):
    minus=[]
    for v in range(0,len(sortedlist)-1):
        minus.append(sortedlist[v].x1()-sortedlist[v+1].x1())
    m=max(minus)
    n=minus.index(m)
    if m>15:
        if (n+1)>(len(sortedlist)-n-1):
            return (sortedlist[0].x1()+sortedlist[0].x2())/2
        else:
            return (sortedlist[-1].x1()+sortedlist[-1].x2())/2
    else:
        return (sortedlist[0].x1()+sortedlist[0].x2())/2
    #return(ee)

def process_hor(sortedlist):
    minus=[]
    for v in range(0,len(sortedlist)-1):
        minus.append(sortedlist[v].y1()-sortedlist[v+1].y1())
    m=max(minus)
    n=minus.index(m)
    if m>15:
        if (n+1)>(len(sortedlist)-n-1):
            return (sortedlist[0].y1()+sortedlist[0].y2())/2
        else:
            return (sortedlist[-1].y1()+sortedlist[-1].y2())/2
    else:
        return (sortedlist[0].y1()+sortedlist[0].y2())/2
    #return(ee)

def process_circel(sortedlist):
    minus=[]
    for v in range(0,len(sortedlist)-1):
        minus.append(sortedlist[v][0]-sortedlist[v+1][0])
    m=max(minus)
    n=minus.index(m)
    if m>15:
        if (n+1)>(len(sortedlist)-n-1):
            return (sortedlist[0][0],sortedlist[0][1])
        else:
            return (sortedlist[-1][0],sortedlist[-1][1])
    else:
        return (sortedlist[0][0],sortedlist[0][1])
    #return(ee)

while(True):
    clock.tick()
    img = sensor.snapshot()
    ex=[]
    ey=[]
    circle=[]
    vertical=[]
    horizontal=[]

    
    #print(lines)
    img.binary(threshold)
    #img.find_edges(image.EDGE_CANNY, threshold=(10, 30))
    img.erode(2,threshold=4)
    lines = img.find_line_segments(threshold = 1500, theta_margin = 25, rho_margin = 20,segment_theshold=150)

    if len(lines)<2:
        if len(lines)==1:
            if lines[0].theta() in range(80,100):
                ey=(lines[0].y1()+lines[0].y2())/2
            elif lines[0].theta() in range(0,10) or lines[0].theta() in range(170,181):
                ex=(lines[0].x1()+lines[0].x2())/2
            else:
                ex=None
                ey=None
            center=None
        else:
            #print("error: no lines are found")
            ex=None
            ey=None
            center=None
    else:
        for i in range(0,len(lines)-1):
            for j in range(i+1,len(lines)):

                l0x1 = lines[i].x1()
                l0y1 = lines[i].y1()
                l0x2 = lines[i].x2()
                l0y2 = lines[i].y2()
                theta1=lines[i].theta()
                if(l0x1 == l0x2):
                    l0x1 = l0x1 + 0.1
                a0 = (l0y2 - l0y1)/(l0x2 - l0x1)
                b0 = l0y1 - a0*l0x1
                l1x1 = lines[j].x1()
                l1y1 = lines[j].y1()
                l1x2 = lines[j].x2()
                l1y2 = lines[j].y2()
                theta2=lines[j].theta()
                if(l1x1 == l1x2):
                    l1x1 = l1x1 + 0.1
                a1 = (l1y2 - l1y1)/(l1x2 - l1x1)
                b1 = l1y1 - a1*l1x1
                if(a0 == a1):
                    a0 = a0 + 0.1
                intersectionx = (b1-b0)/(a0-a1)
                intersectiony = a0*intersectionx + b0

                if (intersectionx<160 and intersectionx>0) and (intersectiony<120 and intersectiony>0):
                    #img.draw_cross(int(intersectionx), int(intersectiony),color = 255)
                    #kk = float(a0) * float(a1)
                    dert_theta=int(theta1)-int(theta2)
                    if theta1 in range(80,100):
                        horizontal.append(lines[i])
                        img.draw_line(lines[i].line(),color=(0,0,255))
                    if theta2 in range(80,100):
                        horizontal.append(lines[j])
                        img.draw_line(lines[j].line(),color=(0,0,255))
                    if theta1 in range(0,10) or theta1 in range(170,181):
                        vertical.append(lines[i])
                        img.draw_line(lines[i].line(),color=(0,0,255))
                    if theta2 in range(0,10) or theta2 in range(170,181):
                        vertical.append(lines[j])
                        img.draw_line(lines[j].line(),color=(0,0,255))
                    if abs(dert_theta)>85 and abs(dert_theta)<95 and ((theta1 in range(80,100))or(theta2 in range(80,100))):
                        img.draw_cross(int(intersectionx), int(intersectiony),color = (255,0,0))
                        img.draw_circle(int(intersectionx), int(intersectiony),10,color = (255,0,0))
                        circle.append((int(intersectionx), int(intersectiony)))

        if len(vertical)>1:
            ver_sortedlist=sorted(vertical,key=lambda line:line.x1(),reverse=True)
            ex=process_ver(ver_sortedlist)
        elif len(vertical)==1:
            ex=(vertical[0].x1()+vertical[0].x2())/2
        else:
            ex=None
        if len(horizontal)>1:
            hor_sortedlist=sorted(horizontal,key=lambda line:line.y1(),reverse=True)
            ey=process_hor(hor_sortedlist)
        elif len(horizontal)==1:
            ey=(horizontal[0].y1()+horizontal[0].y2())/2
        else:
            ey=None

        if len(circle)>1:
            cir_sortedlist=sorted(circle,key=lambda point:point[0],reverse=True)
            center=process_circel(cir_sortedlist)
        elif len(circle)==1:
            center=circle[0]
        else:
            center=None

    '''
    for v in range(0,len(ver_sortedlist)-1):
        minus_ver.append(ver_sortedlist[v]-ver_sortedlist[v+1])
    m=max(minus_ver)
    n=minus_ver.index(m)
    if m>15:
        if (n+1)>(len(ver_sortedlist)-n-1):
            ex=(ver_sortedlist[0].x1()+ver_sortedlist[0].x2())/2
        else:
            ex=(ver_sortedlist[-1].x1()+ver_sortedlist[-1].x2())/2



    for l in lines:
        img.draw_line(l.line(), color = 255)
        if (l.theta()>80)and(l.theta()<100):
            e_y=(l.y1()+l.y2())/2
            ey.append(e_y)
        else:
            e_y=0
            ey.append(e_y)
        if ((l.theta()>0)and(l.theta()<10))or((l.theta()<180)and(l.theta()>170)):
            e_x=(l.x1()+l.x2())/2
            ex.append(e_x)
        else:
            e_x=0
            ex.append(e_x)
        #print(l)

    ex=list(filter(not_zero,ex))
    ey=list(filter(not_zero,ey))
    '''
    num=0
    if ex!=None and ey!=None:
        for r in ROIS:
            blobs = img.find_blobs(GRAYSCALE_THRESHOLD, roi=r, merge=True, pixel_threshold=200)
            if blobs:
                num=num+1
                largest_blobs=max(blobs,key=lambda b:b.pixels())
                img.draw_rectangle(largest_blobs.rect(),color=(255,0,0))
        if num==2:
            flag=1
            #print("this is a corner")
        elif num==3:
            flag=2
            #print("this is a circle")
        else:
            flag=0
            #print("error: ...")
        #print("num=",num)

    if center==None:
        center=(255,255)
    if ex==None:
        ex=255
    if ey==None:
        ey=255
    print(ex,ey,center,flag)#flag=1--corner flag=2--circle
    #print("endl")
    #time.sleep(500)
    #print("FPS %f" % clock.fps())
    img.draw_string(0,0,"FPS %s"%clock.fps())
