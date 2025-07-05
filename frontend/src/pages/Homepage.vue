<template >
    <div class="homepage-container">
        <Map></Map>
        <el-link href="https://www.stadtwerke-huenfeld.de/verkehr/" :icon="InfoFilled" type="primary" style="font-size: large;" target="_blank">Offizielle Karte</el-link>
    </div>
</template>

<script setup>
import Map from '../components/Map.vue';
import overviewState from "../state/overviewState"
import { InfoFilled } from '@element-plus/icons-vue'

</script>

<style scoped>
.homepage-container {
  display: flex;
  flex-direction: column;
  justify-content: center;
  align-items: center;
  color: #0168B3;
  gap: 2rem;
  padding: 2rem;
}
</style>