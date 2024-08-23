#include <ArduinoJson.h>
/*
{"jugadores":[{"nombre":"Jugador1","puntos":0,"juegos":1,"sets":1},{"nombre":"Jugador2","puntos":0,"juegos":0,"sets":0}],
"historial":
[{"set":1,"games":[{"game":1,"puntos":[["0","15"],["0","30"],["0","40"],["",""]]},{"game":2,"puntos":[["15","0"],["30","0"],["40","0"],["",""]]}]},
{"set":2,"games":[{"game":1,"puntos":[["0","15"],["15","15"],["30","15"],["40","15"]]},{"game":2,"puntos":[["15","0"],["15","15"],["15","30"],["15","40"]]}]}]}
*/

// Definir las estructuras
struct Jugador {
  char nombre[20];  // Cambiar a char[] para ahorrar memoria
  int puntos;
  int juegos;
  int sets;
};

struct Punto {
  char puntosJugador1[3];  // Cambiar a char[] con tamaño fijo
  char puntosJugador2[3];
};

struct Game {
  int gameNumero;
  Punto puntos[4];  // Reducir a 4 puntos por juego
};

struct SetHistorial {
  int setNumero;
  Game games[2];  // Reducir a 2 juegos por set
};

struct HistorialPartido {
  SetHistorial sets[2];  // Reducir a 2 sets por partido
};

struct PartidoTenis {
  Jugador jugadores[2];
  HistorialPartido historial;
};

void llenarPartido(PartidoTenis &partido) {
  strcpy(partido.jugadores[0].nombre, "Jugador1");
  partido.jugadores[0].puntos = 0;
  partido.jugadores[0].juegos = 1;
  partido.jugadores[0].sets = 1;

  strcpy(partido.jugadores[1].nombre, "Jugador2");
  partido.jugadores[1].puntos = 0;
  partido.jugadores[1].juegos = 0;
  partido.jugadores[1].sets = 0;

  partido.historial.sets[0] = {1, {
    {1, {{"0", "15"}, {"0", "30"}, {"0", "40"}}},
    {2, {{"15", "0"}, {"30", "0"}, {"40", "0"}}}
  }};

  partido.historial.sets[1] = {2, {
    {1, {{"0", "15"}, {"15", "15"}, {"30", "15"}, {"40", "15"}}},
    {2, {{"15", "0"}, {"15", "15"}, {"15", "30"}, {"15", "40"}}}
  }};
}

// Función optimizada para convertir PartidoTenis a JSON
void estructuraAJsonOptimizada(PartidoTenis &partido, Print &output) {
  StaticJsonDocument<512> doc; // Tamaño reducido para ahorrar memoria

  JsonArray jugadores = doc.createNestedArray("jugadores");
  for (int i = 0; i < 2; i++) {
    JsonObject jugador = jugadores.createNestedObject();
    jugador["nombre"] = partido.jugadores[i].nombre;
    jugador["puntos"] = partido.jugadores[i].puntos;
    jugador["juegos"] = partido.jugadores[i].juegos;
    jugador["sets"] = partido.jugadores[i].sets;
  }

  JsonArray historial = doc.createNestedArray("historial");
  for (int i = 0; i < 2; i++) {
    JsonObject setObj = historial.createNestedObject();
    setObj["set"] = partido.historial.sets[i].setNumero;
    
    JsonArray games = setObj.createNestedArray("games");
    for (int j = 0; j < 2; j++) {
      JsonObject gameObj = games.createNestedObject();
      gameObj["game"] = partido.historial.sets[i].games[j].gameNumero;
      
      JsonArray puntos = gameObj.createNestedArray("puntos");
      for (int k = 0; k < 4; k++) {
        JsonArray punto = puntos.createNestedArray();
        punto.add(partido.historial.sets[i].games[j].puntos[k].puntosJugador1);
        punto.add(partido.historial.sets[i].games[j].puntos[k].puntosJugador2);
      }
    }
  }

  // Serializar directamente al objeto Print (Serial)
  serializeJson(doc, output);
}

void imprimirPartido(PartidoTenis &partido) {
  Serial.println("Detalles del Partido:");
  for (int i = 0; i < 2; i++) {
    Serial.print("Jugador ");
    Serial.print(i + 1);
    Serial.println(":");
    Serial.print("  Nombre: ");
    Serial.println(partido.jugadores[i].nombre);
    Serial.print("  Puntos: ");
    Serial.println(partido.jugadores[i].puntos);
    Serial.print("  Juegos: ");
    Serial.println(partido.jugadores[i].juegos);
    Serial.print("  Sets: ");
    Serial.println(partido.jugadores[i].sets);
    Serial.println();
  }

  Serial.println("Historial de Sets:");
  for (int i = 0; i < 2; i++) {  // Ajustado a 2 sets
    Serial.print("Set ");
    Serial.println(partido.historial.sets[i].setNumero);

    for (int j = 0; j < 2; j++) {  // Ajustado a 2 juegos por set
      Serial.print("  Juego ");
      Serial.println(partido.historial.sets[i].games[j].gameNumero);

      for (int k = 0; k < 4; k++) {  // Ajustado a 4 puntos por juego
        Serial.print("    Puntos: ");
        Serial.print(partido.historial.sets[i].games[j].puntos[k].puntosJugador1);
        Serial.print(" - ");
        Serial.println(partido.historial.sets[i].games[j].puntos[k].puntosJugador2);
      }
    }
    Serial.println();
  }
}

void setup() {
  Serial.begin(9600);

  // Crear la estructura del partido
  PartidoTenis partido;
  llenarPartido(partido);

  // Imprimir los detalles del partido
  imprimirPartido(partido);

  // Imprimir el JSON generado optimizado
  estructuraAJsonOptimizada(partido, Serial);
}

void loop() {
  // No es necesario hacer nada en el loop
}
