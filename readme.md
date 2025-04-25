# 🖱️ Mouse Remapper

**Mouse Remapper** is a powerful and lightweight utility for Windows that lets you remap up to **five mouse buttons** to virtually any common keyboard input. Whether you're gaming, editing, or enhancing productivity, you can assign combinations of keys with advanced behavior like **continuous repeat**, **toggle-repeat until next click**, and **preset profiles**, all through a modern GUI built using **Qt Quick (QML + C++)**.

---

## 🎯 Key Features

- ✅ **Remap 5 Mouse Buttons** to one or more keys:
  - Letters: `A–Z`
  - Digits: `0–9`
  - Special keys: `Tab`, `Enter`, `Escape`, `Space`
  - Navigation keys: `Left`, `Right`, `Up`, `Down`
  - Modifiers: `Ctrl`, `Shift`
- 🔁 **Repeat Mode**: Continuously simulates keypresses while a mouse button is held down.
- 🔄 **Repeat Until Click Mode**: Starts auto-repeat on click, stops it with the next click of the same button.
- 💾 **Preset System**: Save and load remap configurations (`config.json`) at any time.
- 🚀 **Autostart on Windows Boot**: Automatically launches at system startup via Windows registry.
- ⚙️ **Low-Level Global Hooking**: Uses WinAPI’s `SetWindowsHookEx` for robust, system-wide input interception.
- 🖼️ **Clean QML UI**: Intuitive, responsive interface with ComboBoxes and CheckBoxes for seamless configuration.

---

## 🧰 Tech Stack

| Component       | Technology                    |
|----------------|-------------------------------|
| GUI             | Qt Quick (QML), Qt Controls   |
| Backend         | C++17 with Qt Core + WinAPI   |
| Hooking         | `SetWindowsHookEx` (WinAPI)   |
| Key Simulation  | `keybd_event` (WinAPI; deprecated but supported) |
| Config Storage  | JSON (via Qt JSON APIs)       |
| Autostart       | Windows Registry (HKEY_CURRENT_USER) |

---

## 🖥️ GUI Overview

The main interface presents five configurable sections — one per remappable mouse button. Each section offers:

- 🎯 **Key Selection** (up to 3 keys per button)
- ✅ **Enable Remap** – Activate or deactivate mapping for the button
- 🔁 **Repeat** – Send repeated keypresses while held
- 🔄 **Repeat Until Click** – Toggle repeat mode on each button press

### Global Controls

- ▶️ **Start** – Activate the global mouse hook
- 💾 **Save Preset** – Save current mappings to `config.json`
- 📂 **Load Preset** – Load a saved configuration file

---

## 📦 File Structure (Core Modules)

```plaintext
├── resources/
│   └── qml/
│       └── Main.qml          # UI layout and logic (QML)
├── main.cpp                  # App entry point and QML engine setup
├── hookmanager.h/.cpp        # Core mouse hook logic + key simulation
├── configmanager.h/.cpp      # Save/load preset configuration
├── autostart.h/.cpp          # Windows registry autostart setup
├── qml.qrc                   # Qt resource file for embedding QML
└── MouseRemapper.pro         # Qt project file

```

## 🏁 Getting Started

### 🔨 Build Instructions (qmake)

#### Requirements:
- Qt 5.15+ or Qt 6.x (with QML modules)
- Windows (WinAPI usage)
- `qmake` or Qt Creator

#### Clone & Build:
```bash
git clone https://github.com/Felix-au/Mouse-Remapper.git
cd Mouse-Remapper
qmake MouseRemapper.pro
make          # or mingw32-make / nmake depending on your environment
```