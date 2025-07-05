<template>
  <el-card class="map-card">
    <div class="map-panel">
      <div ref="mapContainer" class="map">
        <div class="map-direction-icon">
          <img id="direction-arrow" src="@/assets/arrow.svg" alt="Fahrtrichtung" />
        </div>
      </div>
      
      <el-descriptions :column="2" border>
        <el-descriptions-item label="Nächster Bus (m)">
          {{ overviewState.distanceToNextBus.value }}
        </el-descriptions-item>
        <el-descriptions-item label="Letzte Aktualisierung">
          {{ overviewState.lastUpdate.value }}
        </el-descriptions-item>
      </el-descriptions>


      <div class="legend">
        <div class="legend-item">
          <img src="https://maps.google.com/mapfiles/ms/icons/bus.png" />
          <span>Bus</span>
        </div>
        <div class="legend-item">
          <img src="https://maps.google.com/mapfiles/ms/icons/blue-dot.png" />
          <span>Dein Standort</span>
        </div>
        <div class="legend-item">
          <img src="https://maps.google.com/mapfiles/ms/icons/red-dot.png" />
          <span>Haltepunkt</span>
        </div>
        <div class="legend-item">
          <img src="@/assets/arrow.svg" />
          <span>Fahrtrichtung</span>
        </div>
      </div>
    </div>
  </el-card>
</template>

<script setup>
import { onMounted, ref } from 'vue'
import 'ol/ol.css'
import Map from 'ol/Map'
import View from 'ol/View'
import TileLayer from 'ol/layer/Tile'
import OSM from 'ol/source/OSM'
import VectorLayer from 'ol/layer/Vector'
import VectorSource from 'ol/source/Vector'
import Feature from 'ol/Feature'
import Point from 'ol/geom/Point'
import Style from 'ol/style/Style'
import Stroke from 'ol/style/Stroke'
import Icon from 'ol/style/Icon'
import { fromLonLat, toLonLat } from 'ol/proj'
import GeoJSON from 'ol/format/GeoJSON'
import mapGeoJsonUrl from '@/assets/map.geojson?url'
import apifConfig from "../config/apiConfig"
import { getDistance } from 'ol/sphere';
import busConfig from "../config/busConfig"
import overviewState from "../state/overviewState"
import LineString from 'ol/geom/LineString'

const map = ref(null)
const mapContainer = ref(null)

const center = [9.7687713, 50.6729868]
const updateInterval = 5000

const userMarker = new Feature(new Point(fromLonLat(center)))
const userStyle = new Style({
  image: new Icon({
    src: 'https://maps.google.com/mapfiles/ms/icons/blue-dot.png',
    scale: 1
  })
})

userMarker.setStyle(userStyle)

const busPositions = []
const dynamicSource = new VectorSource({ features: [userMarker] })
const markerLayer = new VectorLayer({ source: dynamicSource })
const haltepunktSource = new VectorSource()
const haltepunktLayer = new VectorLayer({ source: haltepunktSource })

onMounted(async () => {
  const geojson = await fetch(mapGeoJsonUrl).then(res => res.json())
  const routeSource = new VectorSource({
    features: new GeoJSON().readFeatures(geojson, {
      featureProjection: 'EPSG:3857'
    })
  })

  const routeLayer = new VectorLayer({
    source: routeSource,
    style: new Style({
      stroke: new Stroke({ color: '#FF0000', width: 3 })
    })
  })

  map.value = new Map({
    target: mapContainer.value,
    layers: [
      new TileLayer({ source: new OSM() }),
      routeLayer,
      markerLayer,
      haltepunktLayer,
    ],
    view: new View({
      center: fromLonLat(center),
      zoom: 14.1,
      minZoom: 14, 
      maxZoom: 18
    })
  })

  if (navigator.geolocation) {
    navigator.geolocation.watchPosition(
      pos => {
        const coords = fromLonLat([pos.coords.longitude, pos.coords.latitude])
        userMarker.getGeometry().setCoordinates(coords)
      },
      err => console.error(err),
      { enableHighAccuracy: true, maximumAge: 0, timeout: 15000 }
    )
  }
  
  const updateDistanceToNextBus = (busLon, busLat) => {
    const userCoords = userMarker.getGeometry().getCoordinates(); // [x, y] in map projection
    const userLonLat = toLonLat(userCoords);                      // Convert to [lon, lat]
    const busLonLat = [busLon, busLat];                           // Already in [lon, lat]

    const distanceInMeters = getDistance(userLonLat, busLonLat); // Now correct units

    overviewState.distanceToNextBus.value = `${Math.round(distanceInMeters)} m`;
  };

  setInterval(async () => {
  try {
    const res = await fetch(apifConfig['get-gps'])
    let buses = await res.json() || []

    if (!Array.isArray(buses)) {
      buses = [buses]
    }

    overviewState.lastUpdate.value = new Date().toLocaleTimeString('de-DE');

    buses.forEach(({ id, latitude, longitude }) => {
      const existing = busPositions.find(b => b.id === id)
      const coords = fromLonLat([longitude, latitude])

      if (existing) {
        // Compute direction from previous to current
        const dx = longitude - existing.longitude
        const dy = latitude - existing.latitude
        const angleRad = Math.atan2(dy, dx)
        const angleDeg = angleRad * (180 / Math.PI)

        // Rotate the direction arrow (in the map corner)
        const arrow = document.getElementById('direction-arrow')
        if (arrow) {
          arrow.style.transform = `rotate(${angleDeg}deg)`
        }

        // Update position
        existing.feature.getGeometry().setCoordinates(coords)
        existing.latitude = latitude
        existing.longitude = longitude

        updateDistanceToNextBus(longitude, latitude)

      } else {
        const feature = new Feature(new Point(coords))
        feature.setStyle(new Style({
          image: new Icon({
            src: 'https://maps.google.com/mapfiles/ms/icons/bus.png',
            scale: 1
          })
        }))
        updateDistanceToNextBus(longitude, latitude)
        dynamicSource.addFeature(feature)

        // Save initial position (no rotation)
        busPositions.push({ id, latitude, longitude, feature })
      }
    })
  } catch (err) {
    console.error('GPS fetch error', err)
  }
  }, updateInterval)
})


busConfig.haltepunkte.forEach(({ lon, lat }) => {
  const haltepunktFeature = new Feature({
    geometry: new Point(fromLonLat([lon, lat]))
  })

  haltepunktFeature.setStyle(new Style({
    image: new Icon({
      src: 'https://maps.google.com/mapfiles/ms/icons/red-dot.png',
      scale: 1
    })
  }))

  haltepunktSource.addFeature(haltepunktFeature)
})
</script>

<style scoped>
.map {
  width: 100%;
  height: 60vh;
}
.map-card {
  display: flex;
  flex-direction: column; /* stack children vertically */
  gap: 2rem; /* space between the map and the stats */
  color: #0168B3 !important;
  width: 90%;
  max-width: 800px;
  background-color: rgba(255, 255, 255, 0.6);
  border-radius: 12px;
  padding: 1.5rem;
  backdrop-filter: blur(8px);
  border: 1px solid rgba(255, 255, 255, 0.3);
}
.map-panel {
  display: flex;
  flex-direction: column;
  gap: 2rem;
  color: #0168B3;
}

::v-deep(.el-descriptions__label),
::v-deep(.el-descriptions__content) {
  color: #0168B3 !important;
}

.map-direction-icon {
  position: absolute;
  top: 30px;
  right: 30px;
  background: white;
  padding-left: 8px;
  padding-right: 5px;
  padding-top: 5px;
  padding-bottom: 3px;
  border-radius: 8px;
  box-shadow: 0 2px 6px rgba(0, 0, 0, 0.3);
  z-index: 10;
}

.map-direction-icon img {
  width: 30px;
  height: 30px;
  transform: rotate(0deg); /* can update dynamically */
}

.legend {
  display: flex;
  flex-wrap: wrap;
  justify-content: center; /* Center horizontally */
  gap: 1rem;
  align-items: center;
}

.legend-item {
  display: flex;
  align-items: center;
  gap: 0.5rem;
}

.legend-item img {
  width: 20px;
  height: 20px;
}

</style>
