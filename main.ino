#include <ArduinoJson.h>

/*
partido {
  "jugadores": [
    {
      "nombre": "Jugador1",
      "puntos": 0,
      "juegos": 1,
      "sets": 1
    },
    {
      "nombre": "Jugador2",
      "puntos": 0,
      "juegos": 0,
      "sets": 0
    }
  ],
  "historial": [
    {
      "set": 1,
      "games": [
        [["0", "15"],["0", "30"],["0", "40"]],
        [["15", "0"],["30", "0"],["40", "0"]]
      ]
    },
    {
      "set": 2,
      "games": [
        [["0", "15"],["15", "15"],["30", "15"],["40", "15"]],
        [["15", "0"],["15", "15"],["15", "30"],["15", "40"]]
      ]
    }
  ]
}
*/  

// Estructura para la información de cada jugador
struct Jugador {
  String nombre;
  int puntos;
  int juegos;
  int sets;
};

// Estructura para los puntos en un juego específico
struct PuntoJuego {
  String puntoJugador1;
  String puntoJugador2;
};

// Estructura para los juegos dentro de un set
struct Game {
  PuntoJuego puntos[10]; // Asumimos un máximo de 10 puntos por juego, puedes ajustar según sea necesario
  int numPuntos; // Número de puntos registrados en el juego
};

// Estructura para la información de un set
struct Historial {
  int numSet;
  Game games[10]; // Asumimos un máximo de 10 juegos por set, puedes ajustar según sea necesario
  int numGames; // Número de juegos registrados en el set
};

// Estructura principal para el partido de tenis
struct PartidoTenis {
  Jugador jugadores[2];
  Historial historial[5]; // Asumimos un máximo de 5 sets por partido, puedes ajustar según sea necesario
  int numSets; // Número de sets registrados en el historial
};

void estructuraAJson(PartidoTenis &partido, JsonDocument &doc) {
  // Añadir jugadores al JSON
  JsonArray jugadores = doc.createNestedArray("jugadores");
  for (int i = 0; i < 2; i++) {
    JsonObject jugador = jugadores.createNestedObject();
    jugador["nombre"] = partido.jugadores[i].nombre;
    jugador["puntos"] = partido.jugadores[i].puntos;
    jugador["juegos"] = partido.jugadores[i].juegos;
    jugador["sets"] = partido.jugadores[i].sets;
  }

  // Añadir historial de sets al JSON
  JsonArray historial = doc.createNestedArray("historial");
  for (int i = 0; i < partido.numSets; i++) {
    JsonObject set = historial.createNestedObject();
    set["set"] = partido.historial[i].numSet;

    JsonArray games = set.createNestedArray("games");
    for (int j = 0; j < partido.historial[i].numGames; j++) {
      JsonArray puntos = games.createNestedArray();
      for (int k = 0; k < partido.historial[i].games[j].numPuntos; k++) {
        JsonArray punto = puntos.createNestedArray();
        punto.add(partido.historial[i].games[j].puntos[k].puntoJugador1);
        punto.add(partido.historial[i].games[j].puntos[k].puntoJugador2);
      }
    }
  }
}

#include <ArduinoJson.h>

// Definir las estructuras (como se mostró anteriormente)

void setup() {
  Serial.begin(9600);

  // Crear y llenar un objeto PartidoTenis
  PartidoTenis partido;
  
  // Llenar información de los jugadores
  partido.jugadores[0] = {"Jugador1", 0, 1, 1};
  partido.jugadores[1] = {"Jugador2", 0, 0, 0};
  
  // Llenar información del historial de sets y juegos
  partido.numSets = 2;
  
  // Set 1
  partido.historial[0].numSet = 1;
  partido.historial[0].numGames = 2;
  partido.historial[0].games[0].numPuntos = 3;
  partido.historial[0].games[0].puntos[0] = {"0", "15"};
  partido.historial[0].games[0].puntos[1] = {"0", "30"};
  partido.historial[0].games[0].puntos[2] = {"0", "40"};
  partido.historial[0].games[1].numPuntos = 3;
  partido.historial[0].games[1].puntos[0] = {"15", "0"};
  partido.historial[0].games[1].puntos[1] = {"30", "0"};
  partido.historial[0].games[1].puntos[2] = {"40", "0"};
  
  // Set 2
  partido.historial[1].numSet = 2;
  partido.historial[1].numGames = 2;
  partido.historial[1].games[0].numPuntos = 4;
  partido.historial[1].games[0].puntos[0] = {"0", "15"};
  partido.historial[1].games[0].puntos[1] = {"15", "15"};
  partido.historial[1].games[0].puntos[2] = {"30", "15"};
  partido.historial[1].games[0].puntos[3] = {"40", "15"};
  partido.historial[1].games[1].numPuntos = 4;
  partido.historial[1].games[1].puntos[0] = {"15", "0"};
  partido.historial[1].games[1].puntos[1] = {"15", "15"};
  partido.historial[1].games[1].puntos[2] = {"15", "30"};
  partido.historial[1].games[1].puntos[3] = {"15", "40"};
  
  // Convertir la estructura a JSON
  StaticJsonDocument<2048> doc;
  estructuraAJson(partido, doc);

  // Imprimir el JSON en la consola serial
  serializeJson(doc, Serial);
  //Serial.println(serializeJsonPretty(doc, Serial));

}

void loop() {
  // Nada que hacer en el loop
}
