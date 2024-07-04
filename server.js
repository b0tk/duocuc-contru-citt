const express = require('express');
const bodyParser = require('body-parser');
const fs = require('fs');
const path = require('path');
const sqlite3 = require('sqlite3').verbose();

const app = express();
const port = 3000;

app.use(bodyParser.urlencoded({ extended: true }));
app.use(bodyParser.json());

// Configurar la base de datos SQLite
const db = new sqlite3.Database(':memory:');

db.serialize(() => {
  db.run("CREATE TABLE rssi_data (timestamp TEXT, rssi INTEGER)");
});

// Ruta para recibir los datos del ESP32
app.post('/data', (req, res) => {
  const rssi = req.body.rssi;
  const timestamp = new Date().toISOString();
  
  const stmt = db.prepare("INSERT INTO rssi_data VALUES (?, ?)");
  stmt.run(timestamp, rssi);
  stmt.finalize();

  console.log(`Datos recibidos: RSSI=${rssi}, Timestamp=${timestamp}`);
  res.sendStatus(200);
});

// Ruta para mostrar los datos en la página web
app.get('/', (req, res) => {
  res.sendFile(path.join(__dirname, 'index.html'));
});

// Ruta para obtener los datos de RSSI
app.get('/rssi-data', (req, res) => {
  db.all("SELECT * FROM rssi_data", (err, rows) => {
    if (err) {
      res.status(500).json({ error: err.message });
      return;
    }
    res.json(rows);
  });
});

app.listen(port, () => {
  console.log(`Servidor escuchando en http://localhost:${port}`);
});
