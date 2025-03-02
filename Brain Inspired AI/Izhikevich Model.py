'''
Neuron Models: Izhikevich
Haven Kotara

The Izhikevich model was designed to be as biologically plausible as the 
Hodgkin-Huxley model but as computationally efficient as the LIF model.
'''

import numpy as np
import scipy as sp
import matplotlib.pyplot as plt


# Simulate the izhikevich model
def izhikevich(I):
    num_spikes = 0

    for i in range(1, num_steps):
        # Calculate membrane potential
        Vm[i] = Vm[i-1] + dt * ((0.04 * Vm[i-1]**2) + (5 * Vm[i-1]) + 140 - u[i-1] + I)

        # Calculate differential equations
        u[i] = u[i-1] +  dt * (a * ((b * Vm[i-1]) - u[i-1]))

        if Vm[i] >= Vth:
            num_spikes += 1
            Vm[i] = c
            
            u[i] = u[i] + d
    return num_spikes


# Plot Time vs Spike Activity
def graphVoltage(t, Vm, type, color):
    plt.figure()
    plt.plot(t, Vm, 'tab:'+color, label=type)
    plt.legend(loc='upper left')
    plt.title('Time vs. Voltage')
    plt.xlabel('Time (ms)')
    plt.ylabel('Voltage (mV)')
    return plt.show()


# Graph Iinj vs Total Number of Spikes
def graphSpikes(I, spikes):
    plt.figure()
    plt.plot(Iinj, spikes)
    plt.title('Iinj vs. Total Number of Spikes')
    plt.xlabel('Iinj')
    plt.ylabel('Spikes')
    return plt.show()


# Time Parameters
Tmax = 500  # Maximum of 500 ms
dt = 0.5    # Increment time step by half a millisecond
t = np.arange(0, Tmax, dt)
num_steps = int(Tmax / dt)

# Initialize Variables
Vth = 30e-3                 # Firing threshold (mV)
Vm = np.zeros(num_steps)    # Membrane potential (mV)
u = np.zeros(num_steps)     # Membrane recovery variable
total_spikes = 0            # Total spike counts
spikes = []                 # List to hold spike counts
Iinj = []                   # List to hold Iinj values


'''
Tonic Spiking
'''
# Parameters
Vrest = -70  # Reset Voltage    Milivolts (mV)
a = 0.02
b = 0.2
c = -65
d = 6
I = 10          # NanoAmps

# Initialize the starting points
Vm[0] = Vrest
u[0] = Vm[0] * b

num_spikes = izhikevich(I)
print('Tonic Spiking: ' + str(num_spikes))
graphVoltage(t, Vm, 'Tonic Spiking', 'blue')

# Simulate for different Iinj values
for i in range(1, 50):
    num_spikes = izhikevich(i)
    Iinj.append(i)
    total_spikes += num_spikes
    spikes.append(num_spikes)

firing_rate = num_spikes * 2
graphVoltage(t, Vm, 'Tonic Spiking', 'blue')
print('The firing rate of the Izhikevich neuron is ' + str(firing_rate) + ' spikes per second.')


'''
Phasic Spiking
'''
# Parameters
Vrest = -64  # Milivolts (mV)
a = 0.02
b = 0.25
c = -65
d = 6
I = 10          # NanoAmps

# Initialize the starting points
Vm[0] = Vrest
u[0] = Vm[0] * b

num_spikes = izhikevich(I)
print('Phasic Spiking: ' + str(num_spikes))
graphVoltage(t, Vm, 'Phasic Spiking', 'red')

'''
Tonic Bursting
'''
# Parameters
Vrest = -70  # Milivolts (mV)
a = 0.02
b = 0.2
c = -50
d = 2
I = 10          # NanoAmps

# Initialize the starting points
Vm[0] = Vrest
u[0] = Vm[0] * b

num_spikes = izhikevich(I)
print('Tonic Bursting: ' + str(num_spikes))
graphVoltage(t, Vm, 'Tonic Bursting', 'green')

'''
Phasic Bursting
'''
# Parameters
Vrest = -64  # Milivolts (mV)
a = 0.02
b = 0.25
c = -55
d = 0.05
I = 10          # NanoAmps

# Initialize the starting points
Vm[0] = Vrest
u[0] = Vm[0] * b

num_spikes = izhikevich(I)
print('Phasic Bursting: ' + str(num_spikes))
graphVoltage(t, Vm, 'Phasic Bursting', 'orange')

'''
Mixed Mode
'''
# Parameters
Vrest = -70  # Milivolts (mV)
a = 0.02
b = 0.2
c = -55
d = 4
I = 10          # NanoAmps

# Initialize the starting points
Vm[0] = Vrest
u[0] = Vm[0] * b

num_spikes = izhikevich(I)
print('Mixed Mode spikes: ' + str(num_spikes))
graphVoltage(t, Vm, 'Mixed Mode', 'purple')


'''
Graph Iinj vs Total Number of Spikes
'''
graphSpikes(I, spikes)

print('The minimum Iinj needed to cause a spike: 2.7')
print('The firing rate saturates at: 734')
''''''
