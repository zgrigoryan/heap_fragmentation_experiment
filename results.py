import pandas as pd
import matplotlib.pyplot as plt

# Load full dataset
df = pd.read_csv("top_memory_usage.csv")

# Filter only 'fragdemo' process (or any other you're targeting)
filtered_df = df[df['COMMAND'] == 'fragdemo']

# Save filtered results to a new CSV
filtered_df.to_csv("fragdemo_memory.csv", index=False)

print(f"Filtered {len(filtered_df)} entries for fragdemo.")


# Convert Timestamp to a more readable format (optional)
df['Timestamp'] = pd.to_datetime(df['Timestamp'], unit='s')

# Plot memory usage over time (RES memory)
plt.figure(figsize=(10, 6))
plt.plot(df['Timestamp'], df['MEM'], marker='o', label='Resident Memory (MEM)', linestyle='-', color='blue')
plt.xlabel('Time')
plt.ylabel('Memory Usage (KB)')
plt.title('Memory Usage Over Time (Captured with top)')
plt.legend()
plt.grid(True)
plt.tight_layout()

# Show the plot
plt.savefig("plot.png")
