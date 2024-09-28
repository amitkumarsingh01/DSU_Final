from flask import Flask, request
import cv2
import requests
import time

app = Flask(__name__)

camera = cv2.VideoCapture(0)

laptop_url = 'http://10.1.7.64:5001/upload'

@app.route('/capture', methods=['POST'])
def capture_image():
    if not camera.isOpened():
        return "Error: Could not open webcam.", 500

    ret, frame = camera.read()
    if ret:
        temp_image_path = 'temp_image.jpg'
        cv2.imwrite(temp_image_path, frame)
        print(f"Image captured, sending to laptop...")

        with open(temp_image_path, 'rb') as img_file:
            response = requests.post(laptop_url, files={'image': img_file})

        if response.status_code == 200:
            return "Image successfully sent to laptop.", 200
        else:
            return "Error: Could not send image to laptop.", 500
    else:
        return "Error: Could not capture image.", 500

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)

