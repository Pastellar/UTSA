'''
Neuron Models: Leaky Integrate and Fire
Haven Kotara

The LIF model is a simple description of a neuron with four features:
• The entire cell has a single voltage Vm
• The cell has a membrane capacitance Cm
• The cell has a leak channel with a resistance Rm (or conductance GM = 1Rm)
• The cell generates an action potential only when Vm crosses a threshold Vth
'''

import numpy as np
import matplotlib.pyplot as plt


# LIF Neuron Parameters
Cm = 100e-12     # Capacitance          picoFarads
Rm = 100e6       # Membrane resistance  MegaOhms
Vrest = -65e-3   # Reset voltage        MiliVolts
Vth = -25e-3     # Threshold            MiliVolts


# Time Parameters
Tmax = 0.5   # Maximum of 500 ms
dt = 0.001   # Increment time step by 1 ms
t = np.arange(0, Tmax, dt)
num_steps = int(Tmax / dt)


# Injected nA current values
I = np.linspace(0, 1, num_steps) * 1e-9


# Initialize variables and arrays
Vm = np.zeros(num_steps)    # Voltage   MiliVolts
Vm[0] = Vrest
spikes = []     # Array to hold spike counts


# LIF simulation for different I_inj values
def LIF(I):
    num_spikes = 0 # number of spikes
    times = []     # the times they occur
    Iinj = []      # the injected values they occur at
    
    for i in range(1, num_steps):
        # I[i-1] = 0.5e-9   # For manually injecting values
        # I[i] = 0.5e-9

        Vm[i] = Vm[i-1] + dt * (-(Vm[i-1] - Vrest) + (Rm * I[i-1])) / (Cm * Rm)
        

        # If the firing threshold is reached
        if Vm[i] >= Vth:
            num_spikes += 1
            spikes.append(num_spikes)
            Iinj.append(I[i])
            times.append(i)
            Vm[i] = Vrest
    

    return num_spikes, times, Iinj


# Simulate the neuron
num_spikes, times, Iinj = LIF(I)
# num_spikes, times, Iinj = LIF(-0.5e-9)

# Plots the spiking activity over time
plt.figure()
plt.plot(t, Vm, 'tab:red')
# plt.plot(t, Vm, 'tab:red', label='Iinj = 0.5 nA')
for i in times:
    plt.axvline(t[i], linewidth=0.9, linestyle='--', color='g')
# plt.legend(loc='upper left')
plt.title('Time vs. Voltage')
plt.xlabel('Time (ms)')
plt.ylabel('Voltage (mV)')
plt.show()

'''
Part A:
Plot I_inj vs Total Number of Spikes
'''
    
# Plotting Iinj vs Total Number of Spikes
plt.figure()
plt.scatter(Iinj, spikes)
# plt.scatter(Iinj, spikes, label='Iinj = 0.5 nA')
# plt.legend(loc='upper left')
plt.xlabel('Injected Current (nA)')
plt.ylabel('Spikes')
plt.title('Injected Current vs. Total Number of Spikes')
plt.show()


'''
Part B:
Find the minimum current needed to cause a spike
'''

# Found by manually injecting values
min_I_inj = 0.5e-9

print('Minimum I_inj needed to cause a spike:', min_I_inj, 'nA')


'''
Part B:
Find when the firing rate saturates
'''

# Found by manually injecting values
saturation = 4e-9

print('Firing rate saturates at ', saturation, 'nA')

''''''