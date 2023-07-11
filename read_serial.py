import serial

# Open the serial port
ser = serial.Serial('/dev/ttyACM0', baudrate=57600)  # Replace with the appropriate serial port and baud rate

# Open the output file in write mode
with open('output.txt', 'w') as output_file:
    # Read data from the serial port and print to console and file
    while True:
        data = ser.read()  # Read a byte from the serial port
        data_str = data.decode()  # Convert byte to string
        print(data_str, end='')  # Print to console
        output_file.write(data_str)  # Write to the output file
        output_file.flush()  # Flush the output to ensure immediate writing

# Close the serial port
ser.close()
