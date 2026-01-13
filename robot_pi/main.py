"""
MAIN ENTRY POINT

SOFTWARE TEAM:
Replace `get_decision_from_model()` with
your LLM + OpenCV pipeline.
"""

from hardware_interface import execute_action

def get_decision_from_model():
    """
    SOFTWARE TEAM WILL EDIT ONLY THIS FUNCTION
    Must return decision JSON
    """
    return None  # placeholder

while True:
    decision = get_decision_from_model()

    if decision:
        execute_action(decision)
