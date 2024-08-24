#include <ArduinoJson.h>
/*
{"jugadores":[{"nombre":"Jugador1","punto_actual":0,"sets_ganados":1},{"nombre":"Jugador2","punto_actual":0,"sets_ganados":0}],
"historial":[{"set":1,"juegos_ganados_j1":0,"juegos_ganados_j2":0,"games":[{"game":1,"puntos":[["0","15"],["0","30"],["0","40"],["",""]]},
{"game":2,"puntos":[["15","0"],["30","0"],["40","0"],["",""]]}]},
{"set":2,"juegos_ganados_j1":0,"juegos_ganados_j2":0,"games":[{"game":1,"puntos":[["0","15"],["15","15"],["30","15"],["40","15"]]},
{"game":2,"puntos":[["15","0"],["15","15"],["15","30"],["15","40"]]}]}]}
*/



// Definir las estructuras
struct Jugador {
  char nombre[20];  // Cambiar a char[] para ahorrar memoria
  int punto_actual;
  int sets_ganados; //debe ser sets ganados 
};

struct Punto {
  char puntosJugador1[3];  // Cambiar a char[] con tamaño fijo
  char puntosJugador2[3];
};

struct Game {
  int gameNumero;
  Punto puntos[4];  // Reducir a 4 puntos por juego
};

struct historialSet {
  int setNumero;
  int juegos_ganados_j1;
  int juegos_ganados_j2;
  Game games[2];  // Reducir a 2 juegos por set
};

struct HistorialPartido {
  historialSet sets[2];  // Reducir a 2 sets por partido
};

struct PartidoTenis {
  Jugador jugadores[2];
  HistorialPartido historial;
};

void llenarPartido(PartidoTenis &partido) {
  strcpy(partido.jugadores[0].nombre, "Jugador1");
  partido.jugadores[0].punto_actual = 0;
  partido.jugadores[0].sets_ganados = 0;

  strcpy(partido.jugadores[1].nombre, "Jugador2");
  partido.jugadores[1].punto_actual = 0;
  partido.jugadores[1].sets_ganados = 0;

  partido.historial.sets[0] = {1, 0,0,{
    {1, {{"0", "15"}, {"0", "30"}, {"0", "40"}}},
    {2, {{"15", "0"}, {"30", "0"}, {"40", "0"}}}
  }};

  partido.historial.sets[1] = {2, 0,0,{
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
    jugador["punto_actual"] = partido.jugadores[i].punto_actual;
    jugador["sets_ganados"] = partido.jugadores[i].sets_ganados;
  }

  JsonArray historial = doc.createNestedArray("historial");
  for (int i = 0; i < 2; i++) {
    JsonObject setObj = historial.createNestedObject();
    setObj["set"] = partido.historial.sets[i].setNumero;

    setObj["juegos_ganados_j1"] = partido.historial.sets[i].juegos_ganados_j1;    
    setObj["juegos_ganados_j2"] = partido.historial.sets[i].juegos_ganados_j2;        
    
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
    Serial.print("  Punto Actual: ");
    Serial.println(partido.jugadores[i].punto_actual);
    Serial.print("  Sets Ganados: ");
    Serial.println(partido.jugadores[i].sets_ganados);
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
