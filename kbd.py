from pynput import keyboard
import socket

def get_keycode(key):
    # Define the keycode mapping for printable ASCII characters
    keycode_mapping = {
        'a': 30, 'b': 48, 'c': 46, 'd': 32, 'e': 18, 'f': 33, 'g': 34, 'h': 35, 'i': 23, 'j': 36,
        'k': 37, 'l': 38, 'm': 50, 'n': 49, 'o': 24, 'p': 25, 'q': 16, 'r': 19, 's': 31, 't': 20,
        'u': 22, 'v': 47, 'w': 17, 'x': 45, 'y': 21, 'z': 44,
        '1': 2, '2': 3, '3': 4, '4': 5, '5': 6, '6': 7, '7': 8, '8': 9, '9': 10, '0': 11,
        '`': 41, '-': 12, '=': 13, '[': 26, ']': 27, '\\': 43, ';': 39, "'": 40, ',': 51, '.': 52,
        '/': 53, ' ': 57, '\t': 15, '\n': 28,
        '!': 2, '@': 3, '#': 4, '$': 5, '%': 6, '^': 7, '&': 8, '*': 9, '(': 10, ')': 11,
        '~': 41, '_': 12, '+': 13, '{': 26, '}': 27, '|': 43, ':': 39, '"': 40, '<': 51, '>': 52,
        '?': 53,
        # Add more mappings for other characters if needed
    }

    if isinstance(key, keyboard.KeyCode):
        if key.char in keycode_mapping:
            keycode = keycode_mapping[key.char]
            print(f"Regular key pressed: {key.char} (Key code: {keycode})")
        else:
            print("Unknown regular key pressed")
        
    elif isinstance(key, keyboard.Key):
        print(f"Special key pressed: {key}")
        if key == keyboard.Key.esc:
            keycode = 1
        elif key == keyboard.Key.space:
            keycode = 57
        elif key == keyboard.Key.tab:
            keycode = 15
        elif key == keyboard.Key.ctrl:
            keycode = 29
        elif key == keyboard.Key.alt:
            keycode = 56
        elif key == keyboard.Key.enter:
            keycode = 28
        elif key == keyboard.Key.backspace:
            keycode = 14
        elif key == keyboard.Key.delete:
            keycode = 111
        elif key == keyboard.Key.shift:
            keycode = 54
        elif key == keyboard.Key.cmd:
            keycode = 154
        else:
            print("Unknown key pressed")
    # Return the corresponding key code if available, or None if not found
    return keycode



with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    try:
        # Connect to the server
        s.connect(('127.0.0.1', 38104))
    except Exception as e:
        print("Error:", e)
    with keyboard.Events() as events:
        for event in events:
            if isinstance(event, keyboard.Events.Press):
                keycode = get_keycode(event.key)
                s.sendall(bytes([0x01]) + keycode.to_bytes(1, byteorder='little'))
            elif isinstance(event, keyboard.Events.Release):
                keycode = get_keycode(event.key)
                s.sendall(bytes([0x00]) + keycode.to_bytes(1, byteorder='little'))
