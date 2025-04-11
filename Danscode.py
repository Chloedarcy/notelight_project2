import librosa
import matplotlib.pyplot as plt
import numpy as np

# Load the audio file
y, sr = librosa.load("/Users/rkrizan25/Desktop/Programs/Westworld_ Runaway - Kanye West [Piano Cover].mp3", sr=None)

# Create a time axis in seconds
time = np.linspace(0, len(y) / sr, num=len(y))

# Print first few samples for debugging
print(f"Audio Data (y): {y[:10]}")  # First 10 samples
print(f"Sample Rate: {sr}")  # Sample rate (e.g., 44100 Hz)

# Plot waveform with time in seconds
plt.figure(figsize=(10, 4))
plt.plot(time, y)
plt.xlabel("Time (seconds)")  # Updated label
plt.ylabel("Amplitude")
plt.title("Waveform of the Audio")
plt.show()
