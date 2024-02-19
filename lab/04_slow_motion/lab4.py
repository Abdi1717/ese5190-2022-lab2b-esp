import serial
import time

def load_data_from_file(filepath):
    result = []
    with open(filepath, 'r') as file:
        for line in file:
            result.append(line.strip())
    return result

def alter_data_rate(original_data, multiplier, is_slowing_down=True):
    modified_data = []
    if is_slowing_down:
        for item in original_data:
            modified_data.extend([item] * multiplier)
    else:
        modified_data = [original_data[i] for i in range(0, len(original_data), multiplier)]
    return modified_data

def main():
    print("System Initialization...")
    data_file_path = './values.txt'
    
    try:
        serial_connection = serial.Serial('COM12', 115200)
        print(f"Connected to {serial_connection.name}")
    except Exception as error:
        print(f"Error opening serial port: {error}")
        exit()

    while True:
        print("Select operation: Record (R) / Replay (P)")
        user_choice = input().strip().upper()
        
        if user_choice == 'R':
            received_data = []
            print("Recording starts after the Green light. Duration: 10 seconds.")
            serial_connection.write(b'r')
            print("Receiving data...")
            recording_start = time.time()

            while time.time() - recording_start < 10:
                received_line = serial_connection.readline()
                if received_line:
                    received_data.append(received_line.decode().strip())

            serial_connection.write(b'N')
            print("Recording completed.")
            
            with open(data_file_path, 'w') as file:
                for data in received_data:
                    file.write(data + '\n')

        elif user_choice == 'P':
            playback_data = load_data_from_file(data_file_path)
            modified_data = []
            
            print("Playback of the recording. Choose speed mode: Normal / Slow Down (SD) / Speed Up (SU)")
            speed_mode = input().strip().upper()
            print("Enter rate of speed modification:")
            speed_rate = int(input())

            if speed_mode == 'NORMAL':
                modified_data = playback_data
            elif speed_mode == 'SD':
                modified_data = alter_data_rate(playback_data, speed_rate, is_slowing_down=True)
            elif speed_mode == 'SU':
                modified_data = alter_data_rate(playback_data, speed_rate, is_slowing_down=False)

            print("Enter number of times for playback:")
            repeat_times = int(input())
            serial_connection.write(b'p')

            for _ in range(repeat_times):
                for data_point in modified_data:
                    serial_connection.write(data_point.encode())

            serial_connection.write(b'N')
            print("Playback finished.")

if __name__ == "__main__":
    main()
