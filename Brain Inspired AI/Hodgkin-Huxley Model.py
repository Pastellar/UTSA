'''
Neuron Models: Hodgkin-Huxley
Haven Kotara
'''

import numpy as np
import scipy as sp
import matplotlib.pyplot as plt


# Parameters
Cm = 100e-12  # Membrane capacitance (pF)
g_K = 3.6e-6  # Na maximum conductances (µS)
g_Na = 12e-6  # K maximum conductances (µS)
g_L = 30e-9   # Leak conductances (nS)
E_Na = 0.045  # NA equilibrium potential (mV)
E_K = -0.082  # K equilibrium potential (mV)
E_L = -0.060  # Leak equilibrium potential (mV)

# Time Parameters
Tmax = 0.5      # Maximum of 500 ms
dt = 0.00001    # Increment time step by 10 µs
t = np.arange(dt, Tmax, dt)
num_steps = int(Tmax / dt)

# nA Current Parameters
I = np.linspace(0, 100, num=10) * 1e-10
I[50:500] = 1*10e-9


# Initialize Variables
Vm = np.zeros(num_steps)
n = np.zeros(num_steps)
m = np.zeros(num_steps)
h = np.zeros(num_steps)
num_spikes = []
total_spikes = 0

'''
These arrays will store the values of membrane potential, and
gating variables for potassium(K+), Sodium(Na+), and Leak channels,
at each time step.
The num_spikes array will store the spike counts for each injected
current value.
'''

# Calculate the rate constants
def alpha_m (Vm):
    return ((10**5) * (-Vm - 0.045)) / (np.exp(100 *(-Vm - 0.045)) - 1)

def beta_m (Vm):
    return (4 * 10**3) * np.exp((-Vm - 0.07) / 0.018)

def alpha_h (Vm):
    return 70 * np.exp(50 * (-Vm - 0.07))

def beta_h (Vm):
    return (10**3) / (1 + np.exp(100 * (-Vm - 0.04)))

def alpha_n (Vm):
    return ((10**4) * (-Vm - 0.06)) / (np.exp(100 * (-Vm - 0.06)) - 1)

def beta_n (Vm):
    return 125 * np.exp((-Vm - 0.07) / 0.08)


# Initialize the starting points
Vm[0] = -0.062
n[0] = alpha_n(Vm[0]) / (alpha_n(Vm[0]) + beta_n(Vm[0]))
m[0] = alpha_m(Vm[0]) / (alpha_m(Vm[0]) + beta_m(Vm[0]))
h[0] = alpha_h(Vm[0]) / (alpha_h(Vm[0]) + beta_h(Vm[0]))

# Simulate the Hodgkin-Huxley model
def hodgkin_huxley():
    global total_spikes

    for i in range(1, num_steps):
        # Update gating variables
        m[i] = m[i-1] + dt * (alpha_m(Vm[i-1]) * (1 - m[i-1]) - beta_m(Vm[i-1]) * m[i-1])
        h[i] = h[i-1] + dt * (alpha_h(Vm[i-1]) * (1 - h[i-1]) - beta_h(Vm[i-1]) * h[i-1])
        n[i] = n[i-1] + dt * (alpha_n(Vm[i-1]) * (1 - n[i-1]) - beta_n(Vm[i-1]) * n[i-1])

        # Compute membrane potential
        Vm[i] = Vm[i-1] + dt * (I[i] - g_K * (n[i]**4) * (Vm[i-1] - E_K) 
                                - g_Na * (m[i]**3) * h[i-1] * (Vm[i-1] - E_Na) 
                                - g_L * (Vm[i-1] - E_L)) / Cm

    # Count spikes
    spikes, _ = sig.find_peaks(Vm, height=0)  # Detect peaks above 0 mV
    num_spikes.append(len(spikes))
    total_spikes += len(spikes)

    return Vm, num_spikes, total_spikes


hodgkin_huxley(I)

# Plot the voltage for each injected value
plt.figure()
plt.plot(t, Vm)
plt.title('Time vs. Voltage')
plt.xlabel('Time (ms)')
plt.ylabel('Voltage (mV)')
plt.show()


'''
Part A
Plot I_inj vs the total number of spikes
'''

# Plot I_inj vs. total number of spikes
plt.plot(I, num_spikes)
plt.xlabel('Injected Current (nA)')
plt.ylabel('Spikes')
plt.title('Total Number of  vs. Injected Current')
plt.show()


'''
Part B
Find the minimum I_inj needed to cause a spike
'''

min_I_inj = 0.9e-9

'''
Current values were manually injected into the HH model
until a minimum current was determined.
The units are in nano Amps (nA), so the end value is e-9
The minimum current needed to cause a spike is 0.9e-9

Other tested currents include: 3e-9, -3e-9, 0.3e-9, 0.5e-9, and 0.8e-9
Currents above 0 (3e-9) displayed a higher frequency of spiking. 
Thus determining the minimum current must be less than 1. 
Currents between 0 and 1 displayed an inverted 'spike' followed by some smaller
'humps'. No spiking was observed until injecting 0.9e-9. 

'''
                
print('Minimum I_inj needed to cause a spike:', min_I_inj, 'nA')


'''
Part B
Find maximum current that can be passed to the neuron
'''

max_current = 10e-9

'''
Current values were manually injected into the HH model until a 
maximum current was determined.
The units are in nano Amps (nA), so the end value is e-9
The maximum current needed to cause a spike is 10e-9

Other tested currents include: 1000e-9, 500e-9, 400e-9, 80e-9, 50e-9, 25e-9, and 20 e-9
The large currents (Greater than 100) displayed a large initial spike with little to no
'humps' following the spike. After attempting a few currents in the 100s, it became clear the
maximum was a smaller current. 
Similar results were observed when testing values under 100; graphs displayed a large initial
spike, followed by some 'humps'. However, in these cases, the smaller the number got the more
arrows you would see instead of 'humps'. 
Eventually the maximum was determined at an injected value of 10e-9. 
The graph showed an initial spike followed by a string of arrows that never cross 0mV. 
'''        

print('Maximum current that can be passed to the neuron:', max_current, 'nA')


'''
Part C
Determine the membrane threshold for the Hodgkin-Huxley neuron
'''

threshold = -0.06

'''
While injecting values to find the minimum and maximum currents; a threshold
of -0.06 was determined after observing each graph. 
On the graphs, spikes appear to occur after passing -0.06; resulting in a sharp arrow. 
'''

print('Membrane threshold for the neuron:', threshold, 'nA')
