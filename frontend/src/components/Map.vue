<template>
  <el-card class="map-panel">
    <div ref="mapContainer" class="map"></div>
  </el-card>
</template>


<script setup>
import { onMounted, ref } from 'vue';
import 'ol/ol.css';
import Map from 'ol/Map';
import View from 'ol/View';
import TileLayer from 'ol/layer/Tile';
import OSM from 'ol/source/OSM';
import VectorLayer from 'ol/layer/Vector';
import VectorSource from 'ol/source/Vector';
import Feature from 'ol/Feature';
import LineString from 'ol/geom/LineString';
import Style from 'ol/style/Style';
import Stroke from 'ol/style/Stroke';
import { fromLonLat } from 'ol/proj';  // <-- Import projection helper

import busConfig from "../config/busConfig"

const pathWaypoints = busConfig.busRoute;

const map = ref();
const mapContainer = ref(null);

const minLonLat = [9.5, 50.5];   // example lower left corner
const maxLonLat = [10.0, 50.8];  // example upper right corner

// Convert to EPSG:3857
const min = fromLonLat(minLonLat);
const max = fromLonLat(maxLonLat);

const extent = [min[0], min[1], max[0], max[1]];

onMounted(() => {
  const pathFeature = new Feature({
    geometry: new LineString(pathWaypoints).transform('EPSG:4326', 'EPSG:3857'),
  });

  pathFeature.setStyle(
    new Style({
      stroke: new Stroke({
        color: '#FF0000',
        width: 3,
      }),
    })
  );

  const pathLayer = new VectorLayer({
    source: new VectorSource({
      features: [pathFeature],
    }),
  });

  map.value = new Map({
    target: mapContainer.value,  // Use the ref element here
    layers: [
      new TileLayer({
        source: new OSM(),
      }),
      pathLayer,
    ],
    view: new View({
      center: fromLonLat([9.766263179078805, 50.66432224964993]),
      zoom: 16,
      minZoom: 14,
      maxZoom: 20,
    }),
  });
});
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
  background-color: rgba(255, 255, 255, 0.6); /* semi-transparent white */
  border-radius: 12px;
  padding: 1.5rem;
  backdrop-filter: blur(8px); /* optional: gives a frosted glass effect */
  border: 1px solid rgba(255, 255, 255, 0.3); /* soft border */
}

</style>
