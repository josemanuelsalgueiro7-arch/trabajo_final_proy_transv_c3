# Trabajo Práctico Final Arduino
Proyecto Transversal C3 – Facultad de Ingeniería UNMDP

## Sistema de Control de Acceso

### Objetivo

Crear un Sistema de Control de Acceso mediante lectura de tarjetas por aproximación (RFID). El mismo deberá indicar al usuario si está autorizado, si su tarjeta es válida o no, en una pantalla LCD. A su vez, el acceso autorizado deberá indicarse por leds en formato semáforo (VERDE: puede pasar, ROJO: tarjeta inválida, AMARILLO: otras alertas). Se simulará con una función vacía la consulta a una base de datos, para posibles implementaciones en un ambiente real de control de acceso de personal. En forma opcional, se anunciará con una señal sonora los actos de lectura, rechazo y otras alertas.

Crear una función de empadronamiento de tarjetas, mediante comando en puerto serie o botón, que permita agregar una tarjeta como autorizada para el acceso. Este modo deberá ser informado con un led AZUL de lectura, y un texto de “Agregado Satisfactorio” en la pantalla LCD. Si es posible, completar el proceso de empadronamiento con la opción de eliminar una tarjeta.

La comunicación entre el servidor (la computadora con entorno de desarrollo Arduino) y el control de acceso (placa Arduino) debe realizarse mediante cable de red, en un intento de reproducir condiciones reales de un Sistema de Control de Acceso, gestionado remotamente por una oficina de Cómputos.

### Componentes

- Placa Arduino UNO.
- Diodos Led de varios colores, o módulo Led RGB HW479 (en caso de reducir cables).
- Resistencias de 220 Ohms (en el caso de no usar led RGB).
- Pantalla LCD 16x2 con módulo de comunicación I2C integrado.
- Sensor lector de tarjetas RFID-RC522.
- Tarjetas y llaveros de prueba.
- Buzzer piezoeléctrico.

### Tareas a Resolver

Para poder resolver el problema general, es necesario separar en partes el mismo, para ir avanzando en etapas y compartimentar las funciones principales en “módulos” (aunque el código lo verá dentro de el segmento loop, para no perder tiempo en llamadas a funciones):

- **Lectura de Tarjetas**: Primero, se debe lograr leer una tarjeta y obtener la información en el monitor serie.
- **Visado de Tarjetas**: Comparar los datos obtenidos del lector RFID-RC522 con las tarjetas guardadas en un arreglo de cadenas de texto que contienen las tarjetas empadronadas.
- **Procesamiento de datos**: Programar la lógica de aceptación/rechazo/alertas en función de la información obtenida del lector RFID-RC522. Desarrollar el manejo de los pines que controlan los dispositivos de salida visuales y sonoros (por ej., buzzer piezoeléctrico).
- **Muestra de datos e información visual**: Programar la salida de datos en pantalla LCD e información visual de LECTURA/INGRESO/RECHAZO a través del semáforo led (módulo o leds separados).

#### Lectura de Tarjetas por RFID

Para la lectura de las tarjetas por aproximación, usamos el módulo RC522. El mismo trabaja a 3.3V, por lo que es necesario un conversor lógico de 5V a 3.3V (ST1167). Como no lo tenemos, la alternativa más próxima es armar un divisor de tensión sobre las cinco líneas de comunicación que pueden verse afectadas con un voltaje alto (SDA, SCK, MOSI, MISO y RST). Dicho divisor de tensión se armó con resistencias de 220Ω y 470Ω.

A continuación se puede ver el circuito que recibe la información por RFID de las tarjetas:
  
