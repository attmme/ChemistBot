#!/usr/bin/env python
# -*- coding: utf-8 -*-

import mysql.connector
from mysql.connector import Error

import serial
import time
import sys


# Obrim la conexió amb l'USB
ser = serial.Serial("/dev/ttyUSB0")

# print(ser.portstr) # Debug
# Esperem per a que no doni error
time.sleep(2)

# Agafem els arguments
input1 = sys.argv;
arguments = (input1[1] + " " + input1[2] + " " + input1[3]);

# Enviem el comando que volem
ser.write(arguments)

# Tanquem la conexió amb l'USB
ser.close()   


# Fem la conexio a la base de dades
try:
	# Credencials
    connection = mysql.connector.connect(host='localhost', database='chemistbot', user='root', password='')
	
	# Si es conecta correctament, alterem el valor a la base de dades
    if connection.is_connected():
        cursor = connection.cursor()
        cursor.execute("UPDATE medicaments SET quantitat = quantitat - " + str(input1[3]) + " WHERE cartutx = " + str(input1[2]) )
	print(str(input1[3]));
	connection.commit()

# Errors per debug
except Error as e:
	print("Error while connecting to MySQL", e)
finally:
	# Es tanca la conexio
	if (connection.is_connected()):
		cursor.close()
		connection.close()
		print("MySQL connection is closed")

