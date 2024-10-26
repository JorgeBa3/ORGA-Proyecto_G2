# Laboratorio de Organizacion Computacional

# Grupo #3 

| Nombre |Carnet|Trabajado|  
|--|--|--|
| Jorge Alejandro De Leon Batres | 202111277 | 100% |
| Roberto Miguel Garcia Santizo  | 202201724 | 100% |
| Jose Javier Bonilla Salazar    | 202200035 | 100%|
| Fredy Alexander Esteban Pineda | 202110511 |100%|

# Índice
1. Introducción
2. Descripción del Problema
3. Lógica del Sistema
4. Diagramas de Estado
5. Diagramas del Diseño del Circuito
6. Equipo Utilizado
7. Diagramas con Explicación
8. Presupuesto
9. Configuracion del Modulo Bluetooth
10.  Conclusiones
11.  Recomendaciones


# Introducción
Microsoft Windows requiere realizar un juego interactivo para su nuevo sistema operativo por lo cual se le solicita a usted como experto en circuitos que realice un prototipo del juego “Buscaminas”.


# Descripcion del problema
 El juego consiste básicamente en verificar todas las casillas que no tengan una bomba. A diferencia del juego original, en esta versión no hay pistas de bombas aledañas, el juego es puramente aleatorio. El flujo de este es ingresar la posición del bit que queremos verificar y al darle a verificar, se ira a leer esa posición en la RAM del tablero de configuración, y procede a ver si es una bomba o no, en caso sea una bomba, enviara un mensaje de gameover, encender el led de juego terminado (led rojo) y se acaba la partida (no se debe hacer nada especial con esto, solo mostrar el mensaje de game over en la pantalla lcd y encender el led mencionado). En caso no sea una bomba, se ingresa un “1” en el tablero de juego. Una vez ingresado esto, aumenta el punteo del jugador en 1 la cual se presentará en una pantalla lcd. Esto se repite hasta que logre verificar todas las posiciones sin bomba (se envía un mensaje de “ganaste” y encender el led verde) o se seleccione una bomba. Los estados de juego son 3, los cuales son excluyentes. Cuando se inicia el programa, se está jugando o empieza una nueva partida, el led se pone en estado “Jugando” la cual será un led azul. Si se encuentra una bomba, se prende el led de gameover, y si se encuentran todas las posiciones sin bombas, se prende el led de ganaste. Para salir de estos 2 últimos estados hay que presionar el botón de “Nuevo Juego” el cual reseteara los LEDs a “jugando”, reseteara el contador de Puntaje, y el Tablero de Juego.

# Lógica del Sistema
-   **Interfaz de Configuración**:
    
    -   El usuario define la ubicación de las bombas utilizando una **página web** (front-end).
    -   Las posiciones de las bombas pueden ingresarse directamente en la interfaz o mediante un **archivo de entrada** con extensión `.org`.
    -   Esta configuración se almacena en la memoria RAM del sistema, representada por **flip-flops** en una matriz 4x4.
-   **Comunicación Serial**:
    
    -   El **backend** del sistema envía los datos de las posiciones de las bombas al **Arduino** mediante comunicación serial o Bluetooth.
    -   Las posiciones se registran en la **memoria RAM física** en la placa con LEDs que representan si una casilla contiene una bomba.
-   **Validaciones Previas al Juego**:
    
    -   Mientras el juego esté en **modo configuración**, el usuario no podrá empezar a jugar.
    -   Una vez que la configuración esté lista, se habilita el modo de juego.

#  Diagramas de Estado
### 1. **Diagrama de Estado de Configuración del Juego**
![enter image description here](https://raw.githubusercontent.com/JavierB20/fotosOrga/6eaed0b7d6c5cdfaf504dbab54a28734bacb5706/graphviz.svg)
### 2. **Diagrama de Estado del Modo de Juego**
![enter image description here](https://raw.githubusercontent.com/JavierB20/fotosOrga/6eaed0b7d6c5cdfaf504dbab54a28734bacb5706/graphviz%20(1).svg)
### 3. **Integración Completa de Estados**
![enter image description here](https://raw.githubusercontent.com/JavierB20/fotosOrga/6eaed0b7d6c5cdfaf504dbab54a28734bacb5706/graphviz%20(2).svg)

# Equipo utilizado
![enter image description here](https://github.com/JavierB20/fotosOrga/blob/main/WhatsApp%20Image%202024-10-25%20at%2020.29.44_fb85fe9a.jpg?raw=true)

# Diagramas con Explicación

## **1. Diagrama de Estado del Ciclo de Juego**

Este diagrama representa el flujo general del juego: desde la configuración inicial hasta los estados de victoria o derrota.
![enter image description here](https://raw.githubusercontent.com/JavierB20/fotosOrga/6eaed0b7d6c5cdfaf504dbab54a28734bacb5706/graphviz%20(3).svg)

## **2. Diagrama de Estado para la Selección de Celdas**

Este diagrama muestra los estados posibles al interactuar con las celdas del tablero. Cada celda puede estar en uno de varios estados: oculta, marcada con bandera, revelada o contener una bomba.
![enter image description here](https://raw.githubusercontent.com/JavierB20/fotosOrga/6eaed0b7d6c5cdfaf504dbab54a28734bacb5706/graphviz%20(4).svg)

## **3. Diagrama de Estado para la Partida en Curso**

Este diagrama modela los posibles flujos mientras el jugador está en el estado activo del juego, mostrando cómo el progreso puede llevar al éxito o fracaso.
![enter image description here](https://raw.githubusercontent.com/JavierB20/fotosOrga/6eaed0b7d6c5cdfaf504dbab54a28734bacb5706/graphviz%20(5).svg)

# Configuracion del Modulo Bluetooth
![enter image description here](https://github.com/JavierB20/fotosOrga/blob/main/Screenshot%202024-10-25%20213951.png?raw=true)
![enter image description here](https://github.com/JavierB20/fotosOrga/blob/main/Screenshot%202024-10-25%20214010.png?raw=true)
![enter image description here](https://github.com/JavierB20/fotosOrga/blob/main/Screenshot%202024-10-25%20214051.png?raw=true)
![enter image description here](https://github.com/JavierB20/fotosOrga/blob/main/Screenshot%202024-10-25%20214110.png?raw=true)
![enter image description here](https://github.com/JavierB20/fotosOrga/blob/main/Screenshot%202024-10-25%20214128.png?raw=true)

# Presupuesto

Gasto Total: Q300

# Gastos Individuales

 - Jorge Batres: Q75
 - Roberte Garcia: Q75
 - Javie Bonilla: Q75
 - Fredy Esteban: Q75

# Conclusiones
-   **Comprensión de Lógica y Estados del Juego:**  
    El desarrollo del Buscaminas permitió modelar y gestionar múltiples estados del juego, lo que demuestra la importancia de estructurar adecuadamente las reglas y transiciones mediante diagramas de estado y código eficiente.
    
-   **Aplicación de Estructuras de Datos y Algoritmos:**  
    La implementación del juego requiere el uso de matrices para representar el tablero y algoritmos eficientes para colocar bombas, calcular celdas adyacentes y manejar eventos. Esto fomentó habilidades en programación orientada a objetos y manejo de estructuras dinámicas.
    
-   **Interactividad y Control de Errores:**  
    Se aprendió a manejar correctamente las entradas del usuario para evitar errores y brindar una experiencia fluida. La incorporación de banderas y lógica de revelado garantiza una jugabilidad intuitiva y controlada.
    
-   **Simulación de Estados Finales (Victoria/Derrota):**  
    Se logró modelar de manera clara los estados terminales (victoria o derrota) y el ciclo de reinicio del juego, lo que hace la aplicación más robusta y fácil de extender en futuras versiones.
    
-   **Gestión del Tiempo y Recursos:**  
    Este proyecto ayudó a reforzar habilidades de organización y planificación, ya que la lógica del Buscaminas implica gestionar varias fases de desarrollo: desde la conceptualización hasta las pruebas de juego y la depuración.

# Recomendaciones
-   **Incluir un Sistema de Niveles o Dificultad Adaptativa:**  
    Implementar diferentes niveles de dificultad (fácil, medio, difícil) o un sistema que ajuste la complejidad según el progreso del jugador aumentaría la rejugabilidad y el interés por el juego.
    
-   **Optimización de Algoritmos:**  
    Revisar el código para detectar posibles cuellos de botella y optimizar los algoritmos que colocan bombas y calculan celdas adyacentes, especialmente en tableros grandes.
    
-   **Interfaz Gráfica (GUI):**  
    Considerar la implementación de una interfaz gráfica en lugar de solo texto mejoraría la experiencia del usuario. Se podría utilizar librerías como **SFML**, **Qt**, o **SDL** para añadir gráficos y animaciones.
    
-   **Agregar Funcionalidades Extra:**
    
    -   **Temporizador:** Para medir el tiempo que toma al jugador resolver el tablero.
    -   **Tablas de puntuaciones:** Llevar un registro de los mejores tiempos o victorias consecutivas.
    -   **Modo Multijugador:** Ofrecer la posibilidad de jugar con amigos o competir por el mejor tiempo.
-   **Manejo de Errores y Pruebas Automatizadas:**  
    Integrar más pruebas unitarias y sistemas de validación para asegurarse de que cada parte del código funcione correctamente bajo diferentes condiciones.
    
-   **Guardar y Cargar Partidas:**  
    Añadir una opción para que los jugadores puedan guardar su progreso y retomar el juego más tarde mejoraría la experiencia y permitiría mayor flexibilidad.
    
-   **Compatibilidad Multiplataforma:**  
    Probar el juego en diferentes sistemas operativos (Windows, Linux, Mac) aseguraría su portabilidad y disponibilidad para más usuarios.