import threading
import speech_recognition as sr
from kivy.app import App
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.widget import Widget
from kivy.uix.label import Label
from kivy.graphics import Color, Ellipse, Rectangle
from kivy.core.window import Window
from kivy.clock import Clock
from kivy.utils import platform

import llm
import serialconn

import json

if platform == 'android':
    from android.permissions import request_permissions, Permission
    request_permissions([Permission.RECORD_AUDIO, Permission.INTERNET])

Window.size = (300, 400)

class BubbleWidget(Widget):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        with self.canvas:
            Color(0.45, 0.54, 0.85)
            self.bubble = Ellipse(pos=(100, 150), size=(100, 100))

class MainLayout(BoxLayout):
    def __init__(self, **kwargs):
        super().__init__(orientation='vertical', **kwargs)
        
        self.bubble_area = BubbleWidget(size_hint=(1, 0.66))
        self.label = Label(
            text="Initializing...",
            size_hint=(1, 0.34),
            font_size='16sp',
            color=(1, 1, 1, 1),
            bold=True,
            text_size=(Window.width * 0.9, None),
            halign='center',
            valign='middle'
        )
        self.label.canvas.before.clear()
        with self.label.canvas.before:
            Color(0.14, 0.15, 0.17, 1)
            self.rect = Rectangle(size=self.label.size, pos=self.label.pos)
            self.label.bind(size=self._update_rect, pos=self._update_rect)

        self.add_widget(self.bubble_area)
        self.add_widget(self.label)
        
        self.recognizer = sr.Recognizer()
        self.is_listening = False
        Clock.schedule_once(self.start_listening, 1)

    def start_listening(self, dt=None):
        self.is_listening = True
        thread = threading.Thread(target=self.listen_thread_func, daemon=True)
        thread.start()

    def listen_thread_func(self):
        while self.is_listening:
            try:
                with sr.Microphone() as source:
                    Clock.schedule_once(lambda dt: self.update_label("Listening..."))
                    self.recognizer.adjust_for_ambient_noise(source, duration=0.5)
                    audio = self.recognizer.listen(source)
                
                Clock.schedule_once(lambda dt: self.update_label("Recognizing..."))
                # text = self.recognizer.recognize_google(audio)
                text = self.recognizer.recognize_whisper(audio)
                Clock.schedule_once(lambda dt: self.continue_operation(text))

            except sr.UnknownValueError:
                Clock.schedule_once(lambda dt: self.update_label("Could not understand audio"))
            except sr.RequestError as e:
                err=str(e)
                Clock.schedule_once(lambda dt: self.update_label(f"API Error: {err}"))
            except Exception:
                pass 

    def update_label(self, text):
        self.label.text = text
    def continue_operation(self, text):
        self.update_label(text)
        Clock.schedule_once(lambda dt: self.sendToDevice(text))

    def sendToDevice(self, text):
        command = llm.get_response(text)
        print("Command is", command)
        command = json.loads(command.strip())
        serialconn.send_to_arduino(command["command"])

    def _update_rect(self, instance, value):
        self.rect.pos = instance.pos
        self.rect.size = instance.size

class BubbleApp(App):
    def build(self):
        self.title = "AUDIBLE CONTROL"
        return MainLayout()

    def on_stop(self):
        if self.root:
            self.root.is_listening = False

if __name__ == "__main__":
    BubbleApp().run()