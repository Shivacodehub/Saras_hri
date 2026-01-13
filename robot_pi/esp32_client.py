import requests
from config import ESP32_URL

def send_to_esp32(payload):
    """
    payload: dict produced by decision model
    """
    response = requests.post(ESP32_URL, json=payload, timeout=5)
    return response.json()
