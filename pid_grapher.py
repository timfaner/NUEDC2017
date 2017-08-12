import io,numpy,pylab

filepath = '/Users/TimFan/Downloads/a.txt'
raw = []
f = open(filepath,'r')
try:
    while(1):
        try:
            temp = f.readline()
            if temp != '':
                raw.append(f.readline())
            else:
                break
            
        except Exception as e:
            print(e)
except Exception as e:
    print(e)
content = []
for line in raw:
    if len(line) > 3:
        if line[0] == 'X' and line[1] == 'O':
            content.append(line[3:].replace(' \n','').split(' '))


x_offset = []
y_offset = []
x_speed = []
y_speed = []
axis = []
zero = []
i = 0
for line in content:
    
    if len(line) == 5:
        try:
            
            if abs(float(line[0]))>1000 or abs(float(line[1])) >1000 or abs(float(line[2])*100) >1000 or abs(float(line[3])*100) >1000:
                pass
            else:
                i = i+1
                x_offset.append(line[0])
                y_offset.append(line[1])
                x_speed.append(float(line[2])*100)
                y_speed.append(float(line[3])*100)
                axis.append(i)
                zero.append(0)
        except Exception as e:
            print(e)
            

print(x_offset)

pylab.figure(1)
pylab.subplot(211)
pylab.plot(axis,x_offset , 'b', axis,x_speed, 'r',axis,zero,'y')
pylab.title(u'x方向')
pylab.subplot(212)
pylab.plot( axis,y_offset, 'b',axis, y_speed,  'r',axis,zero,'y')
pylab.title(u'y方向')
pylab.show()

