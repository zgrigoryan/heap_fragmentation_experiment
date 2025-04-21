import subprocess
import time
import csv

csv_file = 'top_memory_usage.csv'

def capture_top():
    command = ["top", "-l", "1", "-o", "mem"]  
    result = subprocess.run(command, stdout=subprocess.PIPE, text=True)
    return result.stdout

with open(csv_file, 'w', newline='') as f:
    writer = csv.writer(f)
    writer.writerow(['Timestamp', 'PID', 'COMMAND', 'MEM', 'PURG', 'CMPRS']) 

for _ in range(60): 
    top_output = capture_top()
    
    for line in top_output.splitlines():
        if 'PID' in line:
            continue
        parts = line.split()
        
        if len(parts) < 10:
            continue
        
        timestamp = time.time()  # Current timestamp
        pid = parts[0]           # PID of the process
        command = parts[1]       # Command name
        mem = parts[3]           # Memory used (MEM column)
        purg = parts[4]          # Purgeable memory (PURG column)
        cmprs = parts[5]         # Compressed memory (CMPRS column)

        with open(csv_file, 'a', newline='') as f:
            writer = csv.writer(f)
            writer.writerow([timestamp, pid, command, mem, purg, cmprs])
    
    time.sleep(1)

