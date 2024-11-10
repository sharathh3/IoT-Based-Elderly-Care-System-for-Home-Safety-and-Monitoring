from flask import Flask, request, jsonify, render_template
from flask_cors import CORS
import random
import time
import threading

app = Flask(__name__)
CORS(app)  # Allow cross-origin requests

# Placeholder for the latest data received from the script
latest_data = None

# Define the URL endpoint for receiving data
@app.route('/receive_data', methods=['POST'])
def receive_data():
    global latest_data
    received_data = request.get_json()
    latest_data = received_data
    print(f"Received data: {latest_data}")
    return jsonify({'message': 'Data received successfully'}), 200

# Define the route to get the latest data in JSON format
@app.route('/latest_data', methods=['GET'])
def get_latest_data():
    if latest_data is None:
        return jsonify({'error': 'No data received yet'}), 404
    return jsonify({'data': latest_data})

# Main route to render the HTML UI with Bootstrap
@app.route('/')
def index():
    return render_template('index.html')

# Simulate normal sensor data
def generate_normal_data():
    data = {
        'Distance': round(random.uniform(50, 100), 2),
        'Pressure': 'Small',
        'HRV': round(random.uniform(60, 90), 2),
        'Sugar level': round(random.uniform(70, 80), 2),
        'SpO2': round(random.uniform(91, 100), 2),
        'Accelerometer': "Below Threshold"
    }
    return data

# Simulate problematic sensor data every 20 seconds
def generate_problem_data():
    data = {
        'Distance': round(random.uniform(0, 10), 2),
        'Pressure': 'Large',
        'HRV': round(random.uniform(105, 120), 2),
        'Sugar level': round(random.choice([random.uniform(0, 30), random.uniform(160, 200)]), 2),
        'SpO2': round(random.uniform(70, 79), 2),
        'Accelerometer': "Above Threshold"
    }
    return data

# Background thread to generate and send data periodically
def data_sender():
    url = 'http://127.0.0.1:5000/receive_data'
    counter = 0
    while True:
        if counter % 4 == 0:
            simulated_data = generate_problem_data()
            print("Sending PROBLEM data:", simulated_data)
        else:
            simulated_data = generate_normal_data()
            print("Sending normal data:", simulated_data)

        # Send data to the receive_data endpoint
        requests.post(url, json=simulated_data)
        time.sleep(5)
        counter += 1

# Start the data sender in a separate thread
threading.Thread(target=data_sender, daemon=True).start()

if __name__ == '__main__':
    app.run(debug=True)