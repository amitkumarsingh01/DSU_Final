import pygame
import os

def play_audio():
    audio_path = os.path.join(os.path.dirname(__file__), 'public', 'audio.wav')
    
    pygame.mixer.init()
    pygame.mixer.music.load(audio_path)
    pygame.mixer.music.play()

if __name__ == '__main__':
    play_audio()

    while pygame.mixer.music.get_busy():
        pass
