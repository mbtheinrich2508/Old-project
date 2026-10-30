import numpy as np
import matplotlib.pyplot as plt
from datetime import datetime
from scipy.signal import butter, filtfilt


# Patientendaten
patient_name = "Max Mustermann"
current_datetime = datetime.now().strftime(
    "%Y-%m-%d %H:%M:%S"
)  # aktuelles Datum und Uhrzeit

# Simulationsparameter
num_samples = 5000  # Anzahl der Messpunkte (z.B. 3 Sekunden bei 1000 Hz)
sampling_rate = 500.0  # Abtastrate in Hz
time_interval = 1.0 / sampling_rate  # Zeitabstand zwischen den Messpunkten
time_data = np.arange(num_samples) * time_interval  # Zeitdaten

# Herzfrequenz in Hz (entspricht etwa 60 Schlägen pro Minute)
heart_rate = 1.2  # in Hz, ca 72bpm
ekg_data = np.zeros(num_samples)

# Parameter für die verschiedenen Wellen im EKG
p_wave_duration = 0.08  # Dauer der P-Welle in Sekunden
q_duration = 0.02  # Dauer des Q-Komplexes in Sekunden
r_duration = 0.04  # Dauer der R-Welle in Sekunden
s_duration = 0.02  # Dauer des S-Komplexes in Sekunden
t_wave_duration = 0.12  # Dauer der T-Welle in Sekunden

# Amplituden für die Wellen
p_wave_amplitude = 0.1
q_amplitude = -0.2
r_amplitude = 1.0
s_amplitude = -0.3
t_wave_amplitude = 0.2

# Erzeugen des EKG-Signals mit deutlicheren Q- und S-Spitzen
for i in range(num_samples):
    t = time_data[i] % (1 / heart_rate)  # Zeit innerhalb eines Herzzyklus

    # P-Welle: leichte Auf- und Abwärtsbewegung vor dem QRS-Komplex
    if 0.1 <= t < 0.1 + p_wave_duration:
        ekg_data[i] = p_wave_amplitude * np.sin(np.pi * (t - 0.1) / p_wave_duration)

    # Q-Welle: kurzer negativer Ausschlag
    elif 0.2 <= t < 0.2 + q_duration:
        ekg_data[i] = q_amplitude * np.sin(np.pi * (t - 0.2) / q_duration)

    # R-Welle: starker positiver Ausschlag
    elif 0.2 + q_duration <= t < 0.2 + q_duration + r_duration:
        ekg_data[i] = r_amplitude * np.sin(
            np.pi * (t - (0.2 + q_duration)) / r_duration
        )

    # S-Welle: kurzer negativer Ausschlag nach der R-Welle
    elif (
        0.2 + q_duration + r_duration <= t < 0.2 + q_duration + r_duration + s_duration
    ):
        ekg_data[i] = s_amplitude * np.sin(
            np.pi * (t - (0.2 + q_duration + r_duration)) / s_duration
        )

    # T-Welle: breiter, sanfter Anstieg und Abfall nach dem QRS-Komplex
    elif 0.4 <= t < 0.4 + t_wave_duration:
        ekg_data[i] = t_wave_amplitude * np.sin(np.pi * (t - 0.4) / t_wave_duration)

# Rauschen hinzufügen, um das Signal realistischer zu gestalten
noise_level = 0.3
ekg_data += (np.random.rand(num_samples) - 0.5) * noise_level


def add_sine_drift(signal, fs, drift_freq=0.2, amplitude=0.3):
    t = np.arange(len(signal)) / fs
    drift = amplitude * np.sin(2 * np.pi * drift_freq * t)
    return signal + drift


ekg_data = add_sine_drift(ekg_data, 500, drift_freq=0.03, amplitude=0.5)

# EKG-Daten mit Header in eine Textdatei speichern
with open("ECG_data_Noise2.txt", "w") as f:
    f.write(f"# Patient Name: {patient_name}\n")
    f.write(f"# Date and Time: {current_datetime}\n")
    f.write(f"# Sampling Rate: {sampling_rate} Hz\n")
    f.write("# Time(s)\tAmplitude(mV)\n")
    for t, value in zip(time_data, ekg_data):
        f.write(f"{t:.4f}\t{value:.4f}\n")
print("EKG-Datei erfolgreich erstellt.")


# ekg_data[280:285] = -0.4

fs = sampling_rate  # Abtastrate in Hz
lowcut = 0.5  # untere Grenzfrequenz in Hz
highcut = 40.0  # obere Grenzfrequenz in Hz
order = 4  # Filterordnung
nyquist = 0.5 * fs
low = lowcut / nyquist
high = highcut / nyquist


b, a = butter(order, [low, high], btype="band")
filtered = filtfilt(b, a, ekg_data)
# ekg_data = filtered

# Detaillierter Plot der EKG-Daten
fig = plt.figure(figsize=(14, 7))
ax = fig.add_subplot(111)
ax.grid(False)

ax.plot(time_data, ekg_data, label=" ECG signal", color="blue", linewidth=1.5)

ax.set_xlabel("time [s]", fontsize=14)
ax.set_ylabel("amplitude [mV]", fontsize=14)
# ax.grid(True, linestyle="--", alpha=0.7)
ax.legend(loc="upper right", fontsize=12)
fig.tight_layout()
plt.show()
