'''
Neuron Models: LIF Analysis Case 2
Haven Kotara
'''

import numpy as np
import matplotlib.pyplot as plt


# LIF Neuron Parameters
Cm = 100e-12     # Capacitance          picoFarads
Rm = 100e6       # Membrane resistance  MegaOhms
Vrest = -65e-3   # Reset voltage        MiliVolts
Vth = -25e-3     # Threshold            MiliVolts

# Time Parameters
Tmax = 0.3   # Maximum of 300 ms
dt = 0.001   # Increment time step by 1 ms
t = np.arange(0, Tmax, dt)
num_steps = int(Tmax / dt)


# This LIF neuron will have two inputs
I1 = np.array([0,0,0,0,0,1e-9]*50) #300ms of data
I2 = np.array([0,0,1e-9,0,0,0]*50)


# Initialize variables and arrays
Vm = np.zeros(num_steps)    # Voltage   MiliVolts
Vm[0] = Vrest


# LIF simulation for different I_inj values
def LIF(I1, I2):
    num_spikes = 0  # number of spikes
    times = []      # the times they occur

    for i in range(1, num_steps):
        Vm[i] = Vm[i-1] + dt * (-(Vm[i-1] - Vrest) + (Rm * (I1[i-1] + I2[i-1]))) / (Cm * Rm)

        # Check if a spike occurred
        if Vm[i] >= Vth:
            num_spikes += 1
            times.append(i)

            # Reset the voltage
            Vm[i] = Vrest

    return num_spikes, times


'''
Repeating Case 1:
Part A:
Simulate the LIF neuron and plot time vs. the spiking activity of the
neuron and the inputs I1 and I2 in three subplots.
'''

# Simulate the LIF neuron
num_spikes, times = LIF(I1, I2)

print('The number of spikes at Vth = -25 mV: ', num_spikes)

fig, axs = plt.subplots(3)
fig.tight_layout(pad=3.3)
# Plot Time vs Spiking Activity
axs[0].set_title('Time vs Voltage')
axs[0].plot(t, Vm, label='Vth = -25 mV')
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
# Plot I2 inputs over time
axs[2].set_title('Time vs I2 Inputs')
for i in range(len(I2)):
    if I2[i] != 0:
        axs[2].axvline(i, linewidth=1, ymax=1, color='r')
axs[2].set_xlabel('Time (ms)')
axs[2].set_ylabel('Current (nA)')
plt.show()


'''
Part B:
Set Vth = −0.05. Simulate the LIF neuron and plot time vs. the spiking
activity of the neuron and the inputs I1 and I2 in three subplots.
'''

Vth = -50e-3     # Threshold     MilliVolts

# Simulate the LIF neuron again
num_spikes, times = LIF(I1, I2)

print('The number of spikes at Vth = -50 mV: ', num_spikes)

fig, axs = plt.subplots(3)
fig.tight_layout(pad=3.3)
# Plot Time vs Spiking Activity
axs[0].set_title('Time vs Voltage')
axs[0].plot(t, Vm, label='Vth = -50 mV')
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
# Plot I2 inputs over time
axs[2].set_title('Time vs I2 Inputs')
for i in range(len(I2)):
    if I2[i] != 0:
        axs[2].axvline(i, linewidth=1, ymax=1, color='r')
axs[2].set_xlabel('Time (ms)')
axs[2].set_ylabel('Current (nA)')
plt.show()


'''
Part C:
Compare the plots of the spike activity from the simulations in (a) and
(b), what can be concluded about the impact of Vth.
'''

'''
In simulation A, the membrane potential visually displays consistent, slightly wide, arrows increasing 
to -0.03 mV and remaining between -0.03 mV and -0.04 mV. No action potential is recorded.

In simulation B, the threshold was lowered to -50 miliVolts. The neuron was able to reach the threshold
and produced action potentials 49 times throughout the simulation.

Decreasing the threshold value helps the neuron reach the firing threshold more easily and generates a higher
firing rate. 
'''

'''
Part D:
Now set RM = 100KΩ and Vth = -25mV. Simulate the LIF neuron and
plot time vs. the spiking activity of the neuron and the inputs I1 and I2
in three subplots.
'''

Rm = 100e3       # Resistance   Kilo Ohms
Vth = -25e-3     # Threshold    Milivolts

# Simulate the LIF neuron again
num_spikes, times = LIF(I1, I2)

print('The number of spikes at Rm = 100 KΩ and Vth = -25 mV: ', num_spikes)

fig, axs = plt.subplots(3)
fig.tight_layout(pad=3.3)
# Plot Time vs Spiking Activity
axs[0].set_title('Time vs Voltage')
axs[0].plot(t, Vm, label='Rm = 100KΩ and Vth = -25 mV')
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
# Plot I2 inputs over time
axs[2].set_title('Time vs I2 Inputs')
for i in range(len(I2)):
    if I2[i] != 0:
        axs[2].axvline(i, linewidth=1, ymax=1, color='r')
axs[2].set_xlabel('Time (ms)')
axs[2].set_ylabel('Current (nA)')
plt.show()


'''
Part E:
Compare the plots of the spike activity with the original simulation in (a).
'''

'''
In simulation C, the voltage graph appears inverted with elongated, downwards-facing, arrows. These arrows are
very close together and spike approximately every 3 milliseconds for a total of 99 spikes. 
In comparison, simulation A's upward-facing, shorter, wider, arrows which produce no spikes and remain between
-0.03 and -0.04. 
The threshold is the same for both simulations, the only difference is that the resistance is 100 kiloOhms in simulation C. 

Decreasing the resistance allowed the neuron to generate action potentials. Simulation C is also the only simulation that
produced any spikes. This demonstrates the impact resistance can have on a model; even though the threshold was too high
in the original simulation, decreasing the resistance made it easier for the neuron's membrane potential to reach the
firing threshold.

'''


'''
Part F:
How did τM change and how did this impact the spike activity?
'''

'''
Time membrane (τM) = capacitance (CM) * resistance (RM). The decreased resistance also decreased the time membrane.
Because the value of resistance affects the value of the time membrane, the effects of adjusting the resistance are
similar to the affect of having a lower or higher time membrane value. 
In the case of simulation C, having a lower resistance, and thus lesser time membrane, made it easier for the neuron
to spike and have a higher firing rate.
'''


'''
Case 2:
Part A:
Summarize the impact of changing the threshold and the membrane resistance on the neurons activity.
'''


'''
Adjusting the threshold affects the excitability of a neuron. 
Increasing the threshold makes it difficult for the neuron to reach the firing threshold 
and generate an action potential. A stronger input current is needed to trigger a spike.
** Less frequent firing

Decreasing the threshold makes it easier for the neuron to generate spikes. This allows
weaker currents to generate action potential.
** More frequent firing

Increasing the resistance makes it harder for the neuron to depolarize and reach the firing threshold
because it reduces the flow of current through the membrane, slowing down the charging process.
Decreasing the resistance enhances the neuron's ability to depolarize and reach the firing threshold
because it allows more current to flow through the membrane, accelerating the charging process.

In the first two simulations, adjusting the threshold wasn't enough for the neuron to produce action potential.
While decreasing a threshold can generate more spikes, decreasing it too much leads to a similar situation to 
having too high of a threshold.
Simulation C demonstrated that, in some cases, other values such as the resistance are required to be adjusted
in order for the neuron to produce spikes. The adjustment of the resistance in this simulation also led to a
high firing rate.
'''


'''
Part B:
How would the neurons activity change if we adjusted the capacitance
instead of resistance, why is it different from changing the resistance (Hint:
Look at the variables in the LIF equation)
'''

'''
Increasing the capacitance increases the neuron's ability to store charge for a given
current input. A higher capacitance slows down the charging and discharging process,
making the neuron slower to reach the firing threshold and return to its resting state.
** Slower firing rate and longer inter-spike intervals

Decreasing the capacitance reduces the neuron's ability to store charge. A lower capacitance
speeds up the charging and discharging process, allowing the neuron to reach the firing
threshold more quickly and return to its resting state.
** Faster firing rate and shorter inter-spike intervals

Adjusting the capacitance affects the timing and charge dynamics of the neurons activity.
In other words, changing the capacitance directly influences the charging and discharging
time properties of the neuron. 

Altering the resistance influences the neuron's excitability and responsiveness to input signals.
Changes alter the ease with which a neuron can generate action potentials.
'''