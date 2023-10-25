import math

#Sources
#https://www.google.com/search?q=convert+decimal+minutes+to+decimal+degrees&sca_esv=576533920&rlz=1C1CHBF_enUS856US856&sxsrf=AM9HkKkDQtQS7-VGS5PpWV2uOHrI7qlA6Q%3A1698253853531&ei=HUw5ZfH0H6CsqtsP4e2YEA&oq=convert+decimal+minutes+to+dec&gs_lp=Egxnd3Mtd2l6LXNlcnAaAhgCIh5jb252ZXJ0IGRlY2ltYWwgbWludXRlcyB0byBkZWMqAggAMggQABiKBRiRAjIGEAAYFhgeMgYQABgWGB4yBhAAGBYYHjIGEAAYFhgeMgYQABgWGB4yBhAAGBYYHjIGEAAYFhgeMgYQABgWGB4yBhAAGBYYHki7KFAAWNUicAJ4AZABAZgBrwGgAcoUqgEEMjMuOLgBA8gBAPgBAagCFMICBxAjGIoFGCfCAgQQIxgnwgILEC4Y1AIYsQMYgATCAhEQLhiABBixAxiDARjHARjRA8ICCxAAGIAEGLEDGIMBwgIIEAAYgAQYsQPCAgcQABiKBRhDwgITEC4YigUY5QQYQxiLAxigAxioA8ICBxAjGOoCGCfCAhYQABgDGI8BGOUCGOoCGLQCGIwD2AEBwgIWEC4YAxiPARjlAhjqAhi0AhiMA9gBAcICERAuGIoFGMcBGK8BGI4FGJECwgIQEC4YxwEYsQMY0QMYigUYQ8ICChAAGIAEGBQYhwLCAgUQABiABMICCBAAGIoFGIYD4gMEGAAgQYgGAboGBggBEAEYCw&sclient=gws-wiz-serp#kpvalbx=_Qkw5ZcPtFeWmqtsPit6iwAE_45
#https://learn.adafruit.com/adafruit-ultimate-gps?view=all


def returnRelativeGoal(goalLong, goalLat, goalAlt, currLong, currLat, currAlt):
    #Longitude and Latitude needs to be given in decimal degrees
    KM_TO_CM = 100000
    M_TO_CM = 100
    LAT_TO_KM = 111.32
    EARTH_CIRCUMFERENCE_KM = 40075

    lat_cm = (goalLat - currLat) * LAT_TO_KM * KM_TO_CM
    
    LONG_TO_KM = EARTH_CIRCUMFERENCE_KM * math.cos(goalLat - currLat) / 360
    long_cm = (goalLong - currLong) * LONG_TO_KM * KM_TO_CM

    alt_cm = (goalAlt - currAlt) * M_TO_CM

    return [lat_cm, long_cm, alt_cm]

def convertLatToDecimal(latString, north: bool):
    degree = int(latString[0:2])
    minute = int(latString[2:4])
    second = int(latString[5:])

    degrees = degree + minute/60 + second/3600
    if not north:
        degrees *= -1
    
    return degrees

def convertLongToDecimal(latString, east: bool):
    degree = int(latString[0:2])
    minute = int(latString[2:4])
    second = int(latString[5:])

    degrees = degree + minute/60 + second/3600
    if not east:
        degrees *= -1
    
    return degrees
    

def test():
    ponceLat1 = "3346.196"
    ponceLong1 = 8421.596
    ponceLat2 = 3346.182
    ponceLong2 = 8421.597

    print(convertLatToDecimal(ponceLat1, True))

    #Requires conversion from decimal minutes to degree decimals
    bobbyDodLat1 = 33.772028
    bobbyDodLong1 = 84.392861
    bobbyDodLat2 = 33.772972
    bobbyDodLong2 = 84.392871

    #lat_cm, long_cm, alt_cm = returnRelativeGoal(ponceLong2, ponceLat2, 0, ponceLong1, ponceLat1, 0)
    lat_cm, long_cm, alt_cm = returnRelativeGoal(bobbyDodLong2, bobbyDodLat2, 0, bobbyDodLong1, bobbyDodLat1, 0)
    print(lat_cm / 100)
    print(long_cm / 100)

test()