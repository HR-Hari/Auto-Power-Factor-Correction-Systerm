# ⚡ Automatic Power Factor Correction (APFC) System

This project implements an **Automatic Power Factor Correction (APFC)** system using an **Arduino**, voltage and current sensors, and **relay-controlled capacitor banks**.  
The system continuously measures the power factor of an AC load, detects phase difference through **zero-crossing**, and automatically switches capacitor banks to improve low power factor.

---

## 🚀 Features
- Real-time **voltage and current measurement**
- **Zero-crossing detection** for phase angle estimation  
- Calculation of:
  - Real Power (P)  
  - Reactive Power (Q)  
  - Apparent Power (S)  
  - Power Factor (PF)
- Automatic switching of **capacitor banks** using relays
- Dynamic correction of lagging power factor  
- LCD/serial output for live monitoring  
- Simple, low-cost design with easy expandability

---

## 📘 Project Overview

Power factor is a key parameter in AC power systems. A low power factor increases current demand and reduces system efficiency.  
This project improves power factor by:

1. Measuring **voltage and current waveforms**
2. Detecting the **phase difference** between them
3. Computing **PF = cos(φ)**
4. Automatically adding capacitors to **reduce reactive power**
5. Improving the efficiency of the connected load

This system is suitable for:
- Educational and lab demonstrations  
- Home / small industry PF monitoring  
- Power electronics learning projects  
- Improving understanding of AC power analysis

---

## 🛠️ Hardware Components

| Component | Description |
|----------|-------------|
| Arduino (Uno/Nano) | Main microcontroller |
| ZMPT101B | Voltage sensor |
| SCT-013 | Current sensor |
| Zero-cross detector | For phase measurement |
| Relay module (2–4 channels) | Controls capacitor banks |
| Capacitor bank | Provides reactive power compensation |
| LCD / Serial monitor | (Optional) Display PF & power values |

---

## 🔧 System Architecture




---

## 🧮 How It Works

### 1️⃣ Voltage & Current Sampling  
The Arduino samples voltage and current waveforms at high frequency using ADC.

### 2️⃣ Zero-Crossing Detection  
An interrupt detects the exact moment when the voltage waveform crosses zero.  
The time difference between voltage and current zero crossings gives the phase angle (φ).

### 3️⃣ Power Factor Calculation  
\[
\text{PF} = \cos(\phi)
\]

The firmware computes:

\[
P = VI\cos(\phi), \quad Q = VI\sin(\phi), \quad S = VI
\]

### 4️⃣ Automatic Correction  
- If PF < **threshold** (e.g., 0.85), relay switches add capacitors  
- If PF rises too high, capacitors are removed  
- Maintains stable PF under varying load conditions

---

## 📂 Repository Structure


























    
