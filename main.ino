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


// Definir las estructuras
struct Jugador {
  String nombre;
  int puntos;
  int juegos;
  int sets;
};

struct Game {
  String puntosJugador1;
  String puntosJugador2;
};

struct SetHistorial {
  int setNumero;
  Game games[10]; // Supongamos que un set puede tener hasta 10 games
};

struct HistorialPartido {
  SetHistorial sets[5]; // Supongamos que un partido puede tener hasta 5 sets
};

struct PartidoTenis {
  Jugador jugadores[2];
  HistorialPartido historial[2]; // Supongamos que el historial incluye todos los sets
};

void estructuraAJson(PartidoTenis &partido, JsonDocument &doc) {
  for (int i = 0; i < 2; i++) {
    doc["jugadores"][i]["nombre"] = partido.jugadores[i].nombre;
    doc["jugadores"][i]["puntos"] = partido.jugadores[i].puntos;
    doc["jugadores"][i]["juegos"] = partido.jugadores[i].juegos;
    doc["jugadores"][i]["sets"] = partido.jugadores[i].sets;
  }

  for (int i = 0; i < 2; i++) {
    doc["historial"][i]["set"] = partido.historial[i].sets[0].setNumero;
    JsonArray games = doc["historial"][i].createNestedArray("games");
    for (int j = 0; j < 10; j++) {
      JsonArray game = games.createNestedArray();
      game.add(partido.historial[i].sets[0].games[j].puntosJugador1);
      game.add(partido.historial[i].sets[0].games[j].puntosJugador2);
    }
  }
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
  for (int i = 0; i < 2; i++) {
    Serial.print("Set ");
    Serial.println(partido.historial[i].sets[0].setNumero);

    for (int j = 0; j < 10; j++) {
      String puntosJ1 = partido.historial[i].sets[0].games[j].puntosJugador1;
      String puntosJ2 = partido.historial[i].sets[0].games[j].puntosJugador2;

      if (puntosJ1 != "" && puntosJ2 != "") {
        Serial.print("  Juego ");
        Serial.print(j + 1);
        Serial.print(": ");
        Serial.print(puntosJ1);
        Serial.print(" - ");
        Serial.println(puntosJ2);
      }
    }
    Serial.println();
  }
}

void setup() {
  Serial.begin(9600);

  PartidoTenis partido;
  partido.jugadores[0] = {"Jugador1", 0, 1, 1};
  partido.jugadores[1] = {"Jugador2", 0, 0, 0};

  partido.historial[0].sets[0] = {1, {{"0", "15"}, {"0", "30"}, {"0", "40"}, {"15", "0"}, {"30", "0"}, {"40", "0"}}};
  partido.historial[1].sets[0] = {2, {{"0", "15"}, {"15", "15"}, {"30", "15"}, {"40", "15"}, {"15", "0"}, {"15", "15"}, {"15", "30"}, {"15", "40"}}};

  // Imprimir los detalles del partido en la consola serial
  imprimirPartido(partido);

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

