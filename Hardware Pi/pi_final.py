from flask import Flask, request
import cv2
import os
import time
import requests
from pydub import AudioSegment
import simpleaudio as sa

app = Flask(__name__)

laptop_url = 'http://10.1.7.64:5000/upload'

save_directory = r'/home/sujith/Desktop/dsu/audio_files'
if not os.path.exists(save_directory):
    os.makedirs(save_directory)

def play_audio(audio_path):
    try:
        print("Playing audio...")
        audio = AudioSegment.from_file(audio_path, format="mp3")
        play_obj = sa.play_buffer(audio.raw_data, num_channels=audio.channels, 
                                  bytes_per_sample=audio.sample_width, 
                                  sample_rate=audio.frame_rate)
        play_obj.wait_done()
    except Exception as e:
        print(f"Error playing audio: {e}")

@app.route('/capture', methods=['POST'])
def capture_image():
    camera = cv2.VideoCapture(0)

    if not camera.isOpened():
        return "Error: Could not open webcam.", 500

    ret, frame = camera.read()
    if ret:
        temp_image_path = 'temp_image.jpg'
        cv2.imwrite(temp_image_path, frame)
        print(f"Current image captured, sending to laptop...")

        with open(temp_image_path, 'rb') as img_file:
            response = requests.post(laptop_url, files={'image': img_file})

        if response.status_code == 200:
            print("Image successfully sent to laptop, waiting for response...")

            response_data = response.json()
            audio_file_url = response_data["audio_file"]

            audio_path = os.path.join(save_directory, "output.mp3")
            audio_content = requests.get(audio_file_url)

            if audio_content.status_code == 200:
                with open(audio_path, 'wb') as f:
                    f.write(audio_content.content)
                print(f"Audio file saved to: {audio_path}")

                play_audio(audio_path)
            else:
                print("Error downloading audio file from the laptop.")

            camera.release()

            return "Image processed, audio saved, and played successfully.", 200
        else:
            camera.release()
            return "Error: Could not send image to laptop.", 500
    else:
        camera.release()
        return "Error: Could not capture image.", 500

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)

