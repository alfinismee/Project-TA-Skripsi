# Project-TA-Skripsi
IMPLEMENTASI EMBEDDED SYSTEM BERBASIS MACHINE LEARNING UNTUK PENGHITUNG OTOMATIS BIBIT IKAN LELE (# Automatic Catfish Fry Counting Using Machine Learning and Embedded Systems)

This project is a prototype for counting catfish fry using a combination of Computer Vision and Embedded Systems. It was developed as my undergraduate final project to explore how object detection and sensor-based counting can be integrated into a single system.
The prototype uses YOLOv5 to detect catfish fry through a camera, while an E18-D80NK infrared sensor is used as the primary counting sensor. An ESP32 handles the system's control and sensor input, and the counting results are displayed on an LCD.

## How It Works

The system combines camera-based detection with infrared sensing. YOLOv5 is used to detect catfish fry in the camera feed, while the infrared sensor detects fish passing through the counting path. The ESP32 processes the sensor input and updates the displayed count.

The prototype is designed to operate offline, without requiring an internet connection during operation.

## Development Process

- Collected and annotated catfish fry images using Label Studio.
- Trained the YOLOv5 model using Google Colab.
- Tested the model using camera input.
- Integrated the infrared sensor, ESP32, and LCD.
- Conducted prototype testing to evaluate the counting process.

## Components & Tools

- Python
- YOLOv5
- OpenCV
- Label Studio
- Google Colab
- ESP32
- E18-D80NK Infrared Sensor
- LCD 16×2 I2C
- Arduino IDE

## Testing & Challenges

During testing, I encountered several challenges, including overlapping fish, inconsistent fish movement, water reflections, and fish getting stuck along the counting path. These conditions affected the consistency of the counting results and required adjustments during prototype testing.

The experience helped me understand that developing a Computer Vision-based system involves more than training a model. Data quality, sensor behavior, the physical environment, and the overall system design also affect how the prototype performs.

## What I Learned

Through this project, I gained experience in preparing datasets, training and evaluating object detection models, integrating sensors with a microcontroller, and testing a Machine Learning-based prototype in real-world conditions.

This project also helped me explore the practical challenges of combining Machine Learning with Embedded Systems.
