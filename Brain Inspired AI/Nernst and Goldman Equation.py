'''
Neuron Models: Nernst Equation and Goldman Equation
Haven Kotara

Use the Nernst Equation to determine the resting potentials for Na+, K+, and Cl-.
And the Goldman Equation will be used to determine the resting membrane potential.
'''

import numpy as np
import math


# Constants
R = 8.314           # Gas constant
F = 96485           # Faraday's constant
T = 273.15 + 37     # Temperature in Kelvins
z = 1               # Valence of ions


# Apply the Nernst Equation to calculate the equilibrium potential for an ion
def nernst (Xo, Xi):
    '''
    R:  Gas constant
    F:  Faraday's constant
    T:  Temperature in Kelvins
    z:  Valence of ions
    Xi: Inner membrane concentration
    Xo: Outer membrane concentration
    '''
    E_X = ((R * T) / (z * F)) * (np.log(Xo / Xi))
    return E_X


# Apply the Goldman Equation to calculate the resting membrane potential of the ions
def goldman (E_Na, E_K, E_Cl, P_Na, P_K, P_Cl):
    '''
    E_Na:   Nernst potential of Na+
    E_K:    Nernst potential of K+
    E_Cl:   Nernst potential of Cl-
    P_Na:   Permeability of Na+ ions
    P_K:    Permeability of K+ ions
    P_Cl:   Permeability of Cl- ions
    '''
    return ((R * T) / F) * np.log(((P_K * Xo_K) + (P_Na * Xo_Na) + (P_Cl * Xi_Cl)) / ((P_K * Xi_K) + (P_Na * Xi_Na) + (P_Cl * Xo_Cl)))


# Na+, K+, and Cl- Constants
Xo_Na = 440
Xi_Na = 50
P_Na  = 0.04

Xo_K = 20
Xi_K = 400
P_K  = 1.00

Xo_Cl = 560
Xi_Cl = 40      # 40 - 150
P_Cl  = 0.45

# Calculate the Nernst potentials for Na+, K+ and Cl-
E_Na = nernst(Xo_Na, Xi_Na)
E_K  = nernst(Xo_K, Xi_K)
E_Cl = nernst(Xo_Cl, Xi_Cl)

# Calculate Goldman potential
V_rest = goldman(E_Na, E_K, E_Cl, P_Na, P_K, P_Cl)

# Print the values
print('Resting potential for Na+: %.3f' % E_Na + ' mV\n' )
print('Resting potential for K+: %.3f' % E_K + ' mV\n' )
print('Resting potential for Cl-: %.3f' % E_Cl + ' mV\n' )
print('Resting membrane potential: %.3f' % V_rest + ' mV\n' )
