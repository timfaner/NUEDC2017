def datapharse(raw):
    temp = None
    for i in range(len(raw)):
        if (raw[-i-2] == 0x55) and (raw[-i-1] == 0x53 )and (-i+8 < 0):
            temp = raw[-i-2:]
            break
    if temp:
        Xx = (temp[2] | temp[3] << 8)/32768*180
        Yy = (temp[4] | temp[5] << 8)/32768*180
        Zz = (temp[6] | temp[7] << 8)/32768*180
        return (Xx,Yy,Zz)
    else:
        return False

raw = b'US\x8d\xbe\x0b\xeb\x888\x1a\x0e\xd1US\x81\xc4\x8d\xeb\xc4>\x1a\x0e\x8fUS\x91\xcb\xad\xeb\xd7C\x1b\x0e\xdf'
print(datapharse(raw))