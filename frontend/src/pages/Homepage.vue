<template >
    <div class="homepage-container">
        <Map></Map>
         <el-card class="info-panel">
            <h3>Übersicht</h3>
            <el-descriptions :column="2" border>
                <el-descriptions-item label="Busse Insgesamt">12</el-descriptions-item>
                <el-descriptions-item label="Aktive Busse">9</el-descriptions-item>
                <el-descriptions-item label="Nächster Bus (m)">135</el-descriptions-item>
                <el-descriptions-item label="Letzte Aktualisierung">12:43:22</el-descriptions-item>
            </el-descriptions>
        </el-card>
    </div>
</template>

<script setup>
import Map from '../components/Map.vue';
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

.info-panel {
  color: #0168B3;
  width: 90%;
  max-width: 800px;
  background-color: rgba(255, 255, 255, 0.6); /* semi-transparent white */
  border-radius: 12px;
  padding: 1.5rem;
  backdrop-filter: blur(8px); /* optional: gives a frosted glass effect */
  border: 1px solid rgba(255, 255, 255, 0.3); /* soft border */
  text-align: center;
  font-size: x-large;
}

.info-panel * {
  color: #0168B3 !important;
}
</style>