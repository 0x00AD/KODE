# -*- coding: utf-8 -*-
"""
Created with Anaconda 4.3.30,
in-built Spyder 3.1.4 IDE.

"""

import numpy as np
import matplotlib.pyplot as plt

class Spring:
    def __init__(self, name, force, distance):
        self.name = name
        self.k = force/distance

spring_50cm = Spring("50cm",1000,0.5)
spring_135cm = Spring("135cm",1000,1.25)
spring_rotary_2m = Spring("rotary ~2m", 1000,2)
spring_rotary_3m = Spring("rotary ~3m", 1000,3)


used_spring = spring_50cm

dt = 0.0001
totalTime = 0.060    # This is the main parameter to change
nsamples = int(totalTime/dt)+1

timespace = np.linspace(0, totalTime, nsamples)

class LM: # Linear motor
    def __init__(self, mass, delay, peakforce, contforce, 
                 contI, Kf, DC_busV, maxvelstart, maxvelstop):
        self.mass = mass
        self.delay = delay/1000     # ms to s
        self.peakforce = peakforce
        self.contforce = contforce
        self.contI = contI
        self.Kf = Kf # Force constant, N/A_rms, power effectiveness.
        self.DC_busV = DC_busV
        self.maxvelstart = maxvelstart
        self.maxvelstop = maxvelstop

LMTC4 = LM(2.8, 1.2, 801, 200, 2.6, 77, 325, 2, 4)
LMTC3 = LM(2.1, 2.6, 603, 151, 2.6, 58, 325, 4, 6)
LMTE2 = LM(7.8, 6.5, 1284, 321, 7.7, 41.7, 325, 6, 8)
LMSA22 = LM(2.2, 4.9, 1023, 362, 3.9, 92.5, 600, 2, 4)
LMSA22L = LM(2.2, 4.6, 1023, 362, 8.8, 41.4, 600, 5, 9)
LMTB3 = LM(1.25, 1, 312, 78, 1.3, 60, 325, 4, 6)
LMTB2 = LM(0.88, 1, 208, 52, 1.3, 40, 325, 6, 9)



# I am looking for the time that a motor makes a swing +-10cm
# For this, motor needs to accelerate the inertia mass of itself,
# as well as the expected intertia mass of a spring + all the shit below it
# denoted together as extMass

extMass = 2

# The force will depend on velocity, as indicated by the datasheet. 
# When approaching critical velocity maxvelstart, the force will linearly
# decrease and hit 0N at the maxvelstop.
# The following function is used to limit the force at the critical speeds. 

def limitForceByVel(vel, motor):
    returnforce = -1
    returnforce = np.piecewise(vel, 
                      [vel < motor.maxvelstart,
                       motor.maxvelstart <= vel < motor.maxvelstop, 
                       vel >= motor.maxvelstop],
                 [lambda vel: motor.peakforce, 
                  lambda vel: motor.peakforce*(1-(vel-motor.maxvelstart)/
                                 (motor.maxvelstop - motor.maxvelstart)),
                  lambda vel: 0])
    return float(returnforce)

# Main pipeline:
# force -> acceleration -> speed -> distance

# force decreases as the motor contracts the spring

#####
# This is the main function of th programe. It outputs the
# matrix with the following rows:
# 0) Motor's displacement over time
# 1) Motor's velocity over time
# 2) Motor's acceleration over time
# 3) Force enacted by motor and spring over time.
#####
def getWaveforms(motor):
    force = 0
    acc = 0
    vel = 0
    dist = 0
    
    # Linspaces would be used for logging purposes
    force_space = np.zeros(nsamples)
    acc_space = np.zeros(nsamples)
    vel_space = np.zeros(nsamples)
    dist_space = np.zeros(nsamples)
    
    mass = motor.mass + extMass
    for i, t in np.ndenumerate(timespace):
        if t <= motor.delay: # Start movement after the delay elapsed
            pass
        else:
            if t <= 1: # Maximum documentation time for peak force is 1s
                force = limitForceByVel(vel, motor)
            else:
                force = motor.contforce # Simplification, but we don't really
                                        # use that region
            
            # Let's at first accelerate, then deccelerate for half of the time
            
            if t < totalTime/2:
            
                # Effective force decreases as spring contracts and counteracts
                # the motor.          
                force -= used_spring.k*dist
                
                acc = force/mass
                vel += acc*dt
                
                dist += vel*dt
                
                # Log things
                dist_space[i[0]] = dist # i[0] since ndenumerate returns i as 
                                        # tuple 1x1
                vel_space[i[0]] = vel
                acc_space[i[0]] = acc
                force_space[i[0]] = force
            
            else:
                
                # The motor has some delay before it can act in the opposite
                # direction. During the period of its inactivity only the 
                # spring will act. 
                if t < totalTime/2 + motor.delay:
                    force = used_spring.k*dist
                
                # Now contracted spring helps motor's decceleration      
                force += used_spring.k*dist
                
                acc = force/mass
                vel -= acc*dt   # Decceleration decreases velocity
                
                dist += vel*dt
                
                # Log things
                dist_space[i[0]] = dist # i[0] since ndenumerate returns i as 
                                        # tuple 1x1
                vel_space[i[0]] = vel
                acc_space[i[0]] = acc
                force_space[i[0]] = force
    
    return np.stack((dist_space,vel_space,acc_space,force_space))

# Get results (displacement, velocity, acceleration and force) of motors and 
# plot the displacement waveform. 
result_LMTC4 = getWaveforms(LMTC4)
result_LMTC3 = getWaveforms(LMTC3)
result_LMTE2 = getWaveforms(LMTE2)
result_LMSA22 = getWaveforms(LMSA22)
result_LMSA22L = getWaveforms(LMSA22L)
result_LMTB3 = getWaveforms(LMTB3)
result_LMTB2 = getWaveforms(LMTB2)

''' All motors
waveform_LMTC4, = plt.plot(result_LMTC4[0], label='LMTC4')
waveform_LMTC3, = plt.plot(result_LMTC3[0], label='LMTC3')
waveform_LMTE2, = plt.plot(result_LMTE2[0], label='LMTE2')
waveform_LMSA22, = plt.plot(result_LMSA22[0], label='LMSA22')
waveform_LMSA22L, = plt.plot(result_LMSA22L[0], label='LMSA22L')
waveform_LMTB3, = plt.plot(result_LMTB3[0], label='LMTB3')
waveform_LMTB2, = plt.plot(result_LMTB2[0], label='LMTB2')



plt.legend([waveform_LMTC4, waveform_LMTC3, waveform_LMTE2,
            waveform_LMSA22,waveform_LMSA22L,
            waveform_LMTB3,waveform_LMTB2],
           ['LMTC4', 'LMTC3' 'LMTE2','LMSA22','LMSA22L,','LMTB3','LMTB2'])
'''
# Energy efficient only

waveform_LMTC3, = plt.plot(result_LMTC3[0], label='LMTC3')
waveform_LMTC4, = plt.plot(result_LMTC4[0], label='LMTC4')
waveform_LMTB3, = plt.plot(result_LMTB3[0], label='LMTB3')
waveform_LMTB2, = plt.plot(result_LMTB2[0], label='LMTB2')
plt.legend([waveform_LMTC4,
            waveform_LMTC3,
            waveform_LMTB3,
            waveform_LMTB2
            ],
           ['LMTC4', 'LMTC3', 'LMTB3', 'LMTB2'])

plt.title(str(totalTime*1000)+" ms,  "+
            str("Spring AR length: "+str(used_spring.name)))
plt.grid()
plt.minorticks_on()
plt.grid(which='minor')



# Power consumptions
# Operation time in minutes
def getEnergy(motor, operation_time):
    return motor.contI*motor.DC_busV*operation_time*60

operation_time = 15 # minutes of operation at continuous current
print("LMTC4: "+str(int(getEnergy(LMTC4,operation_time)))+" J")
print("LMTE2: "+str(int(getEnergy(LMTE2,operation_time)))+" J")
print("LMSA22: "+str(int(getEnergy(LMSA22,operation_time)))+" J")
print("LMSA22L: "+str(int(getEnergy(LMSA22L,operation_time)))+" J")
print("LMTB3: "+str(int(getEnergy(LMTB3,operation_time)))+" J")
print("LMTB2: "+str(int(getEnergy(LMTB2,operation_time)))+" J")
