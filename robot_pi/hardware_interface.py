from esp32_client import send_to_esp32
from speech_out import speak

def execute_action(decision_json):
    """
    decision_json STRUCTURE EXPECTED:
    {
      "intent": "...",
      "direction": "...",
      "distance_cm": int,
      "neck_angle": int,
      "speak_text": "..."
    }

    SOFTWARE TEAM:
    You ONLY generate this JSON.
    Do NOT modify below logic.
    """

    # --- SPEECH ---
    speak(decision_json.get("speak_text", ""))

    # --- SEND HARDWARE COMMAND ---
    ack = send_to_esp32({
        "intent": decision_json.get("intent"),
        "direction": decision_json.get("direction"),
        "distance_cm": decision_json.get("distance_cm"),
        "neck_angle": decision_json.get("neck_angle")
    })

    return ack
