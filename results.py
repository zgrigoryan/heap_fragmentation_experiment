import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv('top_memory_usage.csv')

df['Timestamp'] = pd.to_datetime(df['Timestamp'], unit='s')

def to_mb(val):
    if isinstance(val, str):
        if val.endswith('M'):
            try:
                return float(val[:-1]) 
            except ValueError:
                return None  
        elif val.endswith('B'):
            try:
                return float(val[:-1]) / 1024  
            except ValueError:
                return None  
    return None

df['MEM_MB'] = df['MEM'].apply(to_mb)

df = df.dropna(subset=['MEM_MB'])

plt.figure(figsize=(12, 6), facecolor="lightgray")
plt.plot(df['Timestamp'], df['MEM_MB'], marker='o', linestyle='-', color='blue', label='Resident Memory (MEM)')
plt.xlabel('Time')
plt.ylabel('Memory Usage (MB)')
plt.title('Memory Usage Over Time (Captured with top)')
plt.legend()
plt.grid(True)
plt.tight_layout()

plt.savefig("plot.png", dpi=300)
