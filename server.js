// server.js
const express = require('express');
const mysql = require('mysql');
const bodyParser = require('body-parser');
const path = require('path');

const app = express();
const port = 3000;

// Configuración de MySQL
const db = mysql.createConnection({
  host: 'localhost',
  user: 'root',
  password: '',
  database: 'sensor_data'
});

db.connect((err) => {
  if (err) throw err;
  console.log('Conectado a la base de datos MySQL.');
});

// Configuración de Express
app.set('view engine', 'ejs');
app.set('views', path.join(__dirname, 'views'));
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));

// Lista para almacenar errores
let errors = [];

// Ruta para recibir datos del sensor
app.post('/receive_data', (req, res) => {
  const { px, py, pz, roll, pitch } = req.body;
  const sql = 'INSERT INTO data (px, py, pz, roll, pitch) VALUES (?, ?, ?, ?, ?)';
  db.query(sql, [px, py, pz, roll, pitch], (err, result) => {
    if (err) throw err;
    console.log('Datos guardados en la base de datos.');
    res.sendStatus(200);
  });
});

// Endpoint para revisar el estado de error
app.get('/check_reset', (req, res) => {
  if (errors.length > 0) {
    res.status(200).send('Reiniciar');
  } else {
    res.status(204).send();
  }
});

// Endpoint para manejar notificaciones de error
app.post('/notify_error', (req, res) => {
  const error = req.body.error;
  if (error) {
    errors.push(error);
    console.log('Error recibido:', error);
  }
  res.status(200).send('Notificación de error recibida');
});

// Ruta para mostrar los datos en la interfaz web
app.get('/', (req, res) => {
  const sql = 'SELECT * FROM data ORDER BY timestamp DESC';
  db.query(sql, (err, results) => {
    if (err) throw err;
    res.render('index', { data: results });
  });
});

// Ruta para obtener los errores
app.get('/errors', (req, res) => {
  res.json(errors);
});

// Iniciar el servidor
app.listen(port, () => {
  console.log(`Servidor corriendo en http://localhost:${port}`);
});
