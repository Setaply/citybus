const express = require("express");
const cors = require("cors");
const api = express();

api.use(cors());
api.use(express.json());

const positions = [];

api.get('/get-gps', async (req, res) => {
  console.log(new Date().toISOString(), "GET Request");
  res.json(positions);
});

api.post('/post-gps', async (req, res) => {
  console.log(new Date().toISOString(), "POST Request");

  const lon = parseFloat(req.query.longitude);
  const lat = parseFloat(req.query.latitude);
  const id = parseInt(req.query.id) || 1;

  if (isNaN(lon) || isNaN(lat) || isNaN(id)) {
    return res.json({ status: "Error: Invalid Data" });
  }

  const existing = positions.find(p => p.id === id);
  if (existing) {
    existing.latitude = lat;
    existing.longitude = lon;
  } else {
    positions.push({ latitude: lat, longitude: lon, id });
  }

  res.json({ status: "GPS data updated" });
});

api.listen(3000, () => {
  console.log("Server running on port 3000");
});
