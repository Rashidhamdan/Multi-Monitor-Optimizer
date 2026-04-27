# 🖥️ Multi Monitor Optimizer
[Main Window](docs/screenshots/main_window.png)

![C++](https://img.shields.io/badge/C%2B%2B-17-00599C?style=flat-square)
![Qt](https://img.shields.io/badge/Qt-6-41CD52?style=flat-square)
![CMake](https://img.shields.io/badge/CMake-Build-064F8C?style=flat-square)
![Status](https://img.shields.io/badge/Status-Active%20Development-D4A017?style=flat-square)

---

## 🚀 Overview

**Multi Monitor Optimizer** is a professional desktop tool for analyzing and optimizing DPI, PPI, and scaling consistency across multiple displays.

---

## 🚀 Features

- 📊 PPI (Pixels Per Inch) calculation
- 🎯 Reference monitor selection (Auto / Manual)
- ⚖️ Scaling normalization recommendations
- 📉 Delta % comparison between monitors
- 🧠 Smart optimizer for consistent UI experience
- 📊 Visual charts (Qt Charts integration)
- 🧪 Unit-tested core logic (Qt Test)

---

## 📸 Preview

> (Optional: Add screenshots later)

---

## 🧠 Problem

When using multiple monitors with different resolutions and sizes:

- Windows appear **inconsistent in size**
- UI scaling differs
- Workflow becomes inefficient

This tool solves that by:

✔️ Normalizing visual density
✔️ Recommending optimal scaling
✔️ Providing clear analysis

---

## 🏗️ Architecture
```bash
ulti-Monitor-Optimizer/
├── app/            # Application entry points
├── core/           # Business logic (Calculator, Optimizer)
├── gui/            # Qt Widgets UI
├── resources/      # Assets / QRC
├── tests/          # Unit tests (Qt Test)
```

---

## ⚙️ Build Instructions

### Requirements

- Qt 6 (Widgets, Charts, Test)
- CMake ≥ 3.16
- C++17 compiler

### Install dependencies (Ubuntu)
```bash
sudo apt install qt6-base-dev qt6-tools-dev qt6-tools-dev-tools qt6-charts-dev
```

### Build
```bash
git clone https://github.com/Rashidhamdan/Multi-Monitor-Optimizer.git
cd Multi-Monitor-Optimizer
mkdir build && cd build
cmake ..
cmake --build .
```

### ▶️ Run
```bash
./bin/Multi-Monitor-Optimizer
```

### 🧪 Run Tests
```bash
cd build
ctest --verbose
```

### 📊 Example Output
```bash
Monitor 1: 109.7 PPI (Reference)
Monitor 2: 91.8 PPI (Δ -16.3%)
Suggested Scaling: 125%
```

---

## 📸 Screenshots Setup

docs/screenshots/

---

## 💡 Why This Project Matters

This project demonstrates:

- Desktop application development with C++ and Qt
- Clean separation between UI (Qt Widgets) and core logic
- Implementation of mathematical optimization (PPI / scaling normalization)
- A structured architecture with modular components
- Building a real-world engineering tool, not just a demo

---

## 👨‍💻 Author

Rashid Hamdan

- GitHub: https://github.com/Rashidhamdan
- Email: rashid.hamdan@outlook.de
- LinkedIn: https://www.linkedin.com/in/rashid-hamdan-0307ab267

---

## 📄 License

This project is licensed under the MIT License.
