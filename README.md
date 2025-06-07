```
   ____  __   ______  ______  ____  ____  
  / __ \/ /  / __ \ \/ / __ \/ __ \/ __ \ 
 / / / / /__/ / / /\  / / / / / / / /_/ / 
/ /_/ /____/ /_/ / / / /_/ / /_/ / ____/  
\____/     \____/_/_/\____/\____/_/       

       Open Software & AI Tools by OXCORP
```

---

**OXCORP.NINJA** is a creative engineering company that regularly releases free software and open-source developer tools.  
We specialize in Qt, AI automation, and cross-platform deployment tools.

Visit us at 🌐 **[https://www.oxcorp.ninja](https://www.oxcorp.ninja)**

---

# Oxcorp Free Installer

**Oxcorp Free Installer** is an open-source Qt-based installer and launcher system designed for Windows applications.  
It supports remote update detection, zip-based asset deployment, and custom GUI branding.

---

## 🔧 1. Building and Customizing the Launcher

### ✅ Build the Launcher

The launcher is a Qt Widgets application. To build it:

1. Install [Qt 6.5+](https://www.qt.io/download) with MinGW or MSVC.
2. Open a terminal in `launcher/` and run:

```bash
qmake Oxlauncher.pro
mingw32-make release
```

This will output the launcher binary in `launcher/release`.

3. Then, to include all required Qt dependencies, run:

```bash
windeployqt OxLauncher.exe
```

> Make sure you run this command **inside the `launcher/release` folder**.
> This will copy all necessary Qt DLLs and plugins into the same directory, completing the launcher build.

---

### ✅ Customize Launcher Name and Icon

To rename the launcher binary and set your own icon:

* Rename the `.exe` after build: e.g., `MyAppLauncher.exe`
* Replace the icon via `launcher/icon.rc` and `launcher/assets/icon.ico`
* Update the executable name reference in `installscript.qs` (see below)

---

## 🧾 2. Setting Your App Metadata (Installer Branding)

Edit the file:

```
config/config.xml
```

Update these fields to match your application:

```xml
<Name>YourAppName</Name>
<Title>Your App Installer</Title>
<Publisher>Your Company</Publisher>
<TargetDir>@HomeDir@/YourAppName</TargetDir>
<InstallerApplicationIcon>../assets/icon.ico</InstallerApplicationIcon>
```

These values affect the installer title, installation path, and icon.

---

## 🌐 3. Setting Version Check & Update URLs

The launcher fetches version and zip files from a remote server.

To configure these URLs:

Edit:

```
packages/com.oxcorp.launcher/installscript.qs
```

Update these lines:

```js
var baseUrl = "https://your-server.com/updates";
var versionUrl = "https://your-server.com/version.txt";
```

These values will be stored as environment variables (`OXCORP_BASE_URL`, etc.) and read by the launcher.

---

## 🛠️ 4. Creating the Final Installer

After customizing the launcher and installer settings:

1. Open PowerShell and run:

```powershell
.\build_installer.ps1
```

This will:

* Copy the compiled launcher into `packages/com.oxcorp.launcher/data`
* Build the final installer using Qt Installer Framework
* Output `OxcorpInstaller.exe` into the `tools/` directory

You can distribute `tools/OxcorpInstaller.exe` as your single installer.

---

## 📦 5. Hosting and Updating Your App

To distribute your application via remote server:

### 📁 Initial Setup

1. **Create `basic.zip`**

   * Include your compiled app executable and all required subfolders
   * Name it `basic.zip` and upload it to your server at:
     `https://your-server.com/updates/basic.zip`

2. **Create `version.txt`**

   * A plain text file containing the version string, e.g., `v1001` or `1.0.0.1`
   * Upload to:
     `https://your-server.com/version.txt`

### 🔁 For Each Update

3. **Update `version.txt`**

   * Change the version string to a new value (e.g., `v1002`)

4. **Upload Full Zip as `[version].zip`**

   * Create a full new zip archive of your entire app (same structure as `basic.zip`)
   * Upload it to:
     `https://your-server.com/updates/v1002.zip`

> ⚠️ The default logic **always downloads only the latest version zip** if the local version is older.
> For this reason, it's strongly recommended to **upload complete builds** as versioned `.zip` files (not just diffs).

> If you want to change the logic to allow downloading multiple versions or managing per-file updates,
> **you'll need to modify the launcher code accordingly.**

---

## 📂 Project Structure

```
/launcher/                    → Qt-based Launcher source code
/packages/com.oxcorp.launcher/ → Qt Installer Framework package
/config/config.xml            → Installer branding and metadata
/build_installer.ps1          → Script to generate the final installer
/tools/OxcorpInstaller.exe    → Output installer
```

---

## 🤝 Contribution

We welcome contributions, issue reports, and feature requests.
Feel free to fork this repository and adapt it for your application!

---

## 📄 License

MIT License © 2025 Oxcorp Limited  
This project includes components using the Qt LGPL license. See LICENSE file for full details.
