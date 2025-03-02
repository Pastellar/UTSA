#Substitute <orgname> for the city name
s/<orgname>/City of Gainsville, Florida/g

#Redact DL numbers and remove the numbers after the X's
s/DL [0-9]{6}.*/DL XXXXXX/g

#Redact Credit Card numbers
s/\t[4][0-9]{3}-[0-9]{4}-[0-9]{4}-|\t[4][0-9]{11}/\tVISA-/g
s/\t[5][0-9]{3}-[0-9]{4}-[0-9]{4}-|\t[5][0-9]{11}/\tMC-/g
s/\t[3][0-9]{3}-[0-9]{6}-[0-9]{1}|\t[3][0-9]{11}/\tAMEX-/g
s/\t[6][0-9]{3}-[0-9]{4}-[0-9]{4}-|\t[6][0-9]{11}/\tDISC-/g

#Redact TX License Plate numbers
s/TX [0-9]{2}.....| TX[0-9].....|TX [A-Z]{3}[0-9]{4}|TX [A-Z]{3}-[0-9]{4}/TX XXXXXX/g