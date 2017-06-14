# -*- coding: utf-8 -*-
import sqlite3
import serial, json, re
import time
import sys
from pymongo import MongoClient
import RPi.GPIO as GPIO
from time import sleep
from urllib.request import urlopen

sensordevice= "ttyACM0"
actuadoresdevice= "ttyACM1"
client = MongoClient('mongodb://juagarfer4:juagarfer4@ds157621.mlab.com:57621/sedu')

myAPI = "EW34AOYGE91YV5VO"
baseURL = 'https://api.thingspeak.com/update?api_key=%s' % myAPI

arg = ""

AActuadores = serial.Serial('/dev/'+actuadoresdevice, 9600)


def activarVentilador():
    #AActuadores = serial.Serial('/dev/'+actuadoresdevice, 9600)
    AActuadores.write("[L]".encode())
    #AActuadores.close()

def activarMotor():
    #AActuadores = serial.Serial('/dev/'+actuadoresdevice, 9600)
    AActuadores.write("[T]".encode())
    #AActuadores.close()


def is_int(s):
    try: 
        int(s)
        return True
    except ValueError:
        return False

def initializeBD():
    conn = sqlite3.connect('/home/pi/Desktop/Source/Communication/DB.db')
    c = conn.cursor()
    # Create table
    c.execute('''CREATE TABLE IF NOT EXISTS samples
                 (temperature, humidity, luminosity, date, fanstatus, motorstatus)''')
    return conn, c

def insertSampleBD(conn, c, muestra):
    c.execute("INSERT INTO samples VALUES ('"+
              str(muestra["T"])+"','"+
              str(muestra["H"])+"','"+
              str(muestra["L"])+"','"+
              str(muestra["D"])+"','"+
              str(muestra["estadoVentilador"])+"','"+
              str(muestra["estadoMotor"])+"')")
    conn.commit()

def insertSambleMongo(m):
    global client
    sedu = client['sedu'].sedu
    return sedu.insert_one(m).inserted_id

def llamarActuadoresByMuestra(muestra):
    AActuadores = None
    estadoVentilador = 0
    estadoMotor = 0
    ''' LUMINOSIDAD '''
    if float(muestra["L"]) < 250:
        print("Luminosidad baja. Enviando instrucción a actuadores")
        try:
            activarVentilador()
            estadoVentilador = 1
        except serial.SerialException:
            print("Error: Actuadores no conectados")

    ''' TEMPERATURA '''
    if float(muestra["T"]) > 27.0:
        print("Temperatura alta. Enviando instrucción a actuadores")
        try:
            activarMotor()
            estadoMotor = 1
        except serial.SerialException:
            print("Error: Actuadores no conectados")
    return (estadoVentilador, estadoMotor)

def checkSyntax(ln, charord):
    return len(ln) != 0 and ln[0] == "[" and ln[-1] == "]" and charord == ln[1]

def getData(ser, charord):
    res = ""
    ser.write(("["+charord+"]\n").encode())
    ln = str(ser.readline())
    sys.stdout.flush()
    sys.stdin.flush()
    ln = ln.replace('\\n','').replace('\\r','').replace('b','').replace('\'','')
    while not checkSyntax(ln, charord):
        ser.write(("["+charord+"]\n").encode())
        ln = str(ser.readline())
        sys.stdout.flush()
        sys.stdin.flush()
        ln = ln.replace('\\n','').replace('\\r','').replace('b','').replace('\'','')
    res = ln[4:-1]
    return res
    

def lecturaDato(n = -1):
    ASensor = serial.Serial('/dev/'+sensordevice, 9600)
    conn, c = initializeBD()
    i = 0
    while n == -1 or i < n:
        muestra = {c: getData(ASensor, c) for c in ['T', 'H', 'L', 'D']}
        estadoVentilador, estadoMotor = llamarActuadoresByMuestra(muestra)
        muestra["estadoVentilador"] = estadoVentilador
        muestra["estadoMotor"] = estadoMotor
        insertSampleBD(conn, c, muestra)
        insertSambleMongo(muestra)
        insertSampleTS(muestra)
        print(muestra)
        i+=1

def getNumeroMuestras():
    global arg
    print("Introduzca el número de muestras a leer")
    n = -2
    if arg == '1':
        n = -1
    while not is_int(n) or int(n) < -1:
        n = str(input("n (-1 para no parar): "))
    lecturaDato(int(n))

def accionarActuadores():
    print("Introduzca el actuador a activar")
    print("1. Ventilador")
    print("2. Motor")
    print("0. Atrás")
    print("----------------------")
    print("Introduzca el número de muestras a leer")

    options = {"0": main,
               "1": activarVentilador,
               "2": activarMotor,
               }
    n = -1
    while n not in options:
        print()  
        n = str(input("Opción: "))
               
    options[n]()
    if(n != '0'):
        accionarActuadores()

def main(argv=[]):
    global arg
    if len(argv) != 0:
        arg = argv[0]

    
    
    print("Seleccione una opción")
    print("1. Leer datos")
    print("2. Accionar actuadores")
    print("0. Salir")
    print("----------------------")


    options = {"0": lambda: None,
               "1": getNumeroMuestras,
               "2": accionarActuadores
               
            }

    n = -1
    if arg == '1':
        n = "1"
    while n not in options:
        print()  
        n = str(input("Opción: "))
        
    options[n]()

def insertSampleTS(muestra):
    T = float(muestra["T"])
    H = float(muestra["H"])
    L = float(muestra["L"])
    V = float(muestra["estadoVentilador"])
    M = float(muestra["estadoMotor"])
    f = urlopen(baseURL + "&field1=%s&field2=%s&field3=%s&field4=%s&field5=%s" % (T, H, L, V, M))
    f.close()

main(sys.argv[1:])
    
