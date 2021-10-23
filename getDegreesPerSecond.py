"""
Function to map relative motor speeds of C1C0 into the
degrees per second turning of the robot.
"""


from math import pi

def getDegreesPerSec(leftPower, rightPower):
    """
    left and right power are in range [-1.0, 1.0] and
    represent the percentage of the max motor rpm of each motor
    positive values represent forward and negative values are backwards

    returns magnitude of degrees per second turning/rotating
    """
    maxRpm = 150 #from datasheet - not checked yet
    wheelCirc = 13 #in inches, unit must be same as cicoWidth
    cicoWidth = 21 #in inches, unit must be same as wheelCirc

    if ((leftPower >=0 and rightPower >= 0) or (leftPower <=0 and rightPower <= 0)): #same direction
        speedLeft = leftPower*maxRpm*wheelCirc/60 #in inches/second
        speedRight = rightPower*maxRpm*wheelCirc/60 #in inches/second
        speedDif = abs(speedLeft-speedRight)
        degreesPerSec = (360*speedDif)/(2*pi*cicoWidth)
    elif (abs(leftPower) == abs(rightPower)): #rotating in place
        speed = leftPower*maxRpm*wheelCirc/60
        degreesPerSec = (360*speed)/(pi*cicoWidth)
    return degreesPerSec

# print(getDegreesPerSec(1.0,-1.0))

