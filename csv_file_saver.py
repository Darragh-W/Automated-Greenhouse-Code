
import csv
import serial 
import time
from datetime import datetime
import msvcrt
import os

port = 'COM8'
baud = 9600 

sensor_data = serial.Serial(port, baud, timeout = 40) #takes in the data from the USB
print("Connected to: " + port)

#initializes variables for time tracking and file management
startTime = None
currentDate = None
file = None
fileName = None
writer = None


def createNewFile(): #function for creating new file every day and naming it with the date
    global fileName, file, writer, currentDate #accsesses variabsles outside of the function

    currentDate = datetime.now().strftime('%Y-%m-%d')#creates date string for file name
    fileName = f'data_{currentDate}.csv'

    file_exists = os.path.exists(fileName)#checks if the file already exists

    file = open(fileName, mode='a', newline='')
    writer = csv.writer(file)

    #"Temperature (C),Humidity (%RH),Soil Moisture (%),Soil Moisture (mV),Fan Status,Pump Status"

    if not file_exists:#if the file does not exist, it will create a new file and write the header row
        writer.writerow(['Timestamp',
                         'Temperature (C)',
                         'Humidity (%RH)',
                         'VPD (kPa)',
                         'Soil Moisture (%)',
                         'Fan Status',
                         'Pump Status'])
        print(f"Created new file: {fileName}")
    else:
        print(f"Logging to existing file: {fileName}")


createNewFile()


try: #completes this before checking for keyboard input to stop logging

    while True:

        # Press 'q' to stop logging
        if msvcrt.kbhit():
            key = msvcrt.getch().decode('utf-8').lower()
            if key == 'q':
                print("\n'q' pressed. Stopping logging.")
                break

        today = datetime.now().strftime('%Y-%m-%d')
        if today != currentDate: #checks if todays date is not the same as the file name date
            if file:
                file.close()
            createNewFile()    


        try:
            line = sensor_data.readline().decode('utf-8', errors='ignore').rstrip()

        except serial.SerialException:
            print("APC220 disconnected, waiting for reconnection...")
            time.sleep(5)
            try:
                sensor_data.close()
                sensor_data = serial.Serial(port, baud, timeout=5)
                print("Reconnected!")
            except serial.SerialException:
                print("Still disconnected, retrying...")
            continue

        if not line:
            print("No data received. Check APC220 connection.")
            continue

        if line.startswith('Temperature (C)'): #skips header row from arduino
            continue

        data = line.split(',')


        if len(data) != 6:
            print(f"Invalid data format: {line}")
            continue
        
        timeStamp = datetime.now().strftime('%Y-%m-%d %H:%M:%S')



        writer.writerow([timeStamp] + data)
        file.flush()  # Ensure data is written to the file immediately

        print(f"Data saved: {timeStamp}, {data}")

except KeyboardInterrupt: #if ctrl+c is pressed, it will stop the data collection and close the file and serial connection
    print("\nData collection stopped by user.")
    
finally: #close the file and serial connection when done
    sensor_data.close()
    print("\nSerial connection closed.")