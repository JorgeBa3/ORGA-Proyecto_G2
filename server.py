from flask import Flask, jsonify, render_template, request, redirect, url_for
import re, serial, time

app = Flask(__name__)
arduino = serial.Serial('COM8', 9600)

estadoJugando = False
estadoConfigurando = False

@app.route('/')
def index():
    return render_template('index.html')

# Nuevo endpoint para obtener el estado de las bombas
@app.route('/get_bomb_states')
def get_bomb_states():
    arduino.write(b'S')  # Enviar el comando 'S' al Arduino para obtener el estado de la matriz
    line = ""
    
    # Leer hasta que se obtenga la cadena de estados
    while True:
        line = arduino.readline().decode().strip()  # Leer la respuesta del Arduino
        if line and not line.startswith("Activando bomba en la posición:"):  # Ignorar mensajes
            break  # Salir del bucle si obtenemos una línea válida

    try:
        bomb_states = list(map(int, line.split(',')))  # Convertir la cadena en una lista de enteros
    except ValueError:
        return jsonify(error="Error al convertir estados de bombas"), 500

    return jsonify(bomb_states=bomb_states)  # Enviar el estado de las bombas como JSON


@app.route('/newGame', methods=['POST'])
def new_game():
    texto =(f'reset\n')  # Enviar el comando 'reset' al Arduino para reiniciar el juego
    arduino.write(texto.encode())  # Envía el comando al Arduino
    return render_template('index.html')

@app.route('/send_bombs', methods=['POST'])
def send_bombs():
    bomb_positions = request.form.get('bombs')  # Cadena con posiciones como '1,2,3,4,'
    print("Bomb positions:", bomb_positions)  # Muestra las posiciones de las bombas en la consola

    if bomb_positions:
        # Elimina la última coma, si existe, y luego divide las posiciones en una lista
        bomb_positions = bomb_positions.rstrip(',').split(',')
        
        for pos in bomb_positions:
            if pos.isdigit():  # Verifica que la posición sea un número válido
                bomb_pos = int(pos)

                # Asegúrate de que la posición esté en el rango de 1 a 16
                if 1 <= bomb_pos <= 16:
                    # Calcula la fila y la columna en la matriz 4x4
                    row = (bomb_pos - 1) // 4
                    col = (bomb_pos - 1) % 4
                    
                    # Crea la cadena para enviar al Arduino con la posición de la bomba
                    bomb_position_serial = f'{bomb_pos}\n'
                    arduino.write(bomb_position_serial.encode())  # Envía la posición al Arduino
                    time.sleep(0.3)  # Esperar 200ms antes de enviar la siguiente bomba
                    print(f"Enviando bomba en la posición: {bomb_pos} (Fila {row+1}, Columna {col+1})")
                else:
                    print(f"Posición fuera de rango: {pos}")
            else:
                print(f"Valor no numérico recibido: {pos}")
    else:
        print("No se recibieron posiciones de bombas.")
    texto =(f'start\n')  # Enviar el comando 'reset' al Arduino para reiniciar el juego
    arduino.write(texto.encode())  # Envía el comando al Arduino
    return redirect(url_for('index'))


@app.route('/analyze_text', methods=['POST'])
def analyze_text():
    text_content = request.form.get('text_content')  # Obtiene el contenido del cuadro de texto
    if text_content:
        result, bomb_positions = analyze_text_content(text_content)
        
        return render_template('index.html', result=result, bomb_positions=bomb_positions)
    
    return 'Error: No se recibió texto', 400

def analyze_text_content(text):
    lines = text.splitlines()
    lines = [line.strip() for line in lines if line.strip()]  # Eliminar líneas vacías
    
    # Ignorar los comentarios iniciales hasta encontrar conf_ini
    start_index = next((i for i, line in enumerate(lines) if line == 'conf_ini'), -1)
    end_index = next((i for i, line in enumerate(lines) if line == 'conf:fin'), -1)

    if start_index == -1:
        print("Error: El texto debe iniciar con 'conf_ini'")
        return None, None
    
    if end_index == -1 or start_index >= end_index:
        print("Error: El texto debe terminar con 'conf:fin'")
        return None, None

    # Diccionario que asigna valores del 1 al 16 a las posiciones válidas
    position_values = {
        (0, 0): 1, (0, 1): 2, (0, 2): 3, (0, 3): 4,
        (1, 0): 5, (1, 1): 6, (1, 2): 7, (1, 3): 8,
        (2, 0): 9, (2, 1): 10, (2, 2): 11, (2, 3): 12,
        (3, 0): 13, (3, 1): 14, (3, 2): 15, (3, 3): 16
    }

    result = []
    bomb_positions = []  # Lista para almacenar las posiciones de las bombas detectadas

    # Iterar solo sobre las líneas entre conf_ini y conf:fin
    for line in lines[start_index + 1 : end_index]:
        if line.startswith('//'):
            continue  # Ignorar comentarios

        if line.startswith('ADD'):
            match = re.match(r'ADD x: (\d+), y: (\d+)', line)
            if match:
                x = int(match.group(1))
                y = int(match.group(2))
                if (x, y) in position_values:
                    value = position_values[(x, y)]
                    result.append(f"Instrucción válida: {line} -> Valor asignado: {value}")
                    bomb_positions.append((x, y, value))  # Agregar la bomba con su valor
                else:
                    result.append(f"Instrucción inválida: {line} (coordenadas fuera de rango)")
            else:
                result.append(f"Instrucción inválida: {line} (formato incorrecto)")

    print("Resultados de la evaluación:")
    for r in result:
        print(r)

    print("\nPosiciones de bombas detectadas:")
    for pos in bomb_positions:
        print(f"Posición: ({pos[0]},{pos[1]}) -> Valor: {pos[2]}")
        texto = f'{pos[2]}\n'
        arduino.write(texto.encode())  # Envía el comando al Arduino    
        time.sleep(0.3)  # Esperar 200ms antes de enviar la siguiente bomba
    
    
    texto =(f'start\n')  # Enviar el comando 'reset' al Arduino para reiniciar el juego
    arduino.write(texto.encode())  # Envía el comando al Arduino
    return result, bomb_positions


if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
