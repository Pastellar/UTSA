'''
Neuron Models: Computation with a LIF Neuron
Haven Kotara
'''

import numpy as np
import matplotlib.pyplot as plt


# Time Parameters
Tmax = 0.5   # Maximum of 500 ms
dt = 0.001   # Increment time step by 1 ms
t = np.arange(0, Tmax, dt)
num_steps = int(Tmax / dt)


# Initialize variables and arrays
Vm = np.zeros(num_steps)    # Voltage           MiliVolts
Vrest = -65e-3              # Reset voltage     MiliVolts
Vm[0] = Vrest


# LIF simulation for one input
def LIF(I1):
    num_spikes = 0  # number of spikes
    times = []     # the times they occur

    for i in range(1, num_steps):
        Vm[i] = Vm[i-1] + dt * (-(Vm[i-1] - Vrest) + (Rm * I1[i-1])) / (Cm * Rm)

        if Vm[i] >= Vth:
            num_spikes += 1
            times.append(i)
            Vm[i] = Vrest

    return num_spikes, times


# Graph for one input
def graph(t, Vm, times, I1, label):
    fig, axs = plt.subplots(2)
    fig.tight_layout(pad=3.3)
    # Plot Time vs Spiking Activity
    axs[0].set_title('Time vs Voltage')
    axs[0].plot(t, Vm, label=label)
    for i in times:
        axs[0].axvline(t[i], linewidth=0.9, linestyle='--', color='r')
    axs[0].set_xlabel('Time (ms)')
    axs[0].set_ylabel('Voltage (mV)')
    axs[0].legend(loc='upper left')
    # Plot I1 inputs over time
    axs[1].set_title('Time vs I1 Inputs')
    for i in range(len(I1)):
        if I1[i] != 0:
            axs[1].axvline(i, linewidth=1, ymax=1, color='g')
    axs[1].set_xlabel('Time (ms)')
    axs[1].set_ylabel('Current (nA)')
    
    return plt.show()


'''
Part A:
Use the data provided and design three neurons that fire a total of 25
times, 50 times, and 100 times through the simulation respectively.
Data:
I1 = np.array([0,0,1e-9,0,1e-9]*100) #500ms of data
Plot time vs. the spike activity of the three neurons in separate subplots.
'''

# Current 
I1 = np.array([0, 0, 1e-9, 0, 1e-9]*100) #500ms of data

# Neuron 1: 25 spikes
Cm = 125e-12     # Capacitance          picoFarads
Rm = 120e6       # Membrane resistance  MegaOhms
Vth = -30e-3     # Threshold            MiliVolts

# Simulate the LIF neuron
num_spikes, times = LIF(I1)

print('The number of spikes for Neuron 1: ', num_spikes)
graph(t, Vm, times, I1, 'Neuron 1')


# Neuron 2: 50 spikes
Cm = 156e-12     # Pico Farads
Rm = 97e6        # Mega Ohms
Vth = -48e-3     # Mili Volts

# Simulate the LIF neuron
num_spikes, times = LIF(I1)

print('The number of spikes for Neuron 2: ', num_spikes)
graph(t, Vm, times, I1, 'Neuron 2')


# Neuron 3: 100 spikes
Cm = 156e-12     # Pico Farads
Rm = 97e6        # Mega Ohms
Vth = -58e-3     # Mili Volts

# Simulate the LIF neuron
num_spikes, times = LIF(I1)

print('The number of spikes for Neuron 3: ', num_spikes)
graph(t, Vm, times, I1, 'Neuron 3')