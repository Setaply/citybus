<template>
  <el-card class="map-panel">
    <div ref="mapContainer" class="map"></div>
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
import { fromLonLat } from 'ol/proj'
import GeoJSON from 'ol/format/GeoJSON'
import mapGeoJsonUrl from '@/assets/map.geojson?url'
import apifConfig from "../config/apiConfig"

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
      markerLayer
    ],
    view: new View({
      center: fromLonLat(center),
      zoom: 14.1
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

  setInterval(async () => {
    try {
      const res = await fetch(apifConfig['get-gps'])
      const buses = await res.json()

      console.log(buses)

      buses.forEach(({ id, latitude, longitude }) => {
        const existing = busPositions.find(b => b.id === id)
        const coords = fromLonLat([longitude, latitude])

        if (existing) {
          existing.feature.getGeometry().setCoordinates(coords)
          existing.latitude = latitude
          existing.longitude = longitude
        } else {
          const feature = new Feature(new Point(coords))
          feature.setStyle(new Style({
            image: new Icon({
              src: 'https://maps.google.com/mapfiles/ms/icons/bus.png',
              scale: 1
            })
          }))
          dynamicSource.addFeature(feature)
          busPositions.push({ id, latitude, longitude, feature })
        }
      })
    } catch (err) {
      console.error('GPS fetch error', err)
    }
  }, updateInterval)
})
</script>

<style scoped>
.map {
  width: 100%;
  height: 60vh;
}
.map-panel {
  color: #0168B3;
  width: 90%;
  max-width: 800px;
  background-color: rgba(255, 255, 255, 0.6);
  border-radius: 12px;
  padding: 1.5rem;
  backdrop-filter: blur(8px);
  border: 1px solid rgba(255, 255, 255, 0.3);
}
</style>
